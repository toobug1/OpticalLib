#ifndef TMATIX_HPP
#define TMATIX_HPP

#include "tmatrix.h"

template<int N, int M, typename T>
TMatrixBase<M, N, T> TMatrixBase<N, M, T>::transposed() const
{
    TMatrixBase<M, N, T> matrix;

    for (int row = 0; row < M; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            matrix(col, row) = (*this)(row, col);
        }
    }
    return matrix;
}

template<int N, int M, typename T>
TMatrixBase<N, M, T>& TMatrixBase<N, M, T>::set_col(int col, T value)
{
    for (int row = 0; row < M; ++row)
    {
        (*this)(row, col) = value;
    }
    return *this;
}

template<int N, int M, typename T>
TMatrixBase<N, M, T>& TMatrixBase<N, M, T>::set_row(int row, T value)
{
    for (int col = 0; col < N; ++col)
    {
        (*this)(row, col) = value;
    }
    return *this;
}

template<int N, int M, typename T>
TMatrixBase<N, M, T>& TMatrixBase<N, M, T>::set_diag(T value)
{
    for (int row = 0, col = 0; (row < M && col < N); ++row, ++col)
    {
        (*this)(row, col) = value;
    }
    return *this;
}

template<int N, typename T>
T TSquareMatrix<N, T>::determinant() const
{
    int k;
    T sum(0);
    // at first row to expand matrix.
    for (int col = 0; col < N; ++col)
    {
        k = (col%2)==0? 1 : -1;

        TSquareMatrix<N - 1, T> matrixCofactor = this->getCofactorMatrix(0, col);
        T valCurrent = (*this)(0, col);
        sum = sum + k * valCurrent * matrixCofactor.determinant();
    }
    return sum;
}

template<int N, typename T>
TSquareMatrix<N - 1, T> TSquareMatrix<N, T>::getCofactorMatrix(int row, int col) const
{
    T val[(N - 1) * (N - 1)];
    int index = 0;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if ( (i == row) || (j == col))
            {
                continue;
            }
            val[index++] = (*this)(i, j);
        }
    }

    TSquareMatrix<N - 1, T> matrix(val);

    return matrix;
}

template<int N, typename T>
T TSquareMatrix<N, T>::cofactor(int row, int col) const
{
    TSquareMatrix<N - 1, T> matrix = this->getCofactorMatrix(row, col);

    int k = ((row+col)%2)==0? 1 : -1;
    return k*matrix.determinant();
}

template<int N, typename T>
TSquareMatrix<N, T>& TSquareMatrix<N, T>::transpose()
{
    TSquareMatrix<N, T> matrix;

    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            matrix(col, row) = (*this)(row, col);
        }
    }
    (*this) = matrix;
    return *this;
}

template<int N, typename T>
TSquareMatrix<N, T> TSquareMatrix<N, T>::adjugate() const
{
     TSquareMatrix<N, T> matrix;
     for (int row = 0; row < N; ++row)
     {
         for (int col = 0; col < N; ++col)
         {
             matrix(row, col) = this->cofactor(row, col);
         }
     }
     return matrix.transpose();
}

template<int N, typename T>
TSquareMatrix<N, T> TSquareMatrix<N, T>::inverse() const
{
    TSquareMatrix<N, T> matrix_adj = this->adjugate();
    QGenericMatrix<N, N, T> matrixTemp = matrix_adj / this->determinant();
    TSquareMatrix<N, T> matrix;

    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            matrix(row, col) = matrixTemp(row, col);
        }
    }
    return matrix;
}



template<int N, int M, typename T>
std::ostream& operator<<(std::ostream& os, const QGenericMatrix<N, M, T>& matrix)
{
    for(int row = 0; row < M; ++row)
    {
        for(int col = 0; col < N; ++col)
        {
            os << matrix(row, col) << " ";
        }
        os << std::endl;
    }

    return os;
}



#endif // TMatrixBase_HPP

