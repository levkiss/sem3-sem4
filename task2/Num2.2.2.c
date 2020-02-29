#include <stdio.h>
double sum;
float x,an,n;
float proizv;

int main(){
	//scanf("%f",&n);
	scanf("%f",&x);
	scanf("%lf",&sum);
	proizv = 0;


	while (scanf("%f",&an) != EOF){
		proizv = proizv*x + sum;
		sum = sum*x + an;


	}
	printf("%s%lf\n", "Value = ",sum);
	//printf("%lf\n", sumd);

	printf("%s%lf\n", "Proizv = ",proizv);

	getchar();
	return 0;

}