#include <gkg-processing-plugin-gsl-lapack/GSLMatrixImplementation.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>

#include <gsl/gsl_linalg.h>


gkg::GSLMatrixImplementation::GSLMatrixImplementation( gkg::Matrix* matrix,
                                                       int32_t size1,
                                                       int32_t size2 )
                             : gkg::MatrixImplementation( matrix, size1, size2 )
{

  try
  {

    _gslMatrixPointer = ::gsl_matrix_calloc( size1, size2 );

  }
  GKG_CATCH( "gkg::GSLMatrixImplementation::GSLMatrixImplementation( "
             "gkg::Matrix* matrix, int size1, int size2 )" );

}


gkg::GSLMatrixImplementation::~GSLMatrixImplementation()
{

  gsl_matrix_free( _gslMatrixPointer );

}


int32_t gkg::GSLMatrixImplementation::getSize1() const
{

  return _gslMatrixPointer->size1;

}


int32_t gkg::GSLMatrixImplementation::getSize2() const
{

  return _gslMatrixPointer->size2;

}


const double& 
gkg::GSLMatrixImplementation::operator()( int32_t i, int32_t j ) const
{

  try
  {

    return *gsl_matrix_const_ptr( _gslMatrixPointer, i, j );

  }
  GKG_CATCH( "const double& gkg::GSLMatrixImplementation::operator()( "
             "int i, int j ) const" );

}


double& gkg::GSLMatrixImplementation::operator()( int32_t i, int32_t j )
{

  try
  {

    return *gsl_matrix_ptr( _gslMatrixPointer, i, j );

  }
  GKG_CATCH( "double& gkg::GSLMatrixImplementation::operator()( "
             "int i, int j )" );

}


void gkg::GSLMatrixImplementation::setZero()
{
 
  try
  {

    gsl_matrix_set_zero( _gslMatrixPointer );

  }
  GKG_CATCH( "void gkg::GSLMatrixImplementation::setZero()" );

}


void gkg::GSLMatrixImplementation::setIdentity()
{
 
  try
  {

    gsl_matrix_set_identity( _gslMatrixPointer );

  }
  GKG_CATCH( "void gkg::GSLMatrixImplementation::setIdentity()" );

}


void gkg::GSLMatrixImplementation::fill( const double& value )
{
 
  try
  {

    gsl_matrix_set_all( _gslMatrixPointer, value );

  }
  GKG_CATCH( "void gkg::GSLMatrixImplementation::fill( double value )" );

}


double gkg::GSLMatrixImplementation::getDeterminant() const
{

  try
  {

    double determinant = 0.0;
    int32_t signum;

    gsl_matrix* m = gsl_matrix_alloc( _gslMatrixPointer->size1, 
                                      _gslMatrixPointer->size2 );
    gsl_permutation* p = gsl_permutation_alloc( _gslMatrixPointer->size1 );

    gsl_matrix_memcpy( m, _gslMatrixPointer );
    gsl_linalg_LU_decomp( m, p, &signum );

    determinant = gsl_linalg_LU_det( m, signum );

    gsl_permutation_free( p );
    gsl_matrix_free( m );

    return determinant;

  }
  GKG_CATCH( "double gkg::GSLMatrixImplementation::getDeterminant()" );

}


gsl_matrix* gkg::GSLMatrixImplementation::getGSLMatrixPointer() const
{

  return _gslMatrixPointer;

}


gkg::GSLMatrixImplementation* 
gkg::GSLMatrixImplementation::getImplementation( gkg::Matrix* matrix )
{

  return static_cast< gkg::GSLMatrixImplementation* >(
                                                  matrix->getImplementation() );

}


const gkg::GSLMatrixImplementation* 
gkg::GSLMatrixImplementation::getImplementation( const gkg::Matrix* matrix )
{

  return static_cast< const gkg::GSLMatrixImplementation* >(
                                                  matrix->getImplementation() );

}

