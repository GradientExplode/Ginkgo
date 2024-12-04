#include <gkg-mri-reconstruction-correction/DcOffsetCorrection.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


template < class T >
gkg::DcOffsetCorrection< T >::DcOffsetCorrection()
{
}


template < class T >
gkg::DcOffsetCorrection< T >::~DcOffsetCorrection()
{
}


template < class T >
void gkg::DcOffsetCorrection< T >::apply( gkg::Volume< std::complex<T > >& data,
                                          int32_t rawSizeX, 
                                          int32_t rawSizeY,
                                          int32_t rawSizeZ,
                                          std::complex< T >* dcOffset,
                                          bool verbose ) const
{

  try
  {


    int32_t timeCount = data.getSizeT();

    if ( timeCount != 1 )
    {

      throw std::runtime_error( "T size should be equal to 1" );

    }


    std::complex< T > offset( 0.0, 0.0 );
    

    int32_t x, line, slice, count;

    // calculating the average of right 20% of volume
    count = 0;
    for ( slice = 0; slice < rawSizeZ; slice++ )
    {

      for ( line =0; line < rawSizeY; line++ )
      {

        for ( x = ( int32_t )( 0.8 * rawSizeX ); x < rawSizeX; x++ )
        {

          offset += data( x, line, slice );
          count ++;

        }

      }
      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << rawSizeZ
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }
    offset /= ( T )count;

    if ( dcOffset )
    {

      *dcOffset = offset;

    }

    // removing the previous aberage to the entire volume
    for ( slice = 0; slice < rawSizeZ; slice++ )
    {

      for ( line = 0; line < rawSizeY; line++ )
      {

        for ( x = 0; x < rawSizeX; x++ )
        {

          data( x, line, slice ) -= offset;

        }

      }
      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << rawSizeZ
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DcOffsetCorrection< T >::apply( "
             "gkg::Volume< std::complex<T > >& data, "
             "int rawSizeX, int rawSizeY, "
             "std::complex< T >* dcOffset ) const" );

}


template class gkg::DcOffsetCorrection< float >;
template class gkg::DcOffsetCorrection< double >;
