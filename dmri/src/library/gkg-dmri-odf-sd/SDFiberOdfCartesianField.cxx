#include <gkg-dmri-odf-sd/SDFiberOdfCartesianField.h>
#include <gkg-dmri-odf-sd/SphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-odf-sd/SDFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


#define DEFAULT_REGULARIZATION_ORIENTATION_COUNT 300


gkg::SDFiberOdfCartesianField::SDFiberOdfCartesianField(
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

    int32_t kernelWantedVoxelCount =
                             ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );

    if ( kernelWantedVoxelCount )
    {

      float kernelLowerFAThreshold = ( float )specificScalarParameters[ 1 ];
      float kernelUpperFAThreshold = ( float )specificScalarParameters[ 2 ];

      if ( ( kernelLowerFAThreshold < 0.0 ) ||
           ( kernelLowerFAThreshold > 1.0 ) )
      {

        throw std::runtime_error( "invalid kernel lower FA threshold" );

      }
      if ( ( kernelUpperFAThreshold < 0.0 ) ||
           ( kernelUpperFAThreshold > 1.0 ) )
      {

        throw std::runtime_error( "invalid kernel upper FA threshold" );

      }
      if ( kernelLowerFAThreshold > kernelUpperFAThreshold )
      {

        throw std::runtime_error(
          "kernel lower FA threshold must be lower than kernel upper FA "
          "threshold" );

      }

      gkg::DiffusionKernel::TensorType kernelTensorType;
      if ( specificStringParameters[ 0 ] == "symmetric_tensor" )
      {

        kernelTensorType = gkg::DiffusionKernel::SymmetricTensor;

      }
      else
      {

        throw std::runtime_error( "invalid kernel tensor type" );

      }

      gkg::RCPointer< gkg::TestFunction< float > >
        fractionalAnisotropyTestFunction(
          gkg::TestFunctionFactory< float>::getInstance().createTestFunction(
                                                    "be",
                                                    kernelLowerFAThreshold,
                                                    kernelUpperFAThreshold ) );
      _diffusionKernel = gkg::DiffusionKernel(
                      t2,
                      dw,
                      mask,
                      fractionalAnisotropyTestFunction,
                      gkg::DiffusionKernel::DecreasingFractionalAnisotropyOrder,
                      kernelWantedVoxelCount,
                      kernelTensorType,
                      verbose );

    }
    else
    {

      float kernelEigenValue = ( float )specificScalarParameters[ 1 ];
      if ( kernelEigenValue <= 0.0 )
      {

        throw std::runtime_error( "kernel eigenvalue must be positive" );

      }

      float kernelRatio = ( float )specificScalarParameters[ 2 ];
      if ( kernelRatio <= 0.0 )
      {

        throw std::runtime_error( "kernel L1/L2 ratio must be positive" );

      }

      _diffusionKernel = gkg::DiffusionKernel( kernelEigenValue,
                                               kernelRatio );

    }

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 3 ]
                                                  + 0.5 );
    if ( _maximumSphericalHarmonicOrder < 0 )
    {

      throw std::runtime_error(
                          "maximum spherical harmonic order must be positive" );

    }
    if ( _maximumSphericalHarmonicOrder % 2 )
    {

      throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

    }



    int32_t filteringCoefficientCount =
      ( int32_t )specificScalarParameters.size() - 5;


    _filteringCoefficients.reallocate( filteringCoefficientCount );
    _filteringCoefficients.fill( 1.0 );
	
      if ( specificScalarParameters[ 4 ] )
      {

	int32_t f;
	for ( f = 0; f < filteringCoefficientCount; f++ )
	{

	  _filteringCoefficients( f ) =
	    ( float )specificScalarParameters[ 4 + f ];
	  
	}

	_constrainedSphericalDeconvolution = 
	  ( specificScalarParameters[
			       ( int32_t )specificScalarParameters.size() - 1]
	    > 0 ? true : false );

      }
      else
      {
      
	_constrainedSphericalDeconvolution = 
	  ( specificScalarParameters[ 5 ] > 0 ? true : false );
	
      }
      

   _superResolution = false;

    if ( getSphericalHarmonicCount() > dw->getSizeT() )
    {

      _superResolution = true;

    }

    _items.getHeader().addAttribute( "odf_type",
     gkg::TypeOf< gkg::SphericalDeconvolutionFiberOdf >::getName() );

    this->_functor = new gkg::SDFiberOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );

    if ( verbose )
    {

      if ( _constrainedSphericalDeconvolution )
      {

        if ( _superResolution )
        {

          std::cout << "constrained super resolution " << std::flush;

        }
        else
        {

          std::cout << "constrained regularized " << std::flush;

        }

      }

    }
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::SDFiberOdfCartesianField::SDFiberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::SDFiberOdfCartesianField::SDFiberOdfCartesianField(
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

    int32_t kernelWantedVoxelCount = ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );

    if ( kernelWantedVoxelCount )
    {

      float kernelLowerFAThreshold = ( float )specificScalarParameters[ 1 ];
      float kernelUpperFAThreshold = ( float )specificScalarParameters[ 2 ];

      if ( ( kernelLowerFAThreshold < 0.0 ) ||
           ( kernelLowerFAThreshold > 1.0 ) )
      {

        throw std::runtime_error( "invalid kernel lower FA threshold" );

      }
      if ( ( kernelUpperFAThreshold < 0.0 ) ||
           ( kernelUpperFAThreshold > 1.0 ) )
      {

        throw std::runtime_error( "invalid kernel upper FA threshold" );

      }
      if ( kernelLowerFAThreshold > kernelUpperFAThreshold )
      {

        throw std::runtime_error(
          "kernel lower FA threshold must be lower than kernel upper FA threshold" );

      }

      gkg::DiffusionKernel::TensorType kernelTensorType;
      if ( specificStringParameters[ 0 ] == "symmetric_tensor" )
      {

        kernelTensorType = gkg::DiffusionKernel::SymmetricTensor;

      }
      else
      {

        throw std::runtime_error( "invalid kernel tensor type" );

      }

      gkg::RCPointer< gkg::TestFunction< float > >
        fractionalAnisotropyTestFunction(
          gkg::TestFunctionFactory< float>::getInstance().createTestFunction(
                                                    "be",
                                                    kernelLowerFAThreshold,
                                                    kernelUpperFAThreshold ) );
      _diffusionKernel = gkg::DiffusionKernel(
                      t2,
                      dw,
                      mask,
                      fractionalAnisotropyTestFunction,
                      gkg::DiffusionKernel::DecreasingFractionalAnisotropyOrder,
                      kernelWantedVoxelCount,
                      kernelTensorType,
                      verbose );

    }
    else
    {

      float kernelEigenValue = ( float )specificScalarParameters[ 1 ];
      if ( kernelEigenValue <= 0.0 )
      {

        throw std::runtime_error( "kernel eigenvalue must be positive" );

      }

      float kernelRatio = ( float )specificScalarParameters[ 2 ];
      if ( kernelRatio <= 0.0 )
      {

        throw std::runtime_error( "kernel L1/L2 ratio must be positive" );

      }

      _diffusionKernel = gkg::DiffusionKernel( kernelEigenValue,
                                               kernelRatio );

    }

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 3 ]
                                                  + 0.5 );
    if ( _maximumSphericalHarmonicOrder < 0 )
    {

      throw std::runtime_error(
                          "maximum spherical harmonic order must be positive" );

    }
    if ( _maximumSphericalHarmonicOrder % 2 )
    {

      throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

    }

    int32_t filteringCoefficientCount =
      ( int32_t )specificScalarParameters.size() - 5;
    

    _filteringCoefficients.reallocate( filteringCoefficientCount );
    _filteringCoefficients.fill( 1.0 );
	  
      if ( specificScalarParameters[ 4 ] )
      {

	int32_t f;
	for ( f = 0; f < filteringCoefficientCount; f++ )
	{

	  _filteringCoefficients( f ) =
	    ( float )specificScalarParameters[ 4 + f ];
	  
	}

	_constrainedSphericalDeconvolution = 
	  ( specificScalarParameters[
			       ( int32_t )specificScalarParameters.size() - 1]
	    > 0 ? true : false );

      }
      else
      {
    
	_constrainedSphericalDeconvolution = 
	  ( specificScalarParameters[ 5 ] > 0 ? true : false );
	
      }
      

    _superResolution = false;

    if ( getSphericalHarmonicCount() > dw->getSizeT() )
    {

      _superResolution = true;

    }

    _items.getHeader().addAttribute( "odf_type",
     gkg::TypeOf< gkg::SphericalDeconvolutionFiberOdf >::getName() );

    this->_functor = new gkg::SDFiberOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );

    if ( verbose )
    {

      if ( _constrainedSphericalDeconvolution )
      {

        if ( _superResolution )
        {

          std::cout << "constrained super resolution " << std::flush;

        }
        else
        {

          std::cout << "constrained regularized " << std::flush;

        }

      }

    }
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::SDFiberOdfCartesianField::SDFiberOdfCartesianField( "
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


gkg::SDFiberOdfCartesianField::SDFiberOdfCartesianField(
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

    int32_t kernelWantedVoxelCount = 
                             ( int32_t )( specificScalarParameters[ 0 ] + 0.5 );

    if ( kernelWantedVoxelCount )
    {

      float kernelLowerFAThreshold = ( float )specificScalarParameters[ 1 ];
      float kernelUpperFAThreshold = ( float )specificScalarParameters[ 2 ];

      if ( ( kernelLowerFAThreshold < 0.0 ) ||
           ( kernelLowerFAThreshold > 1.0 ) )
      {

        throw std::runtime_error( "invalid kernel lower FA threshold" );

      }
      if ( ( kernelUpperFAThreshold < 0.0 ) ||
           ( kernelUpperFAThreshold > 1.0 ) )
      {

        throw std::runtime_error( "invalid kernel upper FA threshold" );

      }
      if ( kernelLowerFAThreshold > kernelUpperFAThreshold )
      {

        throw std::runtime_error(
          "kernel lower FA threshold must be lower than kernel upper FA threshold" );

      }

      gkg::DiffusionKernel::TensorType kernelTensorType;
      if ( specificStringParameters[ 0 ] == "symmetric_tensor" )
      {

        kernelTensorType = gkg::DiffusionKernel::SymmetricTensor;

      }
      else
      {

        throw std::runtime_error( "invalid kernel tensortype" );

      }

      gkg::RCPointer< gkg::TestFunction< float > >
        fractionalAnisotropyTestFunction(
          gkg::TestFunctionFactory< float>::getInstance().createTestFunction(
                                                    "be",
                                                    kernelLowerFAThreshold,
                                                    kernelUpperFAThreshold ) );
      _diffusionKernel = gkg::DiffusionKernel(
                      t2,
                      dw,
                      mask,
                      fractionalAnisotropyTestFunction,
                      gkg::DiffusionKernel::DecreasingFractionalAnisotropyOrder,
                      kernelWantedVoxelCount,
                      kernelTensorType,
                      verbose );

    }
    else
    {

      float kernelEigenValue = ( float )specificScalarParameters[ 1 ];
      if ( kernelEigenValue <= 0.0 )
      {

        throw std::runtime_error( "kernel eigenvalue must be positive" );

      }

      float kernelRatio = ( float )specificScalarParameters[ 2 ];
      if ( kernelRatio <= 0.0 )
      {

        throw std::runtime_error( "kernel L1/L2 ratio must be positive" );

      }

      _diffusionKernel = gkg::DiffusionKernel( kernelEigenValue,
                                               kernelRatio );

    }

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 3 ]
                                                  + 0.5 );
    if ( _maximumSphericalHarmonicOrder < 0 )
    {

      throw std::runtime_error(
                          "maximum spherical harmonic order must be positive" );

    }
    if ( _maximumSphericalHarmonicOrder % 2 )
    {

      throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

    }

    int32_t filteringCoefficientCount =
      ( int32_t )specificScalarParameters.size() - 5;

    
    _filteringCoefficients.reallocate( filteringCoefficientCount );
    _filteringCoefficients.fill( 1.0 );
    
    if ( specificScalarParameters[ 4 ] )
    {
	
      int32_t f;
      for ( f = 0; f < filteringCoefficientCount; f++ )
      {
	
	_filteringCoefficients( f ) =
	  ( float )specificScalarParameters[ 4 + f ];
	
      }

      _constrainedSphericalDeconvolution = 
	( specificScalarParameters[
				   ( int32_t )specificScalarParameters.size() - 1]
	  > 0 ? true : false );
      
    }
    else
    {
      
      _constrainedSphericalDeconvolution = 
	( specificScalarParameters[ 5 ] > 0 ? true : false );
      
    }
    
   _superResolution = false;

    if ( getSphericalHarmonicCount() > dw->getSizeT() )
    {

      _superResolution = true;

    }

    _items.getHeader().addAttribute( "odf_type",
     gkg::TypeOf< gkg::SphericalDeconvolutionFiberOdf >::getName() );

    this->_functor = new gkg::SDFiberOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );

    if ( verbose )
    {

      if ( _constrainedSphericalDeconvolution )
      {

        if ( _superResolution )
        {

          std::cout << "constrained super resolution " << std::flush;

        }
        else
        {

          std::cout << "constrained regularized " << std::flush;

        }

      }

    }
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::SDFiberOdfCartesianField::SDFiberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::SDFiberOdfCartesianField::~SDFiberOdfCartesianField()
{
}


