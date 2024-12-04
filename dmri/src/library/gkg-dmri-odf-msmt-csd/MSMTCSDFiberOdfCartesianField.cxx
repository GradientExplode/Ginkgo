#include <gkg-dmri-odf-msmt-csd/MSMTCSDFiberOdfCartesianField.h>
#include <gkg-dmri-odf-msmt-csd/MSMTCSDFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


#include <gkg-core-io/Writer_i.h>


#define DEFAULT_POSITIVE_CONSTRAINT_ORIENTATION_COUNT 300



gkg::MSMTCSDFiberOdfCartesianField::MSMTCSDFiberOdfCartesianField(
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const gkg::OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose )
                                   : gkg::OdfCartesianField(
                                                          t2, dw, mask,
                                                          outputOrientationSet )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // preparing from specific scalar and string parameters
    ////////////////////////////////////////////////////////////////////////////

    this->prepare( specificScalarParameters,
                   specificStringParameters,
                   verbose );


    ////////////////////////////////////////////////////////////////////////////
    // computing ODFs
    ////////////////////////////////////////////////////////////////////////////

    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::MSMTCSDFiberOdfCartesianField::"
             "MSMTCSDFiberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::MSMTCSDFiberOdfCartesianField::MSMTCSDFiberOdfCartesianField(
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
                                                          boundingBox,
                                                          superResolution,
                                                          outputOrientationSet )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // preparing from specific scalar and string parameters
    ////////////////////////////////////////////////////////////////////////////

    this->prepare( specificScalarParameters,
                   specificStringParameters,
                   verbose );


    ////////////////////////////////////////////////////////////////////////////
    // computing ODFs
    ////////////////////////////////////////////////////////////////////////////

    calculatingOnSubGrid( boundingBox, superResolution, verbose );



  }
  GKG_CATCH( "gkg::MSMTCSDFiberOdfCartesianField::"
             "MSMTCSDFiberOdfCartesianField( "
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


gkg::MSMTCSDFiberOdfCartesianField::MSMTCSDFiberOdfCartesianField(
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    RCPointer< gkg::Volume< int16_t > > mask,
                    const gkg::BoundingBox< int32_t >& boundingBox,
                    const gkg::OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose )
                                   : gkg::OdfCartesianField(
                                                          t2, dw, mask,
                                                          boundingBox,
                                                          outputOrientationSet )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // preparing from specific scalar and string parameters
    ////////////////////////////////////////////////////////////////////////////

    this->prepare( specificScalarParameters,
                   specificStringParameters,
                   verbose );


    ////////////////////////////////////////////////////////////////////////////
    // computing ODFs
    ////////////////////////////////////////////////////////////////////////////

    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::MSMTCSDFiberOdfCartesianField::"
             "MSMTCSDFiberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::MSMTCSDFiberOdfCartesianField::~MSMTCSDFiberOdfCartesianField()
{
}


int32_t gkg::MSMTCSDFiberOdfCartesianField::getTissueCount() const
{

  try
  {

    return _tissueCount;


  }
  GKG_CATCH( "int32_t "
             "gkg::MSMTCSDFiberOdfCartesianField::getTissueCount() const" );

}


int32_t gkg::MSMTCSDFiberOdfCartesianField::getMaximumSphericalHarmonicOrder(
                                                     int32_t tissueIndex ) const
{

  try
  {

    return _maximumSphericalHarmonicOrders[ tissueIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::MSMTCSDFiberOdfCartesianField::"
             "getMaximumSphericalHarmonicOrder( int32_t tissueIndex ) const" );

}


int32_t 
gkg::MSMTCSDFiberOdfCartesianField::getSphericalHarmonicCoefficientCount(
                                                     int32_t tissueIndex ) const
{

  try
  {

    return _shCoefficientCounts[ tissueIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::MSMTCSDFiberOdfCartesianField::"
             "getSphericalHarmonicCoefficientCount( "
             "int32_t tissueIndex ) const" );

}


int32_t gkg::MSMTCSDFiberOdfCartesianField::
                                 getAllSphericalHarmonicCoefficientCount() const
{

  try
  {

    int32_t allSphericalHarmoniCoefficientCount = 0;
    int32_t tissueIndex = 0;
    for ( tissueIndex = 0; tissueIndex < _tissueCount; tissueIndex++ )
    {

      allSphericalHarmoniCoefficientCount +=
                                            _shCoefficientCounts[ tissueIndex ];

    }

    return allSphericalHarmoniCoefficientCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::MSMTCSDFiberOdfCartesianField::"
             "getAllSphericalHarmonicCoefficientCount() const" );

}


void gkg::MSMTCSDFiberOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // setting or checking the specific scalar parameters
    ////////////////////////////////////////////////////////////////////////////

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 26 );
      specificScalarParameters[  0 ] = 3;     // tissue count
      specificScalarParameters[  1 ] = 100;   // CQP solver max iteration count
      specificScalarParameters[  2 ] = 1e-10; // CQP solver epsilon gap
      specificScalarParameters[  3 ] = 1e-10; // CQP solver epsilon residuals
      specificScalarParameters[  4 ] = 1e-10; // CQP solver epsilon infeasible
      specificScalarParameters[  5 ] = 300;   // WM kernel wanted voxel count
      specificScalarParameters[  6 ] = 0.9;   // WM class tissue threshold
      specificScalarParameters[  7 ] = 0.0;   // not used
      specificScalarParameters[  8 ] = 0.8;   // WM class FA threshold
      specificScalarParameters[  9 ] = 0.0;   // not used
      specificScalarParameters[ 10 ] = 8;     // WM spherical harmonics order

      specificScalarParameters[ 11 ] = 300;   // GM kernel wanted voxel count
      specificScalarParameters[ 12 ] = 0.9;   // GM class tissue threshold
      specificScalarParameters[ 13 ] = 0.0;   // not used
      specificScalarParameters[ 14 ] = 0.2;   // GM class FA threshold
      specificScalarParameters[ 15 ] = 0.0;   // not used
      specificScalarParameters[ 16 ] = 0;     // GM spherical harmonics order

      specificScalarParameters[ 17 ] = 300;   // CSF kernel wanted voxel count
      specificScalarParameters[ 18 ] = 0.9;   // CSF class tissue threshold
      specificScalarParameters[ 19 ] = 0.0;   // not used
      specificScalarParameters[ 20 ] = 0.1;   // CSF class FA threshold
      specificScalarParameters[ 21 ] = 0.0;   // not used
      specificScalarParameters[ 22 ] = 0;     // CSF spherical harmonics order


    }
    else
    {

      int32_t tissueCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );
      int32_t expectedScalarParameterCount = 5 + 6 * tissueCount;

      if ( ( int32_t )specificScalarParameters.size() !=
           expectedScalarParameterCount )
      {

        throw std::runtime_error( 
          std::string( "bad number of specific scalar parameters, " ) +
          gkg::StringConverter::toString( expectedScalarParameterCount ) + 
          " expected whereas " +
          gkg::StringConverter::toString(
                                  ( int32_t )specificScalarParameters.size() ) +
          " are provided" );

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // setting or checking the specific string parameters
    ////////////////////////////////////////////////////////////////////////////

    if ( specificStringParameters.empty() )
    {


      throw std::runtime_error( 
                          "filling specific string parameters is mandatory for "
                          " the multi-shell multi-tissue CSD model" );

    }
    else
    {

      if ( specificStringParameters.size() < 6U )
      {

        throw std::runtime_error( "at least 6 string parameters are required" );

      }

      int32_t tissueCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );


      int32_t tissueIndex = 0;
      int32_t expectedStringParameterCount = 0;
      for ( tissueIndex = 0; tissueIndex < tissueCount; tissueIndex++ )
      {

        // tissue class probability map filename
        expectedStringParameterCount += 1;

        // tissue to diffusion 3D transform type
        std::string transform3dType = specificStringParameters[ 
                                                 expectedStringParameterCount ];
        expectedStringParameterCount += 1;

        if ( ( transform3dType != "none" ) &&
             ( transform3dType != "linear" ) &&
             ( transform3dType != "linear_and_diffeomorphic" ) )
        {

          throw std::runtime_error(
                std::string( "bad tissue to diffusion 3D transform type "
                             "for tissue class " ) +
                gkg::StringConverter::toString( tissueIndex ) );

        }

        if ( transform3dType == "linear" )
        {

          // tissue to diffusion 3D linear transform
          expectedStringParameterCount += 1;

        }
        else if ( transform3dType == "linear_and_diffeomorphic" )
        {

          // tissue to diffusion 3D linear transform
          // tissue to diffusion direct 3D diffeomorph transform
          // tissue to diffusion inverse 3D diffeomorph transform
          expectedStringParameterCount += 3;

        }

        // kernel type (symmetric_tensor)
        // tissue class threshdolding mode
        // FA threshdolding mode
        // FA sorting type (increasing or decreasing)
        expectedStringParameterCount += 4;

      }           


      if ( ( int32_t )specificStringParameters.size() !=
           expectedStringParameterCount )
      {

        throw std::runtime_error( 
          std::string( "bad number of specific string parameters, " ) +
          gkg::StringConverter::toString( expectedStringParameterCount ) + 
          " expected whereas " +
          gkg::StringConverter::toString(
                                  ( int32_t )specificStringParameters.size() ) +
          " are provided" );

      }

    }

  }
  GKG_CATCH( "void gkg::MSMTCSDFiberOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::MSMTCSDFiberOdfCartesianField::prepare(
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // decoding specific scalar parameters
    ////////////////////////////////////////////////////////////////////////////

    _tissueCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );

    if ( _tissueCount < 1 )
    {

      throw std::runtime_error( "at least 1 tissue is required" );

    }

    _cqpSolverMaximumIterationCount = specificScalarParameters[ 1 ];
    if ( _cqpSolverMaximumIterationCount < 0 )
    {

      throw std::runtime_error(
                "CQP solver requires a positive maximum number of iterations" );

    }
    _cqpSolverEpsilonGap = specificScalarParameters[ 2 ];
    if ( _cqpSolverEpsilonGap < 0.0 )
    {

      throw std::runtime_error(
                                 "CQP solver requires a positive epsilon gap" );

    }
    _cqpSolverEpsilonResiduals = specificScalarParameters[ 3 ];
    if ( _cqpSolverEpsilonResiduals < 0.0 )
    {

      throw std::runtime_error(
                           "CQP solver requires a positive epsilon residuals" );

    }
    _cqpSolverEpsilonInfeasible = specificScalarParameters[ 4 ];
    if ( _cqpSolverEpsilonInfeasible < 0.0 )
    {

      throw std::runtime_error(
                          "CQP solver requires a positive epsilon infeasible" );

    }
    std::cout << std::endl;
    std::cout << "_tissueCount=" << _tissueCount << std::endl;
    std::cout << "_cqpSolverMaximumIterationCount=" << _cqpSolverMaximumIterationCount << std::endl;
    std::cout << "_cqpSolverEpsilonGap=" << _cqpSolverEpsilonGap << std::endl;
    std::cout << "_cqpSolverEpsilonResiduals=" << _cqpSolverEpsilonResiduals << std::endl;
    std::cout << "_cqpSolverEpsilonInfeasible=" << _cqpSolverEpsilonInfeasible << std::endl;

    _kernelWantedVoxelCounts.resize( _tissueCount );
    _firstTissueClassThresholds.resize( _tissueCount );
    _secondTissueClassThresholds.resize( _tissueCount );
    _firstFAThresholds.resize( _tissueCount );
    _secondFAThresholds.resize( _tissueCount );
    _maximumSphericalHarmonicOrders.resize( _tissueCount );

    int32_t tissueIndex = 0;
    for ( tissueIndex = 0; tissueIndex < _tissueCount; tissueIndex++ )
    {

    std::cout << "tissueIndex=" << tissueIndex << std::endl;

      _kernelWantedVoxelCounts[ tissueIndex ] =
           ( int32_t )( specificScalarParameters[ 5 + 6 * tissueIndex ] + 0.5 );
      if ( _kernelWantedVoxelCounts[ tissueIndex ] < 0 )
      {

        throw std::runtime_error(
          std::string( "bad tissue class kernel wanted voxel count for " ) +
          "tissue class " +
          gkg::StringConverter::toString( tissueIndex ) );

      }


      _firstTissueClassThresholds[ tissueIndex ] =
                                specificScalarParameters[ 6 + 6 * tissueIndex ];
      _secondTissueClassThresholds[ tissueIndex ] =
                                specificScalarParameters[ 7 + 6 * tissueIndex ];

      if ( ( _firstTissueClassThresholds[ tissueIndex ] < 0.0 ) ||
           ( _secondTissueClassThresholds[ tissueIndex ] > 1.0 ) )
      {

        throw std::runtime_error(
                std::string( "bad tissue class thresholds for tissue class " ) +
                gkg::StringConverter::toString( tissueIndex ) );

      }

      _firstFAThresholds[ tissueIndex ] =
                                specificScalarParameters[ 8 + 6 * tissueIndex ];
      _secondFAThresholds[ tissueIndex ] =
                                specificScalarParameters[ 9 + 6 * tissueIndex ];

      if ( ( _firstFAThresholds[ tissueIndex ] < 0.0 ) ||
           ( _secondFAThresholds[ tissueIndex ] > 1.0 ) )
      {

        throw std::runtime_error(
                std::string( "bad FA thresholds for tissue class " ) +
                gkg::StringConverter::toString( tissueIndex ) );

      }


      _maximumSphericalHarmonicOrders[ tissueIndex ] =
          ( int32_t )( specificScalarParameters[ 10 + 6 * tissueIndex ] + 0.5 );

      if ( ( _maximumSphericalHarmonicOrders[ tissueIndex ] < 0 ) ||
           ( _maximumSphericalHarmonicOrders[ tissueIndex ] % 2 ) )
      {

        throw std::runtime_error(
                std::string( "bad maximum spherical harmonics order for "
                             "tissue class " ) +
                gkg::StringConverter::toString( tissueIndex ) );

      }


    std::cout << "_kernelWantedVoxelCounts[ tissueIndex ]=" << _kernelWantedVoxelCounts[ tissueIndex ] << std::endl;
    std::cout << "_firstTissueClassThresholds[ tissueIndex ]=" << _firstTissueClassThresholds[ tissueIndex ] << std::endl;
    std::cout << "_secondTissueClassThresholds[ tissueIndex ]=" << _secondTissueClassThresholds[ tissueIndex ] << std::endl;
    std::cout << "_firstFAThresholds[ tissueIndex ]=" << _firstFAThresholds[ tissueIndex ] << std::endl;
    std::cout << "_secondFAThresholds[ tissueIndex ]=" << _secondFAThresholds[ tissueIndex ] << std::endl;
    std::cout << "_maximumSphericalHarmonicOrders[ tissueIndex ]=" << _maximumSphericalHarmonicOrders[ tissueIndex ] << std::endl;


    }

    ////////////////////////////////////////////////////////////////////////////
    // decoding specific string parameters
    ////////////////////////////////////////////////////////////////////////////


    std::vector< gkg::DiffusionKernel::TensorType >
      kernelTensorTypes( _tissueCount );
    std::vector< gkg::DiffusionKernel::FractionalAnisotropySortingType >
      kernelFAStortingTypes( _tissueCount );

    _fileNameTissueClassProbabilityMaps.resize( _tissueCount );
    _tissueToDiffusion3DTransformTypes.resize( _tissueCount );
    _fileNameTissueToDiffusion3DLinearTransforms.resize( _tissueCount );
    _fileNameTissueToDiffusionDirect3DDiffeoTransforms.resize( _tissueCount );
    _fileNameTissueToDiffusionInverse3DDiffeoTransforms.resize( _tissueCount );
    _kernelTypes.resize( _tissueCount );
    _tissueClassThresholdingModes.resize( _tissueCount );
    _faThresholdingModes.resize( _tissueCount );
    _faSortingTypes.resize( _tissueCount );

    int32_t offset = 0;
    for ( tissueIndex = 0; tissueIndex < _tissueCount; tissueIndex++ )
    {

      std::cout << "tissueIndex=" << tissueIndex << std::endl;

      // tissue class probability map filename
      _fileNameTissueClassProbabilityMaps[ tissueIndex ] = 
                                             specificStringParameters[ offset ];
      offset += 1;

      // tissue to diffusion 3D transform type
      _tissueToDiffusion3DTransformTypes[ tissueIndex ] =
                                             specificStringParameters[ offset ];
      offset += 1;

      std::cout << "_fileNameTissueClassProbabilityMaps[ tissueIndex ]=" 
                << _fileNameTissueClassProbabilityMaps[ tissueIndex ]
                << std::endl;
      std::cout << "_tissueToDiffusion3DTransformTypes[ tissueIndex ]="
                << _tissueToDiffusion3DTransformTypes[ tissueIndex ]
                << std::endl;


      if ( _tissueToDiffusion3DTransformTypes[ tissueIndex ] == "linear" )
      {

        // tissue to diffusion 3D linear transform
        _fileNameTissueToDiffusion3DLinearTransforms[ tissueIndex ] =
                                             specificStringParameters[ offset ];
        _fileNameTissueToDiffusionDirect3DDiffeoTransforms[ tissueIndex ] = "";
        _fileNameTissueToDiffusionInverse3DDiffeoTransforms[ tissueIndex ] = "";

        offset += 1;

      }
      else if ( _tissueToDiffusion3DTransformTypes[ tissueIndex ] ==
                                                    "linear_and_diffeomorphic" )
      {

        // tissue to diffusion 3D linear transform
        // tissue to diffusion direct 3D diffeomorph transform
        // tissue to diffusion inverse 3D diffeomorph transform
        _fileNameTissueToDiffusion3DLinearTransforms[ tissueIndex ] =
                                             specificStringParameters[ offset ];
        offset += 1;
        _fileNameTissueToDiffusionDirect3DDiffeoTransforms[ tissueIndex ] =
                                             specificStringParameters[ offset ];
        offset += 1;
        _fileNameTissueToDiffusionInverse3DDiffeoTransforms[ tissueIndex ] =
                                             specificStringParameters[ offset ];
        offset += 1;

      }

      std::cout << "_fileNameTissueToDiffusion3DLinearTransforms[ tissueIndex ]="
                << _fileNameTissueToDiffusion3DLinearTransforms[ tissueIndex ]
                << std::endl;
      std::cout << "_fileNameTissueToDiffusionDirect3DDiffeoTransforms[ tissueIndex ]="
                << _fileNameTissueToDiffusionDirect3DDiffeoTransforms[ tissueIndex ]
                << std::endl;
      std::cout << "_fileNameTissueToDiffusionInverse3DDiffeoTransforms[ tissueIndex ]="
                << _fileNameTissueToDiffusionInverse3DDiffeoTransforms[ tissueIndex ]
                << std::endl;







      // kernel type (symmetric_tensor)
      _kernelTypes[ tissueIndex ] = specificStringParameters[ offset ];
      offset += 1;
      std::cout << "_kernelTypes[ tissueIndex ]="
                << _kernelTypes[ tissueIndex ]
                << std::endl;

      if ( _kernelTypes[ tissueIndex ] == "symmetric_tensor" )
      {

        kernelTensorTypes[ tissueIndex ] =
                                          gkg::DiffusionKernel::SymmetricTensor;

      }
      else
      {

        throw std::runtime_error(
                std::string( "invalid kernel tensor type for tissue class " ) +
                gkg::StringConverter::toString( tissueIndex ) );

      }

      // tissue class threshdolding mode
      _tissueClassThresholdingModes[ tissueIndex ] = 
                                             specificStringParameters[ offset ];
      offset += 1;

      std::cout << "_tissueClassThresholdingModes[ tissueIndex ]="
                << _tissueClassThresholdingModes[ tissueIndex ] << std::endl;

      // FA threshdolding mode
      _faThresholdingModes[ tissueIndex ] = specificStringParameters[ offset ];
      offset += 1;
      std::cout << "_faThresholdingModes[ tissueIndex ]="
                << _faThresholdingModes[ tissueIndex ] << std::endl;

      // FA sorting type (increasing or decreasing
      _faSortingTypes[ tissueIndex ] = 
                                             specificStringParameters[ offset ];
      offset += 1;
      std::cout << "_faSortingTypes[ tissueIndex ]="
                << _faSortingTypes[ tissueIndex ] << std::endl;

      if ( _faSortingTypes[ tissueIndex ] == "increasing" )
      {

        kernelFAStortingTypes[ tissueIndex ] =
                      gkg::DiffusionKernel::IncreasingFractionalAnisotropyOrder;

      }
      else if ( _faSortingTypes[ tissueIndex ] == "decreasing" )
      {

        kernelFAStortingTypes[ tissueIndex ] =
                      gkg::DiffusionKernel::DecreasingFractionalAnisotropyOrder;

      }
      else
      {

        throw std::runtime_error(
             std::string( "invalid kernel FA sorting type for tissue class " ) +
             gkg::StringConverter::toString( tissueIndex ) );

      }

    }
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating and computing diffusion kernel(s)
    ////////////////////////////////////////////////////////////////////////////

    _diffusionKernels.resize( _tissueCount );
    for ( tissueIndex = 0; tissueIndex < _tissueCount; tissueIndex++ )
    {

      std::cout << "tissueIndex=" << tissueIndex << std::endl;

      // loading tissue class volume
      gkg::RCPointer< gkg::Volume< float > > 
        tissueClassProbabilityMap( new gkg::Volume< float > );
      gkg::Reader::getInstance().read(
                             _fileNameTissueClassProbabilityMaps[ tissueIndex ],
                             *tissueClassProbabilityMap );

      // loading 3D transform from tissue class volume to diffusion space if any
      gkg::RCPointer< gkg::Transform3d< float > > 
        transform3dFromTissueClassToDiffusion;
      if ( _tissueToDiffusion3DTransformTypes[ tissueIndex ] != "none" )
      {

        gkg::NonLinearTransform3d< float >* 
          nonLinearTransform3d = new gkg::NonLinearTransform3d< float >;
        nonLinearTransform3d->readTrm( 
           _fileNameTissueToDiffusion3DLinearTransforms[ tissueIndex ],
           _fileNameTissueToDiffusionDirect3DDiffeoTransforms[ tissueIndex ],
           _fileNameTissueToDiffusionInverse3DDiffeoTransforms[ tissueIndex ] );

        transform3dFromTissueClassToDiffusion.reset( nonLinearTransform3d );

      }

      // creating FA test function
      gkg::RCPointer< gkg::TestFunction< float > >
        fractionalAnisotropyTestFunction(
          gkg::TestFunctionFactory< float>::getInstance().createTestFunction(
                                         _faThresholdingModes[ tissueIndex ],
                                         _firstFAThresholds[ tissueIndex ],
                                         _secondFAThresholds[ tissueIndex ] ) );

      // creating tissue class test function
      gkg::RCPointer< gkg::TestFunction< float > >
        tissueClassTestFunction(
          gkg::TestFunctionFactory< float>::getInstance().createTestFunction(
                                _tissueClassThresholdingModes[ tissueIndex ],
                                _firstTissueClassThresholds[ tissueIndex ],
                                _secondTissueClassThresholds[ tissueIndex ] ) );

      _diffusionKernels[ tissueIndex ].reset(
          new gkg::DiffusionKernel(
                      _t2,
                      _dw,
                      _mask,
                      tissueClassProbabilityMap,
                      transform3dFromTissueClassToDiffusion,
                      fractionalAnisotropyTestFunction,
                      kernelFAStortingTypes[ tissueIndex ],
                      tissueClassTestFunction,
                      _kernelWantedVoxelCounts[ tissueIndex ],
                      kernelTensorTypes[ tissueIndex ],
                      verbose ) );

      std::cout << "_diffusionKernels[ tissueIndex ]="
                << _diffusionKernels[ tissueIndex ]->getTensor().
                                                 getCoefficients() << std::endl;

      gkg::Volume< uint8_t > candidateVoxelVolume = 
                   _diffusionKernels[ tissueIndex ]->getCandidateVoxelVolume();
      gkg::Writer::getInstance().write(
               std::string( "candidate_voxels_" ) +
               gkg::StringConverter::toString( tissueIndex ),
               candidateVoxelVolume );

      gkg::Volume< uint8_t > selectedVoxelVolume = 
                   _diffusionKernels[ tissueIndex ]->getSelectedVoxelVolume();
      gkg::Writer::getInstance().write(
               std::string( "selected_voxels_" ) +
               gkg::StringConverter::toString( tissueIndex ),
               selectedVoxelVolume );


    }

    _items.getHeader().addAttribute( "odf_type",
            gkg::TypeOf<
              gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf >::getName() );

    getReconstructionMatrices( verbose );

    this->_functor = new gkg::MSMTCSDFiberOdfCartesianFieldFunctor( *this );

  }
  GKG_CATCH( "void gkg::MSMTCSDFiberOdfCartesianField::prepare( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


void gkg::MSMTCSDFiberOdfCartesianField::getReconstructionMatrices(
                                                                  bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "building reconstruction matrix" << std::flush;

    }


    //////////////////////////////////////////////////////////////////////////
    // extracting the input multiple different shell q-space sampling from DW
    //////////////////////////////////////////////////////////////////////////

    gkg::QSpaceSampling*
      qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create(
                                                             *( this->_dw ) );
    gkg::MultipleShellQSpaceSampling* multipleShellQSpaceSampling =
          dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );
    if ( !multipleShellQSpaceSampling )
    {
 
      throw std::runtime_error(
                            "qspace sampling is not of type multiple shell" );

    }

    gkg::RCPointer< gkg::MultipleShellQSpaceSampling >
      inputMultipleShellQSpaceSampling( multipleShellQSpaceSampling );

    // collecting the shell count
    _shellCount = inputMultipleShellQSpaceSampling->getShellCount();


    std::cout << "_shellCount=" << _shellCount << std::endl;

    //////////////////////////////////////////////////////////////////////////
    // computing the spherical harmonics coefficient count for each tissue
    //////////////////////////////////////////////////////////////////////////

    int32_t allSHCoefficientCount = 0;
    _shCoefficientCounts.resize( _tissueCount );
    int32_t t = 0;
    for ( t = 0; t < _tissueCount; t++ )
    {

      std::cout << "tissueIndex=" << t << std::endl;

      _shCoefficientCounts[ t ] =
                ( ( _maximumSphericalHarmonicOrders[ t ] + 1 ) *
                  ( _maximumSphericalHarmonicOrders[ t ] + 2 ) ) / 2;
      std::cout << "_shCoefficientCounts[ t ]=" 
                << _shCoefficientCounts[ t ] << std::endl;
      allSHCoefficientCount += _shCoefficientCounts[ t ];

    }

    std::cout << "allSHCoefficientCount=" << allSHCoefficientCount << std::endl;

    //////////////////////////////////////////////////////////////////////////
    // measure count(s) for the different shell(s)
    //////////////////////////////////////////////////////////////////////////

    int32_t allMeasurementCount = 0;
    _measurementCounts.resize( _shellCount );
    int32_t s = 0;
    for ( s = 0; s < _shellCount; s++ )
    {

      _measurementCounts[ s ] = 
        inputMultipleShellQSpaceSampling->getOrientationSet( s ).getCount();
      allMeasurementCount += _measurementCounts[ s ];

      std::cout << "shell " << s << " : "
                << _measurementCounts[ s ] << " measurements"
                << std::endl;

    }


    //////////////////////////////////////////////////////////////////////////
    // computing the C[s][t]=Q[s]R[t,s] matrices with
    //    - Q being the SH to Cartesian basis transform matrix for shell s and
    //      tissue t
    //    - R being the kernel convolution matrix for tissue t on shell s
    //////////////////////////////////////////////////////////////////////////

    std::vector< std::vector< gkg::Matrix > > CMatrices( _shellCount );

    for ( s = 0; s < _shellCount; s++ )
    {

      const gkg::OrientationSet& orientationSet =
        inputMultipleShellQSpaceSampling->getOrientationSet( s );

      CMatrices[ s ].resize( _tissueCount );
      for ( t = 0; t < _tissueCount; t++ )
      {

        gkg::Matrix shToCartesianBasisMatrix;
        orientationSet.getSHToCartesianBasisTransform(
                                           _maximumSphericalHarmonicOrders[ t ],
                                           shToCartesianBasisMatrix );


        gkg::Matrix kernelConvolutionMatrix =
          _diffusionKernels[ t ]->getSphericalHarmonicsConvolutionMatrix(
                               orientationSet,
                               inputMultipleShellQSpaceSampling->getBValue( s ),
                               _maximumSphericalHarmonicOrders[ t ] );

        CMatrices[ s ][ t ] = shToCartesianBasisMatrix.getComposition( 
                                                      kernelConvolutionMatrix );

        std::cout << "shell " << s << ", tissue " << t << std::endl
                  << "============================================="
                  << std::endl
                  << "shToCartesianBasisMatrix=" << std::endl
                  << shToCartesianBasisMatrix  << std::endl
                  << "kernelConvolutionMatrix=" << std::endl
                  << kernelConvolutionMatrix  << std::endl
                  << "CMatrix (" << CMatrices[ s ][ t ].getSize1()
                  << "," << CMatrices[ s ][ t ].getSize2() << ")=" << std::endl
                  << CMatrices[ s ][ t ] << std::endl;

      }

    }


    //////////////////////////////////////////////////////////////////////////
    // computing the A[t] constraint matrices with A being the SH to
    // Cartesian basis transform matrix for a set of 300 orientations for 
    // which the recomposition of signal for tissue t should be positive
    //////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Matrix > AMatrices( _tissueCount );

    gkg::ElectrostaticOrientationSet
      orientationSet( DEFAULT_POSITIVE_CONSTRAINT_ORIENTATION_COUNT );
    for ( t = 0; t < _tissueCount; t++ )
    {

      orientationSet.getSHToCartesianBasisTransform(
                                           _maximumSphericalHarmonicOrders[ t ],
                                           AMatrices[ t ] );

    }


    //////////////////////////////////////////////////////////////////////////
    // now building the big C matrix:
    //
    //          [ C(shell1, tissue1)   ...     C(shell1, tissueM) ]
    //          [         .                            .          ]
    //          [         .                            .          ]
    //          [         .                            .          ]
    //          [ C(shellN, tissue1)   ...     C(shellN, tissueM) ]
    //////////////////////////////////////////////////////////////////////////

    gkg::Matrix C( allMeasurementCount, allSHCoefficientCount );
    int32_t shCoefficientOffset = 0;
    int32_t measurementOffset = 0;
    for ( s = 0; s < _shellCount; s++ )
    {

      shCoefficientOffset = 0;
      for ( t = 0; t < _tissueCount; t++ )
      {

        C.fill( measurementOffset, shCoefficientOffset, CMatrices[ s ][ t ] );
        shCoefficientOffset += _shCoefficientCounts[ t ];
    
      }
      measurementOffset += _measurementCounts[ s ];

    }

    std::cout << "big C matrix: " << std::endl
              << "=============================================" << std::endl;
    std::cout << C << std::endl;


    //////////////////////////////////////////////////////////////////////////
    // now building the big A matrix:
    //
    //          [     A(tissue1)       ...             0          ]
    //          [         .                            .          ]
    //          [         .                            .          ]
    //          [         .                            .          ]
    //          [         0            ...         A(tissueM)     ]
    //////////////////////////////////////////////////////////////////////////

    _A.reallocate( _tissueCount * DEFAULT_POSITIVE_CONSTRAINT_ORIENTATION_COUNT,
                   allSHCoefficientCount );
    _A.setZero();
    shCoefficientOffset = 0;
    for ( t = 0; t < _tissueCount; t++ )
    {

      _A.fill( t * DEFAULT_POSITIVE_CONSTRAINT_ORIENTATION_COUNT,
               shCoefficientOffset,
               AMatrices[ t ] );
      shCoefficientOffset += _shCoefficientCounts[ t ];

    }

    std::cout << "big A matrix: " << std::endl
              << "=============================================" << std::endl;
    std::cout << _A << std::endl;

    //////////////////////////////////////////////////////////////////////////
    // computing and storing H = C^T C
    //////////////////////////////////////////////////////////////////////////

    _H = C.getTransposition().getComposition( C );


    std::cout << "_H matrix: " << std::endl
              << "=============================================" << std::endl;
    std::cout << _H << std::endl;

    //////////////////////////////////////////////////////////////////////////
    // computing and storing -C
    //////////////////////////////////////////////////////////////////////////

    _minusCTransposed = -C.getTransposition();

    std::cout << "_minusCTransposed matrix: " << std::endl
              << "=============================================" << std::endl;
    std::cout << _minusCTransposed << std::endl;

  }
  GKG_CATCH( "void gkg::MSMTCSDFiberOdfCartesianField::"
             "getReconstructionMatrices( bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::MSMTCSDFiberOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf* 
      msmtCsdFiberOdf = new gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf(
                                                &_outputOrientationSet,
                                                *_t2,
                                                *_dw,
                                                originSite,
                                                _tissueCount,
                                                _shCoefficientCounts,
                                                _H,
                                                _minusCTransposed,
                                                _A,
                                                _cqpSolverMaximumIterationCount,
                                                _cqpSolverEpsilonGap,
                                                _cqpSolverEpsilonResiduals,
                                                _cqpSolverEpsilonInfeasible );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              *msmtCsdFiberOdf );

    }
    else
    {

      return *msmtCsdFiberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::MSMTCSDFiberOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::MSMTCSDFiberOdfCartesianField::newItem(
                                     const gkg::Vector3d< float >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf*
      msmtCsdFiberOdf = new gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf(
                                                &_outputOrientationSet,
                                                *_t2,
                                                *_dw,
                                                originSite,
                                                _tissueCount,
                                                _shCoefficientCounts,
                                                _H,
                                                _minusCTransposed,
                                                _A,
                                                _cqpSolverMaximumIterationCount,
                                                _cqpSolverEpsilonGap,
                                                _cqpSolverEpsilonResiduals,
                                                _cqpSolverEpsilonInfeasible );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              *msmtCsdFiberOdf );

    }
    else
    {

      return *msmtCsdFiberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::MSMTCSDFiberOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


#undef DEFAULT_POSITIVE_CONSTRAINT_ORIENTATION_COUNT

