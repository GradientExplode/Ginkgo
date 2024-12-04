#ifndef _gkg_processing_signal_DericheGaussianCoefficients_h_
#define _gkg_processing_signal_DericheGaussianCoefficients_h_


namespace gkg
{


template < class T >
struct DericheGaussianCoefficients
{

  enum Order
  {

    Smoothing,
    Gradient,
    Laplacian

  };

  DericheGaussianCoefficients( const T& sigma, Order order );

  T n00;
  T n11;
  T n22;
  T n33;

  T n11b;
  T n22b;
  T n33b;
  T n44b;

  T d11;
  T d22;
  T d33;
  T d44;


};


}


#endif
