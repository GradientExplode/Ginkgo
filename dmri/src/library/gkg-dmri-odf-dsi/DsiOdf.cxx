#include <gkg-dmri-odf-dsi/DsiOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-odf-dsi/HanningWindow.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <cmath>


#define EPSILON 1e-6


gkg::DsiOdf::DsiOdf( const gkg::OrientationSet* inputOrientationSet,
                     const gkg::OrientationSet* outputOrientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< int32_t >& site,
                     bool filterOrNot,
                     float R0min, float R0max,
                     bool marginalODF )
            : gkg::OrientationDistributionFunction(
                                 gkg::OrientationDistributionFunction::Standard,
                                 outputOrientationSet )
{

  try
  {

    if ( inputOrientationSet )
    {

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

      if ( bValues.size() != 515 )
      {

        throw std::runtime_error( "This DSI code was designed for a fixed "
                                  "Cartesian grid with 515 points");
 
      }

      // building reconstruction matrix
      gkg::Volume< float > filter( 17, 17, 17 );
      prepareForDSI( filterOrNot, filter );

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

      // building normalized signal vector
      gkg::Vector signal( orientationCount );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        signal( t ) = ( float )dwValue;
        _averageNormalizedDwSignal += signal( t ) / ( float )averageT2;
        _averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      performDSI( inputOrientationSet, signal, filter, 
                  R0min, R0max, marginalODF );

    }

  }
  GKG_CATCH( "gkg::DsiOdf::DsiOdf( "
             "const gkg::OrientationSet* inputOrientationSet,  "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "bool filterOrNot, "
             "bool R0min, bool R0max, "
             "bool marginalODF )" );

}


gkg::DsiOdf::DsiOdf( const gkg::OrientationSet* inputOrientationSet,
                     const gkg::OrientationSet* outputOrientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< float >& site,
                     bool filterOrNot,
                     float R0min, float R0max,
                     bool marginalODF )
            : gkg::OrientationDistributionFunction(
                                 gkg::OrientationDistributionFunction::Standard,
                                 outputOrientationSet )
{

  try
  {
  
    if ( inputOrientationSet )
    {

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

      if ( bValues.size() != 515 )
      {

        throw std::runtime_error( "This DSI code was designed for a fixed "
                                  "Cartesian grid with 515 points");
 
      }

      // building reconstruction matrix
      gkg::Volume< float > filter( 17, 17, 17 );
      prepareForDSI( filterOrNot, filter );

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
      float averageT2;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      averageT2 = ( float )t2Value;
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

      // building normalized signal vector
      gkg::Vector signal( orientationCount );
      float dwValue = 0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        signal( t ) = ( float )dwValue;
        _averageNormalizedDwSignal += signal( t ) / ( float )averageT2;
        _averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;
      
      performDSI( inputOrientationSet, signal, filter, 
                  R0min, R0max, marginalODF );
      
    }

  }
  GKG_CATCH( "gkg::DsiOdf::DsiOdf( "
             "const gkg::OrientationSet* inputOrientationSet,  "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "bool filterOrNot, "
             "float R0min, float R0max, "
             "bool marginalODF )" );

}


gkg::DsiOdf::DsiOdf( const gkg::OrientationSet* inputOrientationSet,
                     const gkg::OrientationSet* outputOrientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< int32_t >& site,
                     const gkg::Volume< float >& filter,
                     float R0min, float R0max,
                     bool marginalODF )
            : gkg::OrientationDistributionFunction(
                                 gkg::OrientationDistributionFunction::Standard,
                                 outputOrientationSet )
{

  try
  {

    if ( inputOrientationSet )
    {

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

      // building normalized signal vector 
      gkg::Vector signal( orientationCount );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        signal( t ) = ( float )dwValue;
        _averageNormalizedDwSignal += signal( t )/ ( float )averageT2;
        _averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      performDSI( inputOrientationSet, signal, filter, 
                  R0min, R0max, marginalODF );
      
    }

  }
  GKG_CATCH( "gkg::DsiOdf::DsiOdf( "
             "const gkg::OrientationSet* inputOrientationSet,  "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "const Volume< float >& filter, "
             "float R0min, float R0max, "
             "bool marginalODF )" );

}


