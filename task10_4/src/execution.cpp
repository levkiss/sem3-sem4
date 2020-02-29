#include "../include/execution.h"

void Executor::execute(RPN & _RPN) //Execute program
{
    std::stack <double> args;
    int i, j, index = 0, size = _RPN.get_free();
    bool tmp_bool;
    double tmp_real,tmp;
    int tmp_int;
    while (index < size)
    {
        cur_lex = _RPN[index];
        switch (cur_lex.get_type())
        {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case LEX_REAL: case RPN_ADDRESS: case RPN_LABEL:
                args.push (cur_lex.get_value());
                break;
            case LEX_ID:
                i = cur_lex.get_value();
                if (TID[i].get_assign())
                {
                    args.push(TID[i].get_value());
                    break;
                }
                else
                    throw "RPN: indefinite identifier";
            case LEX_NOT:
                tmp_bool = args.top();
                args.pop();
                args.push(!tmp_bool);
                break;
            case LEX_OR:
                i = args.top();
                args.pop();
                tmp_int = args.top();
                args.pop();
                args.push (tmp_int || i);
                break;
            case LEX_AND:
                i = args.top();
                args.pop();
                tmp_int = args.top();
                args.pop();
                args.push (tmp_int && i);
                break;  
            case RPN_GO:
                index = args.top() - 1;
                args.pop();
                break;
            case RPN_FGO:
                i = args.top();
                args.pop();
                tmp_bool = args.top();
                args.pop();
                if ( !tmp_bool ) index = i-1;
                break;
            case LEX_WRITE:
                tmp_real = args.top();
                args.pop();
                std::cout << tmp_real << std::endl;
                break;
            case LEX_READ:
                double k;
                i = args.top ();
                args.pop();
                if ( TID[i].get_type () == LEX_INT  )
                {
                    std::cout << "Input int value for " << TID[i].get_name () << std::endl;
                    std::cin >> k;
                }
                else
                    if ( TID[i].get_type () == LEX_REAL )
                    {
                        std::cout << "Input real value for " << TID[i].get_name () << std::endl;
                        std::cin >> k;
                    }
                    else
                    {
                        char j[20];
                    rep:
                        std::cout << "Input boolean value (true or false) for" << TID[i].get_name() << std::endl;
                        std::cin >> j;
                        if (!strcmp(j, "true"))
                            k = 1;
                        else
                            if (!strcmp(j, "false"))
                                k = 0;
                            else
                            {
                                std::cout << "Error in input:true/false" << std::endl;
                                goto rep;
                            }
                    }
                TID[i].put_value (k);
                TID[i].put_assign ();
                break;
            case LEX_PLUS:
                tmp_real = args.top();
                args.pop();
                tmp_real += args.top();
                args.pop();
                args.push ( tmp_real );
                break;
            case LEX_MULT:
                tmp_real = args.top();
                args.pop();
                tmp_real *= args.top();
                args.pop();
                args.push ( tmp_real );
                break;
            case LEX_MINUS:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real - tmp );
                break;
            case LEX_SLASH:
                tmp = args.top();
                args.pop();
                if (tmp != 0)
                {
                    tmp_real = args.top();
                    args.pop();
                    args.push(tmp_real / tmp);
                    break;
                }
                else
                    throw "RPN:divide by zero";
            case LEX_EQ:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real == tmp );
                break;
            case LEX_LSS:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real < tmp);
                break;
            case LEX_GTR:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real > tmp);
                break;
            case LEX_LEQ:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real <= tmp);
                break;
            case LEX_GEQ:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real >= tmp);
                break;
            case LEX_NEQ:
                tmp = args.top();
                args.pop();
                tmp_real = args.top();
                args.pop();
                args.push ( tmp_real != tmp);
                break;
            case LEX_ASSIGN:
                tmp_real = args.top();
                args.pop();
                j = args.top();
                args.pop();
                TID[j].put_value(tmp_real);
                TID[j].put_assign();
                break;
            default:
                throw "Finish.";
        }
        ++index;
    }
    std::cout << std::endl << "Completed." << std::endl;
}