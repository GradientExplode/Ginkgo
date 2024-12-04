#include <gkg-processing-signal/PhaseUnwrapperFunction.h>
#include <gkg-processing-container/Volume_i.h>


void gkg::removeOffset( gkg::Volume< float >& unwrappedPhaseDifference,
                        const gkg::Volume< uint8_t >& qualityMask )
{

  try
  {

    gkg::Volume< float >::iterator u = unwrappedPhaseDifference.begin(),
                                   ue = unwrappedPhaseDifference.end();
    gkg::Volume< uint8_t >::const_iterator q = qualityMask.begin();
    int32_t count = 0;

    float average = 0;
    while ( u != ue )
    {

      if ( *q )
      {

        average += *u;
        ++ count;

      }
      ++ u;
      ++ q;

    }
    average /= ( float )count;


    u = unwrappedPhaseDifference.begin();

    while ( u != ue )
    {

       
      *u -= average;
      ++ u;

    }

  }
  GKG_CATCH( "void gkg::removeOffset( "
             "gkg::Volume< float >& unwrappedPhaseDifference, "
             "const gkg::Volume< unsigned char >& qualityMask )" );

}


void gkg::removeRamp( gkg::Volume< float >& unwrappedPhaseDifference,
                      const gkg::Volume< float >& qualityMap )
{

  try
  {

    int32_t x, y, z;

    int32_t sizeX = unwrappedPhaseDifference.getSizeX();
    int32_t sizeY = unwrappedPhaseDifference.getSizeY();
    int32_t sizeZ = unwrappedPhaseDifference.getSizeZ();

    // estimating ramp according to x, y, and z axis

    float rampX = 0.0;
    float rampY = 0.0;
    float rampZ = 0.0;
    float weightX = 0.0;
    float weightY = 0.0;
    float weightZ = 0.0;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( x + 1 < sizeX )
          {

            rampX += qualityMap( x, y, z ) *
                     ( unwrappedPhaseDifference( x + 1, y, z ) - 
                       unwrappedPhaseDifference( x, y, z ) );
            weightX += qualityMap( x, y, z );

          }
          if ( y + 1 < sizeY )
          {

            rampY += qualityMap( x, y, z ) *
                     ( unwrappedPhaseDifference( x, y + 1, z ) - 
                       unwrappedPhaseDifference( x, y, z ) );
            weightY += qualityMap( x, y, z );

          }
          if ( z + 1 < sizeZ )
          {

            rampZ += qualityMap( x, y, z ) *
                     ( unwrappedPhaseDifference( x, y, z + 1 ) - 
                       unwrappedPhaseDifference( x, y, z ) );
            weightZ += qualityMap( x, y, z );

          }

        }

      }

    }

    if ( weightX > 0 )
    {

      rampX /= weightX;

    }
    if ( weightY > 0 )
    {

      rampY /= weightY;

    }
    if ( weightZ > 0 )
    {

      rampZ /= weightZ;

    }

    // removing ramp(s)
    float middleX = ( float )( sizeX - 1 ) / 2.0;
    float middleY = ( float )( sizeY - 1 ) / 2.0;
    float middleZ = ( float )( sizeZ - 1 ) / 2.0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          unwrappedPhaseDifference( x, y, z ) -= ( rampX * ( x - middleX ) +
                                                   rampY * ( y - middleY ) +
                                                   rampZ * ( z - middleZ ) );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::removeRamp( "
             "gkg::Volume< float >& unwrappedPhaseDifference, "
             "const gkg::Volume< float >& qualityMap )" );

}
