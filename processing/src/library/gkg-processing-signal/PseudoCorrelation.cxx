#include <gkg-processing-signal/PseudoCorrelation.h>
#include <gkg-processing-container/Volume_i.h>

#include <cmath>


static void SqrAvgFilter( const gkg::Volume< float >& in,
                          gkg::Volume< float >& out,
                          int32_t tsize,
                          int32_t addCode )
{

  int32_t sizeX = in.getSizeX();
  int32_t lineCount = in.getSizeY();
  int32_t sliceCount = in.getSizeZ();
  int32_t x, line, slice;

  if ( ( tsize < 3 ) && !addCode )
  {

    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          out( x, line, slice ) = 0.0;

        }

      }

    }

  }
  else
  {

    int32_t n, b, a;
    int32_t hs = tsize / 2;
    float average, r;

    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          average = 0.0;
          for ( n = 0, b = line - hs; b <= line + hs; b++ )
          {

            int32_t bb = b;
            if ( bb < 0 )
            {

              bb = -bb;

            }
            else if ( bb >= lineCount )
            {

              bb = 2 * lineCount - 2 - bb;

            } 
            for ( a = x - hs; a <= x + hs; a++ )
            {

              int32_t aa = a;
              if ( aa < 0 )
              {

                aa = -aa;

              }
              else if ( aa >= sizeX )
              {

                aa = 2 * sizeX - 2 - aa; 

              }
              if ( ( aa >= 0 ) &&
                   ( aa < sizeX - 1 ) &&
                   ( bb >= 0 ) &&
                   ( bb < lineCount - 1 ) )
              {

                r = in( aa, bb, slice );
                average += r;
                ++n;

              }

            }

          }
          r = ( n > 0 ) ? 1.0 / n : 0.0;
          average *= r;
          if ( addCode )
          {

            out( x, line, slice ) += average * average;

          }
          else
          {

            out( x, line, slice ) = average * average;

          }

        }

      }   

    }

  }

} 


void gkg::PseudoCorrelation( const gkg::Volume< float >& phase,
                             gkg::Volume< float >& result, 
                             int32_t tsize )
{

  int32_t sizeX = phase.getSizeX();
  int32_t lineCount = phase.getSizeY();
  int32_t sliceCount = phase.getSizeZ();

  gkg::Volume< float > temp( sizeX, lineCount, sliceCount );

  int32_t x, line, slice;

  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        temp( x, line, slice ) = std::cos( phase( x, line, slice ) + M_PI );

      }

    }

  }

  SqrAvgFilter( temp, result, tsize, 0 );

  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        temp( x, line, slice ) = std::sin( phase( x, line, slice ) + M_PI );

      }

    }

  }
  SqrAvgFilter( temp, result, tsize, 1 );

}
