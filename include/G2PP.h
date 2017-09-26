/*
 * ==========================================================================
 *
 *       Filename:  G2++.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017-09-25
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  James Ding
 *                  james.ding.illinois@gmail.com
 *
 * ==========================================================================
 */

/* 
G2++:                   r[t] = X[t] + Y[t] + Phi[t], X[0] = 0; Y[0] = 0;
                    where:                            
                        dX[t] = -a*X[t] + sigma1*dW[t] = -a*X[t] + sigma1*sqrt(dt)*z1[t]
                        dY[t] = -b*Y[t] + sigma2*dW[t] = -b*Y[t] + sigma2*sqrt(dt)*z2[t]
                        Phi[t]= f_m_(0, t) + sigma1^2/(2*a^2)*[1-e^(-a*t)]^2
                                           + sigma2^2/(2*b^2)*[1-e^(-b*t)]^2
                                           + rho*sigma1*sigma2/(a*b)*[1-e^(-a*t)]*[1-e^(-b*t)]
                        Integral(0,t)->Phi[u]du = Pm(0,t)*exp[-0.5*V(0,T)]
                        Integral(t,T)->Phi[u]du = Pm(0,T)/Pm(0,t)*exp{-0.5*[V(0,T)-V(0,t)]}

                        Integral(t,T)->(X[u] + Y[u])du ~ N [M(t,T), V(t,T)]
                    where:
                        M(t,T) = X[t]*{1-exp[-a*(T-t)]}/a + Y[t]*{1-exp[-b*(T-t)]}/b
                        V(t,T) = {T-t + exp[-a*(T-t)]*2/a - exp[-2*a*(T-t)]/(2*a)-3/(2*a)}*sigma1^2/a^2
                               + {T-t + exp[-b*(T-t)]*2/b - exp[-2*b*(T-t)]/(2*b)-3/(2*b)}*sigma2^2/b^2
                               + <T-t + {exp[-a*(T-t)]-1}/a - {exp[-b*(T-t)]-1}/b + {exp[-(a+b)*(T-t)]-1}/(a+b)>*2*rho*sigma1*sigma2/(a*b)

                    Theoretical Bond Price at settlement t w/ maturity T
                        P(t,T) = Pm(0,T)/Pm(0,t) * exp(0.5*[V(t,T)-V(0,T)+V(0,t)] - M(t,T))
-------------------------------------------------------------------------------------------
Simulation Data Structure Visualization:

Orstein-Uhlenbeck Speed:  [ a         ,      b     ]
                            ||               ||
                            ||   applys to   ||
                            ||               ||
                Matrix[dim=0][path=0][t]  Matrix[dim=1][path=0][t]
                            ||               ||
                           \||/    **       \||/    **
                            \/   *  *        \/   * <=======thread[i+1]
                           X[t]*<==========>Y[t]* *  *
                              *  *  *          * <======thread[i] 
Dynamics:                     *  *  *          *  *  *
                         term *  * *      term *  * *
                              *  *             *  *
                              **               **
                             /\               /\
                            /||\   **        /||\   **
                             ||  *  *         ||  * <=======thread[i+1]
                          z1[t]*<===rho===>z2[t]* *  *
                              *  *  *          * <======thread[i]
Samples(rho):                 *  *  *          *  *  *
                              *  * *           *  * *
                              *  *             *  *
                              **               **
                             /\               /\
                            /||\  applys to  /||\
                             ||               ||
Volatilies:               [ sigma1    ,      sigma2 ]


*/


#ifndef G2PP_H
#define G2PP_H

//#include <math.h>
#include <iostream>
#include "Simulation.h"
#include "Curve.h"
#include "RateModel.h"

enum Procedure{
   PROC_SIMULATION=1, 
   PROC_CORRELATION=2, 
   PROC_EVOLUTION=4
};

enum Parameter{
   PARAM_A=0, 
   PARAM_B, 
   PARAM_SIGMA_1, 
   PARAM_SIGMA_2, 
   PARAM_RHO,
   PARAM_K,
   PARAM_THETA,
   PARAM_XI
};

enum Dimension{
   DIM_X=0, 
   DIM_Y, 
   DIM_VOL
};

class G2PP : public RateModel{
    private:
        Simulation *Sim;        //Simulation Core, responsible for generating random numbers in bulk
        Curve      *YieldCurve; //Yield curve, contains current market condition, and inter/extrapolation utilities

        mat3d      Samples;     //Random matrix containing: dimension 1: z1~N(0,1), i.i.d, dimension 2: z2~N(0,1), i.i.d
        int        nthreads;    //Number of threads for simulation
        int        DIRTY;       //binary process recalcuation flag
    public:
        Error Error;
        
        //Constructor: pass in terms, npaths, ndimensions + distributional paramters of simulation
        G2PP();
        G2PP(Curve *);
        ~G2PP();

        //----Calculation Services-----
        double M(double x, double y);
        double V(double, double);

        //----Getters & Setters-----
        Simulation* getSimEngine();
        

        //-----Simulation Procedural Step Components Recalculation Bitwise Marker/Verifier-----
        void markDirty(int);
        void markDirtyAll();
        void clearDirty(int);
        void clearDirtyAll();

        int isDirty(int);
    
};


#endif
