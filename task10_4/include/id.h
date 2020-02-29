#pragma once

#ifndef id_h
#define id_h

#include "lexem.h"

class Id
{
private:
    std::string name;//Name of id
    Lex_type type;   //Type of id
    double value;    //Value of id
    bool assign;     //Check if assigned
    bool declare;    //Check if declared
    
public:
    Id();//Constructor
    std::string get_name(); //Get name of id
    Lex_type get_type();    //Get type of id
    double get_value();     //Get value if id
    bool get_assign ();     //Get assign from id
    bool get_declare();     //Get declare from id
    void put_name(const char *nm); //Put name into id
    void put_type(Lex_type tp);    //Put type into id
    void put_value(double val);    //Put value into id
    void put_assign ();            //Put assign into id
    void put_declare();            //Put declare into id
};

#endif /* id_h */
