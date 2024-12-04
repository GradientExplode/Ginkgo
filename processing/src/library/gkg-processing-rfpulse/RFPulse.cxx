#include <gkg-processing-rfpulse/RFPulse.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <cmath>


gkg::RFPulse::RFPulse( double timeBandWidth,
                       double passBandRipple,
                       double stopBandRipple,
                       gkg::FIRFilterDesignMethod* firFilterDesignMethod )
             : _timeBandWidth( timeBandWidth ),
               _passBandRipple( passBandRipple ),
               _stopBandRipple( stopBandRipple ),
               _firFilterDesignMethod( firFilterDesignMethod )
{


}


gkg::RFPulse::~RFPulse()
{
}


double gkg::RFPulse::getTimeBandWidth() const
{

  return _timeBandWidth;

}


double gkg::RFPulse::getPassBandRipple() const
{

  return _passBandRipple;

}


double gkg::RFPulse::getStopBandRipple() const
{

  return _stopBandRipple;

}


int32_t gkg::RFPulse::getSampleCount() const
{

  return ( int )_samples.size();

}


const std::complex< double >& gkg::RFPulse::getSample( int32_t index ) const
{

  return _samples[ index ];

}


const std::vector< std::complex< double > >& gkg::RFPulse::getSamples() const
{

  return _samples;

}


gkg::CayleyKleinParameters
gkg::RFPulse::getEffectiveCayleyKleinParameters(
                                        const std::vector< double >& gradientX,
                                        const std::vector< double >& gradientY,
                                        double coordX,
                                        double coordY ) const
{

  if ( ( gradientX.size() != _samples.size() ) ||
       ( gradientY.size() != _samples.size() ) )
  {

    std::cerr << "gkg::RFPulse::getEffectiveCayleyKleinParameters:"
              << " gradientX and gradientY should have same size as RF samples"
              << std::endl;
    return gkg::CayleyKleinParameters();

  }

  int32_t s;
  double phi, grd;
  gkg::Vector3d< double > rotationAxis;
  gkg::CayleyKleinParameters oldParameters;
  gkg::CayleyKleinParameters newParameters;
  for ( s = 0; s < ( int32_t )_samples.size(); s++ )
  {

    grd = gradientX[ s ] * coordX + gradientY[ s ] * coordY;
    phi = std::sqrt( std::norm( _samples[ s ] ) + grd * grd );
    if ( phi > 0.0 )
    {

      rotationAxis = gkg::Vector3d< double >( std::real( _samples[ s ] ),
                                              std::imag( _samples[ s ] ),
                                              grd );
      rotationAxis /= phi;

    }
    else
    {

      rotationAxis = gkg::Vector3d< double >( 0.0, 0.0, 1.0 );

    }
    newParameters = gkg::CayleyKleinParameters( phi, rotationAxis ).cross(
                                                            oldParameters );
    oldParameters = newParameters;

  }

  return newParameters;

}
