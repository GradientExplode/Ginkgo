#ifndef _gkg_processing_numericalanalysis_CMatrix_h_
#define _gkg_processing_numericalanalysis_CMatrix_h_


#include <iostream>
#include <complex>
#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


class CMatrixImplementation;
class CVector;
class Matrix;
class Vector;


class CMatrix
{

  public:

    CMatrix( int32_t size1 = 1, int32_t size2 = 1 );
    CMatrix( const CMatrix& other );
    CMatrix( const Matrix& other );
    virtual ~CMatrix();

    CMatrix& operator=( const CMatrix& other );

    void reallocate( int32_t size1, int32_t size2 );

    int32_t getSize1() const;
    int32_t getSize2() const;

    const std::complex< double >& operator()( int32_t i, int32_t j ) const; 
    std::complex< double >& operator()( int32_t i, int32_t j ); 

    bool isSquare() const;

    void setZero();
    void setIdentity();
    void setRow( int32_t s1, const CVector& row );
    void setColumn( int32_t s2, const CVector& column );
    void fill( const std::complex< double >& value );
    void fill( int32_t offset1,
               int32_t offset2,
               int32_t size1,
               int32_t size2,
               const std::complex< double >& value );
    void fill( int32_t offset1,
               int32_t offset2,
               const Matrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void fill( int32_t offset1,
               int32_t offset2,
               const CMatrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void transpose();
    void conjugate();
    void hermitianTranspose();

    std::complex< double > getTrace() const;
    CMatrix getTransposition() const;
    CMatrix getConjugate() const;
    CMatrix getHermitianTransposition() const;
    CMatrix getComposition( const CMatrix& other ) const;
    CVector getComposition( const CVector& other ) const;
    CMatrix getComposition( const Matrix& other ) const;
    CVector getComposition( const Vector& other ) const;
    CVector toCVector() const;
    Matrix getMagnitude() const;

    CVector getRow( int32_t i ) const;
    CVector getColumn( int32_t j ) const;
    CVector getDiagonal() const;

    void getRows( const std::vector< int32_t >& rowIndices,
                  std::vector< CVector >& rows ) const;
    void getColumns( const std::vector< int32_t >& columnIndices,
                     std::vector< gkg::CVector >& columns ) const;

    CMatrixImplementation* getImplementation() const;

  protected:

    CMatrixImplementation* _cmatrixImplementation;

};


}


gkg::CMatrix 
operator + ( const gkg::CMatrix& thing );

gkg::CMatrix 
operator - ( const gkg::CMatrix& thing );

gkg::CMatrix
operator + ( const gkg::CMatrix& thing1,
             const gkg::CMatrix& thing2 );

gkg::CMatrix
operator + ( const gkg::CMatrix& thing1,
             const gkg::Matrix& thing2 );

gkg::CMatrix
operator - ( const gkg::CMatrix& thing1,
             const gkg::CMatrix& thing2 );

gkg::CMatrix
operator - ( const gkg::CMatrix& thing1,
             const gkg::Matrix& thing2 );

gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const gkg::CMatrix& thing2 );

gkg::CVector
operator * ( const gkg::CMatrix& thing1,
             const gkg::CVector& thing2 );

gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const gkg::Matrix& thing2 );

gkg::CVector
operator * ( const gkg::CMatrix& thing1,
             const gkg::Vector& thing2 );

gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const std::complex< double >& thing2 );

gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const double& thing2 );

gkg::CMatrix
operator / ( const gkg::CMatrix& thing1,
             const std::complex< double >& thing2 );

gkg::CMatrix
operator / ( const gkg::CMatrix& thing1,
             const double& thing2 );


namespace std
{

std::ostream& operator << ( ostream& os, const gkg::CMatrix& thing );

}


#endif
