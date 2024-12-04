#ifndef _gkg_processing_algobase_Convolution1d_i_h_
#define _gkg_processing_algobase_Convolution1d_i_h_


#include <gkg-processing-algobase/Convolution1d.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


inline
void gkg::Convolution1d::convolve( const gkg::Vector& in1,
                                   const gkg::Vector& in2,
                                   gkg::Vector& out ) const
{

  try
  {

    int32_t sizeIn1 = in1.getSize();
    int32_t sizeIn2 = in2.getSize();

    int32_t sizeOut = sizeIn1 + sizeIn2 - 1; 

    if ( out.getSize() != sizeOut )
    {

      out.reallocate( sizeOut );

    }

    int32_t i, k, minimumK, maximumK;
    for ( i = 0; i < sizeOut; i++ )
    {

      minimumK = std::max( 0, i - ( sizeIn1 - 1 ) );
      maximumK = std::min( i, ( sizeIn2 - 1 ) );

      out( i ) = 0.0;
      for ( k = minimumK; k <= maximumK; k++ )
      {

        out( i ) += in1( i - k ) * in2( k );

      }

    }

  }
  GKG_CATCH( "inline "
             "void gkg::Convolution1d::convolve( "
             "const gkg::Vector& in1, "
             "const gkg::Vector& in2, "
             "gkg::Vector& out ) const" );

}


#endif
