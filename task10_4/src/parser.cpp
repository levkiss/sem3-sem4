#include "../include/parser.h"

Parser::Parser(const char * program) : tknzr(program), _RPN(){} //Constructor

void Parser::analyze() //Analyze program
{
    while (!st_int.empty())
        st_int.pop();
    while (!st_lex.empty())
        st_lex.pop();
    get_lex();
    START();    
    _RPN.print();
}

void Parser::get_lex() //Get lexem from tknzr
{
    tknzr.get_lex(cur_lex);
    cur_type = cur_lex.get_type();
    cur_val = cur_lex.get_value();
}

void Parser::START() //Start of program
{
    if (cur_type == LEX_PROGRAM)
        get_lex();
    else
        throw cur_lex;
    
    if (cur_type == LEX_VAR)
    {
        VARPART();
        if (cur_type == LEX_SEMICOLON)
            get_lex();
        else
            throw expected_error(LEX_SEMICOLON, cur_lex.str_lex, cur_lex.num_lex);
    }
    MAIN();
    if (cur_type != LEX_EOF)
        throw expected_error(LEX_EOF, cur_lex.str_lex, cur_lex.num_lex);
}

void Parser::VARPART() //Var part of program
{
    get_lex();
    DESCR();
    while (cur_type == LEX_COMMA){
        get_lex();
        DESCR();
    }
}

void Parser::DESCR() //Desciption of variables
{
    if (cur_type != LEX_ID)
        throw expected_error(LEX_ID, cur_lex.str_lex, cur_lex.num_lex);
    else
    {
        st_int.push (cur_val);
        get_lex();
        while (cur_type == LEX_COMMA)
        {
            get_lex();
            if (cur_type != LEX_ID)
                throw expected_error(LEX_ID, cur_lex.str_lex, cur_lex.num_lex);
            else
            {
                st_int.push (cur_val);
                get_lex();
            }
        }
        if (cur_type != LEX_COLON)
            throw expected_error(LEX_COLON, cur_lex.str_lex, cur_lex.num_lex);
        else
        {
            get_lex();
            if (cur_type == LEX_INT)
            {
                check_declared(LEX_INT);
                get_lex();
            }
            else
                if (cur_type == LEX_BOOL)
                {
                    check_declared(LEX_BOOL);
                    get_lex();
                }
                else
                    if (cur_type == LEX_REAL)
                    {
                        check_declared(LEX_REAL);
                        get_lex();
                    }
            
                    else
                        throw expected_error(LEX_NUM, cur_lex.str_lex, cur_lex.num_lex);
        }
    }
}

void Parser::MAIN() //Main part of program
{
    if (cur_type == LEX_BEGIN)
    {
        get_lex();
        if (cur_type != LEX_END)
        {
            OP();
            while (cur_type == LEX_SEMICOLON)
            {
                get_lex();
                if (cur_type != LEX_END)
                    OP();
                else
                    break;
            }
        }
        
        if (cur_type == LEX_END)
        {
            get_lex();
        }
        else
        {
            throw expected_error(LEX_END, cur_lex.str_lex, cur_lex.num_lex);
        }
    }
    else
        throw expected_error(LEX_BEGIN, cur_lex.str_lex, cur_lex.num_lex);
}

