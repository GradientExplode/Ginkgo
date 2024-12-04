#ifndef _gkg_processing_plugin_gsl_lapack_GSLMatrixImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLMatrixImplementation_h_


#include <gkg-processing-numericalanalysis/MatrixImplementation.h>
#include <gsl/gsl_matrix.h>


namespace gkg
{


class GSLMatrixImplementation : public MatrixImplementation
{

  public:


    GSLMatrixImplementation( Matrix* matrix, int32_t size1, int32_t size2 );
    ~GSLMatrixImplementation();

    int32_t getSize1() const;
    int32_t getSize2() const;

    const double& operator()( int32_t i, int32_t j ) const; 
    double& operator()( int32_t i, int32_t j ); 

    void setZero();
    void setIdentity();
    void fill( const double& value );

    double getDeterminant() const;

    gsl_matrix* getGSLMatrixPointer() const;

    static GSLMatrixImplementation* getImplementation( Matrix* matrix );
    static const GSLMatrixImplementation* getImplementation(
                                                         const Matrix* matrix );

  private:

    gsl_matrix* _gslMatrixPointer;

};


}


#endif
