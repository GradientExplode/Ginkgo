#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::Neighborhood3d::Neighborhood3d()
{
}


gkg::Neighborhood3d::~Neighborhood3d()
{
}


std::list< gkg::Vector3d< int32_t > > 
gkg::Neighborhood3d::getNeighborOffsets(
                               gkg::Neighborhood3d::Type type,
                               int32_t stride,
                               float scalarParameter,
                               gkg::Volume< int16_t >* neighborhoodMask ) const
{

  try
  {

    std::list< gkg::Vector3d< int32_t > > neighborOffsets;
    switch ( type )
    {

      case gkg::Neighborhood3d::Neighborhood2d_4xy:

        {

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, 0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( stride, 0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, -stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, stride, 0 ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood2d_4xz:

        {

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, 0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( stride, 0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, 0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, 0, stride ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood2d_4yz:

        {

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, -stride, 0 ) );
          neighborOffsets.push_back(  
            gkg::Vector3d< int32_t >( 0, stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, 0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, 0, stride ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood2d_8xy:

        {

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, -stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >(  0, -stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride, -stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride,  0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride,  0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, +stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >(  0, +stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride, +stride, 0 ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood2d_8xz:

        {

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, 0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >(  0, 0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride, 0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, 0,  0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride, 0,  0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, 0, +stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >(  0, 0, +stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride, 0, +stride ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood2d_8yz:

        {

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, -stride, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0,  0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, +stride, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, -stride,  0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, +stride,  0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, -stride, +stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0,  0, +stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, +stride, +stride ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood3d_6:

        {

          // CAUTION : the arrangement below corresponds to the one needed by
          // Jeff's algorithm for 3D topology classification

          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( -stride, 0, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, -stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, 0, -stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, 0, +stride ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( 0, +stride, 0 ) );
          neighborOffsets.push_back( 
            gkg::Vector3d< int32_t >( +stride, 0, 0 ) );

        }
        break;

      case gkg::Neighborhood3d::Neighborhood3d_18:

        {

          // CAUTION : the arrangement below corresponds to the one needed by
          // Jeff's algorithm for 3D topology classification
          // It starts with the 6-neighbor points followed by the 18-neighbor
          // ones

          int32_t x = 0;
          int32_t y = 0;
          int32_t z = 0;
          for ( x = -stride; x <= stride; x += stride )
          {

            for ( y = -stride; y <= stride; y += stride )
            {

              for ( z = -stride; z <= stride; z += stride )
              {

                if ( ( std::abs( x ) +
                       std::abs( y ) +
                       std::abs( z ) ) == stride )
                {

                  neighborOffsets.push_back( 
		                          gkg::Vector3d< int32_t >( x, y, z ) );

                }

              }

            }

          }

          for ( x = -stride; x <= stride; x += stride )
          {

            for ( y = -stride; y <= stride; y += stride )
            {

              for ( z = -stride; z <= stride; z += stride )
              {

                if ( ( std::abs( x ) +
                       std::abs( y ) +
                       std::abs( z ) ) == 2 * stride )
                {

                  neighborOffsets.push_back( 
		                          gkg::Vector3d< int32_t >( x, y, z ) );

                }

              }

            }

          }

        }
        break;

      case gkg::Neighborhood3d::Neighborhood3d_26:

        {

          // CAUTION : the arrangement below corresponds to the one needed by
          // Jeff's algorithm for 3D topology classification
          // It starts with the 6-neighbor points followed by the 18-neighbor
          // points, and eneded with the 26-neighbor points

          int32_t x = 0;
          int32_t y = 0;
          int32_t z = 0;
          for ( x = -stride; x <= stride; x += stride )
          {

            for ( y = -stride; y <= stride; y += stride )
            {

              for ( z = -stride; z <= stride; z += stride )
              {

                if ( ( std::abs( x ) + 
                       std::abs( y ) + 
                       std::abs( z ) ) == stride )
                {

                  neighborOffsets.push_back( 
		                          gkg::Vector3d< int32_t >( x, y, z ) );

                }

              }

            }

          }

          for ( x = -stride; x <= stride; x += stride )
          {

            for ( y = -stride; y <= stride; y += stride )
            {

              for ( z = -stride; z <= stride; z += stride )
              {

                if ( ( std::abs( x ) +
                       std::abs( y ) +
                       std::abs( z ) ) == 2 * stride )
                {

                  neighborOffsets.push_back( 
		                          gkg::Vector3d< int32_t >( x, y, z ) );

                }

              }

            }

          }

          for ( x = -stride; x <= stride; x += stride )
          {

            for ( y = -stride; y <= stride; y += stride )
            {

              for ( z = -stride; z <= stride; z += stride )
              {

                if ( ( std::abs( x ) +
                       std::abs( y ) +
                       std::abs( z ) ) == 3 * stride )
                {

                  neighborOffsets.push_back( 
		                          gkg::Vector3d< int32_t >( x, y, z ) );

                }

              }

            }

          }

        }
        break;

      case gkg::Neighborhood3d::Neighborhood3d_Cube:

        {

          int32_t size = ( int32_t )( std::ceil( scalarParameter ) + 0.5f );
          int32_t x = 0;
          int32_t y = 0;
          int32_t z = 0;
          for ( x = -size; x <= size; x++ )
          {

            for ( y = -size; y <= size; y++ )
            {

              for ( z = -size; z <= size; z++ )
              {

                if ( ( x != 0 ) || ( y != 0 ) || ( z!= 0 ) )
                {

                  neighborOffsets.push_back( 
                                           gkg::Vector3d< int32_t >( x, y, z ) );

                }

              }

            }

          }

        }
        break;

      case gkg::Neighborhood3d::Neighborhood3d_Sphere:

        {

          std::list< gkg::Vector3d< float > > voxelBoundaryPoints;
          int32_t x = 0;
          int32_t y = 0;
          int32_t z = 0;
          for ( x = -1; x <= 1; x++ )
          {

            for ( y = -1; y <= 1; y++ )
            {

              for ( z = -1; z <= 1; z++ )
              {

                voxelBoundaryPoints.push_back( gkg::Vector3d< float >(
                                                                  x * 0.5f,
                                                                  y * 0.5f,
                                                                  z * 0.5f ) );

              }

            }

          }

          int32_t size = ( int32_t )( scalarParameter - 0.5 );
          if ( ( float )size != scalarParameter )
          {

            ++size;

          }

          gkg::Vector3d< float > center( 0.5, 0.5, 0.5 );

          for ( x = -size; x <= size; x++ )
          {

            for ( y = -size; y <= size; y++ )
            {

              for ( z = -size; z <= size; z++ )
              {

                if ( ( x != 0 ) || ( y != 0 ) || ( z != 0 ) )
                {

                  gkg::Vector3d< int32_t > site( x, y, z );
                  std::list< gkg::Vector3d< float > >::const_iterator
                    bp = voxelBoundaryPoints.begin(),
                    bpe = voxelBoundaryPoints.end();
                  while ( bp != bpe )
                  {

                    if ( ( gkg::Vector3d< float >( ( float )site.x,
                                                     ( float )site.y, 
                                                     ( float )site.z ) +
                           *bp ).getNorm() < scalarParameter )
                    {

                      neighborOffsets.push_back( site );
                      break;

                    }
                    ++ bp;

                  }

                }

              }

            }

          }

        }
        break;

      case gkg::Neighborhood3d::Neighborhood3d_DistanceFromVoxelBoundaries:

        {

          std::list< gkg::Vector3d< float > > voxelBoundaryPoints;
          int32_t x = 0;
          int32_t y = 0;
          int32_t z = 0;
          for ( x = -1; x <= 1; x++ )
          {

            for ( y = -1; y <= 1; y++ )
            {

              for ( z = -1; z <= 1; z++ )
              {

                voxelBoundaryPoints.push_back( gkg::Vector3d< float >(
                                                                  x * 0.5f,
                                                                  y * 0.5f,
                                                                  z * 0.5f ) );

              }

            }

          }

          int32_t size = ( int32_t )( std::ceil( scalarParameter ) + 0.5f );
          for ( x = -size; x <= size; x++ )
          {

            for ( y = -size; y <= size; y++ )
            {

              for ( z = -size; z <= size; z++ )
              {

                if ( ( x != 0 ) || ( y != 0 ) || ( z != 0 ) )
                {

                  gkg::Vector3d< int32_t > site( x, y, z );
                  bool isVoxelIncluded = false;
                  std::list< gkg::Vector3d< float > >::const_iterator
                    bp = voxelBoundaryPoints.begin(),
                    bpe = voxelBoundaryPoints.end();
                  while ( ( bp != bpe ) && !isVoxelIncluded )
                  {

                    std::list< gkg::Vector3d< float > >::iterator
                      cbp = voxelBoundaryPoints.begin();
                    
                    while ( ( cbp != bpe ) && !isVoxelIncluded )
                    {

                      if ( ( gkg::Vector3d< float >( ( float )site.x,
                                                       ( float )site.y, 
                                                       ( float )site.z ) +
                             *bp - *cbp ).getNorm() < scalarParameter )
                      {

                        neighborOffsets.push_back( site );
                        isVoxelIncluded = true;
                      
                      }
                      ++ cbp;

                    }
                    ++ bp;

                  }

                }

              }

            }

          }

        }
        break;

    }

    if ( neighborhoodMask )
    {

      std::list< gkg::Vector3d< int32_t > >::const_iterator
        o = neighborOffsets.begin(),
        oe = neighborOffsets.end();

      int32_t halfSize = 0;
      while ( o != oe )
      {

        if ( o->x > halfSize )
        {
      
          halfSize = o->x;
      
        }
        if ( o->y > halfSize )
        {
        
          halfSize = o->y;
      
        }
        if ( o->z > halfSize )
        {

          halfSize = o->z;

        }
        ++ o;

      }

      int32_t size = 2 * halfSize + 1;

      gkg::Vector3d< int32_t > center( halfSize, halfSize, halfSize );

      neighborhoodMask->reallocate( size, size, size );
      neighborhoodMask->fill( 0 );
    
      o = neighborOffsets.begin();
      while ( o != oe )
      {

        ( *neighborhoodMask )( center + *o ) = 1;
        ++ o;

      }

    }

    return neighborOffsets;

  }
  GKG_CATCH( "std::list< gkg::Vector3d< int32_t > > "
             "gkg::Neighborhood3d::getNeighborOffsets( "
             "gkg::Neighborhood3d::Type type,"
             "int32_t stride, float scalarParameter, "
             "Volume< int16_t >* neighborhoodMask ) const" );

}