void Parser::OP()
{
    int p0, p1, p2, p3, p4, p5;
    if (cur_type == LEX_ID)
    {
        check_not_declared();
        _RPN.add_lexem(Lexem(RPN_ADDRESS, cur_val));
        get_lex();
        if (cur_type == LEX_ASSIGN)
        {
            get_lex();
            FULLEXPR();
            check_eq();
            _RPN.add_lexem(Lexem(LEX_ASSIGN));
        }
        else
            throw expected_error(LEX_ASSIGN, cur_lex.str_lex, cur_lex.num_lex);
    }
    else
        if (cur_type == LEX_IF)
        {
            get_lex();
            FULLEXPR();
            check_bool();
            p2 = _RPN.get_free();
            _RPN.skip_lexem();
            _RPN.add_lexem(Lexem(RPN_FGO));
            if (cur_type == LEX_THEN)
            {
                get_lex();
                OP();
                p3 = _RPN.get_free();
                _RPN.skip_lexem();
                _RPN.add_lexem(Lexem(RPN_GO));
                _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), p2);
                if (cur_type == LEX_ELSE)
                {
                    get_lex();
                    OP();
                    _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), p3);
                }
                else
                {
                    _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), p3);
                }
            }
            else
                throw expected_error(LEX_THEN, cur_lex.str_lex, cur_lex.num_lex);
        }
        else
            if (cur_type == LEX_WHILE)
            {
                st_break.push((int)0);
                st_continue.push((int)0);
                
                p0 = _RPN.get_free();
                cycles++;
                get_lex();
                FULLEXPR();
                check_bool();
                p1 = _RPN.get_free();
                _RPN.skip_lexem();
                _RPN.add_lexem(Lexem(RPN_FGO));
                if (cur_type == LEX_DO)
                {
                    get_lex();
                    OP();
                    _RPN.add_lexem(Lexem(RPN_LABEL, p0));
                    _RPN.add_lexem(Lexem(RPN_GO));
                    _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), p1);
                    
                    int k;
                    k = st_break.top();
                    st_break.pop();
                    for(int i = 0; i < k; i++)
                    {
                        int pp;
                        pp = prev_label.top();
                        prev_label.pop();
                        _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), pp);
                    }
                    int j;
                    j = st_continue.top();
                    st_continue.pop();
                    for (int i = 0; i < j; i++)
                    {
                        int ppp;
                        ppp = next_label.top();
                        next_label.pop();
                        _RPN.add_lexem(Lexem(RPN_LABEL, p0), ppp);
                    }
                }
                else
                    throw expected_error(LEX_DO, cur_lex.str_lex, cur_lex.num_lex);
                cycles--;
            }
            else
                if (cur_type == LEX_FOR)
                {
                    st_break.push((int)0);
                    st_continue.push((int)0);
                    
                    cycles++;
                    get_lex();
                    Lexem A = cur_lex;
                    
                    if (cur_type == LEX_ID)
                    {
                        check_not_declared();
                        _RPN.add_lexem(Lexem(RPN_ADDRESS, cur_val));
                        get_lex();
                        if (cur_type == LEX_ASSIGN)
                        {
                            get_lex();
                            FULLEXPR();
                            check_eq();
                            _RPN.add_lexem(Lexem(LEX_ASSIGN));
                        }
                        else
                            throw expected_error(LEX_ASSIGN, cur_lex.str_lex, cur_lex.num_lex);
                    }
                    else
                        throw expected_error(LEX_ID, cur_lex.str_lex, cur_lex.num_lex);

                    if (cur_type == LEX_TO)
                    {
                        get_lex();
                    }
                    else
                        throw expected_error(LEX_TO, cur_lex.str_lex, cur_lex.num_lex);
                    
                    p0 = _RPN.get_free();
                    _RPN.add_lexem(A);
                    FULLEXPR();
                    _RPN.add_lexem(Lexem(LEX_LSS));
                    
                    if (cur_type == LEX_DO)
                    {
                        p1 = _RPN.get_free();
                        _RPN.skip_lexem();
                        _RPN.add_lexem(Lexem(RPN_FGO));
                        
                        p4 = _RPN.get_free();
                        _RPN.skip_lexem();
                        _RPN.add_lexem(Lexem(RPN_GO));
                        
                        p5 = _RPN.get_free();
                        _RPN.add_lexem(Lexem(RPN_ADDRESS, A.get_value()));
                        _RPN.add_lexem(A);
                        _RPN.add_lexem(Lexem(LEX_NUM,1));
                        _RPN.add_lexem(Lexem(LEX_PLUS));
                        _RPN.add_lexem(Lexem(LEX_ASSIGN));
                        
                        _RPN.add_lexem(Lexem(RPN_LABEL, p0));
                        _RPN.add_lexem(Lexem(RPN_GO));
                        
                        _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), p4);
                        
                        get_lex();
                        OP();
                    }
                    else
                        throw expected_error(LEX_DO, cur_lex.str_lex, cur_lex.num_lex);
                    
                    _RPN.add_lexem(Lexem(RPN_LABEL, p5));
                    _RPN.add_lexem(Lexem(RPN_GO));
                    
                    _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), p1);

                    int k;
                    k = st_break.top();
                    st_break.pop();
                    for(int i = 0; i < k; i++)
                    {
                        int pp;
                        pp = prev_label.top();
                        prev_label.pop();
                        _RPN.add_lexem(Lexem(RPN_LABEL, _RPN.get_free()), pp);
                    }

                    int j;
                    j = st_continue.top();
                    st_continue.pop();
                    for (int i = 0; i < j; i++)
                    {
                        int ppp;
                        ppp = next_label.top();
                        next_label.pop();
                        _RPN.add_lexem(Lexem(RPN_LABEL, p0), ppp);
                    }
                    
                    cycles--;
                }
                else
                    if (cur_type == LEX_BEGIN)
                    {
                        MAIN();
                    }
                    else
                        if (cur_type == LEX_READ)
                        {
                            get_lex();
                            if (cur_type == LEX_LPAREN)
                            {
                                get_lex();
                                if (cur_type == LEX_ID)
                                {
                                    check_read();
                                    _RPN.add_lexem(Lexem(RPN_ADDRESS, cur_val));
                                    get_lex();
                                }
                                else
                                    throw expected_error(LEX_ID, cur_lex.str_lex, cur_lex.num_lex);
                                if (cur_type == LEX_RPAREN)
                                {
                                    get_lex();
                                    _RPN.add_lexem(Lexem(LEX_READ));
                                }
                                else
                                    throw expected_error(LEX_RPAREN, cur_lex.str_lex, cur_lex.num_lex);
                            }
                            else
                                throw expected_error(LEX_LPAREN, cur_lex.str_lex, cur_lex.num_lex);
                        }
                        else
                            if (cur_type == LEX_WRITE)
                            {
                                get_lex();
                                if (cur_type == LEX_LPAREN)
                                {
                                    get_lex();
                                    FULLEXPR();
                                    if (cur_type == LEX_RPAREN)
                                    {
                                        get_lex();
                                        _RPN.add_lexem(Lexem(LEX_WRITE));
                                    }
                                    else
                                        throw expected_error(LEX_RPAREN, cur_lex.str_lex, cur_lex.num_lex);
                                }
                                else
                                    throw expected_error(LEX_LPAREN, cur_lex.str_lex, cur_lex.num_lex);
                            }
                            else
                                if (cur_type == LEX_BREAK || cur_type == LEX_CONTINUE)
                                {
                                    if (cycles <= 0)
                                        throw cur_lex;
                                    
                                    if (cur_type == LEX_BREAK)
                                    {
                                        int k;
                                        k = st_break.top();
                                        ++k;
                                        st_break.pop();
                                        st_break.push(k);
                                        prev_label.push(_RPN.get_free());
                                        _RPN.skip_lexem();
                                        _RPN.add_lexem(Lexem(RPN_GO));
                                        
                                    }
                                    else
                                    {
                                        int j;
                                        j = st_continue.top();
                                        ++j;
                                        st_continue.pop();
                                        st_continue.push(j);
                                        next_label.push(_RPN.get_free());
                                        _RPN.skip_lexem();
                                        _RPN.add_lexem(Lexem(RPN_GO));
                                    }
                                    get_lex();
                                }
                                else
                                    if(cur_type != LEX_SEMICOLON)
                                        throw expected_error(LEX_SEMICOLON, cur_lex.str_lex, cur_lex.num_lex);
}

