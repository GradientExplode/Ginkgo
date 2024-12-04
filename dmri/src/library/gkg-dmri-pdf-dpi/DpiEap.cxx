#include <gkg-dmri-pdf-dpi/DpiEap.h>
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


#define EPSILON 1e-6


gkg::DpiEap::DpiEap( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< int32_t >& site,
		     int32_t numberOfShells,
		     int32_t numberOfOrientationsPerShell,
		     int32_t maximumSphericalHarmonicOrder )
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      //std::cout << "here3\n";

      // collecting b-value
      std::vector< double > bValues;
      if ( dw.getHeader().hasAttribute( "bvalues" ) )
      {
	  
        dw.getHeader().getAttribute( "bvalues", bValues );

      }
      else
      {

        throw std::runtime_error( "missing 'bvalues' attribute" );

      }

      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      getReconstructionMatrix( dw,
			       bValues,
			       numberOfShells,
			       numberOfOrientationsPerShell,
			       maximumSphericalHarmonicOrder,
                               reconstructionMatrix );

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float averageT2 = ( float )t2( site, 0 );
      for ( t = 1; t < t2Count; t++ )
      {

        averageT2 += ( float )t2( site, t );

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      _averageT2 = averageT2;

      // building input signal vector
      gkg::Vector inputSignal( orientationCount );
      gkg::Vector normalizedSignal( orientationCount );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        inputSignal( t ) = ( float )dwValue;
	normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
	_averageNormalizedDwSignal += inputSignal( t ) / ( float )averageT2;
	_averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      // processing DW spherical harmonics coefficient(s)
      _dwSphericalHarmonicsCoefficients = 
      //reconstructionMatrix.getTransposition().getComposition( normalizedSignal );
      reconstructionMatrix.getTransposition().getComposition( inputSignal );

      // processing EAP spherical harmonics coefficient(s)
      int32_t eapSHCoefficientCount = reconstructionMatrix.getSize2() / 2;
            
      // resizing EAP vector to SH coefficient count
      _values.resize( eapSHCoefficientCount );

      gkg::NumericalAnalysisImplementationFactory* factory =
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      int32_t s, l, m;
      double ratio, oddFactorial;
      for ( s = 0; s < eapSHCoefficientCount; s++ )
      {

	// For the EAP the X vector must be separated into C and D coefficients
	// We only need C coefficients for EAP reconstruction
	// Descoteaux et al MRM 2009 Eq. [5]
	factory->getSymmetricalSphericalHarmonicLM( s, l, m );
	
	if ( l == 0 )
	  oddFactorial = 1;
	else
	  oddFactorial = factory->getOddFactorial( 2*l - 1 );
	
	if ( oddFactorial == 0 ) 
	{
	    
	  std::runtime_error( "division by zero" );
	  
	}
	
	ratio = ( std::pow( (double) - 1, (double) l/2 )*
		  std::pow( 2*M_PI, (double)l - 1 ) ) / oddFactorial;
	
	//if( _dwSphericalHarmonicsCoefficients( 0 ) > 0 )
	_values[ s ] = 2*ratio*( (double)_dwSphericalHarmonicsCoefficients( s ) );

	
	// only missing R_0 ^{l - 2} to obtain EAP @ R_0
	
      }
      
    }

  }
  GKG_CATCH( "gkg::DpiEap::DpiEap( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t numberOfShells, "
	     "int32_t numberOfOrientationsPerShell, "
	     "int32_t maximumSphericalHarmonicOrder )" );

}


