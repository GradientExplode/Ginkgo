#include <gkg-dmri-odf-dot/DotOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class S >
gkg::DotOdf::DotOdf( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< S >& site,
		     int32_t maximumSphericalHarmonicOrder,
		     float effectiveDiffusionTime,
                     float R0,
                     bool marginalODF )
            : gkg::OrientationDistributionFunction(
                       gkg::OrientationDistributionFunction::SphericalHarmonics,
                       orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      gkg::Matrix Bplus;
      getReconstructionMatrix( dw,
                               maximumSphericalHarmonicOrder,
                               reconstructionMatrix,
                               Bplus );

      this->reconstruct( orientationSet,
                         t2,
                         dw,
                         site,
                         effectiveDiffusionTime,
                         R0,
                         marginalODF,
                         reconstructionMatrix,
                         Bplus );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::DotOdf::DotOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "int32_t maximumSphericalHarmonicOrder, "
	     "float effectiveDiffusionTime, "
             "float R0, "
             "bool marginalODF, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& Bplus )" );

}


template < class S >
gkg::DotOdf::DotOdf( const gkg::OrientationSet* orientationSet,
	             const gkg::Volume< float >& t2,
	             const gkg::Volume< float >& dw,
	             const gkg::Vector3d< S >& site,
	             float effectiveDiffusionTime,
                     float R0,
                     bool marginalODF,
	             const gkg::Matrix& reconstructionMatrix,
	             const gkg::Matrix& Bplus )
            : gkg::OrientationDistributionFunction(
                       gkg::OrientationDistributionFunction::SphericalHarmonics,
                       orientationSet )
{

  try
  {

    this->reconstruct( orientationSet,
                       t2,
                       dw,
                       site,
                       effectiveDiffusionTime,
                       R0,
                       marginalODF,
                       reconstructionMatrix,
                       Bplus );

  }
  GKG_CATCH( "template < class S > "
             "gkg::DotOdf::DotOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "float effectiveDiffusionTime, "
             "float R0, "
             "bool marginalODF, "
	     "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& Bplus )" );

}


gkg::DotOdf::~DotOdf()
{
}


const gkg::Vector& gkg::DotOdf::getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::DotOdf::getDwSphericalHarmonicsCoefficients() const" );

}

const gkg::Vector& gkg::DotOdf::getAdcSphericalHarmonicsCoefficients() const
{

  try
  {

    return _adcSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::DotOdf::getAdcSphericalHarmonicsCoefficients() const" );

}


const gkg::Vector& gkg::DotOdf::getAdc() const
{

  try
  {

    return _adc;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::DotOdf::getAdc() const" );

}


void gkg::DotOdf::getReconstructionMatrix( 
					  const HeaderedObject& headeredObject,
					  int32_t maximumSphericalHarmonicOrder,
					  Matrix& reconstructionMatrix,
					  Matrix& Bplus )
  
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

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // processing maximum j according to maximum order
    int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
                  ( maximumSphericalHarmonicOrder + 2 ) ) / 2;

    // measure count
    int32_t M = inputOrientationSet.getCount();

    // building matrix of spherical harmonics on input orientation(s)
    reconstructionMatrix.reallocate( M, N );
    int32_t n, m;

    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;
    for ( m = 0; m < M; m++ )
    {

      orientation = inputOrientationSet.getOrientation( m );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      
      for ( n = 0; n < N; n++ )
      {

        reconstructionMatrix( m, n ) = 
	  factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B)^(-1) B^T
    Bplus.reallocate( N, M );
    factory->getMoorePenrosePseudoInverse( reconstructionMatrix, Bplus );

  }
  GKG_CATCH( "void gkg::DotOdf::getReconstructionMatrix( "
	     "const HeaderedObject& headeredObject, "
	     "int32_t maximumSphericalHarmonicOrder, "
	     "Matrix& reconstructionMatrix, "
	     "Matrix& Bplus )" );
}