gkg::DsiOdf::DsiOdf( const gkg::OrientationSet* inputOrientationSet, 
                     const gkg::OrientationSet* outputOrientationSet,
                     const gkg::Volume< float >& t2,
                     const gkg::Volume< float >& dw,
                     const gkg::Vector3d< float >& site,
                     const gkg::Volume< float> & filter,
                     float R0min, float R0max,
                     bool marginalODF )
            : gkg::OrientationDistributionFunction(
                                 gkg::OrientationDistributionFunction::Standard,
                                 outputOrientationSet )
{

  try
  {
    
    if ( inputOrientationSet )
    {

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
      float averageT2;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      averageT2 = ( float )t2Value;
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

      // building normalized signal vector
      gkg::Vector signal( orientationCount );
      float dwValue = 0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        signal( t ) = ( float )dwValue;
        _averageNormalizedDwSignal += signal( t )/ ( float )averageT2;
        _averageDwSignal += dwValue;

      }

      _averageNormalizedDwSignal /= orientationCount;
      _averageDwSignal /= orientationCount;

      performDSI( inputOrientationSet, signal, filter, R0min, R0max,
                  marginalODF );
      
    }
    
  }
  GKG_CATCH( "gkg::DsiOdf::DsiOdf( "
             "const gkg::OrientationSet* inputOrientationSet,  "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "const gkg::Volume< float >& filter, "
             "float R0min, float R0max )" );

}


gkg::DsiOdf::~DsiOdf()
{
}


float gkg::DsiOdf::getAverageDwSignal() 
{

  try
  {

    return _averageDwSignal;

  }
  GKG_CATCH( "float gkg::DsiOdf::getAverageDwSignal()" );

}


float gkg::DsiOdf::getAverageNormalizedDwSignal() 
{

  try
  {

    return _averageNormalizedDwSignal;

  }
  GKG_CATCH( "float gkg::DsiOdf::getAverageDwSignal()" );

}


float gkg::DsiOdf::getMeanSquaredDisplacement() 
{

  try
  {

    return _meanSquaredDisplacement;

  }
  GKG_CATCH( "float gkg::DsiOdf::getMeanSquaredDisplacement()" );

}


void gkg::DsiOdf::prepareForDSI( bool filterOrNot,
                                 gkg::Volume< float >& filter )
{

  try
  {
    
    if ( filterOrNot ) // get Hanning window filter
    {

      for ( int i = 0; i < filter.getSizeX(); i++ )
      {

        for ( int j = 0; i < filter.getSizeY(); j++ )
        {

          for ( int k = 0; i < filter.getSizeZ(); k++ )
          {

            filter( i, j, k ) = hanningWindow[ i ][ j ][ k ];

          }

        }

      }

    }
    else
    {

      filter.fill( 1.0 );  // Identity 
      
    }

  }
  GKG_CATCH( "void gkg::DsiOdf::prepareForDSI( "
             "bool filterOrNot, "
             "gkg::Volume< float >& filter )" );

}


