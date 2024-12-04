#ifndef _gkg_processing_plugin_gsl_lapack_GSLVectorImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLVectorImplementation_h_


#include <gkg-processing-numericalanalysis/VectorImplementation.h>
#include <gsl/gsl_vector.h>


namespace gkg
{


class GSLVectorImplementation : public VectorImplementation
{

  public:

    GSLVectorImplementation( Vector* vector, int32_t size );
    ~GSLVectorImplementation();

    int32_t getSize() const;

    const double& operator()( int32_t i ) const; 
    double& operator()( int32_t i ); 

    void setZero();
    void setBasis( int32_t i );
    void fill( const double& value );

    gsl_vector* getGSLVectorPointer() const;

    static GSLVectorImplementation* getImplementation( Vector* vector );

  private:

    gsl_vector* _gslVectorPointer;

};


}


#endif
