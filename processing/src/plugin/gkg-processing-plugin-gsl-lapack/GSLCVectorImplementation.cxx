#include <gkg-processing-plugin-gsl-lapack/GSLCVectorImplementation.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLCVectorImplementation::GSLCVectorImplementation( gkg::CVector* cvector,
                                                         int32_t size )
                              : gkg::CVectorImplementation( cvector, size )
{

  try
  {

    _gslCVectorPointer = ::gsl_vector_complex_calloc( size );

  }
  GKG_CATCH( "gkg::GSLCVectorImplementation::GSLCVectorImplementation( "
             "gkg::CVector* cvector, int size )" );

}


gkg::GSLCVectorImplementation::~GSLCVectorImplementation()
{

  gsl_vector_complex_free( _gslCVectorPointer );

}


int32_t gkg::GSLCVectorImplementation::getSize() const
{

  return _gslCVectorPointer->size;

}


const std::complex< double >& 
gkg::GSLCVectorImplementation::operator()( int32_t i ) const
{

  try
  {

    return ( const std::complex< double >& )*gsl_vector_complex_const_ptr(
                                                        _gslCVectorPointer, i );

  }
  GKG_CATCH( "const std::complex< double >& "
             "gkg::GSLCVectorImplementation::operator()( int i ) const" );

}


std::complex< double >& gkg::GSLCVectorImplementation::operator()( int32_t i )
{

  try
  {

    return ( std::complex< double >& )*gsl_vector_complex_ptr(
                                                        _gslCVectorPointer, i );

  }
  GKG_CATCH( "std::complex< double >& "
             "gkg::GSLCVectorImplementation::operator()( int i )" );

}


void gkg::GSLCVectorImplementation::setZero()
{
 
  try
  {

    gsl_vector_complex_set_zero( _gslCVectorPointer );

  }
  GKG_CATCH( "void gkg::GSLCVectorImplementation::setZero()" );

}


void gkg::GSLCVectorImplementation::setBasis( int32_t i )
{
 
  try
  {

    gsl_vector_complex_set_basis( _gslCVectorPointer, ( size_t )i );

  }
  GKG_CATCH( "void gkg::GSLCVectorImplementation::setBasis( int i )" );

}


void gkg::GSLCVectorImplementation::fill( const std::complex< double >& value )
{
 
  try
  {

    gsl_vector_complex_set_all( _gslCVectorPointer,
                                *( ( gsl_complex* )&value ) );

  }
  GKG_CATCH( "void gkg::GSLCVectorImplementation::fill( "
             "const std::complex< double >& value )" );

}


gsl_vector_complex* gkg::GSLCVectorImplementation::getGSLCVectorPointer() const
{

  return _gslCVectorPointer;

}


gkg::GSLCVectorImplementation* 
gkg::GSLCVectorImplementation::getImplementation( gkg::CVector* cvector )
{

  return static_cast< gkg::GSLCVectorImplementation* >(
                                                 cvector->getImplementation() );

}

