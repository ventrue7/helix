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

enum RateModelType {G2pp, Black};

struct Error{
    int SuccessCode;
    string Message;

    int Line;
    string File;

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

};


#endif
