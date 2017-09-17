/*
 * ==========================================================================
 *
 *       Filename:  Simulation.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017-09-16
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  James Ding
 *                  james.ding.illinois@gmail.com
 *
 * ==========================================================================
 */


/* 
                      ******************
                     *                **
             ndims *                *  *
                 *     npaths     *    *
                ******************     *
                *                *     *
          nterms*                *    *
                *                *  *
                *                **
                ******************
*/

#ifndef SIMULATION_H
#define SIMULATION_H

#include "Rand.h"
#include "Matrix.h"
#include <thread>
#include <cmath>

using namespace std;


class Simulation{
    private:
        Rand<double> * Generator;

        void constructGenerator(double, double, int, int);
        void destoryGenerator();
        void fitGenerator(int, int);

        double mu;
        double sigma;
    public:
        Simulation();
        Simulation(double, double, int, int);
        ~Simulation();

        void ugen(mat3d & rand_mtrx, int nthreads, bool match_dims=false);//populate 3D matrix(double) with uniformly distributed number, i.i.d
        void ngen(mat3d &, int, bool match_dims=false);//populate 3D matrix(double) with uniformly distributed number, i.i.d
        
        Rand<double> * getGenerator();
};


#endif //SIMULATION_H
