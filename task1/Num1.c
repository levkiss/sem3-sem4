#include <stdio.h>
short a;
int b;
long c;
float d;
double e;
long double f;


int main()
{
    printf("%lu %s\n", sizeof(a), "байта(ов) занимает тип short");  
    printf("%lu %s\n", sizeof(b), "байта(ов) занимает тип int"); 
    printf("%lu %s\n", sizeof(c), "байта(ов) занимает тип long");
    printf("%lu %s\n", sizeof(d), "байта(ов) занимает тип float");
    printf("%lu %s\n", sizeof(e), "байта(ов) занимает тип double");
    printf("%lu %s\n", sizeof(f), "байта(ов) занимает тип long double");   

    getchar();
    return(0);
}