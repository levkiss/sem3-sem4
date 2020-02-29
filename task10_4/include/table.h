#pragma once

#ifndef table_h
#define table_h

#include "id.h"

class Table //Table of identifications
{
private:
    Id *id;   //Pointer to id
    int size; //Size of table
    int top;  //Top of table
    
public:
    Table(int max_size);        //Constructor
    ~Table();                   //Destructor
    int put_id(const char *buf);//Put id into table
    
    Id & operator [] (int i);   //Take identification
};

#endif /* table_h */
