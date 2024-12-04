#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianField.h>
#include <gkg-dmri-microstructure-ivim/IvimOdf.h>
#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/CustomMultipleDifferentShellQSpaceSampling.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::IvimOdfCartesianField::IvimOdfCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                            : gkg::OdfCartesianField( t2, dw, mask,
                                                      outputOrientationSet )
{

  try
  {

    // initializing from specific scalar parameters
    initialize( specificScalarParameters,
                specificStringParameters,
                verbose );

    // extractiong multiple-shell information from DW header
    extractMultipleShellInformation( verbose );

    // extracting diffusion volumes belonging to the range :
    //     [ _ivimToParenchymaTransitionBValue;
    //       _parenchymaGaussianToNonGaussianTransitionBValue ]
    gkg::RCPointer< gkg::Volume< float > > gaussianPartDw;
    this->extractGaussianPartDw( gaussianPartDw, verbose );

    // computing tensor cartesian field
    if ( verbose )
    {

      std::cout << "computing tensor field from the parenchyma Gaussian part : "
                << std::flush;

    }
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _parenchymaTensorCartesianField.reset( new gkg::TensorCartesianField(
                                                    t2,
                                                    gaussianPartDw,
                                                    mask,
                                                    dtiSpecificScalarParameters,
                                                    dtiSpecificStringParameters,
                                                    verbose ) );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // setting ODF type and functor
    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::IvimOdf >::getName() );

    this->_functor = new gkg::IvimOdfCartesianFieldFunctor( *this );

    // computing ODFs
    if ( verbose )
    {

      std::cout << "computing IVIM field : "
                << std::flush;

    }
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "gkg::IvimOdfCartesianField::"
             "IvimOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::IvimOdfCartesianField::IvimOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< float >& boundingBox,
                 const gkg::Vector3d< float >& superResolution,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        superResolution,
                                                        outputOrientationSet )
{

  try
  {

    // initializing from specific scalar parameters
    initialize( specificScalarParameters,
                specificStringParameters,
                verbose );

    // extractiong multiple-shell information from DW header
    extractMultipleShellInformation( verbose );

    // extracting diffusion volumes belonging to the range :
    //     [ _ivimToParenchymaTransitionBValue;
    //       _parenchymaGaussianToNonGaussianTransitionBValue ]
    gkg::RCPointer< gkg::Volume< float > > gaussianPartDw;
    this->extractGaussianPartDw( gaussianPartDw, verbose );

    // computing tensor cartesian field
    if ( verbose )
    {

      std::cout << "computing tensor field from the parenchyma Gaussian part : "
                << std::flush;

    }
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _parenchymaTensorCartesianField.reset( new gkg::TensorCartesianField(
                                                    t2,
                                                    gaussianPartDw,
                                                    mask,
                                                    boundingBox,
                                                    superResolution,
                                                    dtiSpecificScalarParameters,
                                                    dtiSpecificStringParameters,
                                                    verbose ) );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // setting ODF type and functor
    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::IvimOdf >::getName() );

    this->_functor = new gkg::IvimOdfCartesianFieldFunctor( *this );

    // computing ODFs
    if ( verbose )
    {

      std::cout << "computing IVIM field : "
                << std::flush;

    }
    calculatingOnSubGrid( boundingBox, superResolution, verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "gkg::IvimOdfCartesianField::"
             "IvimOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::IvimOdfCartesianField::IvimOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        outputOrientationSet )
{

  try
  {

    // initializing from specific scalar parameters
    initialize( specificScalarParameters,
                specificStringParameters,
                verbose );

    // extractiong multiple-shell information from DW header
    extractMultipleShellInformation( verbose );

    // extracting diffusion volumes belonging to the range :
    //     [ _ivimToParenchymaTransitionBValue;
    //       _parenchymaGaussianToNonGaussianTransitionBValue ]
    gkg::RCPointer< gkg::Volume< float > > gaussianPartDw;
    this->extractGaussianPartDw( gaussianPartDw, verbose );


     // computing tensor cartesian field
    if ( verbose )
    {

      std::cout << "computing tensor field from the parenchyma Gaussian part : "
                << std::flush;

    }
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _parenchymaTensorCartesianField.reset( new gkg::TensorCartesianField(
                                                    t2,
                                                    gaussianPartDw,
                                                    mask,
                                                    boundingBox,
                                                    dtiSpecificScalarParameters,
                                                    dtiSpecificStringParameters,
                                                    verbose ) );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // setting ODF type and functor
    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::IvimOdf >::getName() );

    this->_functor = new gkg::IvimOdfCartesianFieldFunctor( *this );

    // computing ODFs
    if ( verbose )
    {

      std::cout << "computing IVIM field : "
                << std::flush;

    }
    calculatingOnGrid( boundingBox, verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "gkg::IvimOdfCartesianField::"
             "IvimOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::IvimOdfCartesianField::~IvimOdfCartesianField()
{
}



void gkg::IvimOdfCartesianField::checkOrInitializeDefaultParameters(
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

      // IVIM to parenchyma transition b-value
      specificScalarParameters[ 0 ] = 250.0; 
      // parenchyma Gaussian to non Gaussian transition b-value
      specificScalarParameters[ 1 ] = 1000.0;
      // initial fVIM volume fraction between 0.0 and 1.0
      specificScalarParameters[ 2 ] = 0.0;
      // initial IVIM diffusivity D* in m^2/s (typically 1e-8 m^2/s)
      specificScalarParameters[ 3 ] = 1.0e-8;
      // lower bound of IVIM volume fraction
      specificScalarParameters[ 4 ] = 0.0;
      // lower bound of IVIM diffusivity D*
      specificScalarParameters[ 5 ] = 0.0;
      // upper bound of IVIM  volume fraction
      specificScalarParameters[ 6 ] = 1.0;
      // upper bound of IVIM diffusivity D*
      specificScalarParameters[ 7 ] = 3.0e-8;
      // variation of IVIM  volume fraction
      specificScalarParameters[ 8 ] = 0.1;
      // variation of IVIM diffusivity D*
      specificScalarParameters[ 9 ] = 0.1e-8;
      // 1 for fixed IVIM volume fraction, 0 instead
      specificScalarParameters[ 10 ] = 0;
      // 1 for fixed IVIM diffusivity D*, 0 instead
      specificScalarParameters[ 11 ] = 0;
      // noise standard deviation of normalized signal
      specificScalarParameters[ 12 ] = 50.0;
      // NLP maximum iteration count
      specificScalarParameters[ 13 ] = 1000.0;
      // NLP maximum step size
      specificScalarParameters[ 14 ] = 0.001;

    }
    else
    {

      // check for scalar vector size ////////////////////////////////////////
      if ( specificScalarParameters.size() != 15 )
      {

        throw std::runtime_error( "invalid scalar parameter count" );

      }

      // IVIM to parenchyma transition b-value
      if ( specificScalarParameters[ 0 ] < 0.0 )
      {

        throw std::runtime_error(
                     "IVIM to parenchyma transition b-value must be positive" );

      }

      // parenchyma Gaussian to non Gaussian transition b-value
      if ( specificScalarParameters[ 1 ] < 0.0 )
      {

        throw std::runtime_error(
                    "parenchyma Gaussian to non Gaussian transition b-value "
                    "must be positive" );

      }

      if ( specificScalarParameters[ 1 ] < specificScalarParameters[ 0 ] )
      {

        throw std::runtime_error( 
                 "parenchyma Gaussian to non Gaussian transition b-value "
                 "must be greater than "
                 "IVIM to parenchyma transition b-value" );

      }


      // initial fVIM volume fraction between [<P5>;<P7>]
      if ( ( specificScalarParameters[ 2 ] < specificScalarParameters[ 4 ] ) ||
           ( specificScalarParameters[ 2 ] > specificScalarParameters[ 6 ] ) )
      {

        throw std::runtime_error(
                "initial IVIM volume fraction must belong to the range defined "
                "by parameters <P5> and <P7>" );

      }

      // initial IVIM diffusivity D* between [<P6>;<P8>]
      if ( ( specificScalarParameters[ 3 ] < specificScalarParameters[ 5 ] ) ||
           ( specificScalarParameters[ 3 ] > specificScalarParameters[ 7 ] ) )
      {

        throw std::runtime_error(
                "initial IVIM diffusivity D* must belong to the range defined"
                "by parameters <P6> and <P8>" );

      }

      // lower bound of IVIM volume fraction
      if ( ( specificScalarParameters[ 4 ] < 0.0 ) ||
           ( specificScalarParameters[ 4 ] > 1.0 ) )
      {

        throw std::runtime_error(
            "lower bound of IVIM volume fraction must be belong to [0.0;1.0]" );

      }

      // lower bound of IVIM diffusivity D*
      if ( specificScalarParameters[ 5 ] <= 0.0 )
      {

        throw std::runtime_error(
               "lower bound of IVIM diffusivity D* must be strictly positive" );

      }

      // upper bound of IVIM volume fraction
      if ( ( specificScalarParameters[ 6 ] < 0.0 ) ||
           ( specificScalarParameters[ 6 ] > 1.0 ) )
      {

        throw std::runtime_error(
            "upper bound of IVIM volume fraction must be belong to [0.0;1.0]" );

      }
      if ( specificScalarParameters[ 6 ] < specificScalarParameters[ 4 ] )
      {

        throw std::runtime_error(
                      "upper bound of IVIM volume fraction must be greater than"
                      " lower bound of IVIM volume fraction" );

      }

      // upper bound of IVIM diffusivity D*
      if ( specificScalarParameters[ 7 ] <= 0.0 )
      {

        throw std::runtime_error(
            "upper bound of IVIM diffusivity D* must be strictly positive" );

      }
      if ( specificScalarParameters[ 7 ] < specificScalarParameters[ 5 ] )
      {

        throw std::runtime_error(
                       "upper bound of IVIM diffusivity D* must be greater than"
                       " lower bound of IVIM diffusivity D*" );

      }

      // variation of IVIM volume fraction
      if ( specificScalarParameters[ 8 ] < 0.0 )
      {

        throw std::runtime_error(
                         "variation of IVIM volume fraction must be positive" );

      }
      if ( specificScalarParameters[ 8 ] >
           ( specificScalarParameters[ 6 ] - specificScalarParameters[ 4 ] ) )
      {

        throw std::runtime_error(
                        "variation of IVIM volume fraction must be smaller than"
                        " its lower to upper bound span" );

      }


      // variation of IVIM diffusivity D*
      if ( specificScalarParameters[ 9 ] < 0.0 )
      {

        throw std::runtime_error(
                          "variation of IVIM diffusivity D* must be positive" );

      }
      if ( specificScalarParameters[ 9 ] >
           ( specificScalarParameters[ 7 ] - specificScalarParameters[ 5 ] ) )
      {

        throw std::runtime_error(
                         "variation of IVIM diffusivity D* must be smaller than"
                         " its lower to upper bound span" );

      }

      // 1 for fixed IVIM volume fraction, 0 instead
      if ( ( ( int32_t )( specificScalarParameters[ 10 ] + 0.5 ) != 0 ) &&
           ( ( int32_t )( specificScalarParameters[ 10 ] + 0.5 ) != 1 ) )
      {

        throw std::runtime_error( "fixed IVIM volume fraction must be 0 or 1" );

      }           

      // 1 for fixed IVIM diffusivity D*, 0 instead
      if ( ( ( int32_t )( specificScalarParameters[ 11 ] + 0.5 ) != 0 ) &&
           ( ( int32_t )( specificScalarParameters[ 11 ] + 0.5 ) != 1 ) )
      {

        throw std::runtime_error( "fixed IVIM diffusivity D* must be 0 or 1" );

      }           

      // noise standard deviation of normalized signal
      if ( specificScalarParameters[ 12 ] <= 0.0 )
      {

        throw std::runtime_error(
                "noise standard deviation of normalized signal must be strictly"
                " positive" );

      }

      // NLP maximum iteration count
      if ( specificScalarParameters[ 13 ] < 1.0 )
      {

        throw std::runtime_error(
                      "NLP maximum iteration count must be strictly positive" );

      }

      // NLP maximum step size
      if ( specificScalarParameters[ 14 ] <= 0.0 )
      {

        throw std::runtime_error(
                            "NLP maximum step size must be strictly positive" );

      }

    }


  }
  GKG_CATCH( "void gkg::IvimOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::IvimOdfCartesianField::initialize( 
              const std::vector< double >& specificScalarParameters,
              const std::vector< std::string >& /* specificStringParameters */,
              bool verbose )
{

  try
  {

    // collecting b-value boundaries of Gaussian parenchyma diffusion process
    _ivimToParenchymaTransitionBValue = ( float )specificScalarParameters[ 0 ];
    _parenchymaGaussianToNonGaussianTransitionBValue =
                                        ( float )specificScalarParameters[ 1 ];

    // setting parameters for NLM optimizer
    _initialParameters.reallocate( 2 );
    _initialParameters( 0 ) = specificScalarParameters[ 2 ];
    _initialParameters( 1 ) = specificScalarParameters[ 3 ];

    _noiseVariance = specificScalarParameters[ 12 ];

    _optimizerConstraints.resize( 2U );

    _optimizerConstraints[ 0 ].reset(
                                   new gkg::ClosedIntervalOptimizerConstraint(
                                              specificScalarParameters[ 4 ],
                                              specificScalarParameters[ 6 ] ) );
    _optimizerConstraints[ 1 ].reset(
                                   new gkg::ClosedIntervalOptimizerConstraint(
                                              specificScalarParameters[ 5 ],
                                              specificScalarParameters[ 7 ] ) );

    _nlpOptimizerParameters.resize( 4U );
    _nlpOptimizerParameters[ 0 ] = specificScalarParameters[ 13 ];
    _nlpOptimizerParameters[ 1 ] = specificScalarParameters[ 14 ];
    _nlpOptimizerParameters[ 2 ] =
      ( ( ( int32_t )( specificScalarParameters[ 10 ] + 0.5 ) ) ? 
        0.0 :
        _optimizerConstraints[ 0 ]->constrainedDeltaParameter(
                                              specificScalarParameters[ 8 ] ) );
    _nlpOptimizerParameters[ 3 ] =
      ( ( ( int32_t )( specificScalarParameters[ 11 ] + 0.5 ) ) ? 
        0.0 :
        _optimizerConstraints[ 1 ]->constrainedDeltaParameter(
                                              specificScalarParameters[ 9 ] ) );


    if ( verbose )
    {

      std::cout << "IVIM to parenchyma transition b-value : "
                << _ivimToParenchymaTransitionBValue
                << " s/mm2"
                << std::endl;
      std::cout << "parenchyma Gaussian to non-Gaussian transition b-value : "
                << _parenchymaGaussianToNonGaussianTransitionBValue
                << " s/mm2"
                << std::endl;
      std::cout << "noise variance : " << _noiseVariance << std::endl;
      std::cout << "IVIM volume fraction : "
                << "initial_value="
                << _initialParameters( 0 )
                << " lower_bound="
                << specificScalarParameters[ 4 ]
                << " upper_bound="
                << specificScalarParameters[ 6 ]
                << " fixed_value="
                << ( ( int32_t )( specificScalarParameters[ 10 ] + 0.5 ) )
                << std::endl;
      std::cout << "IVIM diffusivity D* : "
                << "initial_value="
                << _initialParameters( 1 )
                << " lower_bound="
                << specificScalarParameters[ 5 ]
                << " upper_bound="
                << specificScalarParameters[ 7 ]
                << " fixed_value="
                << ( ( int32_t )( specificScalarParameters[ 11 ] + 0.5 ) )
                << std::endl;
      std::cout << "NLP optimizer maximum iteration count : "
                << ( int32_t )( specificScalarParameters[ 13 ] + 0.5 )
                << std::endl;
      std::cout << "NLP optimizer step size : "
                << specificScalarParameters[ 14 ]
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::IvimOdfCartesianField::initialize( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


void gkg::IvimOdfCartesianField::extractMultipleShellInformation( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "extracting multiple-shell information : " << std::endl;

    }

    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create(
                                                             *( this->_dw ) );
    gkg::CustomMultipleDifferentShellQSpaceSampling*
      customMultipleDifferentShellQSpaceSampling =
          dynamic_cast< gkg::CustomMultipleDifferentShellQSpaceSampling* >(
                                                               qSpaceSampling );
    if ( !customMultipleDifferentShellQSpaceSampling )
    {
 
      throw std::runtime_error(
             "qspace sampling is not of type custom multiple different shell" );

    }

    gkg::RCPointer< gkg::CustomMultipleDifferentShellQSpaceSampling >
      inputMultipleShellQSpaceSampling(
                                   customMultipleDifferentShellQSpaceSampling );

    // collecting the shell count
    _shellCount = inputMultipleShellQSpaceSampling->getShellCount();

    // resizing the b-value, index offset and orientation count vectors
    _shellBValues.resize( _shellCount );
    _shellIndexOffsets.resize( _shellCount, 0 );
    _shellOrientationCounts.resize( _shellCount );
    _shellOrientationSets.resize( _shellCount );

    // filling the information
    int32_t shellIndex = 0;
    for ( shellIndex = 0; shellIndex < _shellCount; shellIndex++ )
    {

      _shellBValues[ shellIndex ] = 
                     inputMultipleShellQSpaceSampling->getBValue( shellIndex );
      _shellOrientationSets[ shellIndex ] = 
                     inputMultipleShellQSpaceSampling->getOrientationSet(
                                                                  shellIndex );
      _shellOrientationCounts[ shellIndex ] = 
                                _shellOrientationSets[ shellIndex ].getCount();
      if ( shellIndex < _shellCount - 1 )
      {

        _shellIndexOffsets[ shellIndex + 1 ] += 
                                          _shellIndexOffsets[ shellIndex ] + 
                                          _shellOrientationCounts[ shellIndex ];

      }

    }

    // looking for the 2 shell indices bouding the Gaussian to non-Gaussian
    // parenchyma signal
    _ivimToParenchymaTransitionShellIndex = -1;
    _parenchymaGaussianToNonGaussianTransitionShellIndex = -1;
    for ( shellIndex = 0; shellIndex < _shellCount - 1; shellIndex++ )
    {

      if ( ( _shellBValues[ shellIndex ] < 
             _ivimToParenchymaTransitionBValue ) &&
           ( _shellBValues[ shellIndex + 1 ] >= 
             _ivimToParenchymaTransitionBValue ) )
      {

        _ivimToParenchymaTransitionShellIndex = shellIndex + 1;

      }
           
      if ( ( _shellBValues[ shellIndex ] <= 
             _parenchymaGaussianToNonGaussianTransitionBValue ) &&
           ( _shellBValues[ shellIndex + 1 ] > 
             _parenchymaGaussianToNonGaussianTransitionBValue ) )
      {

        _parenchymaGaussianToNonGaussianTransitionShellIndex = shellIndex;

      }

    }

    if ( _ivimToParenchymaTransitionShellIndex == -1 )
    {


      throw std::runtime_error( "unable to identify the IVIM to parenchyma "
                                "transition shell index" );

    }
    if ( _parenchymaGaussianToNonGaussianTransitionShellIndex == -1 )
    {


      throw std::runtime_error( "unable to identify parenchyma Gaussian to "
                                "non-Gaussian transition shell index" );

    }

    if ( verbose )
    {


     std::cout << "  - b-values : ";
     for ( shellIndex = 0; shellIndex < _shellCount; shellIndex++ )
     {

       std::cout << _shellBValues[ shellIndex ] << ' ';

     }
     std::cout << std::endl;
     std::cout << "  - IVIM to parenchyma transition shell : "
               << "index="
               << _ivimToParenchymaTransitionShellIndex
               << "->b-value="
               << _shellBValues[ _ivimToParenchymaTransitionShellIndex ]
               << "s/mm2"
               << std::endl;
     std::cout << "  - parenchyma Gaussian to non-Gaussian transition shell : "
               << "index="
               << _parenchymaGaussianToNonGaussianTransitionShellIndex
               << "->b-value="
               << _shellBValues[
                         _parenchymaGaussianToNonGaussianTransitionShellIndex ]
               << "s/mm2"
               << std::endl;

    }

    // also collecting raw Q-space information to be used by the next method
    // extractGaussianPartDw()
    _rawBValues = inputMultipleShellQSpaceSampling->getRawBValues();
    _rawOrientations = inputMultipleShellQSpaceSampling->getRawOrientations();
    _deltaBValue = inputMultipleShellQSpaceSampling->getDeltaBValue();
    _homogeneousTransform3d = inputMultipleShellQSpaceSampling->
                                                            getRawTransform3d();
    _gradientCharacteristics = inputMultipleShellQSpaceSampling->
                                                getRawGradientCharacteristics();
    
    _rawDiffusionTimes = 
                       inputMultipleShellQSpaceSampling->getRawDiffusionTimes();
    _rawMotionRotation3ds =
                    inputMultipleShellQSpaceSampling->getRawMotionRotation3ds();


  }
  GKG_CATCH( "void gkg::IvimOdfCartesianField::"
             "extractMultipleShellInformation( bool verbose )" );

}


void gkg::IvimOdfCartesianField::extractGaussianPartDw(
                         gkg::RCPointer< gkg::Volume< float > >& gaussianPartDw,
                         bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "extracting Gaussian part of the DW volume : " << std::flush;

    }

    // collecting input DW volume size and resolution
    gkg::Vector3d< int32_t > size;
    this->_dw->getSize( size );
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    double resolutionT = 1.0;
    this->_dw->getResolution( resolution, resolutionT );

    // computing the T size of the Gaussian part DW volume
    int32_t sizeT = 0;
    int32_t shellIndex = 0;
    for ( shellIndex = _ivimToParenchymaTransitionShellIndex;
          shellIndex <= _parenchymaGaussianToNonGaussianTransitionShellIndex;
          shellIndex++ )
    {

      sizeT += _shellOrientationCounts[ shellIndex ];

    }

    // creating the output Gaussian part DW volume
    gaussianPartDw.reset( new gkg::Volume< float >( size, sizeT ) );
    gaussianPartDw->setResolution( resolution, resolutionT );

    // computing the offset to the first and last+1 b-value/orientation to be
    // taken into account
    int32_t firstRawOffset = 0;
    int32_t lastPlusOneRawOffset = 0;

    for ( shellIndex = 0;
          shellIndex < _ivimToParenchymaTransitionShellIndex;
          shellIndex++ )
    {

      firstRawOffset += _shellOrientationCounts[ shellIndex ];

    }
    for ( shellIndex = 0;
          shellIndex <= _parenchymaGaussianToNonGaussianTransitionShellIndex;
          shellIndex++ )
    {

      lastPlusOneRawOffset += _shellOrientationCounts[ shellIndex ];

    }


    // copying the Gaussian part DW data
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < size.z; z++ )
      {

        for ( y = 0; y < size.y; y++ )
        {

          for ( x = 0; x < size.x; x++ )
          {

            ( *gaussianPartDw )( x, y, z, t ) =
                                  ( *this->_dw )( x, y, z, t + firstRawOffset );

          }

        }

      }

    }

    // extracting the q-space information from the input DW data q-space
    std::vector< float >
      gaussianPartBValues( _rawBValues.begin() + firstRawOffset,
                           _rawBValues.begin() + lastPlusOneRawOffset );
    std::vector< gkg::Vector3d< float > >
      gaussianPartOrientations( _rawOrientations.begin() + firstRawOffset,
                                _rawOrientations.begin() + 
                                lastPlusOneRawOffset );

    gkg::CustomMultipleDifferentShellQSpaceSampling
      gaussianPartQSpaceSampling( gaussianPartBValues,
                                  gaussianPartOrientations,
                                  _deltaBValue,
                                  *_homogeneousTransform3d,
                                  _gradientCharacteristics );

    // adding the new q-space information to the Gaussian part DW volume
    gaussianPartQSpaceSampling.addInformationToHeader( *gaussianPartDw );


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::IvimOdfCartesianField::extractGaussianPartDw( "
             "const gkg::RCPointer< gkg::Volume< float > >& dw, "
             "gkg::RCPointer< gkg::Volume< float > >& gaussianPartDw ) const" );


}


gkg::OrientationDistributionFunction 
gkg::IvimOdfCartesianField::newItem( const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {
   
    gkg::IvimOdf 
      ivimOdf( &_outputOrientationSet,
               *_t2, *_dw,
               originSite,
               *_parenchymaTensorCartesianField->getItem( originSite ),
               _shellCount,
               _shellBValues,
               _shellIndexOffsets,
               _shellOrientationCounts,
               _ivimToParenchymaTransitionShellIndex,
               _parenchymaGaussianToNonGaussianTransitionShellIndex,
               _initialParameters,
               _noiseVariance,
               _optimizerConstraints,
               _nlpOptimizerParameters );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              ivimOdf );

    }
    else
    {

      return ivimOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::IvimOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::IvimOdfCartesianField::newItem( const gkg::Vector3d< float >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::IvimOdf
      ivimOdf( &_outputOrientationSet,
               *_t2, *_dw,
               originSite,
               *_parenchymaTensorCartesianField->getItem( lutSite ),
               _shellCount,
               _shellBValues,
               _shellIndexOffsets,
               _shellOrientationCounts,
               _ivimToParenchymaTransitionShellIndex,
               _parenchymaGaussianToNonGaussianTransitionShellIndex,
               _initialParameters,
               _noiseVariance,
               _optimizerConstraints,
               _nlpOptimizerParameters );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              ivimOdf );

    }
    else
    {

      return ivimOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::IvimOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
