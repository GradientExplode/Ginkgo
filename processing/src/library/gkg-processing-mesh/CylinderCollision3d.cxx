#include <gkg-processing-mesh/CylinderCollision3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>



static float getDeterminant( const gkg::Vector3d< float >& u0,
                             const gkg::Vector3d< float >& u1,
                             const gkg::Vector3d< float >& u2 )
{

  try
  {

    return u0.x *
           ( u1.y * u2.z - u2.y * u1.z ) -
           u1.x *
           ( u0.y * u2.z - u2.y * u0.z ) +
           u2.x *
           ( u0.y * u1.z - u1.y * u0.z );

  }
  GKG_CATCH( "float getDeterminant( "
             "const gkg::Vector3d< float >& u0, "
             "const gkg::Vector3d< float >& u1, "
             "const gkg::Vector3d< float >& u2 )" );


}



float gkg::getClosestDistanceBetweenLines(
                           const gkg::Vector3d< float >& a0,
                           const gkg::Vector3d< float >& a1,
                           const gkg::Vector3d< float >& b0,
                           const gkg::Vector3d< float >& b1,
                           bool clampAll,
                           bool clampA0,
                           bool clampA1,
                           bool clampB0,
                           bool clampB1,
                           gkg::Vector3d< float >* closestPointOnSegmentA,
                           gkg::Vector3d< float >* closestPointOnSegmentB )
{

  try
  {

    // if clampAll = true, set all clamps to true
    bool theClampA0 = clampA0;
    bool theClampA1 = clampA1;
    bool theClampB0 = clampB0;
    bool theClampB1 = clampB1;
    if ( clampAll )
    {
      theClampA0 = true;
      theClampA1 = true;
      theClampB0 = true;
      theClampB1 = true;

    }


    // calculating denominator
    gkg::Vector3d< float > A = a1 - a0;
    gkg::Vector3d< float > B = b1 - b0;
    float magnitudeA = A.getNorm();
    float magnitudeB = B.getNorm();

    gkg::Vector3d< float > uA = A / magnitudeA;
    gkg::Vector3d< float > uB = B / magnitudeB;

    gkg::Vector3d< float > uACrossUB = uA.cross( uB );
    float denominator = uACrossUB.getNorm();
    denominator *= denominator;


    // if lines are parallel (denominator=0) test if lines overlap.
    // if they don't overlap then there is a closest point solution.
    // If they do overlap, there are infinite closest positions, but 
    // there is a closest distance
    if ( denominator == 0.0f )
    {

      float d0 = uA.dot( b0 - a0 );

      // overlap only possible with clamping
      if ( theClampA0 || theClampA1 || theClampB0 || theClampB1 )
      {

        float d1 = uA.dot( b1 - a0 );

        // is segment B before A ?
        if ( ( d0 <= 0.0f ) && ( d0 >= d1 ) )
        {

          if ( theClampA0 && theClampB1 )
          {

            if ( std::fabs( d0 ) < std::fabs(d1) )
            {

              if ( closestPointOnSegmentA )
              {

                *closestPointOnSegmentA = a0;

              }
              if ( closestPointOnSegmentB )
              {

                *closestPointOnSegmentB = b0;

              }
              return ( a0 - b0 ).getNorm();

            }

            if ( closestPointOnSegmentA )
            {

              *closestPointOnSegmentA = a0;

            }
            if ( closestPointOnSegmentB )
            {

              *closestPointOnSegmentB = b1;

            }
            return ( a0 - b1 ).getNorm();

          }

        }
        // is segment B after A ?
        else if ( ( d0 >= magnitudeA ) && ( magnitudeA <= d1 ) )
        {

          if ( theClampA1 && theClampB0 )
          {

            if ( std::fabs( d0 ) < std::fabs( d1 ) )
            {

              if ( closestPointOnSegmentA )
              {

                *closestPointOnSegmentA = a1;

              }
              if ( closestPointOnSegmentB )
              {

                *closestPointOnSegmentB = b0;

              }
              return ( a1 - b0 ).getNorm();

            }

            if ( closestPointOnSegmentA )
            {

              *closestPointOnSegmentA = a1;

            }
            if ( closestPointOnSegmentB )
            {

              *closestPointOnSegmentB = b1;

            }
            return ( a1 - b1 ).getNorm();

          }

        }

      }

      // segments overlap, return distance between parallel segments
      return ( ( ( uA * d0 ) + a0 ) - b0 ).getNorm();

    }

    // crossing lines: calculating the projected closest points
    gkg::Vector3d< float > t = b0 - a0;
    float determinantA = getDeterminant( t, uB, uACrossUB );
    float determinantB = getDeterminant( t, uA, uACrossUB );

    float t0 = determinantA / denominator;
    float t1 = determinantB / denominator;

    // computing the projected closest point on segment A
    gkg::Vector3d< float > pA = a0 + uA * t0; 
    // computing the projected closest point on segment B
    gkg::Vector3d< float > pB = b0 + uB * t1; 

    // clamping projections
    if ( theClampA0 || theClampA1 || theClampB0 || theClampB1 )
    {

      if ( theClampA0 && ( t0 < 0 ) )
      {

        pA = a0;

      }
      else if ( theClampA1 && ( t0 > magnitudeA ) )
      {

        pA = a1;

      }

      if ( theClampB0 && ( t1 < 0 ) )
      {

        pB = b0;

      }
      else if ( theClampB1 && ( t1 > magnitudeB ) )
      {

        pB = b1;

      }

      // clamping projection A
      if ( ( theClampA0 && ( t0 < 0 ) ) ||
           ( theClampA1 && ( t0 > magnitudeA ) ) )
      {

        float dot = uB.dot( pA - b0 );
        if ( theClampB0 && ( dot < 0 ) )
        {

          dot = 0;

        }
        else if ( theClampB1 && ( dot > magnitudeB ) )
        {

          dot = magnitudeB;

        }
        pB = b0 + uB * dot;

      }

      // clamping projection B
      if ( ( theClampB0 && ( t1 < 0 ) ) ||
           ( theClampB1 && ( t1 > magnitudeB ) ) )
      {

        float dot = uA.dot( pB - a0 );
        if ( theClampA0 && ( dot < 0 ) )
        {

          dot = 0;

        }
        else if ( theClampA1 && ( dot > magnitudeA ) )
        {

          dot = magnitudeA;

        }
        pA = a0 + uA * dot;

      }

    }

    if ( closestPointOnSegmentA )
    {

      *closestPointOnSegmentA = pA;

    }
    if ( closestPointOnSegmentB )
    {

      *closestPointOnSegmentB = pB;

    }

    return ( pA - pB ).getNorm();

  }
  GKG_CATCH( "float gkg::getClosestDistanceBetweenLines( "
             "const gkg::Vector3d< float >& a0, "
             "const gkg::Vector3d< float >& a1, "
             "const gkg::Vector3d< float >& b0, "
             "const gkg::Vector3d< float >& b1, "
             "bool clampAll, "
             "bool clampA0, "
             "bool clampA1, "
             "bool clampB0, "
             "bool clampB1, "
             "bool& returnClosestPointOnSegmentA, "
             "gkg::Vector3d< float >* closestPointOnSegmentA, "
             "bool& returnClosestPointOnSegmentB, "
             "gkg::Vector3d< float >* closestPointOnSegmentB )" );

}


bool gkg::hasCylinderCollision3d( const gkg::Vector3d< float >& center1,
                                  const gkg::Vector3d< float >& axis1,
                                  float halfLength1,
                                  float radius1,
                                  const gkg::Vector3d< float >& center2,
                                  const gkg::Vector3d< float >& axis2,
                                  float halfLength2,
                                  float radius2 )
{

  try
  {

    return ( gkg::getClosestDistanceBetweenLines(
                  center1 - axis1 * halfLength1,
                  center1 + axis1 * halfLength1,
                  center2 - axis2 * halfLength2,
                  center2 + axis2 * halfLength2,
                  true ) < ( radius1 + radius2 ) ) ? true : false;

  }
  GKG_CATCH( "bool hasCylinderCollision3d( "
             "const gkg::Vector3d< float >& center1, "
             "const gkg::Vector3d< float >& axis1, "
             "float halfLength1, "
             "float radius1, "
             "const gkg::Vector3d< float >& center2, "
             "const gkg::Vector3d< float >& axis2, "
             "float halfLength2, "
             "float radius2 )" ); 

}


#undef EPSILON

