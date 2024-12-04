#include <gkg-processing-rfpulse/CayleyKleinParameters.h>


gkg::CayleyKleinParameters::CayleyKleinParameters(
                                   double phi,
                                   const gkg::Vector3d< double >& rotationAxis )
{

  gkg::Vector3d< double > tmpRotationAxis( rotationAxis );
  tmpRotationAxis.normalize();
  _alpha = std::complex< double >(
                    ( double )std::cos( phi / 2.0 ),
                    ( double )( - tmpRotationAxis.z * std::sin( phi / 2 ) ) );
  _beta = std::complex< double >(
                    ( double )( tmpRotationAxis.y * std::sin( phi / 2.0 ) ),
                    ( double )( - tmpRotationAxis.x * std::sin( phi / 2.0 ) ) );

}


gkg::CayleyKleinParameters::CayleyKleinParameters(
                                            const std::complex< double >& alpha,
                                            const std::complex< double >& beta )
                           : _alpha( alpha ),
                             _beta( beta )
{
}


gkg::CayleyKleinParameters::CayleyKleinParameters()
                           : _alpha( 1.0, 0.0 ),
                             _beta( 0.0, 0.0 )
{
}


gkg::CayleyKleinParameters::CayleyKleinParameters(
                                       const gkg::CayleyKleinParameters& other )
                           : _alpha( other._alpha ),
                             _beta( other._beta )
{
}


gkg::CayleyKleinParameters::~CayleyKleinParameters()
{
}


gkg::CayleyKleinParameters& gkg::CayleyKleinParameters::operator = (
                                       const gkg::CayleyKleinParameters& other )
{

  _alpha = other._alpha;
  _beta = other._beta;

  return *this;

}


const std::complex< double >& gkg::CayleyKleinParameters::getAlpha() const
{

  return _alpha;

}


const std::complex< double >& gkg::CayleyKleinParameters::getBeta() const
{

  return _beta;

}


gkg::CayleyKleinParameters 
gkg::CayleyKleinParameters::cross( const CayleyKleinParameters& other ) const
{

  // this is the standard SU2 spinor multiplication:
  //                [ a' ]   [ a2 -b2* ]   [ a1 ]   [ a2 a1 - b2* b1 ]
  //                [ b' ] = [ b2  a2* ] x [ b1 ] = [ b2 a1 + a2* b1 ]

  return gkg::CayleyKleinParameters( _alpha * other.getAlpha() - 
                                     std::conj( _beta ) * other.getBeta(),
                                     _beta * other.getAlpha() + 
                                     std::conj( _alpha ) * other.getBeta() );

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::CayleyKleinParameters& thing )
{

  os << "( " << thing.getAlpha() << ", "
             << thing.getBeta() << " )";
  return os;

}
