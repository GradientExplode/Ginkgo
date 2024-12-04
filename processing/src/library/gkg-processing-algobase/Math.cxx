#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


double gkg::ceil125( double x )
{

  try
  {

    if ( x == 0.0 ) 
    {

      return 0.0;

    }

    double sign = ( x > 0) ? 1.0 : -1.0;
    double lx = std::log10( std::fabs( x ) );
    double p10 = std::floor( lx );

    double fr = std::pow( 10.0, lx - p10 );
    if ( fr <= 1.0 )
    {

      fr = 1.0;

    }
    else if ( fr <= 2.0 )
    {

      fr = 2.0;

    }
    else if ( fr <= 5.0 )
    {

      fr = 5.0;

    }
    else
    {

      fr = 10.0;

    }

    return sign * fr * std::pow( 10.0, p10 );

  }
  GKG_CATCH( "double gkg::ceil125( double x )" );

} 



float gkg::getPointToTriangleDistanceAndProjectionPoint(
                                       const gkg::Vector3d< float >& point,
                                       const gkg::Vector3d< float >& vertex1,
                                       const gkg::Vector3d< float >& vertex2,
                                       const gkg::Vector3d< float >& vertex3,
                                       gkg::Vector3d< float >& projectionPoint )
{

  try
  {

    float distance = -1.0f;

    ////////////////////////////////////////////////////////////////////////////
    //  Projection of a point on a plane
    ////////////////////////////////////////////////////////////////////////////
    //  plane: A x + B y + C z = k, contains a point v( vx, vy, vz )
    //  
    //  project the point p( px, py, pz ) to the plane
    //  line perpendicular to the plane:
    //    
    //        x = px + A * t
    //        y = py + B * t
    //        z = pz + C * t
    //
    //  thus  A ( px + A * t ) + B ( py + B * t ) + C ( pz + C * t ) = k
    //
    //        t = ( k - ( A px + B py + C pz ) ) / ( A^2 + B^2 + C^2 );
    //  where k = A vx + B vy + C vz
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > v12 = vertex2 - vertex1;
    gkg::Vector3d< float > v13 = vertex3 - vertex1;
    gkg::Vector3d< float > v23 = vertex3 - vertex2;

    gkg::Vector3d< float > normal = v12.cross( v13 );

    if ( normal.getNorm2() == 0 )
    {

      throw std::runtime_error( "normal is a null vector" );

    }
    normal.normalize();

    float t = normal.dot( vertex1 - point );
    projectionPoint = point + normal * t;

    // checking if the projection point is inside the triangle
    if ( ( ( ( projectionPoint - vertex1 ).cross( v12 ) ).
                                               dot( v13.cross( v12 ) ) > 0 ) &&
         ( ( ( projectionPoint - vertex2 ).cross( v23 ) ).
                                              dot( -v12.cross( v23 ) ) > 0 ) &&
         ( ( ( projectionPoint - vertex3 ).cross( -v13 ) ).
                                               dot( v23.cross( v13 ) ) > 0 ) )
    {

      // case: the projection point is inside the triangle
      if ( t == 0 )
      {

        // the point locates on the triangle
        distance = 0;

      }
      else
      {

        distance = ( point - projectionPoint ).getNorm();

      }

    }
    else
    {

      // case: the projection point is outside the triangle -> comparing the
      // distances between the projection point and each line segment defined
      // by the vertices
      float pointToSegmentDistance =
        std::min( std::min(
          gkg::getPointToSegmentDistance( point, vertex1, vertex2 ),
          gkg::getPointToSegmentDistance( point, vertex2, vertex3 ) ),
          gkg::getPointToSegmentDistance( point, vertex3, vertex1 ) );

      distance = ( float )std::sqrt( ( point - projectionPoint ).getNorm2() +
                              pointToSegmentDistance * pointToSegmentDistance );

    }

    if ( distance < 0 )
    {

      throw std::runtime_error( "distance should not be negative." );

    }

    return distance;

  }
  GKG_CATCH( "float gkg::getPointToTriangleDistanceAndProjectionPoint( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< float >& vertex1, "
             "const gkg::Vector3d< float >& vertex2, "
             "const gkg::Vector3d< float >& vertex3, "
             "gkg::Vector3d< float >& projectionPoint )" );

}


