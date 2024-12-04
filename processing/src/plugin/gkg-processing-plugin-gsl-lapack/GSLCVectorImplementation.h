#ifndef _gkg_processing_plugin_gsl_lapack_GSLCVectorImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLCVectorImplementation_h_


#include <gkg-processing-numericalanalysis/CVectorImplementation.h>
#include <gsl/gsl_vector.h>


namespace gkg
{


class GSLCVectorImplementation : public CVectorImplementation
{

  public:


    GSLCVectorImplementation( CVector* cvector, int32_t size );
    ~GSLCVectorImplementation();

    int32_t getSize() const;

    const std::complex< double >& operator()( int32_t i ) const; 
    std::complex< double >& operator()( int32_t i ); 

    void setZero();
    void setBasis( int32_t i );
    void fill( const std::complex< double >& value );

    gsl_vector_complex* getGSLCVectorPointer() const;

    static GSLCVectorImplementation* getImplementation( CVector* cvector );

  private:

    gsl_vector_complex* _gslCVectorPointer;

};


}


#endif
