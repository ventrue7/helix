#include <iostream>
#include <iomanip>
#include <string>

#include "G2PP.h"

using namespace std;

int main(){
    int nterms = 500000;
    int npaths = 3;
    int ndims = 3;
    cout<<"Multithread Sim Distrubution Analysis"<<endl;
    mat3d matrix(nterms,npaths,ndims);
    double mu = 10.0;
    double sigma = 5.0;

    int nthreads = 2;
    int nstars = 500; 
    
    Simulation *Sim = new Simulation(mu, sigma);
    
    Sim->ngen(matrix,nthreads,true);
    cout<<"----------------------------------"<<endl;
    cout<<"Simulation Dims: "<<Sim->getGenerator()->Dimensions()<<endl;
    cout<<"Simulation Paths: "<<Sim->getGenerator()->Paths()<<endl;
    
    //Distribution analysis by visualization: N(10, 5)
    for (int i=0; i<npaths; ++i){
        for(int j=0; j<ndims; ++j){
        cout<<"Dimension "<<i+1<<", "<<"Path "<<j+1<<endl;
            int p[20] = {};
            for (int k=0; k<nterms; ++k){
                double number = matrix[i][j][k];
                if ((number>=0.0)&&(number<20.0)) ++p[int(number)];
            }
            for (int i=0; i<20; ++i){
                cout << i << "-"<<(i+1) << ":\t";
                cout << string(p[i]*nstars/nterms, '*')<<endl;
            }
        } 
    }

    Sim->ngen(matrix,nthreads);
    cout<<"----------------------------------"<<endl;
    cout<<"Simulation Dims: "<<Sim->getGenerator()->Dimensions()<<endl;
    cout<<"Simulation Paths: "<<Sim->getGenerator()->Paths()<<endl;
    
    //Distribution analysis by visualization: N(10, 5)
    for (int i=0; i<npaths; ++i){
        for(int j=0; j<ndims; ++j){
        cout<<"Dimension "<<i+1<<", "<<"Path "<<j+1<<endl;
            int p[20] = {};
            for (int k=0; k<nterms; ++k){
                double number = matrix[i][j][k];
                if ((number>=0.0)&&(number<20.0)) ++p[int(number)];
            }
            for (int i=0; i<20; ++i){
                cout << i << "-"<<(i+1) << ":\t";
                cout << string(p[i]*nstars/nterms, '*')<<endl;
            }
        } 
    }

    delete Sim;
    return 0;
}