gkg::DpiEap::DpiEap( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< float >& site,
		     int32_t numberOfShells,
		     int32_t numberOfOrientationsPerShell,
		     int32_t maximumSphericalHarmonicOrder )
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      //std::cout << "here4\n";

      // collecting b-value
      std::vector< double > bValues;
      if ( dw.getHeader().hasAttribute( "bvalues" ) )
      {
	  
        dw.getHeader().getAttribute( "bvalues", bValues );

      }
      else
      {

        throw std::runtime_error( "missing 'bvalues' attribute" );

      }


      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      getReconstructionMatrix( dw,
			       bValues,
			       numberOfShells,
			       numberOfOrientationsPerShell,
                               maximumSphericalHarmonicOrder,
                               reconstructionMatrix);

      // getting pointer to quick partial voluming resampler
      gkg::PartialVolumingQuickResampler< float >& resampler =
        gkg::PartialVolumingQuickResampler< float >::getInstance();

      // collecting resolution of data
      gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
      if ( t2.getHeader().hasAttribute( "resolutionX" ) )
      {

        t2.getHeader().getAttribute( "resolutionX", resolution.x );

      }
      if ( t2.getHeader().hasAttribute( "resolutionY" ) )
      {

        t2.getHeader().getAttribute( "resolutionY", resolution.y );

      }
      if ( t2.getHeader().hasAttribute( "resolutionZ" ) )
      {

        t2.getHeader().getAttribute( "resolutionZ", resolution.z );

      }

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float t2Value;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      float averageT2 = ( float )t2Value;
      for ( t = 1; t < t2Count; t++ )
      {

        resampler.resample( t2,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            t2Value,
                            &resolution,
                            t );
        averageT2 += ( float )t2Value;

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      _averageT2 = averageT2;

      // building input signal vector
      gkg::Vector inputSignal( orientationCount );
      gkg::Vector normalizedSignal( orientationCount );
      float dwValue = 0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        inputSignal( t ) = ( float )dwValue;
	normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
	_averageNormalizedDwSignal += inputSignal( t ) / ( float )averageT2;
	_averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      // processing DW spherical harmonics coefficient(s)
      _dwSphericalHarmonicsCoefficients = 
	reconstructionMatrix.getTransposition().getComposition( inputSignal );
      //reconstructionMatrix.getTransposition().getComposition( normalizedSignal );
      // processing EAP spherical harmonics coefficient(s)
      int32_t eapSHCoefficientCount = reconstructionMatrix.getSize2() / 2;

            
      // resizing EAP vector to SH coefficient count
      _values.resize( eapSHCoefficientCount );

      gkg::NumericalAnalysisImplementationFactory* factory =
	gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      int32_t s, l, m;
      double ratio, oddFactorial;
      for ( s = 0; s < eapSHCoefficientCount; s++ )
      {

	// For the EAP the X vector must be separated into C and D coefficients
	// We only need C coefficients for EAP reconstruction
	// Descoteaux et al MRM 2009 Eq. [5]
	factory->getSymmetricalSphericalHarmonicLM( s, l, m );
	
	if ( l == 0 )
	  oddFactorial = 1;
	else
	  oddFactorial = factory->getOddFactorial( 2*l - 1 );
	
	if ( oddFactorial == 0 ) 
	{
	    
	  std::runtime_error( "division by zero" );
	  
	}
	
	ratio = ( std::pow( (double) - 1, (double) l/2 )*
		  std::pow( 2*M_PI, (double)l - 1 ) ) / oddFactorial;
	
	_values[ s ] = 2* ratio*( (double)_dwSphericalHarmonicsCoefficients( s ) );
	// only missing R_0 ^{l - 2} to obtain EAP @ R_0
	

      }

    }

  }
  GKG_CATCH( "gkg::DpiEap::DpiEap( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
	     "int32_t numberOfShells, "
	     "int32_t numberOfOrientationsPerShell, "
	     "int32_t maximumSphericalHarmonicOrder )" );

}


