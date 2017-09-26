/*
 * ==========================================================================
 *
 *       Filename:  G2Enums.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017-09-26
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  James Ding
 *                  james.ding.illinois@gmail.com
 *
 * ==========================================================================
 */
#ifndef G2ENUMS_H
#define G2ENUMS_H

enum RateModelType {RMT_G2PP=0, RMT_BLACK, NUM_MODEL_TYPES};

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
   PARAM_XI,
   NUM_PARAMS
};

enum Dimension{
   DIM_X=0, 
   DIM_Y, 
   DIM_VOL,
   NUM_DIMS
};


#endif
