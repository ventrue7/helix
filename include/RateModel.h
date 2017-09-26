/*
 * ==========================================================================
 *
 *       Filename:  RateModel.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017-09-24
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  James Ding
 *                  james.ding.illinois@gmail.com
 *
 * ==========================================================================
 */
#ifndef RATEMODEL_H
#define RATEMODEL_H

#include <string>

using namespace std;

enum RateModelType {RMT_G2PP=0, RMT_BLACK, NUM_MODEL_TYPES};

struct Error{
    int SuccessCode;
    string Message;

    int Line;
    string File;
    string Function;

    Error(int code, string message): SuccessCode(code), Message(message){}
};

class RateModel{
    protected:
        RateModelType Type;
        string        Description;
    public:
        RateModel(RateModelType type, string description) : Type(type), Description(description) {
        }
        
        virtual ~RateModel(){

        }

        virtual RateModelType getModelType(){
            return Type;
        }

        virtual string getModelDescription(){
            return Description;
        }

        virtual double getPV(double t, double T){
           return 0.0;
        }

};


#endif