gkg::DpiEap::DpiEap( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< int32_t >& site,
		     const std::vector< float >& qValues,
		     const gkg::Matrix& reconstructionMatrix )
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet ),
		  _qValues( qValues )
{

  try
  {

    if ( _orientationSet )
    {

      //      std::cout << "here1\n";

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float averageT2 = ( float )t2( site, 0 );
      for ( t = 1; t < t2Count; t++ )
      {

        averageT2 += ( float )t2( site, t );

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      _averageT2 = averageT2;

      // building input signal vector 
      gkg::Vector inputSignal( orientationCount );
      gkg::Vector normalizedSignal( orientationCount );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        inputSignal( t ) = ( float )dwValue;
	normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
	_averageNormalizedDwSignal += inputSignal( t ) / ( float )averageT2;
	_averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      // processing DW spherical harmonics coefficient(s)
      _dwSphericalHarmonicsCoefficients = 
	reconstructionMatrix.getTransposition().getComposition( inputSignal );
      //reconstructionMatrix.getTransposition().getComposition( normalizedSignal );

      // processing EAP spherical harmonics coefficient(s)
      int32_t eapSHCoefficientCount = reconstructionMatrix.getSize2() / 2;

      // resizing EAP vector to SH coefficient count
      _values.resize( eapSHCoefficientCount );

      gkg::NumericalAnalysisImplementationFactory* factory =
	gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      int32_t s, l, m;
      double ratio, oddFactorial;
      for ( s = 0; s < eapSHCoefficientCount; s++ )
      {

	// For the EAP the X vector must be separated into C and D coefficients
	// We only need C coefficients for EAP reconstruction
	// Descoteaux et al MRM 2009 Eq. [5]
	factory->getSymmetricalSphericalHarmonicLM( s, l, m );

	if ( l == 0 )
	  oddFactorial = 1;
	else
	  oddFactorial = factory->getOddFactorial( 2*l - 1 );
	
	if ( oddFactorial == 0 ) 
	{
	    
	  std::runtime_error( "division by zero" );
	  
	}

	ratio = ( std::pow( (double) - 1.0, (double) l/2.0 )*
		  std::pow( 2.0*M_PI, (double)l - 1.0 ) ) / oddFactorial;

	_values[ s ] = 2 * ratio * 
	  ( (double)_dwSphericalHarmonicsCoefficients( s ) );
	//only missing R_0 ^{l - 2} to obtain EAP @ R_0

      }

     }

  }
  GKG_CATCH( "gkg::DpiEap::DpiEap( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
	     "const std::vector< float >& qValues, "
             "const gkg::Matrix& reconstructionMatrix )" );

}


gkg::DpiEap::DpiEap( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< float >& site,
		     const std::vector< float >& qValues,
		     const gkg::Matrix& reconstructionMatrix)
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet ),
		  _qValues( qValues )
{

  try
  {

    if ( _orientationSet )
    {

      //std::cout << "here2\n";

      // getting pointer to quick partial voluming resampler
      gkg::PartialVolumingQuickResampler< float >& resampler =
        gkg::PartialVolumingQuickResampler< float >::getInstance();

      // collecting resolution of data
      gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
      if ( t2.getHeader().hasAttribute( "resolutionX" ) )
      {

        t2.getHeader().getAttribute( "resolutionX", resolution.x );

      }
      if ( t2.getHeader().hasAttribute( "resolutionY" ) )
      {

        t2.getHeader().getAttribute( "resolutionY", resolution.y );

      }
      if ( t2.getHeader().hasAttribute( "resolutionZ" ) )
      {

        t2.getHeader().getAttribute( "resolutionZ", resolution.z );

      }

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float t2Value;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      float averageT2 = ( float )t2Value;
      for ( t = 1; t < t2Count; t++ )
      {

        resampler.resample( t2,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            t2Value,
                            &resolution,
                            t );
        averageT2 += ( float )t2Value;

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      _averageT2 = averageT2;

      // building input signal vector
      gkg::Vector inputSignal( orientationCount );
      gkg::Vector normalizedSignal( orientationCount );
      float dwValue = 0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        inputSignal( t ) = ( float )dwValue;
	normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
	_averageNormalizedDwSignal += inputSignal( t ) / ( float )averageT2;
	_averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      // processing DW spherical harmonics coefficient(s)
      _dwSphericalHarmonicsCoefficients = 
	reconstructionMatrix.getTransposition().getComposition( inputSignal );

      // processing EAP spherical harmonics coefficient(s)
      int32_t eapSHCoefficientCount = reconstructionMatrix.getSize2() / 2;
            
      // resizing EAP vector to SH coefficient count
      _values.resize( eapSHCoefficientCount );

      gkg::NumericalAnalysisImplementationFactory* factory =
	gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      int32_t s, l, m;
      double ratio, oddFactorial;
      for ( s = 0; s < eapSHCoefficientCount; s++ )
      {

	// For the EAP the X vector must be separated into C and D coefficients
	// We only need C coefficients for EAP reconstruction
	// Descoteaux et al MRM 2009 Eq. [5]
	factory->getSymmetricalSphericalHarmonicLM( s, l, m );
	
	if ( l == 0 )
	  oddFactorial = 1;
	else
	  oddFactorial = factory->getOddFactorial( 2*l - 1 );
	
	if ( oddFactorial == 0 ) 
	{
	    
	  std::runtime_error( "division by zero" );
	  
	}
	
	ratio = ( std::pow( (double) - 1, (double) l/2 )*
		  std::pow( 2*M_PI, (double)l - 1 ) ) / oddFactorial;
	
	_values[ s ] = 2* ratio*( (double)_dwSphericalHarmonicsCoefficients( s ) );
	// only missing R_0 ^{l - 2} to obtain EAP @ R_0
	
      }
      
    }

  }
  GKG_CATCH( "gkg::DpiEap::DpiEap( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
	     "const std::vector< float >& qValues, "
             "const gkg::Matrix& reconstructionMatrix)" );

}


gkg::DpiEap::~DpiEap()
{
}


const gkg::Vector& gkg::DpiEap::getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::DpiEap::getDwSphericalHarmonicsCoefficients() const" );

}


