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
   PROC_GENERATION=1,
   PROC_EVOLUTION=2,
   PROC_SIMULATION=4, 
   PROC_CALCULATION=8, 
   PROC_END
};

enum Parameter{
   PARAM_A=0, 
   PARAM_B, 
   PARAM_SIGMA_1, 
   PARAM_SIGMA_2, 
   PARAM_RHO,
   NUM_PARAMS
};

enum Dimension{
   DIM_X=0, 
   DIM_Y, 
   NUM_DIMS
};


#endif
