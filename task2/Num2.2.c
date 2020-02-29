#include <stdio.h>
double sum;
float x,an;

int main(){
	scanf("%f",&x);
	scanf("%lf",&sum);

	while (scanf("%f",&an) != EOF){
		sum = sum*x + an;

	}
	printf("%lf\n", sum);

	getchar();
	return 0;

}
