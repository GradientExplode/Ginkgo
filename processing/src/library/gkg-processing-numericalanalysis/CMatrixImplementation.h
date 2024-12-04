#ifndef _gkg_processing_numericalanalysis_CMatrixImplementation_h_
#define _gkg_processing_numericalanalysis_CMatrixImplementation_h_


#include <complex>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class CMatrix;


class CMatrixImplementation
{

  public:

    virtual ~CMatrixImplementation();

    virtual CMatrix* getTarget() const;

    virtual int32_t getSize1() const = 0;
    virtual int32_t getSize2() const = 0;

    virtual const std::complex< double >& operator()( int32_t i, 
                                                      int32_t j ) const = 0; 
    virtual std::complex< double >& operator()( int32_t i, int32_t j ) = 0; 

    virtual void setZero() = 0;
    virtual void setIdentity() = 0;
    virtual void fill( const std::complex< double >& value ) = 0;

  protected:

    CMatrixImplementation( CMatrix* matrix,
                           int32_t size1,
                           int32_t size2 );

    CMatrix* _target;

};


}


#endif
