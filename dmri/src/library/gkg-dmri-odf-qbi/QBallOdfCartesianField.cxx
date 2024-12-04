#include <gkg-dmri-odf-qbi/QBallOdfCartesianField.h>
#include <gkg-dmri-odf-qbi/QBallOdf.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::QBallOdfCartesianField::QBallOdfCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                 : gkg::OdfCartesianField( t2, dw, mask, outputOrientationSet )
{

  try
  {

    _equatorPointCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );
    if ( _equatorPointCount < 0 )
    {

      throw std::runtime_error( "equator point count must be positive" );

    }

    double phiFunctionAngle = specificScalarParameters[ 1 ];
    double phiFunctionMaxAngle = specificScalarParameters[ 2 ];
    std::string phiFunctionType = specificStringParameters[ 0 ];

    if ( ( phiFunctionAngle <= 0.0 ) || ( phiFunctionMaxAngle <= 0.0 ) )
    {

      float minimumAngle = this->getMinimumAngleOfInputOrientationSet();
      minimumAngle *= 180.0 / M_PI;
      if ( phiFunctionAngle <= 0.0 )
      {

         phiFunctionAngle = minimumAngle;

      }
      if ( phiFunctionMaxAngle <= 0.0 )
      {

         phiFunctionMaxAngle = 3.0 * minimumAngle;

      }

    }

    if ( phiFunctionType == "gaussian" )
    {

      _phiFunction.reset( new gkg::GaussianPhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else if ( phiFunctionType == "multiquadric" )
    {

      _phiFunction.reset( new gkg::MultiquadricPhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else if ( phiFunctionType == "thinplate" )
    {

      _phiFunction.reset( new gkg::ThinPlateSplinePhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else
    {

      throw std::runtime_error( "invalid phi function type" );

    }

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::QBallOdf >::getName() );

    _basisFunctionCenterSet = _outputOrientationSet;
    _identicalBFCSetAndOOSet = true;

    getReconstructionMatrix( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::QBallOdfCartesianField::QBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) " );

}


gkg::QBallOdfCartesianField::QBallOdfCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::BoundingBox< float >& boundingBox,
                     const gkg::Vector3d< float >& superResolution,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                            : gkg::OdfCartesianField(
                                             t2, dw, mask,
                                             boundingBox, superResolution,
                                             outputOrientationSet )
{

  try
  {

    _equatorPointCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );
    if ( _equatorPointCount < 0 )
    {

      throw std::runtime_error( "equator point count must be positive" );

    }

    double phiFunctionAngle = specificScalarParameters[ 1 ];
    double phiFunctionMaxAngle = specificScalarParameters[ 2 ];
    std::string phiFunctionType = specificStringParameters[ 0 ];

    if ( ( phiFunctionAngle <= 0.0 ) || ( phiFunctionMaxAngle <= 0.0 ) )
    {

      float minimumAngle = this->getMinimumAngleOfInputOrientationSet();
      minimumAngle *= 180.0 / M_PI;
      if ( phiFunctionAngle <= 0.0 )
      {

         phiFunctionAngle = minimumAngle;

      }
      if ( phiFunctionMaxAngle <= 0.0 )
      {

         phiFunctionMaxAngle = 3.0 * minimumAngle;

      }

    }

    if ( phiFunctionType == "gaussian" )
    {

      _phiFunction.reset( new gkg::GaussianPhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else if ( phiFunctionType == "multiquadric" )
    {

      _phiFunction.reset( new gkg::MultiquadricPhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else if ( phiFunctionType == "thinplate" )
    {

      _phiFunction.reset( new gkg::ThinPlateSplinePhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else
    {

      throw std::runtime_error( "invalid phi function type" );

    }

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::QBallOdf >::getName() );

    _basisFunctionCenterSet = _outputOrientationSet;
    _identicalBFCSetAndOOSet = true;

    getReconstructionMatrix( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::QBallOdfCartesianField::QBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) " );

}


gkg::QBallOdfCartesianField::QBallOdfCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::BoundingBox< int32_t >& boundingBox,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                            : gkg::OdfCartesianField(
                                             t2, dw, mask, boundingBox,
                                             outputOrientationSet )
{

  try
  {

    _equatorPointCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );
    if ( _equatorPointCount < 0 )
    {

      throw std::runtime_error( "equator point count must be positive" );

    }

    double phiFunctionAngle = specificScalarParameters[ 1 ];
    double phiFunctionMaxAngle = specificScalarParameters[ 2 ];
    std::string phiFunctionType = specificStringParameters[ 0 ];

    if ( ( phiFunctionAngle <= 0.0 ) || ( phiFunctionMaxAngle <= 0.0 ) )
    {

      float minimumAngle = this->getMinimumAngleOfInputOrientationSet();
      minimumAngle *= 180.0 / M_PI;
      if ( phiFunctionAngle <= 0.0 )
      {

         phiFunctionAngle = minimumAngle;

      }
      if ( phiFunctionMaxAngle <= 0.0 )
      {

         phiFunctionMaxAngle = 3.0 * minimumAngle;

      }

    }

    if ( phiFunctionType == "gaussian" )
    {

      _phiFunction.reset( new gkg::GaussianPhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else if ( phiFunctionType == "multiquadric" )
    {

      _phiFunction.reset( new gkg::MultiquadricPhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else if ( phiFunctionType == "thinplate" )
    {

      _phiFunction.reset( new gkg::ThinPlateSplinePhiFunction(
                                     phiFunctionAngle * M_PI / 180.0,
                                     phiFunctionMaxAngle * M_PI / 180.0   ) );

    }
    else
    {

      throw std::runtime_error( "invalid phi function type" );

    }

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::QBallOdf >::getName() );

    _basisFunctionCenterSet = _outputOrientationSet;
    _identicalBFCSetAndOOSet = true;

    getReconstructionMatrix( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::QBallOdfCartesianField::QBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) " );

}


gkg::QBallOdfCartesianField::QBallOdfCartesianField(
                              gkg::RCPointer< gkg::Volume< float > > t2,
                              gkg::RCPointer< gkg::Volume< float > > dw,
                              gkg::RCPointer< gkg::Volume< int16_t > > mask,
                              int32_t equatorPointCount,
                              gkg::RCPointer< gkg::PhiFunction > phiFunction,
                              const gkg::OrientationSet* outputOrientationSet,
                              const gkg::OrientationSet* basisFunctionCenterSet,
                              bool verbose )
                 : gkg::OdfCartesianField( t2, dw, mask, outputOrientationSet ),
                   _equatorPointCount( equatorPointCount ),
                   _phiFunction( phiFunction )
{

  try
  {

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::QBallOdf >::getName() );

    if ( basisFunctionCenterSet )
    {

      _basisFunctionCenterSet = *basisFunctionCenterSet;
      _identicalBFCSetAndOOSet = false;

    }
    else
    {

      _basisFunctionCenterSet = _outputOrientationSet;
      _identicalBFCSetAndOOSet = true;

    }
    getReconstructionMatrix( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::QBallOdfCartesianField::QBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "int32_t equatorPointCount, "
             "gkg::RCPointer< gkg::PhiFunction > phiFunction, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::OrientationSet* basisFunctionCenterSet, "
             "bool verbose )" );

}


gkg::QBallOdfCartesianField::QBallOdfCartesianField(
                              gkg::RCPointer< gkg::Volume< float > > t2,
                              gkg::RCPointer< gkg::Volume< float > > dw,
                              gkg::RCPointer< gkg::Volume< int16_t > > mask,
                              const gkg::BoundingBox< float >& boundingBox,
                              const gkg::Vector3d< float >& superResolution,
                              int32_t equatorPointCount,
                              gkg::RCPointer< gkg::PhiFunction > phiFunction,
                              const gkg::OrientationSet* outputOrientationSet,
                              const gkg::OrientationSet* basisFunctionCenterSet,
                              bool verbose )
                            : gkg::OdfCartesianField( t2, dw, mask,
                                             boundingBox, superResolution,
                                             outputOrientationSet ),
                              _equatorPointCount( equatorPointCount ),
                              _phiFunction( phiFunction )
{

  try
  {

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::QBallOdf >::getName() );

    if ( basisFunctionCenterSet )
    {

      _basisFunctionCenterSet = *basisFunctionCenterSet;
      _identicalBFCSetAndOOSet = false;

    }
    else
    {

      _basisFunctionCenterSet = _outputOrientationSet;
      _identicalBFCSetAndOOSet = true;

    }
    getReconstructionMatrix( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::QBallOdfCartesianField::QBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "int32_t equatorPointCount, "
             "gkg::RCPointer< gkg::PhiFunction > phiFunction, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::OrientationSet* basisFunctionCenterSet, "
             "bool verbose )" );

}


gkg::QBallOdfCartesianField::QBallOdfCartesianField(
                              gkg::RCPointer< gkg::Volume< float > > t2,
                              gkg::RCPointer< gkg::Volume< float > > dw,
                              gkg::RCPointer< gkg::Volume< int16_t > > mask,
                              const gkg::BoundingBox< int32_t >& boundingBox,
                              int32_t equatorPointCount,
                              gkg::RCPointer< gkg::PhiFunction > phiFunction,
                              const gkg::OrientationSet* outputOrientationSet,
                              const gkg::OrientationSet* basisFunctionCenterSet,
                              bool verbose )
                            : gkg::OdfCartesianField( t2, dw, mask, boundingBox,
                                             outputOrientationSet ),
                              _equatorPointCount( equatorPointCount ),
                              _phiFunction( phiFunction )
{

  try
  {

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::QBallOdf >::getName() );

    if ( basisFunctionCenterSet )
    {

      _basisFunctionCenterSet = *basisFunctionCenterSet;
      _identicalBFCSetAndOOSet = false;

    }
    else
    {

      _basisFunctionCenterSet = _outputOrientationSet;
      _identicalBFCSetAndOOSet = true;

    }
    getReconstructionMatrix( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::QBallOdfCartesianField::QBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "int32_t equatorPointCount, "
             "gkg::RCPointer< gkg::PhiFunction > phiFunction, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::OrientationSet* basisFunctionCenterSet, "
             "bool verbose )" );

}


gkg::QBallOdfCartesianField::~QBallOdfCartesianField()
{
}



int32_t gkg::QBallOdfCartesianField::getEquatorPointCount() const
{

  return _equatorPointCount;

}


const gkg::OrientationSet& 
gkg::QBallOdfCartesianField::getBasisFunctionCenterSet() const
{

  return _basisFunctionCenterSet;

}


void gkg::QBallOdfCartesianField::getReconstructionMatrix( bool verbose )
{

  try
  {

    // measure count
    int32_t M = _inputOrientationSet.getCount();

    // reconstruction point count
    int32_t N = _outputOrientationSet.getCount();

    // basis function center count
    int32_t P = _basisFunctionCenterSet.getCount();

    // equator point count
    int K = _equatorPointCount;

    int32_t m, n, p, k;

    // building matrix of wave vector
    gkg::Matrix Q( 3, M );
    for ( m = 0; m < M; m++ )
    {

      Q( 0, m ) = _inputOrientationSet.getOrientation( m ).x;
      Q( 1, m ) = _inputOrientationSet.getOrientation( m ).y;
      Q( 2, m ) = _inputOrientationSet.getOrientation( m ).z;

    }

    // building matrix of reconstruction point(s)
    gkg::Matrix U( 3, N );
    for ( n = 0; n < N; n++ )
    {

      U( 0, n ) = _outputOrientationSet.getOrientation( n ).x;
      U( 1, n ) = _outputOrientationSet.getOrientation( n ).y;
      U( 2, n ) = _outputOrientationSet.getOrientation( n ).z;

    }

    // building equator point matrix in (xy) plane
    gkg::Matrix C( 3, K );
    for ( k = 0; k < K; k++ )
    {

      C( 0, k ) = std::cos( 2 * M_PI * ( k + 1 ) / ( double )K );
      C( 1, k ) = std::sin( 2 * M_PI * ( k + 1 ) / ( double )K );
      C( 2, k ) = 0.0;

    }

    // building equator point matrice(s) for all point(s)
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
    if ( _identicalBFCSetAndOOSet )
    {

      // allocating final reconstruction matrix
      _reconstructionMatrix.reallocate( N, M );

      // looping over reconstruction point(s)
      gkg::Matrix Sprime;
      double accumulator = 0.0;
      for ( n = 0; n < N; n++ )
      {

        if ( verbose )
        {

          if ( ( ( n + 1 ) % 10 == 0 ) ||
               ( n == 0 ) ||
               ( n == N - 1 ) )
          {

            if ( n )
            {

              std::cout << gkg::Eraser( 22 );

            }
            std::cout << " [ " << std::setw( 7 ) << n + 1
                      << " / " << std::setw( 7 ) << N
                      << " ]" << std::flush;

          }

        }

        // for current reconstruction point n, building K x M reconstruction 
        // matrix
        Sprime = S[ n ].getComposition( Q );
        for ( m = 0; m < M; m++ )
        {

          accumulator = 0.0;
          for ( k = 0; k < K; k++ )
          {

            Sprime( k, m ) =
                      _phiFunction->getValue( gkg::safeAcos( Sprime( k, m ) ) );
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

          _reconstructionMatrix( n, m ) = 0.0;
          for ( k = 0; k < K; k++ )
          {

            _reconstructionMatrix( n, m ) += Sprime( k, m );

          }
          _reconstructionMatrix( n, m ) /= K;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 ) << std::flush;

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

        V( 0, p ) = _basisFunctionCenterSet.getOrientation( p ).x;
        V( 1, p ) = _basisFunctionCenterSet.getOrientation( p ).y;
        V( 2, p ) = _basisFunctionCenterSet.getOrientation( p ).z;

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

          H( p, m ) = _phiFunction->getValue( gkg::safeAcos( H( p, m ) ) );
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
      _reconstructionMatrix.reallocate( N, M );

      // looping over reconstruction point(s)
      gkg::Matrix Sprime;
      gkg::Matrix R;
      for ( n = 0; n < N; n++ )
      {

        if ( verbose )
        {

          if ( ( ( n + 1 ) % 10 == 0 ) ||
               ( n == 0 ) ||
               ( n == N - 1 ) )
          {

            if ( n )
            {

              std::cout << gkg::Eraser( 22 );

            }
            std::cout << " [ " << std::setw( 7 ) << n + 1
                      << " / " << std::setw( 7 ) << N
                      << " ]" << std::flush;

          }

        }

        // for current reconstruction point n, building K x P reconstruction 
        // matrix from basis function center(s)
        Sprime = S[ n ].getComposition( V );
        for ( p = 0; p < P; p++ )
        {

          accumulator = 0.0;
          for ( k = 0; k < K; k++ )
          {

            Sprime( k, p ) =
                      _phiFunction->getValue( gkg::safeAcos( Sprime( k, p ) ) );
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

          _reconstructionMatrix( n, m ) = 0.0;
          for ( k = 0; k < K; k++ )
          {

            _reconstructionMatrix( n, m ) += R( k, m );

          }
          _reconstructionMatrix( n, m ) /= K;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "void gkg::QBallOdfCartesianField::getReconstructionMatrix( "
             "bool verbose )" );

}


void gkg::QBallOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 3 );
      specificScalarParameters[ 0 ] = 50;
      specificScalarParameters[ 1 ] = -1.0;
      specificScalarParameters[ 2 ] = -1.0;

    }
    else
    {

      if ( specificScalarParameters.size() != 3U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

    }
    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "gaussian";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

      if ( ( specificStringParameters[ 0 ] != "gaussian" ) && 
           ( specificStringParameters[ 0 ] != "multiquadric" ) && 
           ( specificStringParameters[ 0 ] != "thinplate" ) )
      {

        throw std::runtime_error( "invalid string parameter" );

      }

    }

  }
  GKG_CATCH( "void gkg::QBallOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


gkg::OrientationDistributionFunction 
gkg::QBallOdfCartesianField::newItem(
                                   const gkg::Vector3d< int32_t >& originSite,
                                   const gkg::Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::QBallOdf( &_outputOrientationSet,
                          *_t2, *_dw,
                          originSite,
                          _reconstructionMatrix );

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::QBallOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::QBallOdfCartesianField::newItem(
                                   const gkg::Vector3d< float >& originSite,
                                   const gkg::Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::QBallOdf( &_outputOrientationSet,
                          *_t2, *_dw,
                          originSite,
                          _reconstructionMatrix );

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::QBallOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
