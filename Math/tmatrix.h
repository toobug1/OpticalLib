#ifndef TMATRIX_H
#define TMATRIX_H

#include <iostream>

#include <QtGui/QGenericMatrix>

template<int N, int M, typename T>
class TMatrix : public QGenericMatrix<N, M, T>
{
    // note: N is col, M is row
public:
    TMatrix() {}
    TMatrix(const T* data) : QGenericMatrix<N, M, T>(data){}
    ~TMatrix() {}

    TMatrix<M, N, T> transposed() const;
    TMatrix<N, M, T>& set_col(int col, T value);
    TMatrix<N, M, T>& set_row(int row, T value);
    TMatrix<N, M, T>& set_diag(T value);

};

template<int N, typename T>
class TSquareMatrix : public TMatrix<N, N, T>
{
public:
    TSquareMatrix() {}
    TSquareMatrix(const T* data) : TMatrix<N, N, T>(data) {}

public:
    T determinant() const;

    TSquareMatrix<N, T> inverse() const;
    TSquareMatrix<N, T> adjugate() const;
    TSquareMatrix<N, T>& transpose();

    T cofactor(int row, int col) const;   // get cofactor at [row,col]
    TSquareMatrix<N - 1, T> getCofactorMatrix(int row, int col) const;
};

template<typename T>
class TSquareMatrix<1, T>
{
    // to halt recursion
public:

   TSquareMatrix<1, T>() : m_data(0) {}
   TSquareMatrix<1, T>(const T* data) : m_data(data[0]) {}
   double determinant() const {return m_data;}
   T& operator ()(int row, int col) {return m_data;}
   const T& operator ()(int row, int col) const {return m_data;}
   T inverse() const {return 1/m_data;}
   T adjugate() const {return 1;}

private:
   T m_data;
};

template<int N, int M, typename T>
std::ostream& operator<<(std::ostream& os, const QGenericMatrix<N, M, T>& matrix);


#endif // TMATRIX_H