const gkg::DiffusionKernel& 
gkg::SDFiberOdfCartesianField::getDiffusionKernel() const
{

  return _diffusionKernel;

}

int32_t gkg::SDFiberOdfCartesianField::getMaximumSphericalHarmonicOrder() const
{

  return _maximumSphericalHarmonicOrder;

}


const gkg::Vector&
gkg::SDFiberOdfCartesianField::getFilteringCoefficients() const
{

  return _filteringCoefficients;

}


bool gkg::SDFiberOdfCartesianField::getConstrainedSuperResolution() const
{

  return _constrainedSphericalDeconvolution;

}


int32_t gkg::SDFiberOdfCartesianField::getSphericalHarmonicCount() const
{

  return ( ( _maximumSphericalHarmonicOrder + 1 ) *
           ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

}


const gkg::Matrix& 
gkg::SDFiberOdfCartesianField::getReconstructionMatrix() const
{

  return _reconstructionMatrix;

}


const gkg::Matrix& 
gkg::SDFiberOdfCartesianField::getB() const
{

  return _B;

}


const gkg::Matrix& 
gkg::SDFiberOdfCartesianField::getBplus() const
{

  return _Bplus;

}


const gkg::Matrix& 
gkg::SDFiberOdfCartesianField::getRegularizationBMatrix() const
{

  return _regularizationBMatrix;

}


const gkg::Matrix& 
gkg::SDFiberOdfCartesianField::getConvolutionMatrix() const
{

  return _convolutionMatrix;

}


void gkg::SDFiberOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 21 );
      specificScalarParameters[ 0 ] = 300;
      specificScalarParameters[ 1 ] = 0.65;
      specificScalarParameters[ 2 ] = 0.85;
      specificScalarParameters[ 3 ] = 4;
                                                 // order
      specificScalarParameters[ 4 ] = 1.0;       // l = 0
      specificScalarParameters[ 5 ] = 1.0;       // l = 2
      specificScalarParameters[ 6 ] = 1.0;       // l = 4
      specificScalarParameters[ 7 ] = 0.5;       // l = 6
      specificScalarParameters[ 8 ] = 0.1;       // l = 8
      specificScalarParameters[ 9 ] = 0.02;      // l = 10
      specificScalarParameters[ 10 ] = 0.002;    // l = 12
      specificScalarParameters[ 11 ] = 0.0005;   // l = 14
      specificScalarParameters[ 12 ] = 0.0001;   // l = 16
      specificScalarParameters[ 13 ] = 0.0001;   // l = 18
      specificScalarParameters[ 14 ] = 0.00001;  // l = 20
      specificScalarParameters[ 15 ] = 0.00001;  // l = 22
      specificScalarParameters[ 16 ] = 0.00001;  // l = 24
      specificScalarParameters[ 17 ] = 0.00001;  // l = 26
      specificScalarParameters[ 18 ] = 0.00001;  // l = 28
      specificScalarParameters[ 19 ] = 0.00001;  // l = 30

      specificScalarParameters[ 20 ] = 0;

    }
    else
    {

      if ( specificScalarParameters.size() < 5 )
      {

        throw std::runtime_error( "invalid scalar parameter count" );

      }

      if ( ( specificScalarParameters[ 3 ] > 30 ) &&
           ( ( int32_t )( specificScalarParameters[
                            specificScalarParameters.size() -
                                                    1U ] + 0.5 ) == 0 ) )
      {

        throw std::runtime_error( "filtered spherical deconvolution should not be used"
                                  " at orders greater than 30" );

      }
      
      int32_t filteringCoefficientCount =
	( int32_t )specificScalarParameters.size() - 5;

      if( filteringCoefficientCount < ( specificScalarParameters[ 3 ] / 2 + 1 ) )
      {
      
	throw std::runtime_error( 
				 "must have a number of filtering coefficients \
for each even order between 0 and maximum spherical harmonic order" );
	
      }


    }
    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "symmetric_tensor";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "invalid string parameter count" );


      }

    }
    
  }
  GKG_CATCH( "void gkg::SDFiberOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::SDFiberOdfCartesianField::getReconstructionMatrix( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "building reconstruction matrix" << std::flush;

    }

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // processing maximum j according to maximum order
    int32_t N = ( ( _maximumSphericalHarmonicOrder + 1 ) *
                  ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

    // measure count
    int32_t M = _inputOrientationSet.getCount();

    
    // building matrix of spherical harmonics on input orientation(s)
    _B.reallocate( M, N );
    int32_t n, o;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;
    for ( o = 0; o< M; o++ )
    {

      orientation = _inputOrientationSet.getOrientation( o );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        _B( o, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B)^(-1) B^T
    _Bplus.reallocate( N, M );
    factory->getMoorePenrosePseudoInverse( _B, _Bplus );

    //
    //    Build Convolution Matrix 
    //
    //    If the fiber ODF was a unit delta function aligned with the  
    //    z-axis, then the DW signal measured should be identical to 
    //    the response function R. In other words, if f_l corresponds to a 
    //    delta function, then s_l must correspond to the response function. 
    //    In this case, as both functions are axially symmetric, all m != 0 
    //    coefficients are zero. Therefore, r_l can be computed by simple 
    //    scalar division of the lth order, m = 0 SH coefficients of R by 
    //    the corresponding coefficient for a delta function. 
    //    The Rot matrix R is simply R_jj = r_l
    //
    //

    //
    //  1 Fiber Synthetic Signal Generation
    //
    gkg::Matrix rotationMatrix( 3, 3 );
    rotationMatrix.setIdentity();

    float bValue = ( this->getBValues() )[ 0 ];

    gkg::Vector coefficients( 6 );
    gkg::Vector eigenValues = _diffusionKernel.getTensor().getEigenValues();
    coefficients( 0 ) = eigenValues( 2 ) * pow( 10.0, 6.0 );
    coefficients( 1 ) = 0.0;
    coefficients( 2 ) = 0.0;
    coefficients( 3 ) = eigenValues( 1 ) * pow( 10.0, 6.0 );
    coefficients( 4 ) = 0.0;
    coefficients( 5 ) = eigenValues( 0 ) * pow( 10.0, 6.0 );  //along z

    gkg::Tensor generatingTensor( 0, coefficients );

    if ( verbose )
    {

      std::cout << "eigen values = ["
                << generatingTensor.getEigenValues()
                << "] mm^2/s and b-value = " 
		<< bValue << " s/mm^2" 
		<< std::endl;
      
    }

    // need bValue in 10^-6 s/mm^2
    gkg::Vector signal = generatingTensor.getRotatedDiffusionWeightedSignal(
                                                         _inputOrientationSet,
                                                         rotationMatrix,
                                                         bValue,
                                                         1.0 );

    gkg::Vector sphericalHarmonicSynthetic1FiberSignal = 
                                               _Bplus.getComposition( signal );

    gkg::Vector diracFunction( N );
    gkg::Vector nonZeroSphericalHarmonicSynthetic1FiberSignal( 
                                      _maximumSphericalHarmonicOrder / 2 + 1 );
    gkg::Vector nonZeroDiracFunction( _maximumSphericalHarmonicOrder / 2 + 1 );

    int32_t i = 0;
    int32_t l, m;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      diracFunction( n ) = factory->getDiracCoefficient( n, 0, 0 );

      if ( m == 0 ) 
      {

        nonZeroSphericalHarmonicSynthetic1FiberSignal( i ) = 
                            sphericalHarmonicSynthetic1FiberSignal( n );
        nonZeroDiracFunction( i ) = diracFunction( n );
        i ++;

      }

    }

    _convolutionMatrix.reallocate( N, N );
    _convolutionMatrix.setZero();

    gkg::Matrix Rinverse( N, N );
    l = 0; m = 0;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );      
      // R(n n) = syntheticSignal(l, m=0) / Dirac(l,m)

      _convolutionMatrix( n, n ) =
        nonZeroSphericalHarmonicSynthetic1FiberSignal( l / 2 ) /
        nonZeroDiracFunction( l / 2 );


      if ( _convolutionMatrix( n, n ) != 0 )
      {

        Rinverse( n, n ) = _filteringCoefficients( l / 2 ) / 
	                                            _convolutionMatrix( n , n );

      }

    }

    // building reconstruction matrix , deconvolution with Rinverse
    _reconstructionMatrix = Rinverse.getComposition( _Bplus );

    if ( _constrainedSphericalDeconvolution )
    {

      getRegularizationMatrices( verbose );

    }



    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::SDFiberOdfCartesianField::getReconstructionMatrix( "
             "bool verbose )" );

}


