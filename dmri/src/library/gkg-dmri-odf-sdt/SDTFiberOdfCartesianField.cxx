#include <gkg-dmri-odf-sdt/SDTFiberOdfCartesianField.h>
#include <gkg-dmri-odf-sdt/SDTFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sdt/SharpeningDeconvolutionTransformFiberOdf.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


#define DEFAULT_REGULARIZATION_ORIENTATION_COUNT 300


gkg::SDTFiberOdfCartesianField::SDTFiberOdfCartesianField(
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

    _regularizationLCurveFactor = ( float )specificScalarParameters[ 4 ];
    
    if ( _regularizationLCurveFactor < 0 )
    {

      throw std::runtime_error( 
                             "regularization L-curve factor must be positive" );

    }

   _constrainedSphericalDeconvolution = 
     ( specificScalarParameters[ 5 ] > 0 ? true : false );
   
   _lambda = ( float ) specificScalarParameters[ 6 ];

   _superResolution = false;	     

    if ( getSphericalHarmonicCount() > dw->getSizeT() )
    {

      _superResolution = true;

    }

    _items.getHeader().addAttribute( "odf_type",
     gkg::TypeOf< gkg::SharpeningDeconvolutionTransformFiberOdf >::getName() );

    this->_functor = new gkg::SDTFiberOdfCartesianFieldFunctor( *this );

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
  GKG_CATCH( "gkg::SDTFiberOdfCartesianField::SDTFiberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::SDTFiberOdfCartesianField::SDTFiberOdfCartesianField(
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

    _regularizationLCurveFactor = ( float )specificScalarParameters[ 4 ];
    if ( _regularizationLCurveFactor < 0 )
    {

      throw std::runtime_error( 
                             "regularization L-curve factor must be positive" );

    }

   _constrainedSphericalDeconvolution = 
                          ( specificScalarParameters[ 5 ] > 0 ? true : false );

   _lambda = specificScalarParameters[ 6 ];
   _superResolution = false;

    if ( getSphericalHarmonicCount() > dw->getSizeT() )
    {

      _superResolution = true;

    }

    _items.getHeader().addAttribute( "odf_type",
     gkg::TypeOf< gkg::SharpeningDeconvolutionTransformFiberOdf >::getName() );

    this->_functor = new gkg::SDTFiberOdfCartesianFieldFunctor( *this );

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
  GKG_CATCH( "gkg::SDTFiberOdfCartesianField::SDTFiberOdfCartesianField( "
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


gkg::SDTFiberOdfCartesianField::SDTFiberOdfCartesianField(
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

    _regularizationLCurveFactor = ( float )specificScalarParameters[ 4 ];
    if ( _regularizationLCurveFactor < 0 )
    {

      throw std::runtime_error( 
                             "regularization L-curve factor must be positive" );

    }

   _constrainedSphericalDeconvolution = 
     ( specificScalarParameters[ 5 ] > 0 ? true : false );
   
   _lambda = specificScalarParameters[ 6 ];

   _superResolution = false;

    if ( getSphericalHarmonicCount() > dw->getSizeT() )
    {

      _superResolution = true;

    }

    _items.getHeader().addAttribute( "odf_type",
     gkg::TypeOf< gkg::SharpeningDeconvolutionTransformFiberOdf >::getName() );

    this->_functor = new gkg::SDTFiberOdfCartesianFieldFunctor( *this );

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
  GKG_CATCH( "gkg::SDTFiberOdfCartesianField::SDTFiberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::SDTFiberOdfCartesianField::~SDTFiberOdfCartesianField()
{
}



const gkg::DiffusionKernel& 
gkg::SDTFiberOdfCartesianField::getDiffusionKernel() const
{

  return _diffusionKernel;

}

int32_t 
gkg::SDTFiberOdfCartesianField::getMaximumSphericalHarmonicOrder() const
{

  return _maximumSphericalHarmonicOrder;

}


float gkg::SDTFiberOdfCartesianField::getRegularizationLCurveFactor() const
{

  return _regularizationLCurveFactor;

}


bool gkg::SDTFiberOdfCartesianField::getConstrainedSuperResolution() const
{

  return _constrainedSphericalDeconvolution;

}


int32_t gkg::SDTFiberOdfCartesianField::getSphericalHarmonicCount() const
{

  return ( ( _maximumSphericalHarmonicOrder + 1 ) *
           ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

}


const gkg::Matrix& 
gkg::SDTFiberOdfCartesianField::getReconstructionMatrix() const
{

  return _reconstructionMatrix;

}


const gkg::Matrix& 
gkg::SDTFiberOdfCartesianField::getB() const
{

  return _B;

}


const gkg::Matrix& 
gkg::SDTFiberOdfCartesianField::getBplus() const
{

  return _Bplus;

}


const gkg::Matrix& 
gkg::SDTFiberOdfCartesianField::getRegularizationBMatrix() const
{

  return _regularizationBMatrix;

}


const gkg::Matrix& 
gkg::SDTFiberOdfCartesianField::getConvolutionMatrix() const
{

  return _convolutionMatrix;

}


void gkg::SDTFiberOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 6 );
      specificScalarParameters[ 0 ] = 300;
      specificScalarParameters[ 1 ] = 0.65;
      specificScalarParameters[ 2 ] = 0.85;
      specificScalarParameters[ 3 ] = 4;
      specificScalarParameters[ 4 ] = 0.006;
      specificScalarParameters[ 5 ] = 0;
      //specificScalarParameters[ 6 ] = 1.0; //lambda

    }
    else
    {

      if ( specificScalarParameters.size() != 6U )
      {

        throw std::runtime_error( "invalid string parameter count" );

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
  GKG_CATCH( "void gkg::SDTFiberOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::SDTFiberOdfCartesianField::getReconstructionMatrix( bool verbose )
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
    int32_t n, m;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;
    for ( m = 0; m < M; m++ )
    {

      orientation = _inputOrientationSet.getOrientation( m );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        _B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Laplace-Beltrami smoothness matrix for regulrization
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t l;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }

    


    // building Bplus matrix with size N x M
    // Bplus = (B^T B + l L )^(-1) B^T
    _Bplus.reallocate( N, M );
    factory->getThikonovPseudoInverse( _B, L,
                                       ( double )_regularizationLCurveFactor,
                                       _Bplus );


    // building Funk-Hecke matrix P
    gkg::Matrix P( N, N );
    P.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      P( n, n ) = 2 * M_PI * factory->getLegendrePolynomial( l, 0.0 );

    }


    // building Sharpening Deconvolution Transform (SDTFiberOdf) matrix
    gkg::Vector eigenValues = _diffusionKernel.getTensor().getEigenValues();
    gkg::Matrix SDT( N, N );
    SDT.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      SDT( n, n ) = factory->getSharpeningDeconvolutionTransformCoefficient(
                                                            l,
                                                            eigenValues( 0 ),
                                                            eigenValues( 1 ) );

    }


    // building reconstruction matrix PBplus
    _reconstructionMatrix = P.getComposition( SDT ).getComposition( _Bplus );

    

    if ( _constrainedSphericalDeconvolution )
    {

      getDeconvolutionMatrices( verbose );

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::SDTFiberOdfCartesianField::getReconstructionMatrix( "
             "bool verbose )" );

}


void gkg::SDTFiberOdfCartesianField::getDeconvolutionMatrices( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "building deconvolution matrices" << std::flush;

    }
    std::cout << "here\n";

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

    //////////////////////////////////////////////////////////////////////
    //  1 Fiber Synthetic Signal Generation
    /////////////////////////////////////////////////////////////////////  
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
		<< bValue << " x 10^-6 s/mm^2" 
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
        i++;

      }

    }

    _convolutionMatrix.reallocate( N, N );
    _convolutionMatrix.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );

      _convolutionMatrix( n, n ) =
        nonZeroSphericalHarmonicSynthetic1FiberSignal( l / 2 ) /
        nonZeroDiracFunction( l / 2 );

    }

    if ( verbose )
    {
	
	std::cout << gkg::Eraser( 30 ) << std::flush;
	
    }

  }
  GKG_CATCH( "void gkg::SDTFiberOdfCartesianField::getReconstructionMatrix( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::SDTFiberOdfCartesianField::newItem(
                                const gkg::Vector3d< int32_t >& originSite,
                                const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::SharpeningDeconvolutionTransformFiberOdf* sdtFiberOdf = 0;

    if ( _constrainedSphericalDeconvolution )
    {

      sdtFiberOdf = new gkg::SharpeningDeconvolutionTransformFiberOdf(
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

      sdtFiberOdf = new gkg::SharpeningDeconvolutionTransformFiberOdf(
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
                              *sdtFiberOdf );

    }
    else
    {

      return *sdtFiberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::SDTFiberOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::SDTFiberOdfCartesianField::newItem(
                                const gkg::Vector3d< float >& originSite,
                                const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::SharpeningDeconvolutionTransformFiberOdf* sdtFiberOdf = 0;

    if ( _constrainedSphericalDeconvolution )
    {

      sdtFiberOdf = new gkg::SharpeningDeconvolutionTransformFiberOdf(
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

      sdtFiberOdf = new gkg::SharpeningDeconvolutionTransformFiberOdf(
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
                              *sdtFiberOdf );

    }
    else
    {

      return *sdtFiberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::SDTFiberOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


#undef DEFAULT_REGULARIZATION_ORIENTATION_COUNT