template < class S >
void gkg::DotOdf::reconstruct( const gkg::OrientationSet* /* orientationSet */,
	                       const gkg::Volume< float >& t2,
	                       const gkg::Volume< float >& dw,
	                       const gkg::Vector3d< S >& site,
		               float effectiveDiffusionTime,
                               float R0,
                               bool marginalODF,
                               const gkg::Matrix& reconstructionMatrix,
                               const gkg::Matrix& Bplus )
{

  try
  {

    if ( _orientationSet )
    {

      // computing average T2 value and normalized signal vector
      float averageT2 = 0.0;
      gkg::Vector normalizedSignal;
      bool isOk = this->computeNormalizedSignal( t2,
                                                 dw,
                                                 site,
                                                 averageT2,
                                                 normalizedSignal );



      // resizing ODF vector to SH coefficient count
      int32_t shCoefficientCount = Bplus.getSize1();
      _values.resize( shCoefficientCount );

      if ( !isOk )
      {

        // setting DW spherical harmonics coefficient(s) to nul vector
        _dwSphericalHarmonicsCoefficients.reallocate( shCoefficientCount );
        _dwSphericalHarmonicsCoefficients.fill( 0.0 );

        // setting ADC to nul vector
        int32_t orientationCount = dw.getSizeT();
        _adc.reallocate( orientationCount );
        _adc.fill( 0.0 );

        // setting ADC spherical harmonics coefficient(s) to nul vector
        _adcSphericalHarmonicsCoefficients.reallocate( shCoefficientCount );
        _adcSphericalHarmonicsCoefficients.fill( 0.0 );


        // setting ODF SH coefficients to nul
        int32_t s;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          _values[ s ] = 0.0f;

        }


      }
      else
      {

        // processing DW spherical harmonics coefficient(s)
        _dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );

        // getting b-value from the DW data header
        std::vector< double > bValues;
        if ( dw.getHeader().hasAttribute( "bvalues" ) )
        {
 
          dw.getHeader().getAttribute( "bvalues", bValues );

        }
        else
        {

          throw std::runtime_error( "missing 'bvalues' attribute" );

        }

        // building ADC vector
        int32_t orientationCount = dw.getSizeT();
        _adc.reallocate( orientationCount );

        int32_t t = 0;
        for ( t = 0; t < orientationCount; t++ )
        {

	  _adc( t ) = -1 / bValues[ t ] * std::log( normalizedSignal( t ) );

        }

        // mono-exponential DOT
        _adcSphericalHarmonicsCoefficients = Bplus.getComposition( _adc );



        performDOT( reconstructionMatrix,
                    Bplus,
		    effectiveDiffusionTime,
                    R0,
                    marginalODF );      

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::DotOdf::reconstruct( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
	     "float effectiveDiffusionTime, "
             "float R0, "
             "bool marginalODF )" );

}


void gkg::DotOdf::getIMatrix( 
	       int32_t maximumSphericalHarmonicOrder,
	       int32_t orientationCount,
	       float effectiveDiffusionTime,
	       float R0, Matrix& IMatrix )
					  
{

  try
  {

    // processing maximum j according to maximum order
    // there are a total of maxorder / 2 + 1 distinct SH order
    // ex: maxorder = 4, l = 0, 2, 4  => N = 3
    int32_t N = maximumSphericalHarmonicOrder / 2 + 1;

    // measure count
    int32_t M = orientationCount;
      
    // building matrix of spherical harmonics on input orientation(s)
    IMatrix.reallocate( M, N );
    int32_t o, l;
    for ( o = 0; o < M; o++ )
    {

      for ( l = 0; l <= maximumSphericalHarmonicOrder; l+=2 )
      {
	
// 	std::cout << "adc : " << _adc( o ) << "\t"
// 	     << "tau : " << effectiveDiffusionTime << "\t"
// 	     << "R0  : " << R0 << "\t" 
// 	     << "l   : " << l << "\t"
// 	     << "I   : " << dotIntegral( _adc( o ), l, 
// 					 effectiveDiffusionTime, R0 ) << "\n";;

	IMatrix( o, l / 2 ) = dotIntegral( _adc( o ), l, 
					   effectiveDiffusionTime, R0 );
	
      }
      
    }

  }
  GKG_CATCH( "void gkg::DotOdfCartesianField::getIMatrix( "
	     "int32_t shCount, "
	     "int32_t orientationCount, "
	     "float effectiveDiffusionTime, "
	     "float R0, Matrix& IMatrix " );
}


double gkg::DotOdf::dotIntegral( float adc, int32_t order,
				 float effectiveDiffusionTime, float R0 )
{
  
  double beta;
  double A;
  double B;
  double I;

  if ( adc <= 0 ) // this should not happen
  {
    
    return 0;

  }

  beta = R0 / sqrt( ( double )adc * ( double )effectiveDiffusionTime );

  // This corresponds to Table 2 in Ozarslan et al NeuroImage 2006
  switch ( order ) {

    case 0 : 
      A = 1.0;
      B = 0.0;
      break;

    case 2 : 

      A = -(1.0+6.0*pow(beta, -2));
      B = 3.0;
      break;

    case 4:

      A = 1.0+20.0*pow(beta,-2)+210.0*pow(beta,-4);
      B = (15.0/2.0)*(1-14.0*pow(beta,-2));
      break;

    case 6:

      A = -(1.0+42.0*pow(beta,-2)+(1575.0/2.0)*pow(beta,-4)+
	    10395.0*pow(beta,-6));
      B = (105.0/8.0)*(1.0-36.0*pow(beta,-2)+396.0*pow(beta,-4));
      break;

    case 8:

      A = 1.0+72.0*pow(beta,-2)+(10395.0/4.0)*pow(beta,-4)+
	45045.0*pow(beta,-6)+
	675675.0*pow(beta,-8);
      B = (315.0/16.0)*(1.0-66.0*pow(beta,-2)+
			1716.0*pow(beta,-4)-17160.0*pow(beta,-6));
      break;

    default :  // To do: for any order compute it
      
      A = 0.0;
      B = 0.0;

    }

  I = A*( exp( -pow( beta, 2.0 ) / 4.0 ) ) / 
    pow( ( 4.0*M_PI*(double)adc*(double)effectiveDiffusionTime ), 1.5 ) + 
    B*( erf( beta / 2.0 )) / (4.0*M_PI*pow( (double)R0, 3.0 ));

    return I;

  }

void gkg::DotOdf::performDOT( const Matrix& reconstructionMatrix,
			      const Matrix& Bplus,
			      float effectiveDiffusionTime,
			      float R0,
			      bool marginalODF )
{

  try
  {
    
    int32_t shCount = reconstructionMatrix.getSize2();
    int32_t orientationCount = reconstructionMatrix.getSize1();
    int32_t maximumSphericalHarmonicOrder = (int32_t)(-3/2 + 
						      sqrt(9/4 - 2*(1 - shCount)));

    if( !marginalODF ) 
    {
      
      // building I matrix, Eq. 6 and 28, Ozarslan et al NeuroImage 2006
      gkg::Matrix IMatrix;   
      getIMatrix( maximumSphericalHarmonicOrder,
		  orientationCount,
		  effectiveDiffusionTime, R0, IMatrix );



      // getting a pointer to the numerical analysis factory
      gkg::NumericalAnalysisImplementationFactory* factory =
	gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
  
      // Eq. 8  Ozarslan et al NeuroImage 2006
      // alphaOrderl is 15 x 1
      // alphaCoefficients are 15 x 3
      gkg::Matrix alphaCoefficients = Bplus.getComposition( IMatrix );

      // Eq. 10 Ozarslan et al NeuroImage 2006
      gkg::Vector dotSphericalHarmonicsCoefficients( shCount ); 
      int32_t i, l, m;
      for ( i = 0; i < shCount; i++ )
      {
    
	factory->getSymmetricalSphericalHarmonicLM( i, l, m );
	dotSphericalHarmonicsCoefficients( i ) = 
	                        pow( -1.0, l / 2.0 )*alphaCoefficients( i, l / 2 );
	
      }
      
      _values.resize( shCount );
      
      for ( i = 0; i < shCount; i++ )
      {

	_values[ i ] = ( float )dotSphericalHarmonicsCoefficients( i );
      
      }

    }
    else
    {
      
      // if marginalODF we need to compute
      // int_0^R0 PDF(R0) R0^2 dR0
      performMarginalOdfComputation( shCount,
				     maximumSphericalHarmonicOrder,
				     orientationCount,
				     reconstructionMatrix,
				     Bplus,
				     effectiveDiffusionTime,
				     R0 );

    }
				   

  }
  GKG_CATCH( "void gkg::DotOdf::performDOT( "
	     "const Matrix& reconstructionMatrix, "
	     "const Matrix& Bplus, "
	     "const Matrix& IMatrix, "
	     "float effectiveDiffusionTime, "
	     "float R0, "
	     "bool marginalODF ) " );

}

void gkg::DotOdf::performMarginalOdfComputation( 
						const int32_t shCount,
						const int32_t maxOrder,
						const int32_t orientationCount,
						const Matrix& reconstructionMatrix,
						const Matrix& Bplus,
						float effectiveDiffusionTime,
						float R0 )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    float radius;
    float radiusIncr = R0 / 10;
    gkg::Matrix IMatrix;   
    int32_t i, l, m;
    gkg::Matrix alphaCoefficients;
    gkg::Vector dotSphericalHarmonicsCoefficients( shCount ); 
    gkg::Vector dot( orientationCount ); 
    dot.fill( 0.0 );

    for ( radius = radiusIncr; radius <= R0; radius += radiusIncr )
    {
      
      //IMatrix.fill( 0.0 );
      getIMatrix( maxOrder,
		  orientationCount,
		  effectiveDiffusionTime, radius, IMatrix );
      
      // Eq. 8  Ozarslan et al NeuroImage 2006
      alphaCoefficients = Bplus.getComposition( IMatrix );

      // Eq. 10 Ozarslan et al NeuroImage 2006      
      for ( i = 0; i < shCount; i++ )
      {
    
	factory->getSymmetricalSphericalHarmonicLM( i, l, m );
	dotSphericalHarmonicsCoefficients( i ) = 
	          radius * radius * pow( -1.0, l / 2.0 ) * 
                  alphaCoefficients( i, l / 2 );
	
      }

      dot = dot + reconstructionMatrix.getComposition( 
                                            dotSphericalHarmonicsCoefficients );

    }   

    // Get SH coefficients of the ODF
    dotSphericalHarmonicsCoefficients = Bplus.getComposition( dot );
    _values.resize( shCount );
    for ( i = 0; i < shCount; i++ )
    {

      _values[ i ] = ( float )dotSphericalHarmonicsCoefficients( i );
      
    }

  }
  GKG_CATCH( "void gkg::DotOdf::performMarginalOdfComputation(" 
	     "const int32_t shCount, "
	     "const int32_t maxOrder, "
	     "const Matrix& reconstructionMatrix, "
	     "const Matrix& Bplus, "
	     "float effectiveDiffusionTime, "
	     "float R0 )" );
}