double gkg::getPointToTriangleDistanceAndProjectionPoint( 
                                      const gkg::Vector3d< double >& point,
                                      const gkg::Vector3d< double >& vertex1,
                                      const gkg::Vector3d< double >& vertex2,
                                      const gkg::Vector3d< double >& vertex3,
                                      gkg::Vector3d< double >& projectionPoint )
{

  try
  {

    double distance = -1.0f;

    ////////////////////////////////////////////////////////////////////////////
    //  Projection of a point on a plane
    ////////////////////////////////////////////////////////////////////////////
    //  plane: A x + B y + C z = k, contains a point v( vx, vy, vz )
    //  
    //  project the point p( px, py, pz ) to the plane
    //  line perpendicular to the plane:
    //    
    //        x = px + A * t
    //        y = py + B * t
    //        z = pz + C * t
    //
    //  thus  A ( px + A * t ) + B ( py + B * t ) + C ( pz + C * t ) = k
    //
    //        t = ( k - ( A px + B py + C pz ) ) / ( A^2 + B^2 + C^2 );
    //  where k = A vx + B vy + C vz
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< double > v12 = vertex2 - vertex1;
    gkg::Vector3d< double > v13 = vertex3 - vertex1;
    gkg::Vector3d< double > v23 = vertex3 - vertex2;

    gkg::Vector3d< double > normal = v12.cross( v13 );


    if ( normal.getNorm2() == 0 )
    {

      throw std::runtime_error( "normal is a null vector" );

    }
    normal.normalize();

    double t = normal.dot( vertex1 - point );
    projectionPoint = point + normal * t;

    // checking if the projection point is inside the triangle
    if ( ( ( ( projectionPoint - vertex1 ).cross( v12 ) ).
                                               dot( v13.cross( v12 ) ) > 0 ) &&
         ( ( ( projectionPoint - vertex2 ).cross( v23 ) ).
                                              dot( -v12.cross( v23 ) ) > 0 ) &&
         ( ( ( projectionPoint - vertex3 ).cross( -v13 ) ).
                                               dot( v23.cross( v13 ) ) > 0 ) )
    {

      // case: the projection point is inside the triangle

      if ( t == 0 )
      {

        // the point locates on the triangle
        distance = 0;

      }
      else
      {

        distance = ( point - projectionPoint ).getNorm();

      }

    }
    else
    {

      // case: the projection point is outside the triangle -> comparing the
      // distances between the projection point and each line segment defined
      // by the vertices
      double pointToSegmentDistance =
        std::min( std::min(
          gkg::getPointToSegmentDistance( point, vertex1, vertex2 ),
          gkg::getPointToSegmentDistance( point, vertex2, vertex3 ) ),
          gkg::getPointToSegmentDistance( point, vertex3, vertex1 ) );

      distance = std::sqrt( ( point - projectionPoint ).getNorm2() +
                            pointToSegmentDistance * pointToSegmentDistance );

    }

    if ( distance < 0 )
    {

      throw std::runtime_error( "distance should not be negative." );

    }

    return distance;

  }
  GKG_CATCH( "double gkg::getPointToTriangleDistanceAndProjectionPoint( "
             "const gkg::Vector3d< double >& point, "
             "const gkg::Vector3d< double >& vertex1, "
             "const gkg::Vector3d< double >& vertex2, "
             "const gkg::Vector3d< double >& vertex3, "
             "gkg::Vector3d< double >& projectionPoint )" );

}




