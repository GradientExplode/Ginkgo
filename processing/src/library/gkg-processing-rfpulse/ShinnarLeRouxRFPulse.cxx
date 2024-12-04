#include <gkg-processing-rfpulse/ShinnarLeRouxRFPulse.h>
#include <gkg-processing-rfpulse/CayleyKleinParameters.h>
#include <gkg-processing-rfpulse/RippleTransform.h>
#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <fstream>


#define ZERO_PADDING_FACTOR   16
#define EPSILON               0.0000001


gkg::ShinnarLeRouxRFPulse::ShinnarLeRouxRFPulse(
                             gkg::CayleyKleinParameters* cayleyKleinParameters,
                             gkg::RippleTransform* rippleTransform,
                             int32_t sampleCount,
                             double timeBandWidth,
                             double passBandRipple,
                             double stopBandRipple,
                             gkg::FIRFilterDesignMethod* firFilterDesignMethod )
                          : gkg::RFPulse( timeBandWidth,
                                          passBandRipple,
                                          stopBandRipple,
                                          firFilterDesignMethod ),
                            _cayleyKleinParameters( cayleyKleinParameters ),
                            _rippleTransform( rippleTransform )
{


  doPolynomB( sampleCount );
  doPolynomA();
  inverseShinnarLeRouxTransform();

}


gkg::ShinnarLeRouxRFPulse::~ShinnarLeRouxRFPulse()
{
}


const std::vector< std::complex< double > >& 
gkg::ShinnarLeRouxRFPulse::getPolynomA() const
{

  return _polynomA;

}


const std::vector< std::complex< double > >& 
gkg::ShinnarLeRouxRFPulse::getPolynomB() const
{

  return _polynomB;

}


const gkg::CayleyKleinParameters& 
gkg::ShinnarLeRouxRFPulse::getPrescribedCayleyKleinParameters() const
{

  return *_cayleyKleinParameters;

}


void gkg::ShinnarLeRouxRFPulse::doPolynomB( int32_t sampleCount )
{

  _polynomB = _firFilterDesignMethod->getImpulseResponse( 
                       sampleCount,
                       _timeBandWidth,
                       _rippleTransform->getPassBandRipple( _passBandRipple ),
                       _rippleTransform->getStopBandRipple( _stopBandRipple ) );

  std::ofstream os( "h.txt" );
  for ( size_t u = 0; u < _polynomB.size(); u++ )
    os << std::real( _polynomB[ u ] ) << std::endl;
  os.close();

  scalePolynomB();

}


void gkg::ShinnarLeRouxRFPulse::scalePolynomB()
{

  std::vector< std::complex< double > >::iterator b = _polynomB.begin(),
                                                  be = _polynomB.end();
  while ( b != be )
  {

    *b *= ( double )std::sqrt( std::norm( _cayleyKleinParameters->getBeta() ) );
    b ++;

  }
  std::ofstream os( "b.txt" );
  for ( size_t u = 0; u < _polynomB.size(); u++ )
    os << _polynomB[ u ] << std::endl;
  os.close();

}


