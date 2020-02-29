#include <iostream>
#include <exception>
#include <cstring>
#include <cstdlib>
#include "matrix.h"

//Конструктор по умолчанию
matrix::matrix() {
    matr = NULL;
    a = -1;
    b = -1;
}
//1
matrix::matrix(int n, int m){
    a=n;
    b=m;
    if ((n <= 0) || (m <= 0))
        throw error_catcher("\"Constructor\" Incorrect input");
    matr = new double *[a];
    for (int i = 0; i < a; i++) {
        matr[i] = new double[b];
        for (int j = 0; j < b; j++)
            matr[i][j] = 0.0;
    }
}
//2
matrix::matrix(double d){
    a=1;
    b=1;
    matr = new double *[a];
    for (int i = 0; i < a; i++) {
        matr[i] = new double[b];
        for (int j = 0; j < b; j++)
            matr[i][j] = d;
    }
}
//3
matrix::matrix(double *d, int m): a(1), b(m) {
    if ((d == NULL) || (d[0] == '\0'))
        throw error_catcher("\"Constructor\" error. Incorrect input");
    matr = new double *[a];
    matr[0] = new double[b];
    for (int i = 0; i < m; i++)
        matr[0][i] = d[i];
}
//4
matrix::matrix(int n, double* d): a(n), b(1) {
    if ((d == NULL) || (d[0] == '\0'))
        throw error_catcher("\"Constructor\" error. Incorrect input");
    matr = new double *[a];
    for (int i = 0; i < n; i++) {
        matr[i] = new double[b];
        matr[i][0] = d[i];
    }
}
//5
matrix::matrix(char *s) {
    if ((s == NULL) || (s[0] == '\0'))
        throw error_catcher("\"Constructor\" error. Incorrect input");
    int len = (int)strlen(s);
    int w = 1, h = 0;
    
    for (int i = 1; i < len; i++)
        if (s[i] == '{') h++;
    for (int i = 1; s[i] != '}'; i++)
        if (s[i] == ',') w++;
    
    if (h == 0)
        throw error_catcher("\"Constructor\" error. Incorrect input");
    a = h;
    b = w;
    matr = new double *[h];
    for (int i = 0; i < h; i++)
        matr[i] = new double[w];
    
    bool in_digit = false;
    int i = 0, j = 0, tmp_index = 0;
    char tmp[100];
    for (int k = 1; k < len; k++) {
        if ((k == 0) && (s[k] != '{'))
             throw error_catcher("\"Constructor\" error. Incorrect input");
        else if (k == 0)
            continue;
        if (not (((s[k] >= '0') && (s[k] <= '9')) || (s[k] == '{') || (s[k] == '}') || (s[k] == ',') || (s[k] == '.') || (s[k] == 'E') || (s[k] == 'e') || (s[k] == '+') || (s[k] == '-')))
            throw error_catcher("\"Constructor\" error. Incorrect symbol in input");
        if (s[k] == '{') {
            if (in_digit)
                throw error_catcher("\"Constructor\" error. Incorrect input");
            j = 0;
            continue;
        }
        if (((s[k] >= '0') && (s[k] <= '9')) || (s[k] == '.') || (s[k] == 'E') || (s[k] == 'e') || (s[k] == '+') || (s[k] == '-')) {
            if (not in_digit) in_digit = true;
            tmp[tmp_index++] = s[k];
            continue;
        }
        if (s[k] == ',') {
            if (not in_digit)
                throw error_catcher("\"Constructor\" error. Incorrect input");
            in_digit = false;
            matr[i][j] = str2double(tmp);
            for (int tmp_i = 0; tmp_i < 100; tmp_i++)
                tmp[tmp_i] = '\0';
            tmp_index = 0;
            j++;
            continue;
        }
        if (s[k] == '}') {
            if ((not in_digit) && (k != len - 1))
                throw error_catcher("\"Constructor\" error. Incorrect input");
            if (in_digit) {
                matr[i][j] = str2double(tmp);
                for (int tmp_i = 0; tmp_i < 100; tmp_i++)
                    tmp[tmp_i] = '\0';
                tmp_index = 0;
                k++;
                in_digit = false;
                i++;
                if (j != w - 1)
                    throw error_catcher("\"Constructor\" error. Incorrect input");
            }
        }
    }
}
//6
matrix matrix::identity(int n) {
    if (n <= 0)
        throw error_catcher("\"identity\" error. Incorrect input");
    matrix x(n,n);
    for (int i = 0; i < n; i++)
        x.set(i, i, 1);
    return x;
}
//7
matrix matrix::diagonal(double *vals, int n) {
    if ((vals == NULL) || (vals[0] == '\0'))
        throw error_catcher("\"Constructor\" error. Incorrect input");
    if (n <= 0)
        throw error_catcher("\"diagonal\" error. Incorrect input");
    matrix x(n,n);
    for (int i = 0; i < n; i++) {
        if (vals[i] == '\0')
            throw error_catcher("\"diagonal\" error. Incorrect input");
        x.set(i, i, vals[i]);
    }
    return x;
}
//9
int matrix::rows(){
    return a;
}
//10
int matrix::columns(){
    return b;
}
//11
void matrix::set(int i, int j, double val) {
    if (matr == NULL)
         throw error_catcher("\"set\" error. Empty matrix");
    if ((i >= a) || (j >= b))
        throw error_catcher("\"set\" error. Incorrect index");
    matr[i][j] = val;
}
//12, 13
matrix matrix::operator[](int i) {
    if (i < 0)
        throw error_catcher("\"operator []\" error. Incorrect input");
    if (a > i) {
        matrix x(matr[i], b);
        return x;
    }
    else if (b > i) {
        double tmp[a];
        for (int j = 0; j < a; j++)
            tmp[j] = matr[j][i];
        matrix x(a, tmp);
        return x;
    }
    else throw error_catcher("\"operator []\" error. Incorrect index");
}
//14
matrix operator*(matrix &x, double d) {
    if (x.matr == NULL)
        throw error_catcher("\"operator *\" error. Empty matrix");
    matrix res(x.a, x.b);
    for (int i = 0; i < x.a; i++)
        for (int j = 0; j < x.b; j++)
            res.set(i, j, x.matr[i][j] * d);
    return res;
}
//14
matrix& matrix::operator*=(double d) {
    if (matr == NULL)
        throw error_catcher("\"operator *=\" error. Empty matrix");
    for (int i = 0; i < a; i++)
        for (int j = 0; j < b; j++)
            matr[i][j] *= d;
    return *this;
}
//15
std::ostream& operator<<(std::ostream &out, matrix &x) {
    if (x.matr == NULL)
        out << "NULL" << std::endl;
    for (int i = 0; i < x.a; i++) {
        for (int j = 0; j < x.b; j++) {
            out << " ";
            out.width(5);
            out << x.matr[i][j];
        }
        out << std::endl;
    }
    return out;
}
//Операция присваивания
matrix &matrix::operator=(const matrix &x) {
    a = x.a;
    b = x.b;
    if (matr != NULL) {
        for (int i = 0; i < a; i++)
            delete[] matr[i];
        delete[] matr;
    }
    if (x.matr == NULL)
        matr = NULL;
    else {
        matr = new double *[a];
        for (int i = 0; i < a; i++) {
            matr[i] = new double[b];
            for (int j = 0; j < b; j++)
                matr[i][j] = x.matr[i][j];
        }
    }
    return *this;
}
//Конструктор копирования
matrix::matrix(const matrix &x) {
    *this = x;
}
//Деструктор
matrix::~matrix(){
    for (int i = 0; i < a; i++)
        delete[] matr[i];
    delete[] matr;
}
// Вспомогательный метод для 5
double matrix::str2double(char *s){
    double res = 0.0;
    int i = 0;
    int e = 0;
    while ((*(s + i) != '\0') && (*(s + i) >= '0') && (*(s + i) <= '9')) {
        res = res * 10.0 + *(s + i) - '0';
        i++;
    }
    if (*(s + i) == '.'){
        i++;
        while ((*(s + i) != '\0') && (*(s + i) >= '0') && (*(s + i) <= '9')) {
            res = res * 10.0 + *(s + i) - '0';
            e--;
            i++;
        }
    }
    if ((*(s + i) == 'e') || (*(s + i) == 'E')) {
        int k = 0;
        i++;
        if (*(s + i) == '-') {
            i++;
            while ((*(s + i) != '\0') && (*(s + i) >= '0') && (*(s + i) <= '9')) {
                k = k * 10.0 + *(s + i) - '0';
                i++;
            }
            e -= k;
        }
        else {
            if (*(s + i) == '+') i++;
            while ((*(s + i) != '\0') && (*(s + i) >= '0') && (*(s + i) <= '9')) {
                k = k * 10.0 + *(s + i) - '0';
                i++;
            }
            e += k;
        }
    }
    while (e > 0) {
        res *= 10.0;
        e--;
    }
    while (e < 0) {
        res *= 0.1;
        e++;
    }
    return res;
}