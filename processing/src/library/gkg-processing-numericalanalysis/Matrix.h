#ifndef _gkg_processing_numericalanalysis_Matrix_h_
#define _gkg_processing_numericalanalysis_Matrix_h_


#include <iostream>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <vector>


namespace gkg
{


class MatrixImplementation;
class Vector;
class CMatrix;
class CVector;


class Matrix
{

  public:

    Matrix( int32_t size1 = 1, int32_t size2 = 1 );
    Matrix( const Matrix& other );
    virtual ~Matrix();

    Matrix& operator=( const Matrix& other );

    void reallocate( int32_t size1, int32_t size2 );

    int32_t getSize1() const;
    int32_t getSize2() const;

    const double& operator()( int32_t i, int32_t j ) const; 
    double& operator()( int32_t i, int32_t j ); 

    bool isSquare() const;

    void setZero();
    void setIdentity();
    void setRow( int32_t s1, const Vector& row );
    void setColumn( int32_t s2, const Vector& column );
    void fill( const double& value );
    void fill( int32_t offset1,
               int32_t offset2,
               int32_t size1,
               int32_t size2,
               const double& value );
    void fill( int32_t offset1,
               int32_t offset2,
               const Matrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void setDiagonal( const double& value );
    void transpose();

    double getTrace() const;
    double getDeterminant() const;
    Matrix getTransposition() const;
    Matrix getComposition( const Matrix& other ) const;
    Vector getComposition( const Vector& other ) const;
    CMatrix getComposition( const CMatrix& other ) const;
    CVector getComposition( const CVector& other ) const;
    Vector toVector() const;
    CVector toCVector() const;

    Vector getRow( int32_t i ) const;
    Vector getColumn( int32_t j ) const;
    Vector getDiagonal() const;

    void getRows( const std::vector< int32_t >& rowIndices,
                  std::vector< Vector >& rows ) const;
    void getColumns( const std::vector< int32_t >& columnIndices,
                     std::vector< gkg::Vector >& columns ) const;

    double getSumOfValues() const;


    MatrixImplementation* getImplementation() const;

  protected:

    MatrixImplementation* _matrixImplementation;

};


}


gkg::Matrix 
operator + ( const gkg::Matrix& thing );

gkg::Matrix 
operator - ( const gkg::Matrix& thing );

gkg::Matrix
operator + ( const gkg::Matrix& thing1,
             const gkg::Matrix& thing2 );

gkg::CMatrix
operator + ( const gkg::Matrix& thing1,
             const gkg::CMatrix& thing2 );

gkg::Matrix
operator - ( const gkg::Matrix& thing1,
             const gkg::Matrix& thing2 );

gkg::CMatrix
operator - ( const gkg::Matrix& thing1,
             const gkg::CMatrix& thing2 );

gkg::Matrix
operator * ( const gkg::Matrix& thing1,
             const gkg::Matrix& thing2 );

gkg::Vector
operator * ( const gkg::Matrix& thing1,
             const gkg::Vector& thing2 );

gkg::CMatrix
operator * ( const gkg::Matrix& thing1,
             const gkg::CMatrix& thing2 );

gkg::CVector
operator * ( const gkg::Matrix& thing1,
             const gkg::CVector& thing2 );

gkg::Matrix
operator * ( const gkg::Matrix& thing1,
             const double& thing2 );

gkg::CMatrix
operator * ( const gkg::Matrix& thing1,
             const std::complex< double >& thing2 );

gkg::Matrix
operator / ( const gkg::Matrix& thing1,
             const double& thing2 );

gkg::CMatrix
operator / ( const gkg::Matrix& thing1,
             const std::complex< double >& thing2 );


namespace std
{

std::ostream& operator << ( ostream& os, const gkg::Matrix& thing );
bool operator == ( const gkg::Matrix& thing1,
                   const gkg::Matrix& thing2 );

}


#endif
