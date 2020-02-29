#pragma once

#ifndef execution_h
#define execution_h

#include <stack>
#include "RPN.h"
#include "table.h"

extern Table TID; //Table of ids

class Executor
{
    Lexem cur_lex;
public:
    Executor() = default;    //Default constructor
    void execute(RPN & _RPN);//Execute program
    
};

#endif /* execution_h */
