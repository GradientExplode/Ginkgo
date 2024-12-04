#include <gkg-dmri-odf-qbi/QBallOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class S >
gkg::QBallOdf::QBallOdf( const gkg::OrientationSet* orientationSet,
                         const gkg::Volume< float >& t2,
                         const gkg::Volume< float >& dw,
                         const gkg::Vector3d< S >& site,
                         int32_t equatorPointCount,
                         const gkg::PhiFunction& phiFunction,
                         const OrientationSet* basisFunctionCenterSet )
              : gkg::OrientationDistributionFunction( 
                  gkg::OrientationDistributionFunction::Standard,
                  orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      if ( basisFunctionCenterSet )
      {

        getReconstructionMatrix( dw,
                                 *orientationSet,
                                 equatorPointCount,
                                 phiFunction,
                                 *basisFunctionCenterSet,
                                 false,
                                 reconstructionMatrix );

      }
      else
      {

        getReconstructionMatrix( dw,
                                 *orientationSet,
                                 equatorPointCount,
                                 phiFunction,
                                 *orientationSet,
                                 true,
                                 reconstructionMatrix );

      }

      this->reconstruct( orientationSet,
                         t2,
                         dw,
                         site,
                         reconstructionMatrix );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::QBallOdf::QBallOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "int32_t equatorPointCount, "
             "const gkg::PhiFunction& phiFunction, "
             "const OrientationSet* basisFunctionCenterSet )" );

}


template < class S >
gkg::QBallOdf::QBallOdf( const gkg::OrientationSet* orientationSet,
                         const gkg::Volume< float >& t2,
                         const gkg::Volume< float >& dw,
                         const gkg::Vector3d< S >& site,
                         const gkg::Matrix& reconstructionMatrix )
              : gkg::OrientationDistributionFunction( 
                  gkg::OrientationDistributionFunction::Standard,
                  orientationSet )
{

  try
  {

    this->reconstruct( orientationSet,
                       t2,
                       dw,
                       site,
                       reconstructionMatrix );

  }
  GKG_CATCH( "template < class S > "
             "gkg::QBallOdf::QBallOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix )" );

}


gkg::QBallOdf::~QBallOdf()
{
}


