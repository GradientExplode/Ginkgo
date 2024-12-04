#ifndef _gkg_processing_morphology_MaurerEDT3Context_i_h_
#define _gkg_processing_morphology_MaurerEDT3Context_i_h_


#include <gkg-processing-morphology/MaurerEDT3Context.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


namespace gkg
{


template < class T >
gkg::MaurerEDT3Context< T >::MaurerEDT3Context( const T sqMaxDistance,
                                                gkg::Volume< T >& out )
                           : gkg::LoopContext< int32_t >(),
                             _sqMaxDistance( sqMaxDistance ),
                             _out( out )
{

  _sizeX = _out.getSizeX();
  _sizeZ = _out.getSizeZ();
  _out.getResolution( _resolution );

}


template < class T >
void gkg::MaurerEDT3Context< T >::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    int32_t x, y;
    int32_t stopIndex = startIndex + count;
    T g[ _sizeZ ];
    T h[ _sizeZ ];

    for ( y = startIndex; y < stopIndex; y++ )
    {

      for ( x = 0; x < _sizeX; x++ )
      {

        int32_t i, l, nS;
        T a, b, c, v, ga, ga2, delta = 0.0;

        for ( i = 0, l = -1; i < _sizeZ; i++, delta += _resolution.z )
        {

          T value = _out( x, y, i );

          if ( value < _sqMaxDistance )
          {

            if ( l < 1 )
            {

              ++l;
              g[ l ] = value;
              h[ l ] = delta;

            }
            else
            {

              bool done = false;

              while ( !done && ( l > 0 ) )
              {

                v = h[ l ];
                a = v - h[ l - 1 ];
                b = delta - v;
                c = a + b;

                if ( ( c * g[ l ] - b * g[ l - 1 ] - a * value ) > 
                     ( a * b * c ) )
                {

                  l--;

                }
                else 
                {

                  done = true;

                }

              }

              ++l;
              g[ l ] = value;
              h[ l ] = delta;

            }

          }

        }

        if ( l != -1 )
        {

          delta = 0.0;
          nS = l;

          for ( i = 0, l = 0; i < _sizeZ; i++, delta += _resolution.z )
          {

            a = h[ l ] - delta;
            ga = g[ l ] + a * a;

            bool done = false;

            while ( !done && ( l < nS ) )
            {

              a = h[ l + 1 ] - delta;
              ga2 = g[ l + 1 ] + a * a;

              if ( ga > ga2 )
              {

                l++;
                ga = ga2;

              }
              else
              {

                done = true;

              }

            }

            _out( x, y, i ) = T( std::sqrt( ga ) );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::MaurerEDT3Context< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


}


#endif
