#include <gkg-dmri-pdf-shore/ShorePdfCartesianField.h>
#include <gkg-dmri-pdf-shore/ShorePdf.h>
#include <gkg-dmri-pdf-shore/ShorePdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-processing-coordinates/DisplacementSetCache.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::ShorePdfCartesianField::ShorePdfCartesianField(
      gkg::RCPointer< gkg::Volume< float > > t2,
      gkg::RCPointer< gkg::Volume< float > > dw,
      gkg::RCPointer< gkg::Volume< int16_t > > mask,
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const std::vector< double >& specificScalarParameters,
      const std::vector< std::string >& specificStringParameters,
      bool verbose )
                            : gkg::PdfCartesianField(
                                                   t2, dw, mask,
                                                   outputMultipleShellSampling )
{

  try
  {

    // extracting the input multiple different shell q-space sampling from
    // the DW volume header
    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create( *dw );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
            dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
      
      throw std::runtime_error(
                   "multiple shell q-space sampling required for SHORE model" );

    }
    else
    {

      _inputMultipleShellQSpaceSampling.reset( multipleShellQSpaceSampling );

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting PDF type
    _items.getHeader().addAttribute( "pdf_type",
                                     gkg::TypeOf< gkg::ShorePdf >::getName() );

    // updating displacement set cache
    gkg::DisplacementSetCache&
      displacementSetCache = gkg::DisplacementSetCache::getInstance();
    displacementSetCache.update( *outputMultipleShellSampling );

    // updating polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
    polarHarmonicsCache.update( _maximumPolarHarmonicsOrder );

    // setting PDF functor
    this->_functor = new gkg::ShorePdfCartesianFieldFunctor( *this );

    // computing PDFs
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::ShorePdfCartesianField::ShorePdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::ShorePdfCartesianField::ShorePdfCartesianField(
      gkg::RCPointer< gkg::Volume< float > > t2,
      gkg::RCPointer< gkg::Volume< float > > dw,
      gkg::RCPointer< gkg::Volume< int16_t > > mask,
      const gkg::BoundingBox< float >& boundingBox,
      const gkg::Vector3d< float >& superResolution,
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const std::vector< double >& specificScalarParameters,
      const std::vector< std::string >& specificStringParameters,
      bool verbose )
                             : gkg::PdfCartesianField(
                                                   t2, dw, mask,
                                                   boundingBox,
                                                   superResolution,
                                                   outputMultipleShellSampling )
{

  try
  {

    // extracting the input multiple different shell q-space sampling from
    // the DW volume header
    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create( *dw );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
            dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
      
      throw std::runtime_error(
                   "multiple shell q-space sampling required for SHORE model" );

    }
    else
    {

      _inputMultipleShellQSpaceSampling.reset( multipleShellQSpaceSampling );

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       boundingBox,
                                       superResolution,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting PDF type
    _items.getHeader().addAttribute( "pdf_type",
                                     gkg::TypeOf< gkg::ShorePdf >::getName() );

    // updating displacement set cache
    gkg::DisplacementSetCache&
      displacementSetCache = gkg::DisplacementSetCache::getInstance();
    displacementSetCache.update( *outputMultipleShellSampling );

    // updating polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
    polarHarmonicsCache.update( _maximumPolarHarmonicsOrder );

    // setting PDF functor
    this->_functor = new gkg::ShorePdfCartesianFieldFunctor( *this );

    // computing PDFs
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::ShorePdfCartesianField::ShorePdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::ShorePdfCartesianField::ShorePdfCartesianField(
      gkg::RCPointer< gkg::Volume< float > > t2,
      gkg::RCPointer< gkg::Volume< float > > dw,
      gkg::RCPointer< gkg::Volume< int16_t > > mask,
      const gkg::BoundingBox< int32_t >& boundingBox,
      const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
      const std::vector< double >& specificScalarParameters,
      const std::vector< std::string >& specificStringParameters,
      bool verbose )
                             : gkg::PdfCartesianField(
                                                   t2, dw, mask,
                                                   boundingBox,
                                                   outputMultipleShellSampling )
{

  try
  {

    // extracting the input multiple different shell q-space sampling from
    // the DW volume header
    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create( *dw );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
            dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
      
      throw std::runtime_error(
                   "multiple shell q-space sampling required for SHORE model" );

    }
    else
    {

      _inputMultipleShellQSpaceSampling.reset( multipleShellQSpaceSampling );

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       boundingBox,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting PDF type
    _items.getHeader().addAttribute( "pdf_type",
                                     gkg::TypeOf< gkg::ShorePdf >::getName() );

    // updating displacement set cache
    gkg::DisplacementSetCache&
      displacementSetCache = gkg::DisplacementSetCache::getInstance();
    displacementSetCache.update( *outputMultipleShellSampling );

    // updating polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
    polarHarmonicsCache.update( _maximumPolarHarmonicsOrder );

    // setting PDF functor
    this->_functor = new gkg::ShorePdfCartesianFieldFunctor( *this );

    // computing PDFs
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::ShorePdfCartesianField::ShorePdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::ShorePdfCartesianField::ShorePdfCartesianField(
           gkg::RCPointer< gkg::Volume< float > > t2,
           gkg::RCPointer< gkg::Volume< float > > dw,
           gkg::RCPointer< gkg::Volume< int16_t > > mask,
           const gkg::Vector3d< int32_t >* outputCartesianGridSize,
           const gkg::Vector3d< double >* outputCartesianGridResolution,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose )
                            : gkg::PdfCartesianField(
                                                 t2, dw, mask,
                                                 outputCartesianGridSize,
                                                 outputCartesianGridResolution )
{

  try
  {

    // extracting the input multiple different shell q-space sampling from
    // the DW volume header
    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create( *dw );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
            dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
      
      throw std::runtime_error(
                   "multiple shell q-space sampling required for SHORE model" );

    }
    else
    {

      _inputMultipleShellQSpaceSampling.reset( multipleShellQSpaceSampling );

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting PDF type
    _items.getHeader().addAttribute( "pdf_type",
                                     gkg::TypeOf< gkg::ShorePdf >::getName() );

    // updating displacement set cache
    gkg::DisplacementSetCache&
      displacementSetCache = gkg::DisplacementSetCache::getInstance();
    displacementSetCache.update( *outputCartesianGridSize,
                                 *outputCartesianGridResolution );

    // updating polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
    polarHarmonicsCache.update( _maximumPolarHarmonicsOrder );

    // setting PDF functor
    this->_functor = new gkg::ShorePdfCartesianFieldFunctor( *this );

    // computing PDFs
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::ShorePdfCartesianField::ShorePdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::ShorePdfCartesianField::ShorePdfCartesianField(
           gkg::RCPointer< gkg::Volume< float > > t2,
           gkg::RCPointer< gkg::Volume< float > > dw,
           gkg::RCPointer< gkg::Volume< int16_t > > mask,
           const gkg::BoundingBox< float >& boundingBox,
           const gkg::Vector3d< float >& superResolution,
           const gkg::Vector3d< int32_t >* outputCartesianGridSize,
           const gkg::Vector3d< double >* outputCartesianGridResolution,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose )
                            : gkg::PdfCartesianField(
                                                 t2, dw, mask,
                                                 boundingBox,
                                                 superResolution,
                                                 outputCartesianGridSize,
                                                 outputCartesianGridResolution )
{

  try
  {

    // extracting the input multiple different shell q-space sampling from
    // the DW volume header
    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create( *dw );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
            dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
      
      throw std::runtime_error(
                   "multiple shell q-space sampling required for SHORE model" );

    }
    else
    {

      _inputMultipleShellQSpaceSampling.reset( multipleShellQSpaceSampling );

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting PDF type
    _items.getHeader().addAttribute( "pdf_type",
                                     gkg::TypeOf< gkg::ShorePdf >::getName() );

    // updating displacement set cache
    gkg::DisplacementSetCache&
      displacementSetCache = gkg::DisplacementSetCache::getInstance();
    displacementSetCache.update( *outputCartesianGridSize,
                                 *outputCartesianGridResolution );

    // updating polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
    polarHarmonicsCache.update( _maximumPolarHarmonicsOrder );

    // setting PDF functor
    this->_functor = new gkg::ShorePdfCartesianFieldFunctor( *this );

    // computing PDFs
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::ShorePdfCartesianField::ShorePdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::ShorePdfCartesianField::ShorePdfCartesianField(
           gkg::RCPointer< gkg::Volume< float > > t2,
           gkg::RCPointer< gkg::Volume< float > > dw,
           gkg::RCPointer< gkg::Volume< int16_t > > mask,
           const gkg::BoundingBox< int32_t >& boundingBox,
           const gkg::Vector3d< int32_t >* outputCartesianGridSize,
           const gkg::Vector3d< double >* outputCartesianGridResolution,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose )
                             : gkg::PdfCartesianField(
                                                 t2, dw, mask,
                                                 boundingBox,
                                                 outputCartesianGridSize,
                                                 outputCartesianGridResolution )
{

  try
  {

    // extracting the input multiple different shell q-space sampling from
    // the DW volume header
    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create( *dw );

    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
            dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
      
      throw std::runtime_error(
                   "multiple shell q-space sampling required for SHORE model" );

    }
    else
    {

      _inputMultipleShellQSpaceSampling.reset( multipleShellQSpaceSampling );

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting PDF type
    _items.getHeader().addAttribute( "pdf_type",
                                     gkg::TypeOf< gkg::ShorePdf >::getName() );

    // updating displacement set cache
    gkg::DisplacementSetCache&
      displacementSetCache = gkg::DisplacementSetCache::getInstance();
    displacementSetCache.update( *outputCartesianGridSize,
                                 *outputCartesianGridResolution );

    // updating polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();
    polarHarmonicsCache.update( _maximumPolarHarmonicsOrder );

    // setting PDF functor
    this->_functor = new gkg::ShorePdfCartesianFieldFunctor( *this );

    // computing PDFs
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::ShorePdfCartesianField::ShorePdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::ShorePdfCartesianField::~ShorePdfCartesianField()
{
}


int32_t gkg::ShorePdfCartesianField::getMaximumPolarHarmonicsOrder() const
{

  try
  {

    return _maximumPolarHarmonicsOrder;

  }
  GKG_CATCH( "int32_t "
             "gkg::ShorePdfCartesianField::getMaximumPolarHarmonicsOrder() "
             "const" );

}


float gkg::ShorePdfCartesianField::getRegularizationLCurveFactor() const
{

  try
  {

    return _regularizationLCurveFactor;

  }
  GKG_CATCH( "float "
             "gkg::ShorePdfCartesianField::getRegularizationLCurveFactor() "
             "const" );

}


void gkg::ShorePdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( !specificStringParameters.empty() )
    {

      throw std::runtime_error( "no string parameter required" );

    }

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 2 );
      specificScalarParameters[ 0 ] = 6;
      specificScalarParameters[ 1 ] = 0.006;

    }

  }
  GKG_CATCH( "void gkg::ShorePdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}



void gkg::ShorePdfCartesianField::initialize( 
              const std::vector< double >& specificScalarParameters,
              const std::vector< std::string >& /* specificStringParameters */ )
{

  try
  {

    if ( specificScalarParameters.size() != 2U )
    {

      throw std::runtime_error( "2 scalar parameters required" );

    }
    _maximumPolarHarmonicsOrder =
                             ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );
    _regularizationLCurveFactor = ( float )specificScalarParameters[ 1 ];

  }
  GKG_CATCH( "void gkg::ShorePdfCartesianField::initialize( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters )" );

}



/*
void gkg::ShorePdfCartesianField::calculatingOnGridTmp(
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 bool verbose )

{

  try
  {

    int32_t sizeX = _t2->getSizeX();
    int32_t sizeY = _t2->getSizeY();
    int32_t sizeZ = _t2->getSizeZ();

    int32_t x, y, z;
    
    // counting the number of sites
    int32_t siteCount = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( ( *_mask )( x, y, z ) && boundingBox.contains( x, y, z ) )
          {

            ++ siteCount;

          }

        }

      }

    }

    // filling sites
    std::vector< gkg::Vector3d< int32_t > > sites( siteCount );

    // filling sites
    int32_t siteIndex = 0;
    
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {


          if ( ( *_mask )( x, y, z ) && boundingBox.contains( x, y, z ) )
          {

            gkg::Vector3d< int32_t >& site = sites[ siteIndex ];
            site.x = x;
            site.y = y;
            site.z = z;
            siteIndex ++;

          }

        }

      }

    }

    // adding sites to Cartesian field 
    this->CartesianField< gkg::ProbabilityDensityFunction >::_siteMap.addSites(
                                                                     0, sites );

    // creating (rank,site)<->index LUT
    this->CartesianField< gkg::ProbabilityDensityFunction >::_rankSiteLut = 
      gkg::RankSiteLut< int32_t, int32_t >(
                              this->CartesianField< gkg::ProbabilityDensityFunction >::_siteMap );

    // creating items and filling LUT
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = this->CartesianField<
              gkg::ProbabilityDensityFunction >::_siteMap.getSites( 0 ).begin(),
      se = this->CartesianField<
              gkg::ProbabilityDensityFunction >::_siteMap.getSites( 0 ).end();
    int32_t count = 0;
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << gkg::Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
 

      gkg::Vector3d< int32_t > site( s->x - boundingBox.getLowerX(),
                                     s->y - boundingBox.getLowerY(),
                                     s->z - boundingBox.getLowerZ() );

      int32_t index = this->CartesianField<
              gkg::ProbabilityDensityFunction >::_rankSiteLut.getIndex( 0,
                                                                        *s );

      gkg::ProbabilityDensityFunction&
        item = this->CartesianField< 
        gkg::ProbabilityDensityFunction >::_items.addTexture( index,
                                                              newItem(
                                                                     *s,
                                                                     site ) );
      this->CartesianField< gkg::ProbabilityDensityFunction >::_lut( site ) =
                                                                          &item;
      ++ count;
      ++ s;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 28 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::ShorePdfCartesianField::calculatingOnGridTmp( "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "bool verbose )" );

}
*/


gkg::ProbabilityDensityFunction
gkg::ShorePdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::Tensor* tensor = _tensorCartesianField->getItem( originSite );

    gkg::RCPointer< gkg::ShorePdf > shorePdf;
    if ( this->_hasMultipleShellOutputSampling )
    {

      shorePdf.reset( new gkg::ShorePdf( &_outputMultipleShellSampling,
                                         *_t2,
                                         *_dw,
                                         originSite,
                                         *_inputMultipleShellQSpaceSampling,
                                         _maximumPolarHarmonicsOrder,
                                         _regularizationLCurveFactor,
                                         *tensor ) );

    }
    else
    {

      shorePdf.reset( new gkg::ShorePdf( &_outputCartesianGridSize,
                                         &_outputCartesianGridResolution,
                                         *_t2,
                                         *_dw,
                                         originSite,
                                         *_inputMultipleShellQSpaceSampling,
                                         _maximumPolarHarmonicsOrder,
                                         _regularizationLCurveFactor,
                                         *tensor ) );

    }

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              *shorePdf );

    }
    else
    {

      return *shorePdf;

    }

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction "
             "gkg::ShorePdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::ProbabilityDensityFunction
gkg::ShorePdfCartesianField::newItem( const gkg::Vector3d< float >& originSite,
                                      const gkg::Vector3d< int32_t >& lutSite )
 
{

  try
  {

    gkg::Tensor* tensor = _tensorCartesianField->getItem( lutSite );


    gkg::RCPointer< gkg::ShorePdf > shorePdf;
    if ( this->_hasMultipleShellOutputSampling )
    {

      shorePdf.reset( new gkg::ShorePdf( &_outputMultipleShellSampling,
                                         *_t2,
                                         *_dw,
                                         originSite,
                                         *_inputMultipleShellQSpaceSampling,
                                         _maximumPolarHarmonicsOrder,
                                         _regularizationLCurveFactor,
                                         *tensor ) );

    }
    else
    {

      shorePdf.reset( new gkg::ShorePdf( &_outputCartesianGridSize,
                                         &_outputCartesianGridResolution,
                                         *_t2,
                                         *_dw,
                                         originSite,
                                         *_inputMultipleShellQSpaceSampling,
                                         _maximumPolarHarmonicsOrder,
                                         _regularizationLCurveFactor,
                                         *tensor ) );

    }

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              *shorePdf );

    }
    else
    {

      return *shorePdf;

    }

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction "
             "gkg::ShorePdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


