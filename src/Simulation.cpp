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

/*
 * match_dims = false(default): Dimensions of Random Generator (paths, dims) is [ 1, nthreads ]
 * match_dims = true: Dimensions of Random Generator (paths, dims) is the same as those of target random matrix 
 * */
void Simulation::ugen(mat3d & rand_mtrx, int nthreads, bool match_dims){
    if (nthreads < 1){
       nthreads = thread::hardware_concurrency();
    }else if (nthreads > rand_mtrx.npaths){
       nthreads = rand_mtrx.npaths;
    }

    switch (nthreads){
        case 1: 
        // optimized for single threade(use main thread to avoid overhead)
           if (match_dims){
              fitGenerator(rand_mtrx.npaths, rand_mtrx.ndims);
              int dim, path, term;
              for (dim=0; dim<rand_mtrx.ndims; ++dim){
                 for (path=0; path<rand_mtrx.npaths;++path){
                    for (term=0; term<rand_mtrx.nterms;++term){
                       rand_mtrx[dim][path][term] = Generator->urand(dim, path);
                    }
                 }
              }
           }else{
              fitGenerator(1, 1);
              int extra_paths = rand_mtrx.npaths % nthreads;
              int paths_per_thread = (rand_mtrx.npaths - extra_paths)/nthreads;

              int dim, path, term;
              for (dim=0; dim<rand_mtrx.ndims; ++dim){
                 for (path=0; path<rand_mtrx.npaths;++path){
                    for (term=0; term<rand_mtrx.nterms;++term){
                       rand_mtrx[dim][path][term] = Generator->urand(1, 1);
                    }
                 }
              }
           }
           break;
           

        default:
            int extra_paths = rand_mtrx.npaths % nthreads;
            int paths_per_thread = (rand_mtrx.npaths - extra_paths)/nthreads;
            
            thread * threads = new thread[nthreads];
            if (match_dims){
               fitGenerator(rand_mtrx.npaths, rand_mtrx.ndims);
               int t;
               for (t = 0; t<(nthreads-1); ++t){
                  threads[t] = thread([&](int begin, int end)->void{
                        int dim, path, term;
                        for (dim=0; dim<rand_mtrx.ndims; ++dim){
                           for (path=begin; path<end; ++path){
                              for (term=0; term<rand_mtrx.nterms; ++term){
                                 rand_mtrx[dim][path][term] = Generator->urand(dim, path);
                              }
                           }
                        }
                  }, t*paths_per_thread, (t+1)*paths_per_thread);
               }

               threads[nthreads-1] = thread([&]()->void{
                     int dim, path, term;
                     for (dim=0; dim<rand_mtrx.ndims; ++dim){
                        for (path = (nthreads-1)*paths_per_thread; path < rand_mtrx.npaths; ++path){
                           for (term=0; term<rand_mtrx.nterms; ++term){
                              rand_mtrx[dim][path][term] = Generator->urand(dim, path);
                           }
                        }
                     }
               });

            }else{
               fitGenerator(1, nthreads);
               int t;
               for (t = 0; t<(nthreads-1); ++t){
                  threads[t] = thread([&](int begin, int end)->void{
                        int dim, path, term;
                        for (dim=0; dim<rand_mtrx.ndims; ++dim){
                           for (path=begin; path<end; ++path){
                              for (term=0; term<rand_mtrx.nterms; ++term){
                                 rand_mtrx[dim][path][term] = Generator->urand(1, t);
                              }
                           }
                        }
                  }, t*paths_per_thread, (t+1)*paths_per_thread);
               }

               threads[nthreads-1] = thread([&]()->void{
                     int dim, path, term;
                     for (dim=0; dim<rand_mtrx.ndims; ++dim){
                        for (path = (nthreads-1)*paths_per_thread; path < rand_mtrx.npaths; ++path){
                           for (term=0; term<rand_mtrx.nterms; ++term){
                              rand_mtrx[dim][path][term] = Generator->urand(dim, (nthread-1));
                           }
                        }
                     }
               });
            }
            

            for (t=0; t < nthreads; ++t){
                threads[t].join();
            }

            delete [] threads;
            break;
    }
}

