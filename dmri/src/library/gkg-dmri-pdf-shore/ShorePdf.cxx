#include <gkg-dmri-pdf-shore/ShorePdf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-processing-coordinates/DisplacementSetCache.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>



float gkg::ShorePdf::_piToThePowerThreeAndAHalf = 
                                           ( float )std::pow( M_PI, 7.0 / 2.0 );
float gkg::ShorePdf::_piToThePowerOneAndAHalf =
                                           ( float )std::pow( M_PI, 3.0 / 2.0 );
float gkg::ShorePdf::_piToThePowerOneHalf =
                                           ( float )std::pow( M_PI, 1.0 / 2.0 );


template < class S >
gkg::ShorePdf::ShorePdf(
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const gkg::Volume< float >& t2,
      const gkg::Volume< float >& dw,
      const gkg::Vector3d< S >& site,
      int32_t maximumPolarHarmonicsOrder,
      float regularizationLCurveFactor,
      const gkg::Tensor& tensor )
              : gkg::ProbabilityDensityFunction(
                               gkg::ProbabilityDensityFunction::PolarHarmonics,
                               outputMultipleShellSampling )
{

  try
  {

    if ( _outputMultipleShellSampling )
    {


      //////////////////////////////////////////////////////////////////////////
      // updating displacement set cache
      //////////////////////////////////////////////////////////////////////////

      gkg::DisplacementSetCache&
        displacementSetCache = gkg::DisplacementSetCache::getInstance();
      displacementSetCache.update( *_outputMultipleShellSampling );

      //////////////////////////////////////////////////////////////////////////
      // extracting the input multiple different shell q-space sampling from DW
      //////////////////////////////////////////////////////////////////////////

      gkg::RCPointer< gkg::MultipleShellQSpaceSampling >
        inputMultipleShellQSpaceSampling(
                              this->getInputMultipleShellQSpaceSampling( dw ) );

      //////////////////////////////////////////////////////////////////////////
      // estimating the SHORE model
      //////////////////////////////////////////////////////////////////////////

      this->reconstruct( *inputMultipleShellQSpaceSampling,
                         t2,
                         dw,
                         site,
                         maximumPolarHarmonicsOrder,
                         regularizationLCurveFactor,
                         tensor );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::ShorePdf::ShorePdf( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "int32_t maximumPolarHarmonicsOrder, "
             "float regularizationLCurveFactor, "
             "const gkg::Tensor& tensor )" );

}


template < class S >
gkg::ShorePdf::ShorePdf(
         const gkg::Vector3d< int32_t >* outputCartesianGridSize,
         const gkg::Vector3d< double >* outputCartesianGridResolution,
         const gkg::Volume< float >& t2,
	 const gkg::Volume< float >& dw,
	 const gkg::Vector3d< S >& site,
         int32_t maximumPolarHarmonicsOrder,
         float regularizationLCurveFactor,
         const gkg::Tensor& tensor )
              : gkg::ProbabilityDensityFunction(
                               gkg::ProbabilityDensityFunction::PolarHarmonics,
                               outputCartesianGridSize,
                               outputCartesianGridResolution )
{

  try
  {

    if ( _outputCartesianGridSize && _outputCartesianGridResolution )
    {


      //////////////////////////////////////////////////////////////////////////
      // updating displacement set cache
      //////////////////////////////////////////////////////////////////////////

      gkg::DisplacementSetCache&
        displacementSetCache = gkg::DisplacementSetCache::getInstance();
      displacementSetCache.update( *_outputCartesianGridSize,
                                   *_outputCartesianGridResolution );

      //////////////////////////////////////////////////////////////////////////
      // extracting the input multiple different shell q-space sampling from DW
      //////////////////////////////////////////////////////////////////////////

      gkg::RCPointer< gkg::MultipleShellQSpaceSampling >
        inputMultipleShellQSpaceSampling(
                              this->getInputMultipleShellQSpaceSampling( dw ) );

      //////////////////////////////////////////////////////////////////////////
      // estimating the SHORE model
      //////////////////////////////////////////////////////////////////////////

      this->reconstruct( *inputMultipleShellQSpaceSampling,
                         t2,
                         dw,
                         site,
                         maximumPolarHarmonicsOrder,
                         regularizationLCurveFactor,
                         tensor );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::ShorePdf::ShorePdf( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Volume< float >& t2, "
	     "const gkg::Volume< float >& dw, "
	     "const gkg::Vector3d< S >& site, "
             "int32_t maximumPolarHarmonicsOrder, "
             "float regularizationLCurveFactor, "
             "const gkg::Tensor& tensor )" );

}


template < class S >
gkg::ShorePdf::ShorePdf(
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const gkg::Volume< float >& t2,
      const gkg::Volume< float >& dw,
      const gkg::Vector3d< S >& site,
      const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
      int32_t maximumPolarHarmonicsOrder,
      float regularizationLCurveFactor,
      const gkg::Tensor& tensor )
              : gkg::ProbabilityDensityFunction(
                               gkg::ProbabilityDensityFunction::PolarHarmonics,
                               outputMultipleShellSampling )
{

  try
  {

    if ( _outputMultipleShellSampling )
    {

      //////////////////////////////////////////////////////////////////////////
      // updating displacement set cache
      //////////////////////////////////////////////////////////////////////////

      gkg::DisplacementSetCache&
        displacementSetCache = gkg::DisplacementSetCache::getInstance();
      displacementSetCache.update( *_outputMultipleShellSampling );


      //////////////////////////////////////////////////////////////////////////
      // estimating the SHORE model
      //////////////////////////////////////////////////////////////////////////

      this->reconstruct( inputMultipleShellQSpaceSampling,
                         t2,
                         dw,
                         site,
                         maximumPolarHarmonicsOrder,
                         regularizationLCurveFactor,
                         tensor );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::ShorePdf::ShorePdf( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::MultipleShellQSpaceSampling& "
             "inputMultipleShellQSpaceSampling, "
             "int32_t maximumPolarHarmonicsOrder, "
             "float regularizationLCurveFactor, "
             "const gkg::Tensor& tensor )" );

}


template < class S >
gkg::ShorePdf::ShorePdf(
       const gkg::Vector3d< int32_t >* outputCartesianGridSize,
       const gkg::Vector3d< double >* outputCartesianGridResolution,
       const gkg::Volume< float >& t2,
       const gkg::Volume< float >& dw,
       const gkg::Vector3d< S >& site,
       const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
       int32_t maximumPolarHarmonicsOrder,
       float regularizationLCurveFactor,
       const gkg::Tensor& tensor )
              : gkg::ProbabilityDensityFunction(
                               gkg::ProbabilityDensityFunction::PolarHarmonics,
                               outputCartesianGridSize,
                               outputCartesianGridResolution )
{

  try
  {

    if ( _outputCartesianGridSize && _outputCartesianGridResolution )
    {


      //////////////////////////////////////////////////////////////////////////
      // updating displacement set cache
      //////////////////////////////////////////////////////////////////////////

      gkg::DisplacementSetCache&
        displacementSetCache = gkg::DisplacementSetCache::getInstance();
      displacementSetCache.update( *_outputCartesianGridSize,
                                   *_outputCartesianGridResolution );

      //////////////////////////////////////////////////////////////////////////
      // estimating the SHORE model
      //////////////////////////////////////////////////////////////////////////

      this->reconstruct( inputMultipleShellQSpaceSampling,
                         t2,
                         dw,
                         site,
                         maximumPolarHarmonicsOrder,
                         regularizationLCurveFactor,
                         tensor );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::ShorePdf::ShorePdf( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::MultipleShellQSpaceSampling& "
             "inputMultipleShellQSpaceSampling, "
             "int32_t maximumPolarHarmonicsOrder, "
             "float regularizationLCurveFactor, "
             "const gkg::Tensor& tensor )" );

}


gkg::ShorePdf::~ShorePdf()
{
}


gkg::MultipleShellQSpaceSampling* 
gkg::ShorePdf::getInputMultipleShellQSpaceSampling( 
                                          const gkg::Volume< float >& dw ) const
{

  try
  {

    gkg::HeaderedObject headeredObject = dw;

    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create(
                                                             headeredObject );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
          dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );

    if ( !multipleShellQSpaceSampling )
    {
 
      throw std::runtime_error(
                            "qspace sampling is not of type multiple shell" );

    }
    return multipleShellQSpaceSampling;

  }
  GKG_CATCH( "gkg::MultipleShellQSpaceSampling* "
             "gkg::ShorePdf::getInputMultipleShellQSpaceSampling( "
             "const gkg::Volume< float >& dw ) const" );

}


template < class S >
void gkg::ShorePdf::reconstruct( 
       const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
       const gkg::Volume< float >& t2,
       const gkg::Volume< float >& dw,
       const gkg::Vector3d< S >& site,
       int32_t maximumPolarHarmonicsOrder,
       float regularizationLCurveFactor,
       const gkg::Tensor& tensor )
{

  try
  {

    int32_t
      globalQSpacePointCount = inputMultipleShellQSpaceSampling.getCount();


    //////////////////////////////////////////////////////////////////////////
    // extracting the diffusion time
    //////////////////////////////////////////////////////////////////////////

    float diffusionTime = 
                         inputMultipleShellQSpaceSampling.getDiffusionTime( 0 );

    // sanity check
    int32_t inputShellCount = inputMultipleShellQSpaceSampling.getShellCount();
    int32_t inputShellIndex = 0;
    for ( inputShellIndex = 1; inputShellIndex < inputShellCount;
          inputShellIndex++ )
    {

      if ( !gkg::equal( inputMultipleShellQSpaceSampling.getDiffusionTime(
                                                              inputShellIndex ),
                        diffusionTime,
                        1e-3f ) )
      {

        throw std::runtime_error( "all diffusion times must be equal" );

      }

    }


    //////////////////////////////////////////////////////////////////////////
    // computing normalized signal
    //////////////////////////////////////////////////////////////////////////

    float averageT2 = 0.0;
    gkg::Vector normalizedSignal;
    bool isOk = this->computeNormalizedSignal( t2,
                                               dw,
                                               site,
                                               averageT2,
                                               normalizedSignal );


    //////////////////////////////////////////////////////////////////////////
    // degenerated signal
    //////////////////////////////////////////////////////////////////////////
    if ( !isOk )
    {


      int32_t polarHarmonicsCoefficientCount =
                ( ( maximumPolarHarmonicsOrder / 2 + 1 ) *
                  ( maximumPolarHarmonicsOrder / 2 + 2 ) *
                  ( 2 * maximumPolarHarmonicsOrder + 3 ) ) / 6;
      _values.resize( polarHarmonicsCoefficientCount + 7  );
      _values[ 0 ] = maximumPolarHarmonicsOrder;
      _values[ 1 ] = 0.0;
      _values[ 2 ] = 0.0;
      _values[ 3 ] = 0.0;
      _values[ 4 ] = 2.0 * diffusionTime *
                     std::sqrt(
                               tensor.getEigenValue( gkg::Tensor::Maximum ) );
      _values[ 5 ] = 2.0 * diffusionTime *
                     std::sqrt(
                               tensor.getEigenValue( gkg::Tensor::Middle ) );
      _values[ 6 ] =  2.0 * diffusionTime *
                     std::sqrt(
                               tensor.getEigenValue( gkg::Tensor::Minimum ) );
      int32_t i = 0;
      for ( i = 0; i < polarHarmonicsCoefficientCount; i++ )
      {

        _values[ 7 + i ] = 0.0;

      }
      ///////////////// need to tune the parameters to get a sphere

    }
    else
    {

      // computing scalings
      gkg::Vector3d< float > scalings;
      scalings.x = 2.0 * diffusionTime *
                     std::sqrt(
                               tensor.getEigenValue( gkg::Tensor::Maximum ) );
      scalings.y = 2.0 * diffusionTime *
                     std::sqrt(
                               tensor.getEigenValue( gkg::Tensor::Middle ) );
      scalings.z = 2.0 * diffusionTime *
                     std::sqrt(
                               tensor.getEigenValue( gkg::Tensor::Minimum ) );

      // getting rotation matrix stemming from tensor eigenvectors
      gkg::Rotation3d< float > rotation3d = tensor.getRotation3d();

      // updating polar harmonics cache
      gkg::PolarHarmonicsCache&
        polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
      polarHarmonicsCache.update( maximumPolarHarmonicsOrder );
      int32_t polarHarmonicsCoefficientCount =
                                    polarHarmonicsCache.getCoefficientCount();
      // computing the rotated q-space point(s)
      std::vector< gkg::Vector3d< float > > 
        rotatedQSpacePoints( globalQSpacePointCount );
      int32_t q = 0;
      for ( q = 0; q < globalQSpacePointCount; q++ )
      {

        rotation3d.getDirect(
          inputMultipleShellQSpaceSampling.getOrientation( q ) *
          ( float )( inputMultipleShellQSpaceSampling.getQMagnitude( q ) / 
                     ( 2 * M_PI ) ),
          rotatedQSpacePoints[ q ] );

      }

      // allocating and computing the SHORE basis matrix
      gkg::Matrix shoreBasisMatrix( globalQSpacePointCount,
                                    polarHarmonicsCoefficientCount );
      int32_t basisIndex = 0;
      int32_t qSpaceIndex = 0;
      for ( qSpaceIndex = 0; qSpaceIndex < globalQSpacePointCount;
            qSpaceIndex++ )
      {

        for ( basisIndex = 0; basisIndex < polarHarmonicsCoefficientCount; 
              basisIndex++ )
        {

          shoreBasisMatrix( qSpaceIndex, basisIndex ) =
                               polarHarmonicsCache.getQSpaceBasisFunction3dAt(
                                                          rotatedQSpacePoints,
                                                          scalings,
                                                          qSpaceIndex,
                                                          basisIndex );

        }

      }

      // getting a pointer to the numerical analysis factory
      gkg::NumericalAnalysisImplementationFactory* factory =
        gkg::NumericalAnalysisSelector::getInstance().
                                                   getImplementationFactory();

      // computing the Laplace-Beltrami regularization matrix
      gkg::Matrix L( polarHarmonicsCoefficientCount,
                     polarHarmonicsCoefficientCount );
      this->getLaplaceBeltramiRegularizationMatrix( factory,
                                                    polarHarmonicsCache,
                                                    scalings,
                                                    L );

      // inverting the SHORE matrix using Thikonov regularization to get
      // the final reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      factory->getThikonovPseudoInverse(
                                     shoreBasisMatrix, L,
                                     ( double )regularizationLCurveFactor,
                                     reconstructionMatrix );

      // now applying SHORE reconstruction matrix
      gkg::Vector shoreCoefficients =
                      reconstructionMatrix.getComposition( normalizedSignal );

      // filling value(s)
      _values.resize( polarHarmonicsCoefficientCount + 7  );
      _values[ 0 ] = maximumPolarHarmonicsOrder;
      rotation3d.getDirectAngles( _values[ 1 ],
                                  _values[ 2 ],
                                  _values[ 3 ] );
      _values[ 4 ] = scalings.x;
      _values[ 5 ] = scalings.y;
      _values[ 6 ] = scalings.z;
      int32_t i = 0;
      for ( i = 0; i < polarHarmonicsCoefficientCount; i++ )
      {

        _values[ 7 + i ] = ( float )shoreCoefficients( i );

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "void gkg::ShorePdf::reconstruct( "
             "const gkg::MultipleShellQSpaceSampling* "
             "inputMultipleShellQSpaceSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "int32_t maximumPolarHarmonicsOrder, "
             "float regularizationLCurveFactor, "
             "const gkg::Tensor& tensor )" );

}


void gkg::ShorePdf::getLaplaceBeltramiRegularizationMatrix(
                           gkg::NumericalAnalysisImplementationFactory* factory,
                           const gkg::PolarHarmonicsCache& polarHarmonicsCache,
                           const gkg::Vector3d< float >& scalings,
                           gkg::Matrix& L ) const
{

  try
  {

    int32_t size1 = L.getSize1();
    int32_t size2 = L.getSize2();

    const std::vector< Vector3d< int32_t > >&
      basisIndices = polarHarmonicsCache.getBasisIndices();


    int32_t i = 0;
    int32_t j = 0;
    for ( i = 0; i < size1; i++ )
    {

      const gkg::Vector3d< int32_t >& indicesForI = basisIndices[ i ];
      for ( j = 0; j < size2; j++ )
      {

        const gkg::Vector3d< int32_t >& indicesForJ = basisIndices[ j ];

        L( i, j ) = ( ( scalings.x * scalings.x * scalings.x ) /
                      ( scalings.y * scalings.z ) ) *
                    this->getLaplaceS( factory, indicesForI.x, indicesForJ.x ) *
                    this->getLaplaceU( indicesForI.y, indicesForJ.y ) *
                    this->getLaplaceU( indicesForI.z, indicesForJ.z ) +
 
                    ( ( 2.0 * scalings.x * scalings.y ) / scalings.z ) *
                    this->getLaplaceT( indicesForI.x, indicesForJ.x ) *
                    this->getLaplaceT( indicesForI.y, indicesForJ.y ) *
                    this->getLaplaceU( indicesForI.z, indicesForJ.z ) +
 
                    ( ( scalings.y * scalings.y * scalings.y ) /
                      ( scalings.z * scalings.x ) ) *
                    this->getLaplaceS( factory, indicesForI.y, indicesForJ.y ) *
                    this->getLaplaceU( indicesForI.z, indicesForJ.z ) *
                    this->getLaplaceU( indicesForI.x, indicesForJ.x ) +
 
                    ( ( 2.0 * scalings.y * scalings.z ) / scalings.x ) *
                    this->getLaplaceT( indicesForI.y, indicesForJ.y ) *
                    this->getLaplaceT( indicesForI.z, indicesForJ.z ) *
                    this->getLaplaceU( indicesForI.x, indicesForJ.x ) +
 
                    ( ( scalings.z * scalings.z * scalings.z ) /
                      ( scalings.x * scalings.y ) ) *
                    this->getLaplaceS( factory, indicesForI.z, indicesForJ.z ) *
                    this->getLaplaceU( indicesForI.x, indicesForJ.x ) *
                    this->getLaplaceU( indicesForI.y, indicesForJ.y ) +
 
                    ( ( 2.0 * scalings.x * scalings.z ) / scalings.y ) *
                    this->getLaplaceT( indicesForI.z, indicesForJ.z ) *
                    this->getLaplaceT( indicesForI.x, indicesForJ.x ) *
                    this->getLaplaceU( indicesForI.y, indicesForJ.y );

      }

    }

  }
  GKG_CATCH( "void gkg::ShorePdf::getLaplaceBeltramiRegularizationMatrix( "
             "gkg::NumericalAnalysisImplementationFactory* factory, "
             "const gkg::PolarHarmonicsCache& polarHarmonicsCache, "
             "const gkg::Vector3d< float >& scalings, "
             "gkg::Matrix& L ) const" );

}


float gkg::ShorePdf::getLaplaceS(
                           gkg::NumericalAnalysisImplementationFactory* factory,
                           int32_t n, int32_t m ) const
{

  try
  {

    return 2.0 * ( n % 2 ? -1.0 : +1.0 ) * _piToThePowerThreeAndAHalf *
           ( ( ( n == m ) ? 1.0 : 0.0 ) *
             3.0 * ( 2.0 * ( float )n * ( float )n + 2.0 * ( float )n + 1.0 ) +

             ( ( ( n + 2 == m ) ? 1.0 : 0.0 ) *
               ( 6.0 + 4.0 * ( float )n ) +
               ( ( n + 4 == m ) ? 1.0 : 0.0 ) ) *
             std::sqrt( factory->getFactorialFunction( ( double )m ) /
                        factory->getFactorialFunction( ( double )n ) )+

             ( ( ( n == m + 1 ) ? 1.0 : 0.0 ) *
               ( 6.0 + 4.0 * ( float )m )  +
               ( ( n == m + 1 ) ? 1.0 : 0.0 ) ) *
              std::sqrt( factory->getFactorialFunction( ( double )n ) /
                        factory->getFactorialFunction( ( double )m ) )
           ); 

  }
  GKG_CATCH( "void gkg::ShorePdf::getLaplaceS( "
             "gkg::NumericalAnalysisImplementationFactory* factory, "
             "int32_t n, int32_t m ) const" );

}


float gkg::ShorePdf::getLaplaceT( int32_t n, int32_t m ) const
{

  try
  {

    return ( ( n + 1 ) % 2 ? -1.0 : +1.0 ) * _piToThePowerOneAndAHalf *
           ( ( ( n == m ) ? 1.0 : 0.0 ) *
             ( 1.0 + 2.0 * ( float )n ) +
 
             ( ( n == m + 2 ) ? 1.0 : 0.0 ) *
             std::sqrt( ( float )n * ( ( float )n - 1.0 ) ) +
 
             ( ( n + 2 == m ) ? 1.0 : 0.0 ) *
             std::sqrt( ( float )m * ( ( float )m - 1.0 ) ) );

  }
  GKG_CATCH( "void gkg::ShorePdf::getLaplaceT( "
             "int32_t n, int32_t m ) const" );

}


float gkg::ShorePdf::getLaplaceU( int32_t n, int32_t m ) const
{

  try
  {

    return ( ( ( n == m ) ? 1.0 : 0.0 ) *
            ( n % 2 ? -1.0 : +1.0 ) ) / 
           ( 2.0 * _piToThePowerOneHalf );

  }
  GKG_CATCH( "void gkg::ShorePdf::getLaplaceU( "
             "int32_t n, int32_t m ) const" );

}



template
gkg::ShorePdf::ShorePdf(
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const gkg::Volume< float >& t2,
      const gkg::Volume< float >& dw,
      const gkg::Vector3d< int32_t >& site,
      int32_t maximumPolarHarmonicsOrder,
      float regularizationLCurveFactor,
      const gkg::Tensor& tensor );

template
gkg::ShorePdf::ShorePdf(
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const gkg::Volume< float >& t2,
      const gkg::Volume< float >& dw,
      const gkg::Vector3d< float >& site,
      int32_t maximumPolarHarmonicsOrder,
      float regularizationLCurveFactor,
      const gkg::Tensor& tensor );


template
gkg::ShorePdf::ShorePdf(
         const gkg::Vector3d< int32_t >* outputCartesianGridSize,
         const gkg::Vector3d< double >* outputCartesianGridResolution,
         const gkg::Volume< float >& t2,
	 const gkg::Volume< float >& dw,
	 const gkg::Vector3d< int32_t >& site,
         int32_t maximumPolarHarmonicsOrder,
         float regularizationLCurveFactor,
         const gkg::Tensor& tensor );

template
gkg::ShorePdf::ShorePdf(
         const gkg::Vector3d< int32_t >* outputCartesianGridSize,
         const gkg::Vector3d< double >* outputCartesianGridResolution,
         const gkg::Volume< float >& t2,
	 const gkg::Volume< float >& dw,
	 const gkg::Vector3d< float >& site,
         int32_t maximumPolarHarmonicsOrder,
         float regularizationLCurveFactor,
         const gkg::Tensor& tensor );

template
gkg::ShorePdf::ShorePdf(
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const gkg::Volume< float >& t2,
      const gkg::Volume< float >& dw,
      const gkg::Vector3d< int32_t >& site,
      const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
      int32_t maximumPolarHarmonicsOrder,
      float regularizationLCurveFactor,
      const gkg::Tensor& tensor );

template
gkg::ShorePdf::ShorePdf(
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const gkg::Volume< float >& t2,
      const gkg::Volume< float >& dw,
      const gkg::Vector3d< float >& site,
      const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
      int32_t maximumPolarHarmonicsOrder,
      float regularizationLCurveFactor,
      const gkg::Tensor& tensor );

template
gkg::ShorePdf::ShorePdf(
       const gkg::Vector3d< int32_t >* outputCartesianGridSize,
       const gkg::Vector3d< double >* outputCartesianGridResolution,
       const gkg::Volume< float >& t2,
       const gkg::Volume< float >& dw,
       const gkg::Vector3d< int32_t >& site,
       const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
       int32_t maximumPolarHarmonicsOrder,
       float regularizationLCurveFactor,
       const gkg::Tensor& tensor );

template
gkg::ShorePdf::ShorePdf(
       const gkg::Vector3d< int32_t >* outputCartesianGridSize,
       const gkg::Vector3d< double >* outputCartesianGridResolution,
       const gkg::Volume< float >& t2,
       const gkg::Volume< float >& dw,
       const gkg::Vector3d< float >& site,
       const gkg::MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
       int32_t maximumPolarHarmonicsOrder,
       float regularizationLCurveFactor,
       const gkg::Tensor& tensor );
