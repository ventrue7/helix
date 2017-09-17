/*
 * ==========================================================================
 *
 *       Filename:  Rand.h
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

#ifndef RAND_H
#define RAND_H

//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <map>
#include <random>

using namespace std;
//This class creates a 2D pool of random number engines with ndims & npaths <user definited> for multi-threaded use
//Warning: do not share a particular random engine between objects in multithreaded settings

#ifdef WIN32 
    typedef mt19937 Engine;
#else
    typedef mt19937_64 Engine;
#endif 
        
template<class T>
class Rand{
    private:
        Engine ** Engines;
        int ndims;
        int npaths;
    public:

        normal_distribution<T> norm_dist;
        uniform_real_distribution<T> uniform_dist;
        
        //Constructor
        Rand(int, int, T, T); //instantiate object with random_device produced seeds (system entropy)
        Rand(int, int, T, T, unsigned int**);//instantiate object with user provided seeds
        
        //Destructor
        ~Rand(void);

        //generate uniform random i.i.d using (ndim, npath)th generator in the pool 
        T urand(int, int);
        //generate normal random i.i.d using (ndim, npath)th generator in the pool 
        T nrand(int, int);
        
        //utilities
        void seed();//seed/re-seed all generators in the pool with random_device produced intergers
        void seed(unsigned int**);//seed/re-seed all generators with user provided interger seeds
        
        Engine& getEngine(int, int);
        Engine* operator[](int);

        int Paths();
        int Dimensions();
        
};


template <class T>
Rand<T>::Rand(int paths, int dims, T mu, T sigma) : Engines(new Engine* [dims]), npaths(paths), ndims(dims), norm_dist(mu, sigma), uniform_dist(mu, sigma)
{
    random_device rd{};

    int i,j;
    for (i = 0; i<ndims; ++i){
        Engines[i] = new Engine[npaths];
        for (j = 0; j<npaths; ++j){
            Engines[i][j]=Engine(rd()); //seed engine with system entropy
        }
    } 
    
}

template<class T>
Rand<T>::Rand(int paths, int dims, T mu, T sigma, unsigned int**rseed) : Engines(new Engine* [dims]), npaths(paths), ndims(dims), norm_dist(mu, sigma), uniform_dist(mu, sigma)
{
    int i,j;
    for (i = 0; i<ndims; ++i){
        Engines[i] = new Engine[npaths];
        for (j = 0; j<npaths; ++j){
            Engines[i][j]=Engine(rseed[i][j]); //seed engine with user defined seeds
        }
    }
}

template <class T>
Rand<T>::~Rand(void){
    int i;
    for (i = 0; i<ndims; ++i){
        delete [] Engines[i];
    }
    delete [] Engines;
}

template<class T>
T Rand<T>::urand(int dim, int path){
    return uniform_dist(Engines[dim][path]);
}

template<class T>
T Rand<T>::nrand(int dim, int path){//Access Random Generators Pool by id (id<=engine_pool_size), return norm dist value
   return norm_dist(Engines[dim][path]);
}

template<class T>
void Rand<T>::seed(){
    random_device rd{};

    int i,j;
    for (i=0; i<ndims; ++i){
        for (j=0; j<npaths; ++j){
            Engines[i][j].seed(rd());
        }
    }
}

template<class T>
void Rand<T>::seed(unsigned int**rseed){
    int i,j;
    for (i=0; i<ndims; ++i){
        for (j=0; j<npaths; ++j){
            Engines[i][j].seed(rseed[i][j]);
        }
    }
}

template<class T>
Engine& Rand<T>::getEngine(int dim, int path){
    return Engines[dim][path];
}

template<class T>
Engine* Rand<T>::operator[](int dim){
    return Engines[dim];
}

template<class T>
int Rand<T>::Paths(){
    return npaths;
}

template<class T>
int Rand<T>::Dimensions(){
    return ndims;
}
#endif //RAND_H
