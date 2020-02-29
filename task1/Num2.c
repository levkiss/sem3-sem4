#include <stdio.h>
short int rashir;
short int maloe;


short int eight_bits(short int maloe) 
{
	int sum = 0; 
	short int bit = 1;
	short int ost = 0;
	int size = sizeof(maloe)*8;

	sum += maloe;                //расширяем малое
	sum >>= size;                //сдвигаем число так чтобы левые size битов находились справа
    for (int i=0; i<size; i++) 
    {
        bit <<= i;
        ost += (sum & 1)*bit;     //в цикле переносим в ost расширение побитово
    	sum >>= 1;
        bit = 1;
    }
    return(ost);
}

int main()
{
    printf("%s","Давайте начнем! Введите любое малое число: ");    
    scanf("%hd", &maloe);                //вводим малое число
    rashir= eight_bits(maloe);
    printf("%s %hd\n","При расширении введенного числа, в знаковых битах находится ", rashir);
   
    getchar();
    return(0);
}