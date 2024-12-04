#include <gkg-mri-reconstruction-correction/BandPassAsymmetryCorrection.h>


template < class T >
gkg::BandPassAsymmetryCorrection< T >::BandPassAsymmetryCorrection()
{
}


template < class T >
gkg::BandPassAsymmetryCorrection< T >::~BandPassAsymmetryCorrection()
{
}


template < class T >
void gkg::BandPassAsymmetryCorrection< T >::apply(
                gkg::Volume< std::complex< T > >& rawVolume,
                const std::vector< int32_t >& flip,
                const std::vector< T >& receiverFilterMag,
                const std::vector< T >& receiverFilterPha,
                const T& receiverMinFrequencyStart,
                const T& receiverMaxFrequencyEnd,
                const T& receiverFrequencyStart,
                const T& receiverFrequencyEnd ) const
{

  try
  {

    int32_t timeCount = data.getSizeT();
    int32_t sliceCount = data.getSizeZ();
    int32_t lineCount = data.getSizeY();
    int32_t sizeX = data.getSizeX();

    // sanity checks
    if ( timeCount != 1 )
    {

      throw std::runtime_error( "T size should be equal to 1" );

    }

    if ( receiverFilterMag.size() != receiverFilterPha.size() )
    {

      throw std::runtime_error( "receiverFilterMag and receiverFilterPha"
                                "are not consistent" );

    }

    if ( rawVolume.getSizeY() != ( int32_t )flip.size() )
    {

      throw std::runtime_error( "flip and raw volume inconsistency" );

    }

    int32_t x, line, slice;
    int32_t fftSizeX = 1 << ( int32_t )std::ceil( std::log( ( double )sizeX ) /
                                                  std::log( 2.0 ) );
    int32_t receiverFilterSize = ( int32_t )receiverFilterMag.size();

    // allocating a BP asymmetry correction filter
    std::vector< T > bpFilterMag( fftSizeX );
    std::vector< T > bpFilterPha( fftSizeX );

    // reinterpolating receiver correction filter to fit real acquisition
    // BP information
    T maxDeltaFrequency = ( receiverMaxFrequencyEnd -
                            receiverMinFrequencyStart ) / 
                          ( ( T )receiverFilterSize - 1 )

    T deltaFrequency = ( receiverFrequencyEnd - receiverFrequencyStart ) /
                       ( ( T )fftSizeX - 1 );

    if ( deltaFrequency != maxDeltaFrequency )
    {

       // interpolate to calculate the value of elements in the receiver
       T frequency, ix;
       int32_t iMinus, iPlus;
       T delta1, delta2;
       for ( x = 0; x < fftSizeX; x++ )
       {

         // processing the current frequency
         frequency = receiverFrequencyStart + x * deltaFrequency;

         // translating it in terms of offset from minimum start frequency
         ix = ( frequency - receiverMinFrequencyStart ) / maxDeltaFrequency;

         iMinus = ( int32_t )( std::floor( ix ) + 0.5 );
         iPlus = ( int32_t )( std::ceil( ix ) + 0.5 );

         // calculating inerpolation weights
         delta1 = ix - ( T )iMinus;
         delta1 = ( T )iPlus - ix;

         if ( iMinus < 0 )
         {

           iMinus = 0;
           iPlus = 1;

         }

         // processing BP asymmetry filter coefficient
         bpFilterMag[ x ] = ( 1 - delta1 ) * receiverFilterMag[ iMinus ] +
                            ( 1 - delta2 ) * receiverFilterMag[ iPlus ];
         bpFilterPha[ x ] = ( 1 - delta1 ) * receiverFilterPha[ iMinus ] +
                            ( 1 - delta2 ) * receiverFilterPha[ iPlus ];

       } 

    }
    else
    {

       int32_t offset = ( int32_t )( ( receiverFrequencyStart -
                                       receiverMinFrequencyStart ) /
                                      maxDeltaFrequency );

       for ( x = 0; x < fftSizeX; x++ )
       {

         // processing BP asymmetry filter coefficient
         bpFilterMag[ x ] = receiverFilterMag[ offset + x ];
         bpFilterPha[ x ] = receiverFilterPha[ offset + x ];

       }

    }

    // normalizing reinterpolated receiver magnitude by value at nul frequency
    T temp = bpFilterMag[ fftSizeX / 2 ];
    if ( temp != 0 )
    {

      for ( x = 0; x < fftSizeX; x++ )
      {

        bpFilterMag[ x ] /= temp;     

      }

    }

    // normalizing reinterpolated receiver phase by value at nul frequency
    temp = bpFilterPha[ fftSizeX / 2 ];
    for ( x = 0; x < fftSizeX; x++ )
    {

      bpFilterMag[ x ] -= temp;     

    }

    // convert interpolated pahse & magnitude correction to complex
    std::vector< std::complex< T > > bpFilter( fftSizeX );
    for ( x = 0; x < fftSizeX; x++ )
    {

      bpFilter[ x ] = std::polar( bpFilterMag[ x ],
                                  bpFilterPha[ x ] * M_PI / 180.0 );

    }

    // allocating vector for working
    std::vector< std::complex< T > > dataLine( fftSizeX );


    int32_t offsetX = ( fftSizeX - sizeX ) / 2;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < offsetX; x++ )
        {

          dataLine[ x ] = std::complex< T >( 0.0, 0.0 );

        }

        for ( x = 0; x < sizeX; x++ )
        {

          dataLine[ offsetX + x ] = rawVolume( x, line, slice );

        }

        for ( x = 0; x < offsetX; x++ )
        {

          dataLine[ offsetX + sizeX + x ] = std::complex< T >( 0.0, 0.0 );

        }

        gkg::FourierTransform< T >::getInstance().applyCenteredDirect(
                                                                  dataLine );

        
        if ( flip[ line ] >= 0 )
        {

          for ( x = 0; x < fftSizeX; x++ )
          {

            dataLine[ x ] *= bpFilter[ x ];

          }

        }
        else
        {

          for ( x = 0; x < fftSizeX; x++ )
          {

            dataLine[ x ] *= bpFilter[ fftSizeX - 1 - x ];

          }

        }

        gkg::FourierTransform< T >::getInstance().applyCenteredInverse(
                                                                  dataLine );

        for ( x = 0; x < sizeX; x++ )
        {

          // we divide by fftSizeX because inverse FFT does not do it!
          rawVolume( x, line, slice ) = dataLine[ offsetX + x ] / ( T )fftSizeX;

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BandPassAsymmetryCorrection< T >::apply( "
             "gkg::Volume< std::complex< T > >& rawVolume, "
             "const std::vector< int >& flip, "
             "const std::vector< T >& receiverFilterMag, "
             "const std::vector< T >& receiverFilterPha, "
             "const T& receiverMinFrequencyStart, "
             "const T& receiverMaxFrequencyEnd, "
             "const T& receiverFrequencyStart, "
             "const T& receiverFrequencyEnd ) const " );

}


template class gkg::BandPassAsymmetryCorrection< float >;
template class gkg::BandPassAsymmetryCorrection< double >;
