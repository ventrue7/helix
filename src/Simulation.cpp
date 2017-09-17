#include "Simulation.h"

Simulation::Simulation(){
    mu = 0.0;
    sigma = 1.0;
}

Simulation::Simulation(double mu, double sigma, int npaths, int ndims):mu(mu),sigma(sigma),Generator(new Rand<double>(npaths,ndims,mu,sigma)) {
}

Simulation::~Simulation(){
    delete Generator;
}
//private
void Simulation::constructGenerator(double mu, double sigma, int npaths, int ndims){
    this->mu = mu;
    this->sigma = sigma;
    this->Generator = new Rand<double>(npaths, ndims, mu, sigma);
}

//private
void Simulation::destoryGenerator(){
   delete Generator; 
}
//private
//make sure generator has the same dimensions as those of the random matrix specified by user
void Simulation::fitGenerator(int npaths, int ndims){
    if (Generator){//check if generator has been instantiated
        if (Generator->Paths()!=npaths || Generator->Dimensions()!=ndims){
            destoryGenerator();
            constructGenerator(this->mu, this->sigma, npaths, ndims);
        }
    }else{
        constructGenerator(this->mu, this->sigma, npaths, ndims);
    }
}

//Multithreaded random matrix generation processes:
//thread needs an instance of Simulation to operate member function on, here we used lambda expression
//lambda expression:
//[a,&b] where a is captured by value and b is captured by reference.
//[this] captures the this pointer by value
//[&] captures all automatic variables odr-used in the body of the lambda by reference
//[=] captures all automatic variables odr-used in the body of the lambda by value
//[] captures nothing/
void Simulation::ugen(mat3d & random_matrix, int nthreads){
    fitGenerator(random_matrix.npaths, random_matrix.ndims);
     
    if (nthreads<1) {nthreads = thread::hardware_concurrency();}
    
    switch (nthreads){
        case 1: 
        // optimized for single threade(use main thread to avoid overhead)
            int dim, path, term;
            for (dim=0; dim<random_matrix.ndims; ++dim){
                for (path=0; path<random_matrix.npaths;++path){
                    for (term=0; term<random_matrix.nterms;++term){
                        random_matrix[dim][path][term] = Generator->urand(dim, path);
                    }
                }
            }
            break;

        default:
            if (nthreads> random_matrix.npaths){
                nthreads = random_matrix.npaths;
            }
            int extra_paths = random_matrix.npaths % nthreads;
            int paths_per_thread = (random_matrix.npaths - extra_paths)/nthreads;

            thread * threads = new thread[nthreads];
            int t;
            for (t = 0; t<(nthreads-1); ++t){
                threads[t] = thread([&](int begin, int end)->void{
                    int dim, path, term;
                    for (dim=0; dim<random_matrix.ndims; ++dim){
                        for (path=begin; path<end; ++path){
                            for (term=0; term<random_matrix.nterms; ++term){
                                random_matrix[dim][path][term] = Generator->urand(dim, path);
                            }
                        }
                    }
                     
                }, t*paths_per_thread, (t+1)*paths_per_thread);
            }

            threads[nthreads-1] = thread([&]()->void{
                int dim, path, term;
                for (dim=0; dim<random_matrix.ndims; ++dim){
                    for (path = (nthreads-1)*paths_per_thread; path < random_matrix.npaths; ++path){
                        for (term=0; term<random_matrix.nterms; ++term){
                            random_matrix[dim][path][term] = Generator->urand(dim, path);
                        }
                    }
                }
            });

            for (t=0; t < nthreads; ++t){
                threads[t].join();
            }

            delete [] threads;
            break;
    }
}

void Simulation::ngen(mat3d & random_matrix, int nthreads){//nthreads<=random_matrix.npaths 
    fitGenerator(random_matrix.npaths, random_matrix.ndims);

    if (nthreads<1) {nthreads = thread::hardware_concurrency();}
    
    switch (nthreads){
        case 1: 
        // optimized for single threaded (use main thread to avoid overhead)
            int dim, path, term;
            for (dim=0; dim<random_matrix.ndims; ++dim){
                for (path=0; path<random_matrix.npaths;++path){
                    for (term=0; term<random_matrix.nterms;++term){
                        random_matrix[dim][path][term] = Generator->nrand(dim, path);
                    }
                }
            }
            break;

        default:
            if (nthreads> random_matrix.npaths){
                nthreads = random_matrix.npaths;
            }
            int extra_paths = random_matrix.npaths % nthreads;
            int paths_per_thread = (random_matrix.npaths - extra_paths)/nthreads;

            thread * threads = new thread[nthreads];
            int t;
            for (t = 0; t<(nthreads-1); ++t){
                threads[t] = thread([&](int begin, int end)->void{
                    int dim, path, term;
                    for (dim=0; dim<random_matrix.ndims; ++dim){
                        for (path=begin; path<end; ++path){
                            for (term=0; term<random_matrix.nterms; ++term){
                                random_matrix[dim][path][term] = Generator->nrand(dim, path);
                            }
                        }
                    }
                     
                }, t*paths_per_thread, (t+1)*paths_per_thread);
            }

            threads[nthreads-1] = thread([&]()->void{
                int dim, path, term;
                for (dim=0; dim<random_matrix.ndims; ++dim){
                    for (path=(nthreads-1)*paths_per_thread; path<random_matrix.npaths; ++path){
                        for (term=0; term<random_matrix.nterms; ++term){
                            random_matrix[dim][path][term] = Generator->nrand(dim, path);
                        }
                    }
                }
            });

            for (t=0; t < nthreads; ++t){
                threads[t].join();
            }

            delete [] threads;
            break;
    }
}


Rand<double> * Simulation::getGenerator(){
    return Generator;
}
