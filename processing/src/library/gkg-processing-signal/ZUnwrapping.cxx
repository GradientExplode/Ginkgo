#include <gkg-processing-signal/ZUnwrapping.h>
#include <gkg-processing-container/Volume_i.h>


static float getDistanceWithMask( gkg::Volume< float >& wrappedPhaseDifference,
                                  const gkg::Volume< float >& qualityMap,
                                  const gkg::Volume< int16_t > mask,
                                  int32_t referenceSlice,
                                  int32_t shiftedSlice,
                                  int32_t shift )
{

  int32_t sizeX = wrappedPhaseDifference.getSizeX();
  int32_t lineCount = wrappedPhaseDifference.getSizeY();

  float difference, distance = 0.0;

  int32_t x, line;
  for ( line = 0; line < lineCount; line++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      difference = wrappedPhaseDifference( x, line, referenceSlice ) -
                   ( wrappedPhaseDifference( x, line, shiftedSlice ) +
                     shift * 2 * M_PI );
      if ( mask( x, line, referenceSlice ) && mask( x, line, shiftedSlice ) )
      {

        distance += std::min( qualityMap( x, line, referenceSlice ),
                              qualityMap( x, line, shiftedSlice ) ) *
                    difference * difference;

      }

    }

  }
  return distance;

}


static void applyShift( gkg::Volume< float >& wrappedPhaseDifference,
                        int32_t shiftedSlice,
                        int32_t shift )
{

  int32_t sizeX = wrappedPhaseDifference.getSizeX();
  int32_t lineCount = wrappedPhaseDifference.getSizeY();

  int32_t x, line;
  for ( line = 0; line < lineCount; line++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      wrappedPhaseDifference( x, line, shiftedSlice ) += shift * 2 * M_PI;

    }

  }

}


void gkg::ZUnwrapping( gkg::Volume< float >& wrappedPhaseDifference,
                       const gkg::Volume< float >& qualityMap,
                       const gkg::Volume< int16_t >& mask,
                       int32_t referenceSlice )
{

  int32_t sliceCount = wrappedPhaseDifference.getSizeZ();
  int32_t slice;
  int32_t shift;
  float distance;

  // unwrapping from reference to top of volume
  for ( slice = referenceSlice + 1; slice < sliceCount; slice++ )
  {

    float minimumDistance = getDistanceWithMask( wrappedPhaseDifference,
                                                 qualityMap,
                                         	 mask,
                                         	 slice - 1,
                                         	 slice,
                                         	 0 );
    int32_t minimumShift = 0;

    for ( shift = 1; shift < 10; shift++ )
    {

      distance = getDistanceWithMask( wrappedPhaseDifference,
                              	      qualityMap,
                              	      mask,
                              	      slice - 1,
                              	      slice,
                              	      shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = shift;

      }

      distance = getDistanceWithMask( wrappedPhaseDifference,
                             	      qualityMap,
                             	      mask,
                             	      slice - 1,
                             	      slice,
                             	      -shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = -shift;

      }

    }

    applyShift( wrappedPhaseDifference, slice, minimumShift );

  }


  // unwrapping from reference to bottom of volume
  for ( slice = referenceSlice - 1; slice >= 0; slice-- )
  {

    float minimumDistance = getDistanceWithMask( wrappedPhaseDifference,
                                         	 qualityMap,
                                         	 mask,
                                         	 slice,
                                         	 slice + 1,
                                         	 0 );
    int32_t minimumShift = 0;

    for ( shift = 1; shift < 10; shift++ )
    {

      distance = getDistanceWithMask( wrappedPhaseDifference,
                              	      qualityMap,
                              	      mask,
                              	      slice + 1,
                              	      slice,
                              	      shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = shift;

      }

      distance = getDistanceWithMask( wrappedPhaseDifference,
                              	      qualityMap,
                              	      mask,
                              	      slice + 1,
                              	      slice,
                              	      -shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = -shift;

      }

    }

    applyShift( wrappedPhaseDifference, slice, minimumShift );

  }

}


