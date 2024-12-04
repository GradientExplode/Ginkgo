#ifndef _gkg_processing_plugin_gsl_lapack_GSLCMatrixImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLCMatrixImplementation_h_


#include <gkg-processing-numericalanalysis/CMatrixImplementation.h>
#include <gsl/gsl_matrix.h>


namespace gkg
{


class GSLCMatrixImplementation : public CMatrixImplementation
{

  public:


    GSLCMatrixImplementation( CMatrix* cmatrix, int32_t size1, int32_t size2 );
    ~GSLCMatrixImplementation();

    int32_t getSize1() const;
    int32_t getSize2() const;

    const std::complex< double >& operator()( int32_t i, int32_t j ) const; 
    std::complex< double >& operator()( int32_t i, int32_t j ); 

    void setZero();
    void setIdentity();
    void fill( const std::complex< double >& value );

    gsl_matrix_complex* getGSLCMatrixPointer() const;

    static GSLCMatrixImplementation* getImplementation( CMatrix* cmatrix );

  private:

    gsl_matrix_complex* _gslCMatrixPointer;

};


}


#endif