void Parser::FULLEXPR() //Full expression
{
    EXPR();
    if (cur_type == LEX_EQ || cur_type == LEX_LSS || cur_type == LEX_GTR || cur_type == LEX_LEQ || cur_type == LEX_GEQ || cur_type == LEX_NEQ)
    {
        st_lex.push(cur_type);
        get_lex();
        EXPR();
        check_ops();
    }
}

void Parser::EXPR() //Expression
{
    TERM();
    while (cur_type == LEX_PLUS || cur_type == LEX_MINUS || cur_type == LEX_OR)
    {
        st_lex.push(cur_type);
        get_lex();
        TERM();
        check_ops();
    }
}

void Parser::TERM() //Term
{
    FACTOR();
    while (cur_type == LEX_MULT || cur_type == LEX_SLASH || cur_type == LEX_AND)
    {
        st_lex.push (cur_type);
        get_lex();
        FACTOR();
        check_ops();
    }
}

void Parser::FACTOR() //Factor
{
    if (cur_type == LEX_ID)
    {
        check_not_declared();
        _RPN.add_lexem(Lexem(LEX_ID, cur_val));
        get_lex();
    }
    else if (cur_type == LEX_NUM)
    {
        st_lex.push(LEX_INT);
        _RPN.add_lexem(cur_lex);
        get_lex();
    }
    else if (cur_type == LEX_REAL)
    {
        st_lex.push(LEX_REAL);
        _RPN.add_lexem(cur_lex);
        get_lex();
    }
    else if (cur_type == LEX_TRUE)
    {
        st_lex.push (LEX_BOOL);
        _RPN.add_lexem(Lexem(LEX_TRUE, 1));
        get_lex();
    }
    else if (cur_type == LEX_FALSE)
    {
        st_lex.push(LEX_BOOL);
        _RPN.add_lexem(Lexem(LEX_FALSE, 0));
        get_lex();
    }
    else if (cur_type == LEX_NOT)
    {
        get_lex();
        FACTOR();
        check_not();
    }
    else if (cur_type == LEX_LPAREN)
    {
        get_lex();
        FULLEXPR();
        if (cur_type == LEX_RPAREN)
            get_lex();
        else
            throw expected_error(LEX_RPAREN, cur_lex.str_lex, cur_lex.num_lex);
    }
    else
        throw expected_error(LEX_LPAREN, cur_lex.str_lex, cur_lex.num_lex);
}