float gkg::DpiEap::getAverageDwSignal() 
{

  try
  {

    return _averageDwSignal;

  }
  GKG_CATCH( "float gkg::DpiEap::getAverageDwSignal()" );

}

float gkg::DpiEap::getAverageNormalizedDwSignal() 
{

  try
  {

    return _averageNormalizedDwSignal;

  }
  GKG_CATCH( "float gkg::DpiEap::getAverageDwSignal()" );

}

///////////////////////////////////////////////////////////////////////////////
//  P(0) = int_q E(q) dq  done numerically with getAverageNormalizeDwSignal()
//  or analytically where
// 
//  P(0) = qmax^2  * sqrt(pi) ( c_0 + d_0*2*qmax/3 )
//////////////////////////////////////////////////////////////////////////////
float gkg::DpiEap::getAnalyticalEapAtZero() 
{

  try
  {

    int32_t N = _dwSphericalHarmonicsCoefficients.getSize() / 2;
    float qmax;
    
    if( _qValues.size() == 0 )
      qmax = 1.0;
    else
      qmax = _qValues[ _qValues.size() - 1 ];

    return qmax*qmax*std::sqrt(M_PI)*( 
			     _dwSphericalHarmonicsCoefficients( 0 ) +
			     _dwSphericalHarmonicsCoefficients( N )*2*qmax/3 )
           / _averageT2;
    
  }
  GKG_CATCH( "float gkg::DpiEap::getAnalyticEapAtZero()" );

}


////////////////////////////////////////////////////////////////////////
//  MSD = int_R P(R) R^2 dR  
//  Can be solved analytically 
// 
//  MSD = 2*c_0*rmax^3 / (3*sqrt(pi)) 
//  The rmax multiplication will be handled later in the Functors...
///////////////////////////////////////////////////////////////////////      
float gkg::DpiEap::getAnalyticalMeanSquaredDisplacement() 
{

  try
  {

    ///////////////////////////////////////////////////////////
    // This is a hack!!! c0 coefficient should not be negative.
    // If so, all spherical values are negative...
    // (Don't know why, only happens for the 45 degree phantom
    //////////////////////////////////////////////////////////
//     int32_t N = _dwSphericalHarmonicsCoefficients.getSize() / 2;

//     return 2*std::sqrt( M_PI )*_dwSphericalHarmonicsCoefficients( 0 ) / 
//       (3*M_PI*M_PI) + 
//       1/(4*std::sqrt( M_PI ))*_dwSphericalHarmonicsCoefficients( N );

    if ( _dwSphericalHarmonicsCoefficients( 0 ) > 0 )
      return 2 * _dwSphericalHarmonicsCoefficients( 0 )
                                                     / (3 * std::sqrt( M_PI ))
	* _averageT2; 
    else
      return 2 * _dwSphericalHarmonicsCoefficients( 
			     _dwSphericalHarmonicsCoefficients.getSize() / 2) 
	/ (3 * std::sqrt( M_PI )) * _averageT2; 
    
    //return 2 * std::sqrt( M_PI ) * _values[ 0 ] / 3 * _averageT2 );     

  }
  GKG_CATCH( "float gkg::DpiEap::getAnalyticMeanSquaredDisplacement()" );

}


