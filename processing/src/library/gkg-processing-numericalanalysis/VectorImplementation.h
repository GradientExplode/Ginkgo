#ifndef _gkg_processing_numericalanalysis_VectorImplementation_h_
#define _gkg_processing_numericalanalysis_VectorImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Vector;


class VectorImplementation
{

  public:

    virtual ~VectorImplementation();

    virtual Vector* getTarget() const;

    virtual int32_t getSize() const = 0;

    virtual const double& operator()( int32_t i ) const = 0; 
    virtual double& operator()( int32_t i ) = 0; 

    virtual void setZero() = 0;
    virtual void setBasis( int32_t i ) = 0;
    virtual void fill( const double& value ) = 0;

  protected:

    VectorImplementation( Vector* vector,
                          int32_t size );

    Vector* _target;

};


}


#endif