float gkg::getPointToTriangleDistance( const gkg::Vector3d< float >& point,
                                       const gkg::Vector3d< float >& vertex1,
                                       const gkg::Vector3d< float >& vertex2,
                                       const gkg::Vector3d< float >& vertex3 )
{

  try
  {

    gkg::Vector3d< float > projectionPoint;
    return gkg::getPointToTriangleDistanceAndProjectionPoint( point,
                                                              vertex1,
                                                              vertex2,
                                                              vertex3,
                                                              projectionPoint );

  }
  GKG_CATCH( "float gkg::getPointToTriangleDistance( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< float >& vertex1, "
             "const gkg::Vector3d< float >& vertex2, "
             "const gkg::Vector3d< float >& vertex3 )" );

}


double gkg::getPointToTriangleDistance( const gkg::Vector3d< double >& point,
                                        const gkg::Vector3d< double >& vertex1,
                                        const gkg::Vector3d< double >& vertex2,
                                        const gkg::Vector3d< double >& vertex3 )
{

  try
  {

    gkg::Vector3d< double > projectionPoint;
    return gkg::getPointToTriangleDistanceAndProjectionPoint( point,
                                                              vertex1,
                                                              vertex2,
                                                              vertex3,
                                                              projectionPoint );

  }
  GKG_CATCH( "double gkg::getPointToTriangleDistance( "
             "const gkg::Vector3d< double >& point, "
             "const gkg::Vector3d< double >& vertex1, "
             "const gkg::Vector3d< double >& vertex2, "
             "const gkg::Vector3d< double >& vertex3 )" );

}


float gkg::getPointToSegmentDistance( const gkg::Vector3d< float >& point,
                                      const gkg::Vector3d< float >& endPoint1,
                                      const gkg::Vector3d< float >& endPoint2 )
{

  try
  {

    gkg::Vector3d< float > direction = endPoint2 - endPoint1;
    float t = direction.dot( point - endPoint1 ) / direction.getNorm2();

    float distance = -1.0f;
    if ( t <= 0 )
    {

      distance = ( point - endPoint1 ).getNorm();

    }
    else if ( ( 0 < t ) && ( t < 1 ) )
    {

      distance = ( point - ( endPoint1 + direction * t ) ).getNorm();

    }
    else if ( t >= 1 )
    {

      distance = ( point - endPoint2 ).getNorm();

    }

    if ( distance < 0 )
    {

      throw std::runtime_error( "distance should not be negative." );

    }

    return distance;

  }
  GKG_CATCH( "float gkg::getPointToSegmentDistance( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< float >& endPoint1, "
             "const gkg::Vector3d< float >& endPoint2 )" );

}


double gkg::getPointToSegmentDistance(
                                      const gkg::Vector3d< double >& point,
                                      const gkg::Vector3d< double >& endPoint1,
                                      const gkg::Vector3d< double >& endPoint2 )
{

  try
  {

    gkg::Vector3d< double > direction = endPoint2 - endPoint1;
    double t = direction.dot( point - endPoint1 ) / direction.getNorm2();

    double distance = -1.0f;
    if ( t <= 0 )
    {

      distance = ( point - endPoint1 ).getNorm();

    }
    else if ( ( 0 < t ) && ( t < 1 ) )
    {

      distance = ( point - ( endPoint1 + direction * t ) ).getNorm();

    }
    else if ( t >= 1 )
    {

      distance = ( point - endPoint2 ).getNorm();

    }

    if ( distance < 0 )
    {

      throw std::runtime_error( "distance should not be negative." );

    }

    return distance;

  }
  GKG_CATCH( "float gkg::getPointToSegmentDistance( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< float >& endPoint1, "
             "const gkg::Vector3d< float >& endPoint2 )" );

}