void Parser::check_declared(Lex_type type) //Check variable if declared
{
    int i;
    while (!st_int.empty())
    {
        i = st_int.top();
        st_int.pop();
        if (TID[i].get_declare())
            throw semantic_error("used twice", cur_lex.str_lex, cur_lex.num_lex);
        else
        {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}

void Parser::check_not_declared() //Check variable if not declared
{
    if (TID[cur_val].get_declare())
        st_lex.push (TID[cur_val].get_type());
    else
        throw semantic_error("not declared", cur_lex.str_lex, cur_lex.num_lex);
}

void Parser::check_ops() //Check operands types in expression
{
    Lex_type t1, t2, op, t = LEX_INT, r = LEX_BOOL;
    t2 = st_lex.top();
    st_lex.pop(); 
    op = st_lex.top();
    st_lex.pop(); 
    t1 = st_lex.top();
    st_lex.pop();
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH)
        r = LEX_INT;
    if (op == LEX_OR || op == LEX_AND)
        t = LEX_BOOL;
    if (t1 == t2  &&  t1 == t)
    {
        st_lex.push(r);
        _RPN.add_lexem(Lexem(op));
        return;
    }
    t = LEX_REAL;
    r = LEX_BOOL;
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH)
        r = LEX_REAL;
    if (op == LEX_OR || op == LEX_AND)
        t = LEX_BOOL;
    if (t1 == t2  &&  t1 == t)
        st_lex.push(r);
    else
        throw semantic_error("wrong types are in operation", cur_lex.str_lex, cur_lex.num_lex);
    _RPN.add_lexem(Lexem(op));
}

void Parser::check_not() //Check operand with "not"
{
    if (st_lex.top() != LEX_BOOL)
        throw semantic_error("wrong type is in not", cur_lex.str_lex, cur_lex.num_lex);
    else
    {
        st_lex.pop();
        st_lex.push(LEX_BOOL);
        _RPN.add_lexem(Lexem(LEX_NOT));
    }
}

void Parser::check_eq() //Check operands type in assignment
{
    Lex_type i = st_lex.top();
    st_lex.pop();
    if (i != st_lex.top())
        throw semantic_error("wrong types are in :=", cur_lex.str_lex, cur_lex.num_lex);
    st_lex.pop();
}

void Parser::check_bool() //Check boolean in assignment
{
    if (st_lex.top() != LEX_BOOL)
        throw semantic_error("expression is not boolean", cur_lex.str_lex, cur_lex.num_lex);
    st_lex.pop();
}

void Parser::check_read() //Check variable if not declared in read
{
    if (!TID [cur_val].get_declare())
        throw semantic_error("not declared", cur_lex.str_lex, cur_lex.num_lex);
}