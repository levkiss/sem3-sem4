#include "../include/tokenizing.h"

/*Constructor*/
Lexem::Lexem(Lex_type t, double v , int str , int num) : t_lex(t), v_lex(v), str_lex(str), num_lex(num){}

/*Get type of lexem*/
Lex_type Lexem::get_type(){return t_lex;}

/*Get value of lexem*/
double Lexem::get_value(){return v_lex;}

std::ostream & operator<< (std::ostream &s, Lexem l) //Print lexem
{
    s << '(' << l.t_lex << ',' << l.v_lex << ");";
    return s;
}