float gkg::getPartialTriangleAreas( const Vector3d< float >& projectPoint,
                                    const Vector3d< float >& vertex1,
                                    const Vector3d< float >& vertex2,
                                    const Vector3d< float >& vertex3,
                                    float& area1,
                                    float& area2,
                                    float& area3 )
{

  try
  {

    // area for vertex1
    float height = gkg::getPointToSegmentDistance( projectPoint,
                                                   vertex2, vertex3 );
    float base = ( vertex3 - vertex2 ).getNorm();
    area1 = ( height * base ) / 2.0;

     // area for vertex2
    height = gkg::getPointToSegmentDistance( projectPoint,
                                             vertex1, vertex3 );
    base = ( vertex3 - vertex1 ).getNorm();
    area2 = ( height * base ) / 2.0;
                                                  
    // area for vertex3
    height = gkg::getPointToSegmentDistance( projectPoint,
                                             vertex1, vertex2 );
    base = ( vertex2 - vertex1 ).getNorm();
    area3 = ( height * base ) / 2.0;
 
    return ( area1 + area2 + area3 );

  }
  GKG_CATCH( "float getPartialTriangleAreas( "
             "const Vector3d< float >& projectPoint, "
             "const Vector3d< float >& vertex1, "
             "const Vector3d< float >& vertex2, "
             "const Vector3d< float >& vertex3, "
             "float& area1, "
             "float& area2, "
             "float& area3 )" );

}


double gkg::getPartialTriangleAreas( const Vector3d< double >& projectPoint,
                                     const Vector3d< double >& vertex1,
                                     const Vector3d< double >& vertex2,
                                     const Vector3d< double >& vertex3,
                                     double& area1,
                                     double& area2,
                                     double& area3 )
{

  try
  {

    // area for vertex1
    double height = gkg::getPointToSegmentDistance( projectPoint,
                                                    vertex2, vertex3 );
    double base = ( vertex3 - vertex2 ).getNorm();
    area1 = ( height * base ) / 2.0;

     // area for vertex2
    height = gkg::getPointToSegmentDistance( projectPoint,
                                             vertex1, vertex3 );
    base = ( vertex3 - vertex1 ).getNorm();
    area2 = ( height * base ) / 2.0;
                                                  
    // area for vertex3
    height = gkg::getPointToSegmentDistance( projectPoint,
                                             vertex1, vertex2 );
    base = ( vertex2 - vertex1 ).getNorm();
    area3 = ( height * base ) / 2.0;
 
    return ( area1 + area2 + area3 );

  }
  GKG_CATCH( "double getPartialTriangleAreas( "
             "const Vector3d< double >& projectPoint, "
             "const Vector3d< double >& vertex1, "
             "const Vector3d< double >& vertex2, "
             "const Vector3d< double >& vertex3, "
             "double& area1, "
             "double& area2, "
             "double& area3 )" );

}


float gkg::getTriangleArea( const gkg::Vector3d< float >& site1,
                            const gkg::Vector3d< float >& site2,
                            const gkg::Vector3d< float >& site3 )
{

  try
  {

    return 0.5f * std::fabs( site1.x * ( site3.y - site2.y ) +
                             site2.x * ( site1.y - site3.y ) +
                             site3.x * ( site2.y - site1.y ) );

  }
  GKG_CATCH( "float gkg::getTriangleArea( "
             "const gkg::Vector3d< float >& site1, "
             "const gkg::Vector3d< float >& site2, "
             "const gkg::Vector3d< float >& site3 )" );

}


double gkg::getTriangleArea( const gkg::Vector3d< double >& site1,
                             const gkg::Vector3d< double >& site2,
                             const gkg::Vector3d< double >& site3 )
{

  try
  {

    return 0.5 * std::fabs( site1.x * ( site3.y - site2.y ) +
                            site2.x * ( site1.y - site3.y ) +
                            site3.x * ( site2.y - site1.y ) );

  }
  GKG_CATCH( "double gkg::getTriangleArea( "
             "const gkg::Vector3d< double >& site1, "
             "const gkg::Vector3d< double >& site2, "
             "const gkg::Vector3d< double >& site3 )" );

}
