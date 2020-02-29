#include <stdio.h>
#include <time.h>

int num;


int fib_rek(int n){
	if (n==0 || n==1){
		return n;
	}
	else {
		return fib_rek(n-1)+fib_rek(n-2);
	}
}

int fib_iter(int n){
	int a[n];
	a[0] = 0;
    a[1] = 1;
    for(int i = 2; i <= n; i++){
        a[i] = a[i - 1] + a[i - 2];
	}
	return a[n];
}


int main(){
	time_t begin, end;
	float del;
	printf("%s\n", "Enter numbers ");
	while (scanf("%d", &num) != EOF){
		begin = time(NULL);
		printf("%s %d\n","Rekurs Fib= ",fib_rek(num));
		end = time(NULL);
		del = difftime(end, begin);
		printf("%s %d\n","Iter Fib = ", fib_iter(num));
		begin = time(NULL);
		del -= difftime(begin, end);
		printf("%f", del);


	}


	getchar();
	return(0);
}