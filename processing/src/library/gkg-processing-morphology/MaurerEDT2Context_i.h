#ifndef _gkg_processing_morphology_MaurerEDT2Context_i_h_
#define _gkg_processing_morphology_MaurerEDT2Context_i_h_


#include <gkg-processing-morphology/MaurerEDT2Context.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-exception/Exception.h>


namespace gkg
{


template < class IN, class OUT >
gkg::MaurerEDT2Context< IN, OUT >::MaurerEDT2Context( 
                                    const gkg::Volume< IN >& in,
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT sqMaxDistance,
                                    gkg::Volume< OUT >& out )
                                 : gkg::LoopContext< int32_t >(),
                                   _in( in ),
                                   _testFunction( testFunction ),
                                   _sqMaxDistance( sqMaxDistance ),
                                   _out( out )
{

  _sizeX = _in.getSizeX();
  _sizeY = _in.getSizeY();
  _in.getResolution( _resolution );

}


template < class IN, class OUT >
void gkg::MaurerEDT2Context< IN, OUT >::doIt( int32_t startIndex, 
                                              int32_t count )
{

  try
  {

    int32_t x, y, z;
    int32_t stopIndex = startIndex + count;
    OUT g[ _sizeY ];
    OUT h[ _sizeY ];

    for ( z = startIndex; z < stopIndex; z++ )
    {

      for ( y = 0; y < _sizeY; y++ )
      {

        OUT d = _sqMaxDistance;

        for ( x = 0; x < _sizeX; x++ )
        {

          if ( _testFunction.getState( _in( x, y, z ) ) )
          {

            _out( x, y, z ) = OUT( 0 );
            d = OUT( 0 );

          }
          else
          {

            if ( d < _sqMaxDistance )
            {

              _out( x, y, z ) = ++d;

            }
            else
            {

              _out( x, y, z ) = _sqMaxDistance;

            }

          }

        }

        --x;

        if ( _out( x, y, z ) < _sqMaxDistance )
        {

          for ( ; x >= 0; x-- )
          {

            OUT value = _out( x, y, z );

            if ( !value )
            {

              d = OUT( 0 );

            }
            else if ( d < _sqMaxDistance )
            {

              ++d;

              if ( d < value )
              {

                value = d;

              }

            }

            value = OUT( value * _resolution.x );
            _out( x, y, z ) = value * value;

          }

        }

      }

      for ( x = 0; x < _sizeX; x++ )
      {

        int32_t i, l, nS;
        OUT a, b, c, v, ga, ga2, delta = 0.0;

        for ( i = 0, l = -1; i < _sizeY; i++, delta += _resolution.y )
        {

          OUT value = _out( x, i, z );

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

          for ( i = 0, l = 0; i < _sizeY; i++, delta += _resolution.y )
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

            _out( x, i, z ) = ga;

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "bool gkg::MaurerEDT2Context< IN, OUT >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


}


#endif
