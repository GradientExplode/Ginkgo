#ifndef _gkg_processing_numericalanalysis_Vector_h_
#define _gkg_processing_numericalanalysis_Vector_h_


#include <iostream>
#include <complex>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


class VectorImplementation;
class Matrix;
class CVector;

class Vector
{

  public:

    Vector( int32_t size = 1 );
    Vector( const Vector& other );
    Vector( const std::vector< int32_t >& other );
    Vector( const std::vector< float >& other );
    Vector( const std::vector< double >& other );
    Vector( const float* values, int32_t count );
    Vector( const double* values, int32_t count );
    Vector( const Vector3d< float >& other );
    Vector( const Vector3d< double >& other );
    virtual ~Vector();

    Vector& operator=( const Vector& other );
    Vector& operator=( const std::vector< int32_t >& other );
    Vector& operator=( const std::vector< float >& other );
    Vector& operator=( const std::vector< double >& other );
    Vector& operator=( const Vector3d< float >& other );
    Vector& operator=( const Vector3d< double >& other );

    Vector& operator /= ( const double& thing );
    Vector& operator *= ( const double& thing );

    void reallocate( int32_t size );

    int32_t getSize() const;

    const double& operator()( int32_t i ) const; 
    double& operator()( int32_t i ); 

    void setZero();
    void setBasis( int32_t i );
    void fill( const double& value );

    Matrix getDiagonal() const;
    double getComposition( const Vector& other ) const;
    std::complex< double > getComposition( const CVector& other ) const;
    Matrix toMatrix() const;

    double getMean() const;
    double getStdDev() const;
    double getNorm() const;
    Vector cross( const Vector& other ) const;
    double getSumOfValues() const;
    double getSumOfAbsoluteValues() const;
    double getSumOfSquareValues() const;

    Vector& normalize();

    VectorImplementation* getImplementation() const;

  protected:

    VectorImplementation* _vectorImplementation;

};


}


gkg::Vector 
operator + ( const gkg::Vector& thing );

gkg::Vector 
operator - ( const gkg::Vector& thing );

gkg::Vector
operator + ( const gkg::Vector& thing1,
             const gkg::Vector& thing2 );

gkg::CVector
operator + ( const gkg::Vector& thing1,
             const gkg::CVector& thing2 );

gkg::Vector
operator - ( const gkg::Vector& thing1,
             const gkg::Vector& thing2 );

gkg::CVector
operator - ( const gkg::Vector& thing1,
             const gkg::CVector& thing2 );

gkg::Vector
operator * ( const gkg::Vector& thing1,
             const double& thing2 );

gkg::Vector
operator * ( const double& thing1,
             const gkg::Vector& thing2 );

gkg::Vector
operator * ( const gkg::Vector& thing1,
             const gkg::Matrix& thing2 );

gkg::CVector
operator * ( const gkg::Vector& thing1,
             const std::complex< double >& thing2 );

                         
gkg::Vector
operator / ( const gkg::Vector& thing1,
             const double& thing2 );

gkg::CVector
operator / ( const gkg::Vector& thing1,
             const std::complex< double >& thing2 );


namespace std
{

std::ostream& operator << ( ostream& os, const gkg::Vector& thing );
bool operator == ( const gkg::Vector& thing1,
                   const gkg::Vector& thing2 );

}


#endif