template 
gkg::DotOdf::DotOdf( const gkg::OrientationSet* orientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< int32_t >& site,
		     int32_t maximumSphericalHarmonicOrder,
		     float effectiveDiffusionTime,
                     float R0,
                     bool marginalODF );

template 
gkg::DotOdf::DotOdf( const gkg::OrientationSet* orientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< float >& site,
		     int32_t maximumSphericalHarmonicOrder,
		     float effectiveDiffusionTime,
                     float R0,
                     bool marginalODF );


template
gkg::DotOdf::DotOdf( const gkg::OrientationSet* orientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< int32_t >& site,
		     float effectiveDiffusionTime,
                     float R0,
                     bool marginalODF,
                     const gkg::Matrix& reconstructionMatrix,
                     const gkg::Matrix& Bplus );

template
gkg::DotOdf::DotOdf( const gkg::OrientationSet* orientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< float >& site,
		     float effectiveDiffusionTime,
                     float R0,
                     bool marginalODF,
                     const gkg::Matrix& reconstructionMatrix,
                     const gkg::Matrix& Bplus );

template
void gkg::DotOdf::reconstruct( const gkg::OrientationSet* orientationSet,
                               const gkg::Volume< float >& t2,
                               const gkg::Volume< float >& dw,
                               const gkg::Vector3d< int32_t >& site,
                               float effectiveDiffusionTime,
                               float R0,
                               bool marginalODF,
                               const gkg::Matrix& reconstructionMatrix,
                               const gkg::Matrix& Bplus );

template
void gkg::DotOdf::reconstruct( const gkg::OrientationSet* orientationSet,
                               const gkg::Volume< float >& t2,
                               const gkg::Volume< float >& dw,
                               const gkg::Vector3d< float >& site,
                               float effectiveDiffusionTime,
                               float R0,
                               bool marginalODF,
                               const gkg::Matrix& reconstructionMatrix,
                               const gkg::Matrix& Bplus );