////////////////////////////////////////////////////////////////////////////////
//
// This code is directly adapted from Wedeen et al MRM 2005
//
// Signal is acquired on a 515 cartesian grid on a lattice of radius 5.
// ( a 10x10x10 cartesian cube has 1000 points but here, taking only points
//   in a sphere of radius 5, gives the 515 points )
//
// Then, the signal is placed in the center of a grid 17 x 17 x 17
// The extra space is zero-padded.  Hence, the center of q-space is at 
// (8,8,8) and the signal is centered, [0-7], [8, 16]
//
// Once, the signal is correctly placed at the center of this grid,
// the only thing to do is to evaluate the FFT centered along each axis, X,Y,Z
//
////////////////////////////////////////////////////////////////////////////////
void gkg::DsiOdf::performDSI( const gkg::OrientationSet* inputOrientationSet,  
                              const gkg::Vector& inputSignal,  
                              const gkg::Volume< float >& filter,
                              const float R0min, const float R0max,
                              const bool marginalODF )
{

  try
  {

    // Turn debug on to save signal, signal filtered and pdf volumes
    // 
    bool debug = false;

    int32_t gridSize = filter.getSizeX();
    int32_t orientationCount = inputOrientationSet->getCount();

    // this should never happen
    if ( inputSignal.getSize() != orientationCount ) 
    {

      throw std::runtime_error( "orientation count != signal size" );
    
    }

    // Allocating grid points for PDF in real space
    //  
    gkg::Volume< float > pdf( gridSize, gridSize, gridSize );
    pdf.fill( 0.0 );
    gkg::Vector data( orientationCount );

    // Allocating grid points for signal in q-space
    // 
    gkg::Volume< float > signal( gridSize, gridSize, gridSize );
    gkg::Volume< std::complex< float > > complex_signal(
                                                 gridSize, gridSize, gridSize );
    signal.fill( 0.0 );
    complex_signal.fill( ( std::complex< float > ) 0.0 );

    // Define center of q-space
    int32_t center = gridSize / 2;

    if( debug )
    {

      std::cout << " Center of q-space is : " << center << std::endl;

    }

    // Allocating current orientation
    gkg::Vector3d< float > orientation;

    // Filling the data to the signal grid
    int32_t o;
    for ( o = 0; o < orientationCount; o++ ) 
    {

      orientation = inputOrientationSet->getOrientation( o );

      signal( center + ( int32_t )orientation.x,
              center + ( int32_t )orientation.y,
              center + ( int32_t )orientation.z ) = inputSignal( o );

      std::complex< float > complexInputSignal( inputSignal( o ), 0.0 );
      complex_signal( center + ( int32_t )orientation.x, 
                      center + ( int32_t )orientation.y,
                      center + ( int32_t )orientation.z ) = complexInputSignal;

    } 

    if ( debug )
    {
    
      std::cout << "Writing input signal : " << std::flush;
      gkg::Writer::getInstance().write( "signal.ima",
                                        signal, false, "gis" );
      std::cout << "done" << std::endl;

    }

    // filtering the signal ( could use gkg::FilterFunction )
    // 
    int32_t i, j, k;
    for ( i = 0; i < gridSize; i++ )
    {
      
      for ( j = 0; j < gridSize; j++ )
      {
 
        for ( k = 0; k < gridSize; k++ )
        {
      
          signal( i, j, k ) *= filter( i, j, k );
          complex_signal( i, j, k ) *= filter( i, j, k );
   
        }
 
      }
      
    }

    if ( debug )
    {
      
      std::cout << "Writing filtered signal : " << std::flush;
      gkg::Writer::getInstance().write( "signal_filtered.ima",
                                        signal, false, "gis" );
      std::cout << "done" << std::endl;
      
    }

    // Fourier Transform along each of the axis separately, X, Y, Z
    gkg::FourierTransform3d< float >::getInstance().applyCenteredDirect(
                                       complex_signal,
                                       gkg::FourierTransform3d< float >::X_AXIS, 
                                       0, false );
    
    gkg::FourierTransform3d< float >::getInstance().applyCenteredDirect(
                                       complex_signal,
                                       gkg::FourierTransform3d< float >::Y_AXIS, 
                                       0, false );

    gkg::FourierTransform3d< float >::getInstance().applyCenteredDirect(
                                       complex_signal,
                                       gkg::FourierTransform3d< float >::Z_AXIS, 
                                       0, false );

    ////////////////////////////////////////////////////////////////////////////
    // MeanSquaredDisplacement (MSD)
    //
    // Sum of all PDF value weighted by radius squared
    // MSD = int_R PDF(R)R^2dR
    ////////////////////////////////////////////////////////////////////////////
    float meanSquaredDisplacement = 0.0;

    for ( i = 0; i < gridSize; i++ )
    {
      
      for ( j = 0; j < gridSize; j++ )
      {
 
        for ( k = 0; k < gridSize; k++ )
        {
             
          float a = real( complex_signal( i, j, k ) );
          float b = imag( complex_signal( i, j, k) );
          pdf( i, j, k ) = std::sqrt( a * a + b * b );

          meanSquaredDisplacement += pdf( i, j, k ) *
                                     ( ( center - i ) * ( center - i ) + 
                                       ( center - j ) * ( center - j ) + 
                                       ( center - k ) * ( center - k ) );
          
        }
 
      }
 
    }
    _meanSquaredDisplacement = meanSquaredDisplacement;

    if ( debug )
    {
      
      std::cout << "Writing FFT of signal : " << std::flush;
      gkg::Writer::getInstance().write( "fft.ima",
                                        pdf, false, "gis" );
      std::cout << "done" << std::endl;

      std::cout << "Mean Squared Displacement : " 
                << _meanSquaredDisplacement << std::endl;
      
    }

    ////////////////////////////////////////////////////////////////////////////
    //  Now computing the DSI ODF
    // 
    //  Marginal : Wedeen et al, MRM 2005
    //  odf = int_ROmin^R0max pdf( r ) r^2 dr
    //
    //  Tuch : Tuch's thesis DSI
    //  odf = int_ROmin^R0max pdf( r ) dr
    ////////////////////////////////////////////////////////////////////////////
    float rIncrement = 0.1;
    float radius;
    
    int32_t outputOrientationCount = _orientationSet->getCount();
    // Resize ODF values
    _values.resize( outputOrientationCount );
    _values.assign( outputOrientationCount, 0.0 );

    if( debug )
    {

      std::cout << "ODF computation between " 
                << R0min << " and " << R0max 
                << std::endl;
      
    }

    for( radius = R0min; radius <= R0max; radius += rIncrement )
    {
      
      // Computing the PDF at a specifi radius via interpolation
      interpolatePDF( pdf, radius, _values, marginalODF );

    }

    // normalizing the ODF
    float sum = 0.0;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      sum += ( float )_values[ o ];

    }
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      _values[ o ] = ( float )_values[ o ] / sum;
      
    }

  }
  GKG_CATCH( "void gkg::DsiOdf::performDSI( "
             "const gkg::OrientationSet* inputOrientationSet,  "
             "const gkg::Vector& signal,  "
             "const gkg::Volume< float >& filter, "
             "const float R0min, const float R0max ) " );

}


