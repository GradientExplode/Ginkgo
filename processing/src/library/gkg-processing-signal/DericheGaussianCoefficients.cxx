#include <gkg-processing-signal/DericheGaussianCoefficients.h>
#include <cmath>


template < class T >
gkg::DericheGaussianCoefficients< T >::DericheGaussianCoefficients(
                            const T& sigma,
                            gkg::DericheGaussianCoefficients< T >::Order order )
{

  T a0 = 0;
  T a1 = 0;
  T c0 = 0;
  T c1 = 0;
  T b0 = 0;
  T b1 = 0;
  T w0 = 0;
  T w1 = 0;

  switch ( order )
  {

    case gkg::DericheGaussianCoefficients< T >::Smoothing:

      a0 = 0.657 / sigma;
      a1 = 1.979 / sigma;
      c0 = -0.258 / sigma;
      c1 = -0.239 / sigma;
      b0 = 1.906;
      b1 = 1.881;
      w0 = 0.651;
      w1 = 2.053;
      break;

    case gkg::DericheGaussianCoefficients< T >::Gradient:

      a0 = -0.173f / ( sigma * sigma );
      a1 = -2.004f / ( sigma * sigma );
      c0 = 0.173f / ( sigma * sigma );
      c1 = 0.444f / ( sigma * sigma );
      b0 = 1.561f;
      b1 = 1.594f;
      w0 = 0.700f;
      w1 = 2.145f;
      break;

    case gkg::DericheGaussianCoefficients< T >::Laplacian:

      a0 = -0.724f / ( sigma * sigma * sigma );
      a1 = 1.689f / ( sigma * sigma * sigma );
      c0 = 0.325f / ( sigma * sigma * sigma );
      c1 = -0.721f / ( sigma * sigma * sigma );
      b0 = 1.295f;
      b1 = 1.427f;
      w0 = 0.779f;
      w1 = 2.234f;
      break;

  }

  T b0s = b0 / sigma;
  T b1s = b1 / sigma;
  T w0s = w0 / sigma;
  T w1s = w1 / sigma;
  T eb0s = ( T )exp( ( double )( -b0s ) );
  T eb1s = ( T )exp( ( double )( -b1s ) );
  T e2b0s = ( T )exp( ( double )( -2.0f * b0s ) );
  T e2b1s = ( T )exp( ( double )( -2.0f * b1s ) );
  T cw0s = ( T )cos( ( double )w0s );
  T sw0s = ( T )sin( ( double )w0s );
  T cw1s = ( T )cos( ( double )w1s );
  T sw1s = ( T )sin( ( double )w1s );

  n00 = a0 + c0;
  n11 = ( eb1s * ( c1 * sw1s - ( c0 + 2.0f * a0 ) * cw1s ) +
          eb0s * ( a1 * sw0s - ( 2.0f * c0 + a0 ) * cw0s ) );
  n22 = ( 2.0f * eb1s * eb0s * ( ( a0 + c0 ) * cw1s * cw0s - 
          cw1s * a1 * sw0s - cw0s * c1 * sw1s ) + c0 * e2b0s + a0 * e2b1s );
  n33 = ( eb1s * e2b0s * ( c1 * sw1s - cw1s * c0 ) + 
          eb0s * e2b1s * ( a1 * sw0s - cw0s * a0 ) );

  d11 = -2.0f * eb1s * cw1s - 2.0f * eb0s * cw0s;
  d22 = 4.0f * cw1s * cw0s * eb0s * eb1s + e2b1s + e2b0s;
  d33 = -2.0f * cw0s * eb0s * e2b1s - 2.0f * cw1s * eb1s * e2b0s;
  d44 = e2b0s * e2b1s;

  // antisymmetrical for derivative
  if ( order == gkg::DericheGaussianCoefficients< T >::Gradient )
  {

    n11b = d11 * n00 - n11;
    n22b = d22 * n00 - n22;
    n33b = d33 * n00 - n33;
    n44b = d44 * n00;

  }
  else  // symmetrical for smoothing and laplacian
  {

    n11b = n11 - d11 * n00;
    n22b = n22 - d22 * n00;
    n33b = n33 - d33 * n00;
    n44b = -1.0f * d44 * n00;

  }

}


template class gkg::DericheGaussianCoefficients< float >;
template class gkg::DericheGaussianCoefficients< double >;
