#include <gkg-processing-numericalanalysis/IllMatrixInverser.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-exception/Exception.h>


gkg::IllMatrixInverser::IllMatrixInverser()
{
}


gkg::IllMatrixInverser::~IllMatrixInverser()
{
}



void gkg::IllMatrixInverser::inverse(
                                   const gkg::Matrix& A,
                                   const gkg::Vector& b,
                                   gkg::Vector& x ) const
{

  try
  {

    gkg::CMatrix cA( A );
    gkg::CVector cB( b );

    gkg::CVector cX( x.getSize() );

    inverse( cA, cB, cX );

    int32_t size = cX.getSize();
    x.reallocate( size );

    int32_t i;
    for ( i = 0; i < size; i++ )
    {

      x( i ) = std::real( cX( i ) );

    }

  }
  GKG_CATCH( "void gkg::IllMatrixInverser::inverse( "
             "const gkg::Matrix& A, "
             "const gkg::Vector& b, "
             "gkg::Vector& x ) const" );

}
