#ifndef matrix_h
#define matrix_h

class matrix {
private:
    int a, b;
    double **matr;
    double str2double(char *s);                     // Вспомогательный метод для строкового представления
public:
    matrix();                                       //Конструктор по умолчанию
    matrix(int n, int m);                           //матрица размера м на н наполненная нулями
    matrix(double d);                               //матрица 1 на 1 с данным элементом
    matrix(double*, int m);                         //матрица - строка из массива длинны м
    matrix(int n, double*);                         //матрица - столбец
    matrix(char*);                                  //из строкового предстваления
    matrix(const matrix &x);                        //Конструктор копирования
    static matrix identity(int n);                  //единичнвая матрица
    static matrix diagonal(double *vals, int n);    //диагональная матрица с заданными эл на диагонали
    int rows();                                     //число строк
    int columns();                                  //число столбцов
    void set(int i, int j, double val);             // просвоить значение элементу
    matrix operator[](int i);                       //
    friend matrix operator*(matrix &x, double d);   //14
    matrix& operator*=(double d);                   //14
    friend std::ostream &operator<<(std::ostream &out, matrix &x);//перегрузка оператора  <<
    matrix& operator=(const matrix& x);             //операция присваивания
    ~matrix();                                      //Деструктор
};

class error_catcher : public std::exception {
private:
    char * error_message;
public:
    error_catcher(const char *s){
        error_message = new char[strlen(s)];
        strcpy(error_message, s);
    }
    virtual const char* what() const _NOEXCEPT {return error_message;};
};

#endif