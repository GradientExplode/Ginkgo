#ifndef _gkg_processing_numericalanalysis_MatrixImplementation_h_
#define _gkg_processing_numericalanalysis_MatrixImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Matrix;


class MatrixImplementation
{

  public:

    virtual ~MatrixImplementation();

    virtual Matrix* getTarget() const;

    virtual int32_t getSize1() const = 0;
    virtual int32_t getSize2() const = 0;

    virtual const double& operator()( int32_t i, int32_t j ) const = 0; 
    virtual double& operator()( int32_t i, int32_t j ) = 0; 

    virtual void setZero() = 0;
    virtual void setIdentity() = 0;
    virtual void fill( const double& value ) = 0;

    virtual double getDeterminant() const = 0;

  protected:

    MatrixImplementation( Matrix* matrix,
                          int32_t size1,
                          int32_t size2 );

    Matrix* _target;

};


}


#endif