void gkg::DsiOdf::interpolatePDF( const Volume< float >& pdf,
                                  const float radius,
                                  std::vector< float >& odfValues,
                                  const bool marginalODF )
{

  try
  {

    float x, y, z, fx, fy, fz, cx, cy, cz, xd, yd, zd, i1, i2, j1, j2, w1, w2;

    int32_t gridSize = pdf.getSizeX();
    int32_t center = gridSize / 2;

    int32_t orientationCount = _orientationSet->getCount();
    gkg::Vector3d< float > orientation;
    float value;
    int32_t o;

    for( o = 0; o < orientationCount; o++ ) 
    {

      orientation = _orientationSet->getOrientation( o );
      x = radius * orientation.x;
      y = radius * orientation.y;
      z = radius * orientation.z;
    
      xd = x - floorf( x );
      yd = y - floorf( y );
      zd = z - floorf( z );

      fx = center + floorf( x );
      fy = center + floorf( y );
      fz = center + floorf( z );

      cx = center + ceilf( x );
      cy = center + ceilf( y );
      cz = center + ceilf( z );

      i1 = pdf( ( int32_t )fx, ( int32_t )fy, ( int32_t )fz ) * ( 1 - zd ) + 
           pdf( ( int32_t )fx, ( int32_t )fy, ( int32_t )cz ) * zd;

      i2 = pdf( ( int32_t )fx, ( int32_t )cy, ( int32_t )fz ) * ( 1 - zd ) + 
           pdf( ( int32_t )fx, ( int32_t )cy, ( int32_t )cz ) * zd;

      j1 = pdf( ( int32_t )cx, ( int32_t )fy, ( int32_t )fz ) * ( 1 - zd ) + 
           pdf( ( int32_t )cx, ( int32_t )fy, ( int32_t )cz ) * zd;

      j2 = pdf( ( int32_t )cx, ( int32_t )cy, ( int32_t )fz ) * ( 1 - zd ) + 
           pdf( ( int32_t )cx, ( int32_t )cy, ( int32_t )cz ) * zd;
    
      w1 = i1 * ( 1 - yd ) + i2 * yd;
      w2 = j1 * ( 1 - yd ) + j2 * yd;
    
      // Getting the interpolated value
      value = w1 * ( 1 - xd ) + w2 * xd;

      if ( marginalODF ) 
      {
        
        odfValues[ o ] = ( float )odfValues[ o ] + radius * radius * value;      
        
      }
      else
      {

        odfValues[ o ] = ( float )odfValues[ o ] + value; 
        
      }

    }
    
  }
  GKG_CATCH( "void gkg::DsiOdf::interpolatePDF( "
             "const Volume< float >& pdf, "
             "const float radius, "
             "std::vector< float >& odfValues, "
             "const bool marginalODF )" );

}


#undef EPSILON