void gkg::ShinnarLeRouxRFPulse::doPolynomA()
{

  int32_t nextPowerOfTwo = 1 << ( int32_t )ceil( 
                                           log( ( double )_polynomB.size() ) /
                                           log( 2.0 ) );
  nextPowerOfTwo *= 2;

  // we do some oversampling in order to be more accurate
  std::vector< std::complex< double > > 
    tempPolynomB( nextPowerOfTwo * ZERO_PADDING_FACTOR,
                  std::complex< double >( 0.0f, 0.0f ) );

  int32_t i;
  for ( i = 0; i < ( int32_t )_polynomB.size(); i++ )
  {

    tempPolynomB[ i ] = _polynomB[ i ];

  }


  // performing the Fourier transform
  gkg::FourierTransform< double >::getInstance().applyRawDirect( tempPolynomB);

  double maxNormOfB = 0.0f;
  double tempNormOfB = 0.0;
  std::vector< std::complex< double > >::iterator b = tempPolynomB.begin(),
                                                 be = tempPolynomB.end();
  while ( b != be )
  {

    tempNormOfB = std::norm( *b );
    if ( tempNormOfB > maxNormOfB )
    {

      maxNormOfB = tempNormOfB;

    }
    b ++;

  }

  // we force polynom B to have items with magnitude lower than 1.0
  if ( maxNormOfB >= 1.0f )
  {

    b = tempPolynomB.begin();
    be = tempPolynomB.end();
    while ( b != be )
    {

      *b /= std::sqrt( maxNormOfB + EPSILON );
      b ++;

    }

  }

  // we compute temporary alpha magnitude
  std::vector< double > tempMagnitudeA( nextPowerOfTwo * ZERO_PADDING_FACTOR );
  std::vector< double >::iterator am = tempMagnitudeA.begin(),
                                 ame = tempMagnitudeA.end();
  b = tempPolynomB.begin();
  be = tempPolynomB.end();
  while ( am != ame )
  {

    *am = ( double )std::sqrt( 1.0 - std::norm( *b ) );
    am ++;
    b ++;

  }

  // we process oversampled alpha polynom:
  // we compute the phase of alpha, knowing that magnitude and phase are
  // Hilbert transform pair
  // we want the Hilbert transform of the log magnitude, so we generate
  // the analytic version of the signal knowing that the imaginary part will be
  // the negative of the Hilbert transform of the log magnitude
  std::vector< std::complex< double > > 
    tempPolynomA( nextPowerOfTwo * ZERO_PADDING_FACTOR );
  std::vector< std::complex< double > >::iterator a = tempPolynomA.begin(),
                                                 ae = tempPolynomA.end();
  am = tempMagnitudeA.begin();
  ame = tempMagnitudeA.end();
  while ( a != ae )
  {

    *a = std::complex< double >( std::log( *am ), 0.0f );
    a ++;
    am ++;

  }

  gkg::FourierTransform< double >::getInstance().applyRawDirect( tempPolynomA);

  // we leave DC and halfway point untouched
  for ( i = 1; i < ( int32_t )tempPolynomA.size() / 2 - 1; i++ )
  {

    tempPolynomA[ i ] *= 2.0f;

  }
  for ( i = ( int32_t )tempPolynomA.size() / 2 + 1;
        i < ( int32_t )tempPolynomA.size(); i++ )
  {

    tempPolynomA[ i ] = std::complex< double >( 0.0f, 0.0f );

  }

  gkg::FourierTransform< double >::getInstance().applyRawInverse(tempPolynomA);

  a = tempPolynomA.begin();
  ae = tempPolynomA.end();
  while ( a != ae )
  {

    *a /= ( double )tempPolynomA.size();
    a ++;

  }

  std::ofstream os( "a.txt" );
  for ( size_t u = 0; u < tempPolynomA.size(); u++ )
    os << tempPolynomA[ u ] << std::endl;
  os.close();


  // we compute the minimum phase temporary alpha
  am = tempMagnitudeA.begin();
  ame = tempMagnitudeA.end();
  a = tempPolynomA.begin();
  ae = tempPolynomA.end();
  while ( a != ae )
  {

    *a = std::polar( *am, -std::imag( *a ) );
    a ++;
    am ++;

  } 

  // we compute the minimum phase alpha coefficients  
  gkg::FourierTransform< double >::getInstance().applyRawInverse(tempPolynomA);
  _polynomA = std::vector< std::complex< double > >( _polynomB.size() );
  for ( i = 0; i < ( int32_t )_polynomA.size(); i++ )
  {

    _polynomA[ ( int32_t )_polynomA.size() - i - 1 ] =
      tempPolynomA[ i ] / ( double )tempPolynomA.size();

  }

}


// we use the definitions from SLR paper
void gkg::ShinnarLeRouxRFPulse::inverseShinnarLeRouxTransform()
{

  int32_t i, j;
  double cj;
  std::complex< double > sj;
  double phi;
  double theta;

  _samples = std::vector< std::complex< double > >( _polynomA.size() );

  // we do copy because we don't want to erase initial polynomials A and B
  std::vector< std::complex< double > >  a = _polynomA;
  std::vector< std::complex< double > >  b = _polynomB;

  std::vector< std::complex< double > >  tempA( a.size() );
  std::vector< std::complex< double > >  tempB( b.size() );

  // we perform SLR inverse recursion
  for ( j = ( int32_t )a.size() - 1; j >= 0; j-- )
  {

    cj = std::sqrt( 1.0 / ( 1 + std::norm( b[ j ] ) /
                                std::norm( a[ j ] )  ) );
    sj = std::conj( b[ j ] / a[ j ] ) * cj;

    // get phi and theta
    phi = 2 * std::atan2( std::sqrt( std::norm( sj ) ), cj );
    theta = std::arg( sj );

    // get RF from phi and theta
    _samples[ j ] = std::polar( phi, theta );

    // create the new polynomals now
    for ( i = 0; i <= j; i++ )
    {

      tempA[ i ] = cj * a[ i ] + sj * b[ i ];
      tempB[ i ] = - std::conj( sj ) * a[ i ] + cj * b[ i ];

    }

    // copy new polynomials to old ones
    for ( i = 0; i < j; i++ )
    {

      a[ i ] = tempA[ i + 1 ];
      b[ i ] = tempB[ i ];

    }

  }

}


#undef ZERO_PADDING_FACTOR
#undef EPSILON
