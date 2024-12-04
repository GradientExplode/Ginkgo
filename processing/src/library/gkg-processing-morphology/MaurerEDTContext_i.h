#ifndef _gkg_processing_morphology_MaurerEDTContext_i_h_
#define _gkg_processing_morphology_MaurerEDTContext_i_h_


#include <gkg-processing-morphology/MaurerEDTContext.h>
#include <gkg-core-exception/Exception.h>


namespace gkg
{


template < class T >
gkg::MaurerEDTContext< T >::MaurerEDTContext()
                          : gkg::LoopContext< int32_t >()
{
}


template < class T >
bool gkg::MaurerEDTContext< T >::voronoiEDT( std::vector< double >& f, 
                                             int32_t size,
                                             double resolution,
                                             T sqMaxDistance ) const
{

  try
  {

    int32_t i, l, nS;
    double a, b, c, v, ga, ga2, delta = 0.0;

    std::vector< double > g( size );
    std::vector< double > h( size );

    for ( i = 0, l = -1; i < size; i++, delta += resolution )
    {

      if ( f[ i ] < sqMaxDistance )
      {

        if ( l < 1 )
        {

          ++l;
          g[ l ] = f[ i ];
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

            if ( ( c * g[ l ] - b * g[ l - 1 ] - a * f[ i ] ) > ( a * b * c ) )
            {

              l--;

            }
            else 
            {

              done = true;

            }

          }

          ++l;
          g[ l ] = f[ i ];
          h[ l ] = delta;

        }

      }

    }

    if ( l == -1 )
    {

      return false;

    }

    delta = 0.0;
    nS = l;

    for ( i = 0, l = 0; i < size; i++, delta += resolution )
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

      f[ i ] = ga;

    }

    return true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MaurerEDTContext< T >::voronoiEDT( "
             "std::vector< double >& f, int32_t sizeY, "
             "double resolutionY, T sqMaxDistance ) const" );

}


}


#endif
