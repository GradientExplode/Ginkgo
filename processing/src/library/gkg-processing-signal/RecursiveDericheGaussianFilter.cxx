#include <gkg-processing-signal/RecursiveDericheGaussianFilter.h>
#include <gkg-processing-signal/DericheGaussianCoefficients.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::RecursiveDericheGaussianFilter< T >::RecursiveDericheGaussianFilter()
{
}


template < class T >
gkg::RecursiveDericheGaussianFilter< T >::~RecursiveDericheGaussianFilter()
{
}


template < class T >
void gkg::RecursiveDericheGaussianFilter< T >::recurse(
                      const gkg::DericheGaussianCoefficients< T >& coefficients,
                      const std::vector< T >& in,
                      std::vector< T >& out )
{

  try
  {

    if ( in.size() < 4U )
    {

      throw std::runtime_error( "input size should be greater or equal to 4" );
      return;

    }

    // resizing temporary buffer if needed
    if ( _temporary.size() != in.size() )
    {

      _temporary.resize( in.size() );

    }

    // resizing ouput buffer if needed
    if ( out.size() != in.size() )
    {

      out.resize( in.size() );

    }

    size_t n = in.size() - 4U;

    // Calcul des yk+
    typename std::vector< T >::iterator optr0 = out.begin();
    typename std::vector< T >::iterator optr1 = optr0 + 1U;
    typename std::vector< T >::iterator optr2 = optr1 + 1U;
    typename std::vector< T >::iterator optr3 = optr2 + 1U;
    typename std::vector< T >::iterator optr4 = optr3 + 1U;

    typename std::vector< T >::const_iterator iptr0 = in.begin();
    typename std::vector< T >::const_iterator iptr1 = iptr0 + 1U;
    typename std::vector< T >::const_iterator iptr2 = iptr1 + 1U;
    typename std::vector< T >::const_iterator iptr3 = iptr2 + 1U;

    *optr0 = coefficients.n00 * *iptr0;
    *optr1 = coefficients.n00 * *iptr1 +
             coefficients.n11 * *iptr0 -
             coefficients.d11 * *optr0;
    *optr2 = coefficients.n00 * *iptr2 +
             coefficients.n11 * *iptr1 +
             coefficients.n22 * *iptr0 - 
             coefficients.d11 * *optr1 -
             coefficients.d22 * *optr0;
    *optr3 = coefficients.n00 * *iptr3 +
             coefficients.n11 * *iptr2 +
             coefficients.n22 * *iptr1 +
             coefficients.n33 * *iptr0 -
             coefficients.d11 * *optr2 -
             coefficients.d22 * *optr1 -
             coefficients.d33 * *optr0;
    ++ iptr0;
    ++ iptr1;
    ++ iptr2;
    ++ iptr3;

    for ( ; n--; )
    {

      *optr4 = coefficients.n00 * *iptr3 +
               coefficients.n11 * *iptr2 +
               coefficients.n22 * *iptr1 +
               coefficients.n33 * *iptr0 -
               coefficients.d11 * *optr3 -
               coefficients.d22 * *optr2 - 
               coefficients.d33 * *optr1 -
               coefficients.d44 * *optr0;
      ++ optr0;
      ++ optr1;
      ++ optr2;
      ++ optr3;
      ++ optr4;

      ++ iptr0;
      ++ iptr1;
      ++ iptr2;
      ++ iptr3;

    }

    // Calcul des yk-
    optr0 = _temporary.begin() + in.size() - 1U;
    optr1 = optr0 - 1U;
    optr2 = optr1 - 1U;
    optr3 = optr2 - 1U;
    optr4 = optr3 - 1U;

    iptr0 = in.begin() + in.size() - 1U;
    iptr1 = iptr0 - 1U;
    iptr2 = iptr1 - 1U;
    iptr3 = iptr2 - 1U;

    *optr0 = ( T )0.0;
    *optr1 = coefficients.n11b * *iptr0;
    *optr2 = coefficients.n11b * *iptr1 +
             coefficients.n22b * *iptr0 -
             coefficients.d11 * *optr1 -
             coefficients.d22 * *optr0;
    *optr3 = coefficients.n11b * *iptr2 +
             coefficients.n22b * *iptr1 +
             coefficients.n33b * *iptr0 -
             coefficients.d11 * *optr2 -
             coefficients.d22 * *optr1 -
             coefficients.d33 * *optr0;

    for ( n = in.size() - 4U; n--; )
    {

      *optr4-- = coefficients.n11b * *iptr3 +
                 coefficients.n22b * *iptr2 +
                 coefficients.n33b * *iptr1 +
                 coefficients.n44b * *iptr0 -
                 coefficients.d11 * *optr3 - 
                 coefficients.d22 * *optr2 - 
                 coefficients.d33 * *optr1 -
                 coefficients.d44 * *optr0;

      -- optr0;
      -- optr1;
      -- optr2;
      -- optr3;
      -- iptr0;
      -- iptr1;
      -- iptr2;
      -- iptr3;

    }

    // Calcul des yk = yk+ + yk-
    optr0 = out.begin();
    iptr0 = _temporary.begin();

    for ( n = in.size(); n--; optr0++ )
    {

      *optr0 += *iptr0++;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RecursiveDericheGaussianFilter< T >::recurse( "
             "const gkg::DericheGaussianCoefficients< T >& coefficients, "
             "const std::vector< T >& in, "
             "std::vector< T >& out )" );

}


template class gkg::RecursiveDericheGaussianFilter< float >;
template class gkg::RecursiveDericheGaussianFilter< double >;