void Simulation::ngen(mat3d & rand_mtrx, int nthreads, bool match_dims){//nthreads<=rand_mtrx.npaths
   if (nthreads < 1){
       nthreads = thread::hardware_concurrency();
    }else if (nthreads > rand_mtrx.npaths){
       nthreads = rand_mtrx.npaths;
    }

    switch (nthreads){
        case 1: 
        // optimized for single threade(use main thread to avoid overhead)
           if (match_dims){
              fitGenerator(rand_mtrx.npaths, rand_mtrx.ndims);
              int dim, path, term;
              for (dim=0; dim<rand_mtrx.ndims; ++dim){
                 for (path=0; path<rand_mtrx.npaths;++path){
                    for (term=0; term<rand_mtrx.nterms;++term){
                       rand_mtrx[dim][path][term] = Generator->nrand(dim, path);
                    }
                 }
              }
           }else{
              fitGenerator(1, 1);
              int extra_paths = rand_mtrx.npaths % nthreads;
              int paths_per_thread = (rand_mtrx.npaths - extra_paths)/nthreads;

              int dim, path, term;
              for (dim=0; dim<rand_mtrx.ndims; ++dim){
                 for (path=0; path<rand_mtrx.npaths;++path){
                    for (term=0; term<rand_mtrx.nterms;++term){
                       rand_mtrx[dim][path][term] = Generator->nrand(1, 1);
                    }
                 }
              }
           }
           break;
           

        default:
            int extra_paths = rand_mtrx.npaths % nthreads;
            int paths_per_thread = (rand_mtrx.npaths - extra_paths)/nthreads;
            
            thread * threads = new thread[nthreads];
            if (match_dims){
               fitGenerator(rand_mtrx.npaths, rand_mtrx.ndims);
               int t;
               for (t = 0; t<(nthreads-1); ++t){
                  threads[t] = thread([&](int begin, int end)->void{
                        int dim, path, term;
                        for (dim=0; dim<rand_mtrx.ndims; ++dim){
                           for (path=begin; path<end; ++path){
                              for (term=0; term<rand_mtrx.nterms; ++term){
                                 rand_mtrx[dim][path][term] = Generator->nrand(dim, path);
                              }
                           }
                        }
                  }, t*paths_per_thread, (t+1)*paths_per_thread);
               }

               threads[nthreads-1] = thread([&]()->void{
                     int dim, path, term;
                     for (dim=0; dim<rand_mtrx.ndims; ++dim){
                        for (path = (nthreads-1)*paths_per_thread; path < rand_mtrx.npaths; ++path){
                           for (term=0; term<rand_mtrx.nterms; ++term){
                              rand_mtrx[dim][path][term] = Generator->nrand(dim, path);
                           }
                        }
                     }
               });

            }else{
               fitGenerator(1, nthreads);
               int t;
               for (t = 0; t<(nthreads-1); ++t){
                  threads[t] = thread([&](int begin, int end)->void{
                        int dim, path, term;
                        for (dim=0; dim<rand_mtrx.ndims; ++dim){
                           for (path=begin; path<end; ++path){
                              for (term=0; term<rand_mtrx.nterms; ++term){
                                 rand_mtrx[dim][path][term] = Generator->nrand(1, t);
                              }
                           }
                        }
                  }, t*paths_per_thread, (t+1)*paths_per_thread);
               }

               threads[nthreads-1] = thread([&]()->void{
                     int dim, path, term;
                     for (dim=0; dim<rand_mtrx.ndims; ++dim){
                        for (path = (nthreads-1)*paths_per_thread; path < rand_mtrx.npaths; ++path){
                           for (term=0; term<rand_mtrx.nterms; ++term){
                              rand_mtrx[dim][path][term] = Generator->nrand(dim, (nthread-1));
                           }
                        }
                     }
               });
            }
            

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
