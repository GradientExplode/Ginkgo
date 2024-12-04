#ifndef _gkg_processing_numericalanalysis_CVectorImplementation_h_
#define _gkg_processing_numericalanalysis_CVectorImplementation_h_


#include <complex>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class CVector;


class CVectorImplementation
{

  public:

    virtual ~CVectorImplementation();

    virtual CVector* getTarget() const;

    virtual int32_t getSize() const = 0;

    virtual const std::complex< double >& operator()( int32_t i ) const = 0; 
    virtual std::complex< double >& operator()( int32_t i ) = 0; 

    virtual void setZero() = 0;
    virtual void setBasis( int32_t i ) = 0;
    virtual void fill( const std::complex< double >& value ) = 0;

  protected:

    CVectorImplementation( CVector* vector,
                           int32_t size );

    CVector* _target;

};


}


#endif
