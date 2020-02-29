#include "../include/table.h"

Table::Table(int max_size) //Constructor
{
    id = new Id[size = max_size];
    top = 1;
}

Table::~Table() //Destructor
{
    delete[] id;
}

int Table::put_id(const char *buf) //Put id into table
{
    for (int j = 1; j < top; j++)
        if (std::string(buf) == id[j].get_name())
            return j;
    id[top].put_name(buf);
    ++top;
    return top-1;
}

/*Take identification*/
Id& Table::operator[] (int i){return id[i];}