void gkg::DpiEap::getReconstructionMatrix(
                                const gkg::HeaderedObject& headeredObject,
				std::vector< double > bValues,
				int32_t numberOfShells,
				int32_t numberOfOrientationsPerShell,
                                int32_t maximumSphericalHarmonicOrder,
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
    int32_t M = numberOfOrientationsPerShell;

    // building matrix of spherical harmonics on input orientation(s)
    gkg::Matrix B( M, N );
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

        B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // Laplacian smoothing matrix
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t l;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;
      L( N + n, N + n ) = sphericalLaplacian * sphericalLaplacian;
      
    }

    // compute irregular and regular solid harmonics exponents for each shell
    // I1 = 1/ q1^{l(j) + 1}, I2 = 1/ q2^{l(j) + 1}, etc...
    // R1 = q1^{l(j)}, R2 = q2^{l(j)}, etc...
    gkg::Matrix IrregularSolidHarmonicCoefficients[ numberOfShells ];
    gkg::Matrix RegularSolidHarmonicCoefficients[ numberOfShells ];

    int32_t s, j;
    //std::vector < float > qValues;

    for( s = 0; s < numberOfShells; s++)
    {
      
      // transform bValue into qValue
      // first divide by 1000 and than take square root
      // we do not take effective diffusion time and 4 pi in consideration
      // we simply use the relation that q propto sqrt(b)
      double bValue = bValues[ s*numberOfOrientationsPerShell ];
      bValue /= 1000;
      _qValues.push_back( std::sqrt( bValue ) / (2*M_PI) );
      
    }
    
    for( s = 0; s < numberOfShells; s++)
    {

      j = 0;

      IrregularSolidHarmonicCoefficients[ s ].reallocate( N, N );
      RegularSolidHarmonicCoefficients[ s ].reallocate( N, N );

      for( n = 0; n <= maximumSphericalHarmonicOrder; n+=2) 
      {
	
	for( m = -n; m <= n; m++) 
	{
	  
	  if(n == 0) 
	  {

	    if ( _qValues [ s ] == 0 ) 
	    {
	      
	      throw std::runtime_error( "qValue is null : division by zero" );

	    }
	    else
	    {
	      
	      IrregularSolidHarmonicCoefficients[ s ]( j, j ) = 1 / _qValues[ s ]; 

	    }
	    
	    RegularSolidHarmonicCoefficients[ s ]( j, j ) = 1;    
	  
	  }
	  else 
	  {

	    if ( _qValues[ s ] == 0 ) 
	    {
	      
	      throw std::runtime_error( "qValue is null : division by zero" );

	    }
	    else
	    {
	    
	      IrregularSolidHarmonicCoefficients[ s ]( j, j ) = 
		1 / std::pow( _qValues[ s ], n + 1 ); 
	      
	    }

	    RegularSolidHarmonicCoefficients[ s ]( j, j )  = 
	      std::pow( _qValues[ s ], n );     

	  }
	  
	  j++;

	}

      }

    }      

    gkg::Matrix IrregularSolidHarmonicMatrices[ numberOfShells ];
    gkg::Matrix RegularSolidHarmonicMatrices[ numberOfShells ];

    for( s = 0; s < numberOfShells; s++ ) 
    {
      
      IrregularSolidHarmonicMatrices[ s ] = B.getComposition(
					 IrregularSolidHarmonicCoefficients[ s ] );

      RegularSolidHarmonicMatrices[ s ] = B.getComposition(
					 RegularSolidHarmonicCoefficients[ s ] );

    }

    // Descoteaux et al MRM 2009, Eq. [10]
    // Laplacian Assumption for the signal
    // E(q) = sum_j [ c_j / q^(l(j)+1) + d_j q^(l(j)) ] Y (theta, phi)

    // A = [ Irr1*B , Reg1*B
    //       Irr2*B , Reg2*B
    //       ...
    //       Irrn*B , Regn*B ]
    gkg::Matrix A( 2*N , numberOfShells*M );
    for( n = 0; n < A.getSize1(); n++ ) 
    {
      
      s = 1;
      for( m = 0; m < A.getSize2(); m++ ) 
      {      
	  
	if( m / s == M )
	  s++;

	//std::cout << "n,m : " << n << "," << m << "\t";
	if( n < N) 
	{ // irregural coefficients: first half of the A matrix
	  
	  //std::cout << "first : " << m - (s-1)*M << "," << n << "\n";
	  A( n, m ) = IrregularSolidHarmonicMatrices[ s-1 ]( m - (s-1)*M, n );	  
	  
	}
	else 
	{  // regural coefficients: 2nd half of the B matrix
	    
	  //std::cout << "second : " << m - (s-1)*M << "," << n - N << "\n";
	  A( n, m ) = RegularSolidHarmonicMatrices[ s-1 ]( m - (s-1)*M, n - N );
	  
	}
	
      }
      
    }
    
    // building reconstruction matrix Aplus
    // reconstructionMatrix = (A^T A)^(-1) A^T
    reconstructionMatrix.reallocate( A.getSize2(), A.getSize1() );
    double lambda = 1.0;
    //factory->getMoorePenrosePseudoInverse( A, _reconstructionMatrix );    
    factory->getThikonovPseudoInverse( A.getTransposition(), L,
    				       lambda,
    				       reconstructionMatrix );
    reconstructionMatrix = reconstructionMatrix.getTransposition();
    
    //factory->getMoorePenrosePseudoInverse( A, reconstructionMatrix );
    
  }
  GKG_CATCH( "void gkg::DpiEap::getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::OrientationSet& outputOrientationSet, "
	     "int32_t numberOfShells,"
	     "int32_t numberOfOrientationsPerShell,"
             "int32_t maximumSphericalHarmonicOrder, "
             "gkg::Matrix& reconstructionMatrix )" );

}


#undef EPSILON
