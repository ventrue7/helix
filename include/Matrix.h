/*
 * ==========================================================================
 *
 *       Filename:  Matrix.h
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
#ifndef MATRIX_H
#define MATRIX_H


struct mat3d{
    int nterms;
    int npaths;
    int ndims;

    double *** value;

    //allocate and zero memory dynamically
    mat3d(int terms, int paths, int dims):value(new double **[dims]),nterms(terms),npaths(paths),ndims(dims){
        int dim, path;
        for (dim=0; dim<ndims; ++dim){
            value[dim] = new double*[npaths];
            for (path=0; path<npaths; ++path){
                value[dim][path] = new double [terms]{0}; //zero memory
            }
        }
    }
    
    //delete matrix memory
    ~mat3d(){
       int dim, path;
        for (dim=0; dim<ndims; ++dim){
            for (path=0; path<npaths; ++path){
                   delete [] value[dim][path];
            }
            delete [] value[dim];
        }
        delete [] value;
    }
    
    //operator overload
    double ** operator[](int dim){
        return value[dim];
    }

};

struct mat2d{
    int nrows;
    int ncols;
    
    double ** value;
    
    //allocate and zero memory dynamically
    mat2d(int n_rows, int n_cols):value(new double *[n_cols]),nrows(n_rows),ncols(n_cols){
        int col;
        for (col=0; col<ncols; ++col){
            value[col] = new double [nrows]{0}; //zero memory
        }
    }
    
    //delete matrix memory
    ~mat2d(){
        int  col;
        for (col=0; col<ncols; ++col){
            delete [] value[col];
        }
        delete [] value;
    }
    
    //operator overload
    double * operator[](int col){
        return value[col];
    }
}; 

class Matrix{
    mat2d mat;
    Matrix(int n_row, int n_col) : mat(n_row, n_col) {

    }

    int nColns(){
        return mat.ncols;
    }

    int nRows(){
        return  mat.nrows;
    }

    double * operator [] (int col){
        return mat[col];
    }

    double operator() (int row, int col){
        return mat[col][row];
    }

    Matrix operator* (Matrix & B){
        if (this->nColns() == B.nRows()){
            Matrix C = Matrix(this->nRows(), B.nColns());

            int col, row, i;
            for ( col = 0; col < C.nColns(); ++col ){
                for ( row = 0; row < C.nRows(); ++row ){
                    for ( i = 0; i < B.nRows(); ++i){
                        C[col][row] += B[col][i]*(*this[i][row]);
                    }
                }
            }
            return C;
        }else{
            return *this;
        }
    }

    Matrix Inverse(){
        return *this;
    }
};

#endif
