#include "G2PP.h"

G2PP::G2PP()
    : RateModel(RateModelType::RMT_G2PP, "G2++: Gaussian 2-factor short rate model"),
    Sim(new Simulation()),
    YieldCurve(nullptr),
    Samples(nullptr),
    Dirty(0),
    Coefs(new double[NUM_PARAMS]{0}),
    Peris(new int[NUM_SIMS]{2}),
    Flags(new int[NUM_PARAMS]{PROC_EVOLUTION}),
    ModelError(1,"Success!")
{
    this->markDirtyAll();
}

G2PP::G2PP(Curve * curve)
    : RateModel(RateModelType::RMT_G2PP, "G2++: Gaussian 2-factor short rate model"),
    Sim(new Simulation()),
    YieldCurve(curve),
    Samples(nullptr),
    Dirty(0),
    Coefs(new double[NUM_PARAMS]{0}),
    Peris(new int[NUM_SIMS]{2}),
    Flags(new int[NUM_PARAMS]{PROC_EVOLUTION}),
    ModelError(1,"Success!")
{
    this->markDirtyAll();
}

G2PP::~G2PP(){
    if (Samples){
        delete Samples;
    }
    delete [] Coefs;
    delete [] Peris;
    delete [] Flags;
    delete Sim;
}


//----Calculation Services-----
double G2PP::getZCBPrice(double t, double T){
    if (!YieldCurve){
        ModelError.Message = "getZCBPrice(): current market condition unknown";
        ModelError.Code = 0;

        ModelError.Function = __func__;
        ModelError.Line = __LINE__;
        ModelError.File = __FILE__;

        throw ModelError;
    }

    double a = Coefs[PARAM_A];
    double b = Coefs[PARAM_B];
    double vol1 = Coefs[PARAM_SIGMA_1];
    double vol2 = Coefs[PARAM_SIGMA_2];
    double rho = Coefs[PARAM_RHO];

    int terms = Peris[SIM_NTERMS];
    int paths = Peris[SIM_NPATHS];
    int dims = Peris[SIM_NDIMS];
    int threads = Peris[SIM_NTHREADS];

    double dt = (double)t/terms;
    if (Samples){
        if (Samples->nterms != terms || Samples->npaths != paths){
            delete Samples;
            Samples = new mat3d(terms,paths,dims);
            markDirty(PROC_GENERATION);
        }
    }else{
        Samples = new mat3d(terms,paths,dims);
        markDirty(PROC_GENERATION);
    }

    if (isDirty(PROC_GENERATION)){
        Sim->ngen(*Samples,threads);
    }

    double * X;
    double * Y;
    if (isDirty(PROC_EVOLUTION)){
        X = new double[paths]{0};
        Y = new double[paths]{0};

        int i,j;
        for (i = 0; i < paths; ++j){
            X[i] = vol1*sqrt(dt)*(*Samples)[DIM_X][i][0];
            Y[i] = vol2*sqrt(dt)*(*Samples)[DIM_X][i][0];
        }

        for (i = 1; i < paths; ++j){
            for (j = 0; j < terms; ++j){
                X[i] += -a*X[i]*dt + vol1*sqrt(dt)*(*Samples)[DIM_X][i][j];
                Y[i] += -b*Y[i]*dt + vol2*sqrt(dt)*(*Samples)[DIM_X][i][j];
            }
        }
    }

    delete [] X;
    delete [] Y;
    return 0.0;
}

double G2PP::getZCBPrice(double *t, double T){
    if (!YieldCurve){
        ModelError.Message = "getZCBPrice(): current market condition unknown";
        ModelError.Code = 0;

        ModelError.Function = __func__;
        ModelError.Line = __LINE__;
        ModelError.File = __FILE__;

        throw ModelError;
    }

    return 0.0;
}

double G2PP::M(double x, double y, double T_t){
    double a = Coefs[PARAM_A];
    double b = Coefs[PARAM_B];
    
    double fact1 = a ? x*(1.-exp(-a*(T_t)))/a : x*(T_t);
    double fact2 = b ? y*(1.-exp(-b*(T_t)))/b : y*(T_t);

    return (fact1 + fact2);
}

double G2PP::V(double T_t){
    double a = Coefs[PARAM_A];
    double b = Coefs[PARAM_B];
    double vol1 = Coefs[PARAM_SIGMA_1];
    double vol2 = Coefs[PARAM_SIGMA_2];
    double rho = Coefs[PARAM_RHO];

    double fact1 = a ? vol1*vol1/a/a*(T_t+2./a*exp(-a*(T_t))-.5/a*exp(-2.*a*(T_t))-1.5/a) : vol1*vol1*(T_t);
    double fact2 = b ? vol2*vol2/b/b*(T_t+2./a*exp(-b*(T_t))-.5/b*exp(-2.*b*(T_t))-1.5/b) : vol2*vol2*(T_t);
    double fact12= 0;

    if ( a && b ){
        fact12 = vol1*vol2/a/b*(T_t + (exp(-a*(T_t))-1.)/a + (exp(-b*(T_t))-1.)/b - (exp(-(a+b)*(T_t))-1.)/(a+b));
    }else{
        if ( !a && !b ){
            fact12 = vol1*vol2*(T_t);
        }else if (a && !b){
            fact12 = vol1*vol2/a*(T_t + (exp(-a*(T_t))-1.)/a );
        }else{
            fact12 = vol1*vol2/b*(T_t + (exp(-b*(T_t))-1.)/b );
        }
    }
    return (fact1 + fact2 + 2.*rho*fact12);
}


//----Getters & Setters-----
void G2PP::setParameter(Parameter key, double value){
    Coefs[key] = value;
    markDirtyFrom(Flags[key]);
}

void G2PP::setParameters(Parameter * keys, double * values, int length){
    int i;
    if (length > NUM_PARAMS){
        ModelError.Message = "setParameters(): size greater than possible";
        ModelError.Code = 0;

        ModelError.Function = __func__;
        ModelError.Line = __LINE__;
        ModelError.File = __FILE__;

        throw ModelError;
    }
    for (i=0; i<length; ++i){
        setParameter(keys[i], values[i]);
    }
}

Simulation* G2PP::getSimEngine(){
   return Sim;
}


//----Bitwise marker----
void G2PP::markDirtyFrom(int step){
    if (step){
        int i;
        for (i=step; i<NUM_PROCS; i<<=1u){
            Dirty|=i;
        }
    }
}

void G2PP::markDirty(int step){
    Dirty|=step;
}

void G2PP::markDirtyAll(){
    int i;
    for (i=PROC_GENERATION; i<NUM_PROCS; i<<=1u){
        Dirty|=i;
    }
}

void G2PP::clearDirty(int step){
    Dirty&=~step;
}

void G2PP::clearDirtyAll(){
    Dirty=0;
}

int G2PP::isDirty(int step){
    return Dirty & step;
}
