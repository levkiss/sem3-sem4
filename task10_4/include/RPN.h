#pragma once

#ifndef RPN_h
#define RPN_h

#include "lexem.h"
#include "table.h"
#include <vector>
#include <stdlib.h>

extern Table TID; //Table of ids

class RPN
{
private:
    std::vector <Lexem> RPN_array;   //RPN array
    int used_mem = 0;   //used memory in RPN_array
    
public:
    RPN();  //RPN constructor
    ~RPN(); //RPN destructor
    void add_lexem(Lexem Lex);        //Add lexem into end of array
    void add_lexem(Lexem Lex, int i); //Add lexem into array
    int get_free();   //Get index of free part of an array
    void print();     //Print RPN_array
    void skip_lexem();//Save place for lexem in array
    
    Lexem & operator[] (int index); //Return pos element of RPN_array
};

#endif /* RPN_h */
