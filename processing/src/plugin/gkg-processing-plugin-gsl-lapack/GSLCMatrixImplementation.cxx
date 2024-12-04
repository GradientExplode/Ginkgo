#include <gkg-processing-plugin-gsl-lapack/GSLCMatrixImplementation.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLCMatrixImplementation::GSLCMatrixImplementation( gkg::CMatrix* cmatrix,
                                                         int32_t size1,
                                                         int32_t size2 )
                              : gkg::CMatrixImplementation( cmatrix,
                                                            size1, size2 )
{

  try
  {

    _gslCMatrixPointer = ::gsl_matrix_complex_calloc( size1, size2 );

  }
  GKG_CATCH( "gkg::GSLCMatrixImplementation::GSLCMatrixImplementation( "
             "gkg::CMatrix* cmatrix, int size1, int size2 )" );

}


gkg::GSLCMatrixImplementation::~GSLCMatrixImplementation()
{

  gsl_matrix_complex_free( _gslCMatrixPointer );

}


int32_t gkg::GSLCMatrixImplementation::getSize1() const
{

  return _gslCMatrixPointer->size1;

}


int32_t gkg::GSLCMatrixImplementation::getSize2() const
{

  return _gslCMatrixPointer->size2;

}


const std::complex< double >& 
gkg::GSLCMatrixImplementation::operator()( int32_t i, int32_t j ) const
{

  try
  {

    return ( const std::complex< double >& )*gsl_matrix_complex_const_ptr(
                                                     _gslCMatrixPointer, i, j );

  }
  GKG_CATCH( "const std::complex< double >& "
             "gkg::GSLCMatrixImplementation::operator()( "
             "int i, int j ) const" );

}


std::complex< double >& 
gkg::GSLCMatrixImplementation::operator()( int32_t i, int32_t j )
{

  try
  {

    return ( std::complex< double >& )*gsl_matrix_complex_ptr(
                                                     _gslCMatrixPointer, i, j );

  }
  GKG_CATCH( "std::complex< double >& "
             "gkg::GSLCMatrixImplementation::operator()( "
             "int i, int j )" );

}


void gkg::GSLCMatrixImplementation::setZero()
{
 
  try
  {

    gsl_matrix_complex_set_zero( _gslCMatrixPointer );

  }
  GKG_CATCH( "void gkg::GSLCMatrixImplementation::setZero()" );

}


void gkg::GSLCMatrixImplementation::setIdentity()
{
 
  try
  {

    gsl_matrix_complex_set_identity( _gslCMatrixPointer );

  }
  GKG_CATCH( "void gkg::GSLCMatrixImplementation::setIdentity()" );

}


void gkg::GSLCMatrixImplementation::fill( const std::complex< double >& value )
{
 
  try
  {

    gsl_matrix_complex_set_all( _gslCMatrixPointer,
                                *( ( const gsl_complex* )&value ) );

  }
  GKG_CATCH( "void gkg::GSLCMatrixImplementation::fill( "
             "const std::complex< double >& value )" );

}


gsl_matrix_complex* gkg::GSLCMatrixImplementation::getGSLCMatrixPointer() const
{

  return _gslCMatrixPointer;

}


gkg::GSLCMatrixImplementation* 
gkg::GSLCMatrixImplementation::getImplementation( gkg::CMatrix* cmatrix )
{

  return static_cast< gkg::GSLCMatrixImplementation* >(
                                                 cmatrix->getImplementation() );

}

