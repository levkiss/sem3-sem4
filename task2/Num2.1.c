#include <stdio.h>
double koren;
double epselon;
double iks;

double Nuton(double iksa, double eps) 
{
	double x_i0=1;
    double x_i1;
    x_i1=0.5*(x_i0+iksa/x_i0);
    while ((x_i0 - x_i1)>=eps || (x_i0 - x_i1)<=-eps){
        x_i0 = x_i1;
        x_i1 = 0.5*(x_i0+ iksa/x_i0);
        //printf("%lf", x_i0-x_i1);
    } 
    return(x_i1);
}

int main()
{   
    printf("%s","Введите число эпсилон ");
    scanf("%lf\n", &epselon);  
    while (scanf("%lf/n", &iks) != EOF) {
        //printf("%s","Введите икс ");
        //scanf("%lf/n", &iks);
        if (iks > 0) {
            koren = Nuton(iks, epselon);
            printf("%s %.10g\n", "Корень из введенного числа равен ", koren);
        }
        else{
            printf("%s\n", "К сожалению корень из введенного числа вычислить нельзя, т.к оно отрицательное");
        }
    }
   
    getchar();
    return(0);
}