void gkg::SDFiberOdfCartesianField::getRegularizationMatrices( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "building regularization matrices" << std::flush;

    }

    //
    // get orientation set for 300 directions on the sphere
    // and build basis matrix Bregularization
    //
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // number of SH coefficients
    int32_t N = ( int32_t )( ( _maximumSphericalHarmonicOrder + 1 ) *
                             ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

    // number of regularization orientations
    int32_t regularizationOrientationCount =
      DEFAULT_REGULARIZATION_ORIENTATION_COUNT;

    gkg::ElectrostaticOrientationSet regularizationOrientationSet( 
					DEFAULT_REGULARIZATION_ORIENTATION_COUNT );

    int32_t n, o;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;

    _regularizationBMatrix.reallocate( regularizationOrientationCount, N );
    for ( o = 0; o < regularizationOrientationCount; o++ )
    {

      orientation = regularizationOrientationSet.getOrientation( o );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        _regularizationBMatrix( o, n ) =
          factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::SDFiberOdfCartesianField::getReconstructionMatrix( "
	     "bool verbose )" );
}

 
gkg::OrientationDistributionFunction 
gkg::SDFiberOdfCartesianField::newItem(
				const gkg::Vector3d< int32_t >& originSite,
				const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::SphericalDeconvolutionFiberOdf* sdFiberOdf = 0;

    if ( _constrainedSphericalDeconvolution )
    {

      sdFiberOdf = new gkg::SphericalDeconvolutionFiberOdf(
                                                      &_outputOrientationSet,
                                                      *_t2,
                                                      *_dw,
                                                      originSite,
                                                      _reconstructionMatrix,
                                                      _B,
                                                      _Bplus,
                                                      _regularizationBMatrix,
                                                      _convolutionMatrix,
                                                      _superResolution );

    }
    else 
    {

      sdFiberOdf = new gkg::SphericalDeconvolutionFiberOdf(
                                                      &_outputOrientationSet,
                                                      *_t2,
                                                      *_dw,
                                                      originSite,
                                                      _reconstructionMatrix,
                                                      _Bplus );

    }

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              *sdFiberOdf );

    }
    else
    {

      return *sdFiberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::SDFiberOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::SDFiberOdfCartesianField::newItem(
                                const gkg::Vector3d< float >& originSite,
                                const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::SphericalDeconvolutionFiberOdf* sdFiberOdf = 0;

    if ( _constrainedSphericalDeconvolution )
    {

      sdFiberOdf = new gkg::SphericalDeconvolutionFiberOdf(
                                                      &_outputOrientationSet,
                                                      *_t2,
                                                      *_dw,
                                                      originSite,
                                                      _reconstructionMatrix,
                                                      _B,
                                                      _Bplus,
                                                      _regularizationBMatrix,
                                                      _convolutionMatrix,
                                                      _superResolution );

    }
    else 
    {

      sdFiberOdf = new gkg::SphericalDeconvolutionFiberOdf(
                                                      &_outputOrientationSet,
                                                      *_t2,
                                                      *_dw,
                                                      originSite,
                                                      _reconstructionMatrix,
                                                      _Bplus );

    }

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              *sdFiberOdf );

    }
    else
    {

      return *sdFiberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::SDFiberOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}




#undef DEFAULT_REGULARIZATION_ORIENTATION_COUNT

