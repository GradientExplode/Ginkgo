#include <gkg-processing-plugin-gsl-lapack/GSLVectorImplementation.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLVectorImplementation::GSLVectorImplementation( gkg::Vector* vector,
                                                       int32_t size )
                             : gkg::VectorImplementation( vector, size )
{

  try
  {

    _gslVectorPointer = ::gsl_vector_calloc( size );

  }
  GKG_CATCH( "gkg::GSLVectorImplementation::GSLVectorImplementation( "
             "gkg::Vector* vector, int size )" );

}


gkg::GSLVectorImplementation::~GSLVectorImplementation()
{

  gsl_vector_free( _gslVectorPointer );

}


int32_t gkg::GSLVectorImplementation::getSize() const
{

  return _gslVectorPointer->size;

}


const double& gkg::GSLVectorImplementation::operator()( int32_t i ) const
{

  try
  {

    return *gsl_vector_const_ptr( _gslVectorPointer, i );

  }
  GKG_CATCH( "const double& gkg::GSLVectorImplementation::operator()( "
             "int i ) const" );

}


double& gkg::GSLVectorImplementation::operator()( int32_t i )
{

  try
  {

    return *gsl_vector_ptr( _gslVectorPointer, i );

  }
  GKG_CATCH( "double& gkg::GSLVectorImplementation::operator()( "
             "int i )" );

}


void gkg::GSLVectorImplementation::setZero()
{
 
  try
  {

    gsl_vector_set_zero( _gslVectorPointer );

  }
  GKG_CATCH( "void gkg::GSLVectorImplementation::setZero()" );

}


void gkg::GSLVectorImplementation::setBasis( int32_t i )
{
 
  try
  {

    gsl_vector_set_basis( _gslVectorPointer, ( size_t )i );

  }
  GKG_CATCH( "void gkg::GSLVectorImplementation::setBasis( int i )" );

}


void gkg::GSLVectorImplementation::fill( const double& value )
{
 
  try
  {

    gsl_vector_set_all( _gslVectorPointer, value );

  }
  GKG_CATCH( "void gkg::GSLVectorImplementation::fill( double value )" );

}


gsl_vector* gkg::GSLVectorImplementation::getGSLVectorPointer() const
{

  return _gslVectorPointer;

}


gkg::GSLVectorImplementation* 
gkg::GSLVectorImplementation::getImplementation( gkg::Vector* vector )
{

  return static_cast< gkg::GSLVectorImplementation* >(
                                                  vector->getImplementation() );

}