static float getDistance( gkg::Volume< float >& wrappedPhaseDifference,
                          const gkg::Volume< float >& qualityMap,
                          int32_t referenceSlice,
                          int32_t shiftedSlice,
                          int32_t shift )
{

  int32_t sizeX = wrappedPhaseDifference.getSizeX();
  int32_t lineCount = wrappedPhaseDifference.getSizeY();

  float difference, distance = 0.0;

  int32_t x, line;
  for ( line = 0; line < lineCount; line++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      difference = wrappedPhaseDifference( x, line, referenceSlice ) -
                   ( wrappedPhaseDifference( x, line, shiftedSlice ) +
                     shift * 2 * M_PI );
      distance += std::min( qualityMap( x, line, referenceSlice ),
                            qualityMap( x, line, shiftedSlice ) ) *
                  difference * difference;

    }

  }
  return distance;

}


void gkg::ZUnwrapping( gkg::Volume< float >& wrappedPhaseDifference,
                       const gkg::Volume< float >& qualityMap,
                       int32_t referenceSlice )
{

  int32_t sliceCount = wrappedPhaseDifference.getSizeZ();
  int32_t slice;
  int32_t shift;
  float distance;

  // unwrapping from reference to top of volume
  for ( slice = referenceSlice + 1; slice < sliceCount; slice++ )
  {

    float minimumDistance = getDistance( wrappedPhaseDifference,
                                         qualityMap,
                                         slice - 1,
                                         slice,
                                         0 );
    int32_t minimumShift = 0;

    for ( shift = 1; shift < 10; shift++ )
    {

      distance = getDistance( wrappedPhaseDifference,
                              qualityMap,
                              slice - 1,
                              slice,
                              shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = shift;

      }

      distance = getDistance( wrappedPhaseDifference,
                              qualityMap,
                              slice - 1,
                              slice,
                              -shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = -shift;

      }

    }

    applyShift( wrappedPhaseDifference, slice, minimumShift );

  }


  // unwrapping from reference to bottom of volume
  for ( slice = referenceSlice - 1; slice >= 0; slice-- )
  {

    float minimumDistance = getDistance( wrappedPhaseDifference,
                                         qualityMap,
                                         slice,
                                         slice + 1,
                                         0 );
    int32_t minimumShift = 0;

    for ( shift = 1; shift < 10; shift++ )
    {

      distance = getDistance( wrappedPhaseDifference,
                              qualityMap,
                              slice + 1,
                              slice,
                              shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = shift;

      }

      distance = getDistance( wrappedPhaseDifference,
                              qualityMap,
                              slice + 1,
                              slice,
                              -shift );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumShift = -shift;

      }

    }

    applyShift( wrappedPhaseDifference, slice, minimumShift );

  }

}


void gkg::ZLineUnwrapping( gkg::Volume< float >& wrappedPhaseDifference,
                           const gkg::Volume< float >& qualityMap,
                           int32_t referenceSlice )
{

  int32_t sizeX = wrappedPhaseDifference.getSizeX();
  int32_t lineCount = wrappedPhaseDifference.getSizeY();
  int32_t sliceCount = wrappedPhaseDifference.getSizeZ();

  int32_t x, line, slice, changeCount;

  float jump;

  for ( line = 0; line < lineCount; line++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      do
      {
 
        changeCount = 0;

        for ( slice = referenceSlice; slice < sliceCount - 1; slice++ )
        {

          if ( qualityMap( x, line, slice ) >= 0.5 )
          {

            jump = wrappedPhaseDifference( x, line, slice ) -
                   wrappedPhaseDifference( x, line, slice + 1 );
            if ( jump >= 3.5 )
            {

              wrappedPhaseDifference( x, line, slice + 1 ) += 2.0 * M_PI;
              ++ changeCount;

            }
            else if ( jump <= -3.5 )
            {

              wrappedPhaseDifference( x, line, slice + 1 ) -= 2.0 * M_PI;
              ++ changeCount;

            }

          }

        }

        for ( slice = referenceSlice; slice > 0; slice-- )
        {

          if ( qualityMap( x, line, slice ) >= 0.5 )
          {

            jump = wrappedPhaseDifference( x, line, slice ) -
                   wrappedPhaseDifference( x, line, slice - 1 );
            if ( jump >= 3.5 )
            {

              wrappedPhaseDifference( x, line, slice - 1 ) += 2.0 * M_PI;
              ++ changeCount;

            }
            else if ( jump <= -3.5 )
            {

              wrappedPhaseDifference( x, line, slice - 1 ) -= 2.0 * M_PI;
              ++ changeCount;

            }

          }

        }

      }
      while ( changeCount != 0 );

    }

  }

}