void gkg::QBallOdf::getReconstructionMatrix(
                              const gkg::HeaderedObject& headeredObject,
                              const gkg::OrientationSet& outputOrientationSet,
                              int32_t equatorPointCount,
                              const gkg::PhiFunction& phiFunction,
                              const gkg::OrientationSet& basisFunctionCenterSet,
                              bool identicalBFCSetAndOOSet,
                              gkg::Matrix& reconstructionMatrix )
{

  try
  {

    // getting orientation count
    int32_t orientationCount = 0;

    if ( headeredObject.getHeader().hasAttribute( "sizeT" ) )
    {

      headeredObject.getHeader().getAttribute( "sizeT", orientationCount );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeT'" );

    }

    // building orientation matrix
    std::vector< double > bValues( orientationCount );
    if ( headeredObject.getHeader().hasAttribute( "bvalues" ) )
    {

      headeredObject.getHeader().getAttribute( "bvalues", bValues );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'bvalues'" );

    }

    gkg::GenericObjectList genericObjectListOrientations( orientationCount );
    if ( headeredObject.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      headeredObject.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }

    std::vector< gkg::Vector3d< float > > orientations( orientationCount );
    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();
      
      orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
      orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
      orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

    }
    gkg::OrientationSet inputOrientationSet( orientations );

    // measure count
    int32_t M = inputOrientationSet.getCount();

    // reconstruction point count
    int32_t N = outputOrientationSet.getCount();

    // basis function center count
    int32_t P = basisFunctionCenterSet.getCount();

    // equator point count
    int K = equatorPointCount;

    int32_t m, n, p, k;

    // building matrix of wave vector
    gkg::Matrix Q( 3, M );
    for ( m = 0; m < M; m++ )
    {

      Q( 0, m ) = inputOrientationSet.getOrientation( m ).x;
      Q( 1, m ) = inputOrientationSet.getOrientation( m ).y;
      Q( 2, m ) = inputOrientationSet.getOrientation( m ).z;

    }

    // building matrix of reconstruction point(s)
    gkg::Matrix U( 3, N );
    for ( n = 0; n < N; n++ )
    {

      U( 0, n ) = outputOrientationSet.getOrientation( n ).x;
      U( 1, n ) = outputOrientationSet.getOrientation( n ).y;
      U( 2, n ) = outputOrientationSet.getOrientation( n ).z;

    }

    // building equator point matrix in (xy) plane
    gkg::Matrix C( 3, K );
    for ( k = 0; k < K; k++ )
    {

      C( 0, k ) = std::cos( 2 * M_PI * ( k + 1 ) / ( double )K );
      C( 1, k ) = std::sin( 2 * M_PI * ( k + 1 ) / ( double )K );
      C( 2, k ) = 0.0;

    }

    // building equator point(s) for all point(s)
    std::vector< gkg::Matrix > S( N );
    for ( n = 0; n < N; n++ )
    {

      S[ n ].reallocate( K, 3 );
      if ( ( U( 2, n ) == -1.0 ) || ( U( 2, n ) == +1.0 ) )
      {

        for ( k = 0; k < K; k++ )
        {

          S[ n ]( k, 0 ) = C( 0, k );
          S[ n ]( k, 1 ) = C( 1, k );
          S[ n ]( k, 2 ) = C( 2, k );

        }

      }
      else
      {

        for ( k = 0; k < K; k++ )
        {

          S[ n ]( k, 0 ) = ( ( U( 0, n ) * U( 0, n ) ) /
                             ( 1 + U( 2, n ) ) - 1 ) *
                           C( 0, k ) +
                           ( U( 0, n ) * U( 1, n ) / ( 1 + U( 2, n ) ) ) *
                           C( 1, k );
          S[ n ]( k, 1 ) = ( U( 0, n ) * U( 1, n ) / ( 1 + U( 2, n ) ) ) *
                           C( 0, k ) +
                           ( ( U( 1, n ) * U( 1, n ) ) / 
                             ( 1 + U( 2, n ) ) - 1 ) *
                           C( 1, k );
          S[ n ]( k, 2 ) = U( 0, n ) * C( 0, k ) + U( 1, n ) * C( 1, k );

        }

      }

    }

    // fast reconstruction matrix when basic function center(s) and
    // reconstruction point(s) are the same
    if ( identicalBFCSetAndOOSet )
    {

      // allocating final reconstruction matrix
      reconstructionMatrix.reallocate( N, M );

      // looping over reconstruction point(s)
      gkg::Matrix Sprime;
      double accumulator = 0.0;
      for ( n = 0; n < N; n++ )
      {

        // for current reconstruction point n, building K x M reconstruction 
        // matrix
        Sprime = S[ n ].getComposition( Q );
        for ( m = 0; m < M; m++ )
        {

          accumulator = 0.0;
          for ( k = 0; k < K; k++ )
          {

            Sprime( k, m ) =
                        phiFunction.getValue( gkg::safeAcos( Sprime( k, m ) ) );
            accumulator += Sprime( k, m );

          }
          if ( accumulator > 0 )
          {

            for ( k = 0; k < K; k++ )
            {

              Sprime( k, m ) /= accumulator;

            }

          }

        }

        // for current reconstruction point n, filling final reconstruction 
        // matrix with Sprime element(s)
        for ( m = 0; m < M; m++ )
        {

          reconstructionMatrix( n, m ) = 0.0;
          for ( k = 0; k < K; k++ )
          {

            reconstructionMatrix( n, m ) += Sprime( k, m );

          }
          reconstructionMatrix( n, m ) /= K;

        }

      }

    }
    // slow reconstruction matrix when basic function center(s) and
    // reconstruction point(s) are different
    else
    {

      // building matrix of the centers of basis function(s)
      gkg::Matrix V( 3, P );
      for ( p = 0; p < P; p++ )
      {

        V( 0, p ) = basisFunctionCenterSet.getOrientation( p ).x;
        V( 1, p ) = basisFunctionCenterSet.getOrientation( p ).y;
        V( 2, p ) = basisFunctionCenterSet.getOrientation( p ).z;

      }

      // building P x M convolution matrix
      gkg::Matrix H;
      H = V.getTransposition().getComposition( Q );
      double accumulator = 0;
      for ( p = 0; p < P; p++ )
      {

        accumulator = 0.0;
        for ( m = 0; m < M; m++ )
        {

          H( p, m ) = phiFunction.getValue( gkg::safeAcos( H( p, m ) ) );
          accumulator += H( p, m );

        }
        if ( accumulator > 0.0 )
        {

          for ( m = 0; m < M; m++ )
          {

            H( p, m ) /= accumulator;

          }

        }

      }

      // allocating final reconstruction matrix
      reconstructionMatrix.reallocate( N, M );

      // looping over reconstruction point(s)
      gkg::Matrix Sprime;
      gkg::Matrix R;
      for ( n = 0; n < N; n++ )
      {

        // for current reconstruction point n, building K x P reconstruction 
        // matrix from basis function center(s)
        Sprime = S[ n ].getComposition( V );
        for ( p = 0; p < P; p++ )
        {

          accumulator = 0.0;
          for ( k = 0; k < K; k++ )
          {

            Sprime( k, p ) =
                        phiFunction.getValue( gkg::safeAcos( Sprime( k, p ) ) );
            accumulator += Sprime( k, p );

          }
          if ( accumulator > 0 )
          {

            for ( k = 0; k < K; k++ )
            {

              Sprime( k, p ) /= accumulator;

            }

          }

        }

        // for current reconstruction point n, building K x M reconstruction 
        // matrix
        R = Sprime.getComposition( H );

        // for current reconstruction point n, filling final reconstruction 
        // matrix with R element(s)
        for ( m = 0; m < M; m++ )
        {

          reconstructionMatrix( n, m ) = 0.0;
          for ( k = 0; k < K; k++ )
          {

            reconstructionMatrix( n, m ) += R( k, m );

          }
          reconstructionMatrix( n, m ) /= K;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::QBallOdf::getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "int32_t equatorPointCount, "
             "const gkg::PhiFunction& phiFunction, "
             "const gkg::OrientationSet& basisFunctionCenterSet, "
             "gkg::Matrix& reconstructionMatrix)" );

}


template < class S >
void gkg::QBallOdf::reconstruct( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Vector3d< S >& site,
                                 const gkg::Matrix& reconstructionMatrix )
{

  try
  {

    if ( orientationSet )
    {

      // computing average T2 value and normalized signal vector
      float averageT2 = 0.0;
      gkg::Vector normalizedSignal;
      bool isOk = this->computeNormalizedSignal( t2,
                                                 dw,
                                                 site,
                                                 averageT2,
                                                 normalizedSignal );

      // resizing ODF vector to orientation count
      int32_t orientationCount = _orientationSet->getCount();
      _values.resize( orientationCount );

      if ( !isOk )
      {

        // setting ODF to nul
        int32_t o;
        for ( o = 0; o < orientationCount; o++ )
        {

          _values[ o ] = 0.0f;

        }

      }
      else
      {

        // processing ODF
        gkg::Vector probabilities;
        probabilities = reconstructionMatrix.getComposition( normalizedSignal );

        // normalizing
        float sum = 0.0;
        int32_t o;
        for ( o = 0; o < orientationCount; o++ )
        {

          sum += ( float )probabilities( o );

        }

        // copying normalized probabilities
        for ( o = 0; o < orientationCount; o++ )
        {

          _values[ o ] = ( float )probabilities( o ) / sum;

        }

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "void gkg::QBallOdf::reconstruct( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix )" );

}


template 
gkg::QBallOdf::QBallOdf( const gkg::OrientationSet* orientationSet,
                         const gkg::Volume< float >& t2,
                         const gkg::Volume< float >& dw,
                         const gkg::Vector3d< int32_t >& site,
                         int32_t equatorPointCount,
                         const PhiFunction& phiFunction,
                         const OrientationSet* basisFunctionCenterSet );

template 
gkg::QBallOdf::QBallOdf( const gkg::OrientationSet* orientationSet,
                         const gkg::Volume< float >& t2,
                         const gkg::Volume< float >& dw,
                         const gkg::Vector3d< float >& site,
                         int32_t equatorPointCount,
                         const PhiFunction& phiFunction,
                         const OrientationSet* basisFunctionCenterSet );


template
gkg::QBallOdf::QBallOdf( const gkg::OrientationSet* orientationSet,
                         const gkg::Volume< float >& t2,
                         const gkg::Volume< float >& dw,
                         const gkg::Vector3d< int32_t >& site,
                         const gkg::Matrix& reconstructionMatrix );

template
gkg::QBallOdf::QBallOdf( const gkg::OrientationSet* orientationSet,
                         const gkg::Volume< float >& t2,
                         const gkg::Volume< float >& dw,
                         const gkg::Vector3d< float >& site,
                         const gkg::Matrix& reconstructionMatrix );

template
void gkg::QBallOdf::reconstruct( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Vector3d< int32_t >& site,
                                 const gkg::Matrix& reconstructionMatrix );

template
void gkg::QBallOdf::reconstruct( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Vector3d< float >& site,
                                 const gkg::Matrix& reconstructionMatrix );
 

