#ifndef _gkg_processing_numericalanalysis_CVector_h_
#define _gkg_processing_numericalanalysis_CVector_h_


#include <iostream>
#include <complex>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


class CVectorImplementation;
class CMatrix;
class Matrix;
class Vector;


class CVector
{

  public:

    CVector( int32_t size = 1 );
    CVector( const CVector& other );
    CVector( const Vector& other );
    CVector( const std::vector< int32_t >& other );
    CVector( const std::vector< float >& other );
    CVector( const std::vector< double >& other );
    CVector( const std::vector< std::complex< float > >& other );
    CVector( const std::vector< std::complex<double > >& other );
    CVector( const Vector3d< float >& other );
    CVector( const Vector3d< double >& other );
    virtual ~CVector();

    CVector& operator=( const CVector& other );
    CVector& operator=( const std::vector< int32_t >& other );
    CVector& operator=( const std::vector< float >& other );
    CVector& operator=( const std::vector< double >& other );
    CVector& operator=( const Vector3d< float >& other );
    CVector& operator=( const Vector3d< double >& other );

    void reallocate( int size );

    int32_t getSize() const;

    const std::complex< double >& operator()( int32_t i ) const; 
    std::complex< double >& operator()( int32_t i ); 

    void setZero();
    void setBasis( int32_t i );
    void fill( const std::complex< double >& value );
    void conjugate();

    CVector getConjugate() const;
    CMatrix getDiagonal() const;
    std::complex< double > getComposition( const CVector& other ) const;
    std::complex< double > getComposition( const Vector& other ) const;
    Vector getMagnitude() const;
    CMatrix toCMatrix() const;
    gkg::Vector getRealPart() const;
    gkg::Vector getImaginaryPart() const;

    std::complex< double > getMean() const;

    CVectorImplementation* getImplementation() const;

  protected:

    CVectorImplementation* _cvectorImplementation;

};


}


gkg::CVector 
operator + ( const gkg::CVector& thing );

gkg::CVector 
operator - ( const gkg::CVector& thing );

gkg::CVector
operator + ( const gkg::CVector& thing1,
             const gkg::CVector& thing2 );

gkg::CVector
operator + ( const gkg::CVector& thing1,
             const gkg::Vector& thing2 );

gkg::CVector
operator - ( const gkg::CVector& thing1,
             const gkg::CVector& thing2 );

gkg::CVector
operator - ( const gkg::CVector& thing1,
             const gkg::Vector& thing2 );

gkg::CVector
operator * ( const gkg::CVector& thing1,
             const std::complex< double >& thing2 );

gkg::CVector
operator * ( const gkg::CVector& thing1,
             const double& thing2 );

gkg::CVector
operator * ( const gkg::CVector& thing1,
             const gkg::CMatrix& thing2 );

gkg::CVector
operator * ( const gkg::CVector& thing1,
             const gkg::Matrix& thing2 );

gkg::CVector
operator / ( const gkg::CVector& thing1,
             const std::complex< double >& thing2 );

gkg::CVector
operator / ( const gkg::CVector& thing1,
             const double& thing2 );


namespace std
{

std::ostream& operator << ( ostream& os, const gkg::CVector& thing );

}


#endif
