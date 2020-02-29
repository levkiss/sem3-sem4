#include "../include/id.h"

Id::Id() //Constructor
{
    declare = false;
    assign = false;
}

/*Get name of string*/
std::string Id::get_name(){return name;}

/*Get type of id*/
Lex_type Id::get_type(){return type;}

/*Get value if id*/
double Id::get_value(){return value;}

/*Get assign from id*/
bool Id::get_assign(){return assign;}

/*Get declare from id*/
bool Id::get_declare(){return declare;}

/*Put name into string*/
void Id::put_name(const char *nm){name = std::string(nm);}

/*Put type into id*/
void Id::put_type(Lex_type tp){type = tp;}

/*Put value into id*/
void Id::put_value(double val) {value = val;}

/*Put assign into id*/
void Id::put_assign(){Id::assign = true;}

/*Put declare into id*/
void Id::put_declare(){declare = true;}
