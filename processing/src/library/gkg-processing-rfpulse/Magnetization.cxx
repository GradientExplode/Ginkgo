#include <gkg-processing-rfpulse/Magnetization.h>
#include <gkg-processing-rfpulse/RFPulse.h>


gkg::Magnetization::Magnetization( double Mx, double My, double Mz )
                   : _Mxy( Mx, My ),
                     _Mz( Mz )
{
}


gkg::Magnetization::Magnetization( const std::complex< double >& Mxy,
                                   double Mz )
                   : _Mxy( Mxy ),
                     _Mz( Mz )
{
}


gkg::Magnetization::Magnetization( const Magnetization& other )
                   : _Mxy( other._Mxy ),
                     _Mz( other._Mz )
{
}


gkg::Magnetization::~Magnetization()
{
}


double gkg::Magnetization::getMx() const
{

  return std::real( _Mxy );

}


double gkg::Magnetization::getMy() const
{

  return std::imag( _Mxy );

}


double gkg::Magnetization::getMz() const
{

  return _Mz;

}


const std::complex< double >& gkg::Magnetization::getMxy() const
{

  return _Mxy;

}


gkg::Magnetization gkg::Magnetization::applyBlochEquation(
                                         const gkg::RFPulse& rfPulse,
                                         const std::vector< double >& gradientX,
                                         const std::vector< double >& gradientY,
                                         double coordX,
                                         double coordY,
                                         bool crushed ) const
{

  gkg::CayleyKleinParameters
    cayleyKleinParameters = rfPulse.getEffectiveCayleyKleinParameters(
                                                              gradientX,
                                                              gradientY,
                                                              coordX,
                                                              coordY );
  if ( crushed )
  {

    cayleyKleinParameters = gkg::CayleyKleinParameters(
                                   std::complex< double >( 0.0, 0.0 ),
                                   cayleyKleinParameters.getBeta() );

  }

  // using Bloch equation (without T1 & T2 decay):
  //       [ Mxy'  ]   [ (a*)^2      -b^2    2(a*)b      ]   [ Mxy  ]
  //       [ Mxy'* ] = [ -(b*)^2     a^2     2a(b*)      ] x [ Mxy* ]
  //       [ Mz'   ]   [ -(a*)(b*)   -ab     a(a*)-b(b*) ]   [ Mz   ]
  // note: we know that Mz' will be real, so we tae the real part only!

  std::complex< double > a = cayleyKleinParameters.getAlpha();
  std::complex< double > b = cayleyKleinParameters.getBeta();
  std::complex< double > ac = std::conj( a );
  std::complex< double > bc = std::conj( b );
  std::complex< double > Mxyc = std::conj( _Mxy );


  return gkg::Magnetization(   ac * ac *  _Mxy
                             -  b *  b *   Mxyc
                             + ac *  b * 2.0 * _Mz,
                             std::real( - ac * bc * _Mxy
                                        -  a *  b *  Mxyc
                                        + ( a * ac - b * bc ) * _Mz ) );

}
