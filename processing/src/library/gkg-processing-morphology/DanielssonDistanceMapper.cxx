#include <gkg-processing-morphology/DanielssonDistanceMapper.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <map>
#include <iostream>


template < class IN >
gkg::DanielssonDistanceMapper< IN >::DanielssonDistanceMapper(
                                   const gkg::TestFunction< IN >& testFunction )
                                 : _testFunction( testFunction )
{
}


template < class IN >
gkg::DanielssonDistanceMapper< IN >::~DanielssonDistanceMapper()
{
}


template < class IN >
void gkg::DanielssonDistanceMapper< IN >::getComponents(
                         const gkg::Volume< IN >& volume,
                         gkg::Volume< gkg::Vector3d< int32_t > >& components,
                         bool verbose ) const
{

  try
  {

    if ( ( volume.getSizeX() == 1 ) ||
         ( volume.getSizeY() == 1 ) ||
         ( volume.getSizeZ() == 1 ) )
    {

      get2dComponents( volume, components, verbose );


    }
    else
    {

      get3dComponents( volume, components, verbose );


    }

  }
  GKG_CATCH( "template < class IN > "
             "void gkg::DanielssonDistanceMapper< IN >::getComponents( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< gkg::Vector3d< int > >& components, "
             "bool verbose ) const" );

}


template < class IN > template < class OUT >
void gkg::DanielssonDistanceMapper< IN >::getDistance(
                                            const gkg::Volume< IN >& volume,
                                            gkg::Volume< OUT >& distance,
                                            bool verbose ) const
{

  try
  {

    // allocating stuff for vector's map
    gkg::Volume< gkg::Vector3d< int32_t > > components;
    getComponents( volume, components, verbose );

    // allocating stuff for distance map
    reallocate( volume,distance );

    // computing Voronoi's map
    if ( verbose )
    {

      std::cout << "computing distance map" << std::flush;

    }
    int32_t x, y, z, t;
    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            const gkg::Vector3d< int32_t >& c = components( x, y, z );
            distance( x, y, z ) = ( OUT )c.getDoubleNorm();

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > template < class OUT > "
             "void gkg::DanielssonDistanceMapper< IN >::getDistance( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< OUT >& distance, "
             "bool verbose ) const" );

}


template < class IN > template < class OUT >
void gkg::DanielssonDistanceMapper< IN >::getVoronoi(
                                  const gkg::Volume< IN >& volume,
                                  const TestFunction< IN >& outsideTestFunction,
                                  gkg::Volume< OUT >& voronoi,
                                  const OUT& outsideValue,
                                  bool verbose ) const
{

  try
  {

    // allocating stuff for vector's map
    gkg::Volume< gkg::Vector3d< int32_t > > components;
    getComponents( volume, components, verbose );

    // allocating stuff for Voronoi's map
    reallocate( volume, voronoi );

    std::map< IN, int64_t > labelLut;

    // initializing Voronoi's map
    typename gkg::Volume< IN >::const_iterator i = volume.begin(),
                                               ie = volume.end();
    typename gkg::Volume< OUT >::iterator ov = voronoi.begin();
    int64_t label = 1;
    while ( i != ie )
    {

      if ( _testFunction.getState( *i ) )
      {

        typename std::map< IN, int64_t >::const_iterator
          l = labelLut.find( *i );
        if ( l != labelLut.end() )
        {

          *ov = ( OUT )l->second;


        }
        else
        {

          *ov = ( OUT )label;
          ++ label;

        }

      }
      else
      {

        *ov = ( OUT )0;

      }
      ++ i;
      ++ ov;

    }


    // computing Voronoi's map
    if ( verbose )
    {

      std::cout << "computing Voronoi's map" << std::flush;

    }
    int32_t x, y, z, t;
    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 1; z < sizeZ - 1; z++ )
      {

        for ( y = 1; y < sizeY - 1; y++ )
        {

          for ( x = 1; x < sizeX - 1; x++ )
          {

            const gkg::Vector3d< int32_t >& c = components( x, y, z );
            voronoi( x, y, z ) = voronoi( x + c.x, y + c.y, z + c.z ); 

          }

        }

      }

    }

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 1; z < sizeZ - 1; z++ )
      {

        for ( y = 1; y < sizeY - 1; y++ )
        {

          for ( x = 1; x < sizeX - 1; x++ )
          {

            if ( outsideTestFunction.getState( volume( x, y, z, t ) ) )
            {

              voronoi( x, y, z ) = outsideValue; 

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 23 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > template < class OUT > "
             "void gkg::DanielssonDistanceMapper< IN >::getVoronoi( "
             " const gkg::Volume< IN >& volume, "
             "const TestFunction< IN >& outsideTestFunction, "
             "gkg::Volume< OUT >& voronoi, "
             "const OUT& outsideValue, "
             "bool verbose ) const" );

}


template < class IN >
void gkg::DanielssonDistanceMapper< IN >::get3dComponents(
                            const gkg::Volume< IN >& volume,
                            gkg::Volume< gkg::Vector3d< int32_t > >& components,
                            bool verbose ) const
{

  try
  {

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();

    // allocating stuff for vector's map
    reallocate( volume, components );

    // getting maximum length
    int32_t maxLength = std::max( sizeX, std::max( sizeY, sizeZ ) );
    gkg::Vector3d< int32_t > maxCoordinates( 2 * maxLength,
                                             2 * maxLength,
                                             2 * maxLength );
    gkg::Vector3d< int32_t > minCoordinates( 0, 0, 0 );

    // initializing Voronoi's map and vector components map
    typename gkg::Volume< IN >::const_iterator i = volume.begin(),
                                               ie = volume.end();
    gkg::Volume< gkg::Vector3d< int32_t > >::iterator oc = components.begin();
    while ( i != ie )
    {

      if ( _testFunction.getState( *i ) )
      {

        *oc = minCoordinates;

      }
      else
      {

        *oc = maxCoordinates;

      }
      ++ i;
      ++ oc;

    }

    // computing distance transform
    int32_t x, y, z, t, d;
    std::vector< int32_t > offsets( 3 );
    offsets[ 0 ] = 0;
    offsets[ 1 ] = 0;
    offsets[ 2 ] = 0;

    if ( verbose )
    {

      std::cout << "forward sweep along X" << std::flush;

    }
    // forward sweep along X
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 1; z < sizeZ; z++ )
      {

        for ( y = 1; y < sizeY; y++ )
        {

          for ( x = 1; x < sizeX; x++ )
          {

            for ( d = 0; d < 3; d++ )
            {

              -- offsets[ d ];
              updateLocalDistance( components, x, y, z, offsets );
              offsets[ d ] = 0;

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << "backward sweep along X" << std::flush;

    }
    // backward sweep along X
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 1; z < sizeZ; z++ )
      {

        for ( y = 1; y < sizeY; y++ )
        {

          for ( x = sizeX - 2; x >= 0; x-- )
          {

            for ( d = 0; d < 3; d++ )
            {

              if ( d == 0 )
              {

                ++ offsets[ d ];

              }
              else
              {

                -- offsets[ d ];

              }
              updateLocalDistance( components, x, y, z, offsets );
              offsets[ d ] = 0;              

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << "forward sweep along Y" << std::flush;

    }
    // forward sweep along Y
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 1; z < sizeZ; z++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( y = 1; y < sizeY; y++ )
          {

            for ( d = 0; d < 3; d++ )
            {

              -- offsets[ d ];
              updateLocalDistance( components, x, y, z, offsets );
              offsets[ d ] = 0;

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << "backward sweep along Y" << std::flush;

    }
    // backward sweep along Y
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 1; z < sizeZ; z++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( y = sizeY - 2; y >= 0 ; y-- )
          {

            for ( d = 0; d < 3; d++ )
            {

              if ( d == 1 )
              {

                ++ offsets[ d ];

              }
              else
              {

                -- offsets[ d ];

              }
              updateLocalDistance( components, x, y, z, offsets );
              offsets[ d ] = 0;              

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << "forward sweep along Z" << std::flush;

    }
    // forward sweep along Z
    for ( t = 0; t < sizeT; t++ )
    {

      for ( y = 1; y < sizeY; y++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( z = 1; z < sizeZ; z++ )
          {

            for ( d = 0; d < 3; d++ )
            {

              -- offsets[ d ];
              updateLocalDistance( components, x, y, z, offsets );
              offsets[ d ] = 0;

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << "backward sweep along Z" << std::flush;

    }
    // backward sweep along Z
    for ( t = 0; t < sizeT; t++ )
    {

      for ( y = 1; y < sizeY; y++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( z = sizeZ - 2; z >= 0 ; z-- )
          {

            for ( d = 0; d < 3; d++ )
            {

              if ( d == 2 )
              {

                ++ offsets[ d ];

              }
              else
              {

                -- offsets[ d ];

              }
              updateLocalDistance( components, x, y, z, offsets );
              offsets[ d ] = 0;              

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > "
             "void gkg::DanielssonDistanceMapper< IN >::get3dComponents( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< gkg::Vector3d< int > >& components, "
             "bool verbose ) const" );


}


template < class IN >
void gkg::DanielssonDistanceMapper< IN >::get2dComponents(
                            const gkg::Volume< IN >& volume,
                            gkg::Volume< gkg::Vector3d< int32_t > >& components,
                            bool verbose ) const
{

  try
  {

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();

    // allocating stuff for vector's map
    reallocate( volume, components );

    // getting maximum length
    int32_t maxLength = std::max( sizeX, std::max( sizeY, sizeZ ) );
    gkg::Vector3d< int32_t > maxCoordinates( 2 * maxLength,
                                             2 * maxLength,
                                             2 * maxLength );
    gkg::Vector3d< int32_t > minCoordinates( 0, 0, 0 );

    // initializing Voronoi's map and vector components map
    typename gkg::Volume< IN >::const_iterator i = volume.begin(),
                                               ie = volume.end();
    gkg::Volume< gkg::Vector3d< int32_t > >::iterator oc = components.begin();
    while ( i != ie )
    {

      if ( _testFunction.getState( *i ) )
      {

        *oc = minCoordinates;

      }
      else
      {

        *oc = maxCoordinates;

      }
      ++ i;
      ++ oc;

    }

    // computing distance transform
    int32_t x, y, z, t;
    std::vector< int32_t > offsets( 3 );
    offsets[ 0 ] = 0;
    offsets[ 1 ] = 0;
    offsets[ 2 ] = 0;


    if ( sizeZ == 1 )
    {

      if ( verbose )
      {

        std::cout << "forward sweep along X" << std::flush;

      }
      // forward sweep along X
      for ( t = 0; t < sizeT; t++ )
      {

        for ( y = 1; y < sizeY; y++ )
        {

          for ( x = 1; x < sizeX; x++ )
          {

            -- offsets[ 0 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 0 ] = 0;
            -- offsets[ 1 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 1 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << "backward sweep along X" << std::flush;

      }
      // backward sweep along X
      for ( t = 0; t < sizeT; t++ )
      {

        for ( y = 1; y < sizeY; y++ )
        {

          for ( x = sizeX - 2; x >= 0; x-- )
          {

            ++ offsets[ 0 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 0 ] = 0;
            -- offsets[ 1 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 1 ] = 0;

          }

        }

      }

      if ( verbose )
      {

      std::cout << gkg::Eraser( 22 ) << "forward sweep along Y" << std::flush;

      }
      // forward sweep along Y
      for ( t = 0; t < sizeT; t++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( y = 1; y < sizeY; y++ )
          {

            -- offsets[ 0 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 0 ] = 0;
            -- offsets[ 1 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 1 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << "backward sweep along Y" << std::flush;

      }
      // backward sweep along Y
      for ( t = 0; t < sizeT; t++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( y = sizeY - 2; y >= 0 ; y-- )
          {

            -- offsets[ 0 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 0 ] = 0;
            ++ offsets[ 1 ];
            updateLocalDistance( components, x, y, 0, offsets );
            offsets[ 1 ] = 0;

          }

        }

      }

    }
    else if ( sizeY == 1 )
    {

      if ( verbose )
      {

        std::cout << "forward sweep along X" << std::flush;

      }
      // forward sweep along X
      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 1; z < sizeZ; z++ )
        {

          for ( x = 1; x < sizeX; x++ )
          {

            -- offsets[ 0 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 0 ] = 0;
            -- offsets[ 2 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << "backward sweep along X" << std::flush;

      }
      // backward sweep along X
      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 1; z < sizeZ; z++ )
        {

          for ( x = sizeX - 2; x >= 0; x-- )
          {

            ++ offsets[ 0 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 0 ] = 0;
            -- offsets[ 2 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 )
                  << "forward sweep along Z" << std::flush;

      }
      // forward sweep along Z
      for ( t = 0; t < sizeT; t++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( z = 1; z < sizeZ; z++ )
          {

            -- offsets[ 0 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 0 ] = 0;
            -- offsets[ 2 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << "backward sweep along Z" << std::flush;

      }
      // backward sweep along Z
      for ( t = 0; t < sizeT; t++ )
      {

        for ( x = 1; x < sizeX; x++ )
        {

          for ( z = sizeZ - 2; z >= 0 ; z-- )
          {

            -- offsets[ 0 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 0 ] = 0;
            ++ offsets[ 2 ];
            updateLocalDistance( components, x, 0, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

    }
    else if ( sizeX == 1 )
    {

      if ( verbose )
      {

      std::cout << gkg::Eraser( 22 ) << "forward sweep along Y" << std::flush;

      }
      // forward sweep along Y
      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 1; z < sizeZ; z++ )
        {

          for ( y = 1; y < sizeY; y++ )
          {

            -- offsets[ 1 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 1 ] = 0;
            -- offsets[ 2 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << "backward sweep along Y" << std::flush;

      }
      // backward sweep along Y
      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 1; z < sizeZ; z++ )
        {

          for ( y = sizeY - 2; y >= 0 ; y-- )
          {

            ++ offsets[ 1 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 1 ] = 0;
            -- offsets[ 2 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 )
                  << "forward sweep along Z" << std::flush;

      }
      // forward sweep along Z
      for ( t = 0; t < sizeT; t++ )
      {

        for ( y = 1; y < sizeY; y++ )
        {

          for ( z = 1; z < sizeZ; z++ )
          {

            -- offsets[ 1 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 1 ] = 0;
            -- offsets[ 2 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << "backward sweep along Z" << std::flush;

      }
      // backward sweep along Z
      for ( t = 0; t < sizeT; t++ )
      {

        for ( y = 1; y < sizeY; y++ )
        {

          for ( z = sizeZ - 2; z >= 0 ; z-- )
          {

            -- offsets[ 1 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 1 ] = 0;
            ++ offsets[ 2 ];
            updateLocalDistance( components, 0, y, z, offsets );
            offsets[ 2 ] = 0;

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > "
             "void gkg::DanielssonDistanceMapper< IN >::get2dComponents( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< gkg::Vector3d< int > >& components, "
             "bool verbose ) const" );


}


template < class IN > template < class OUT >
void gkg::DanielssonDistanceMapper< IN >::reallocate( 
                                                 const gkg::Volume< IN >& in,
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    // reallocating data
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

  }
  GKG_CATCH( "template < class IN > template < class OUT > "
             "void gkg::DanielssonDistanceMapper< IN >::reallocate( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out )" );

}


template < class IN >
void gkg::DanielssonDistanceMapper< IN >::updateLocalDistance(
                            gkg::Volume< gkg::Vector3d< int32_t > >& components,
                            int32_t x, int32_t y, int32_t z,
                            const std::vector< int32_t >& offsets ) const
{

  try
  {

    gkg::Vector3d< int32_t >& hereValue = components( x, y, z );
    gkg::Vector3d< int32_t >& thereValue = components( x + offsets[ 0 ],
                                                       y + offsets[ 1 ],
                                                       z + offsets[ 2 ] );

    if ( thereValue.getNorm2() < hereValue.getNorm2() )
    {

      hereValue.x = thereValue.x + offsets[ 0 ];
      hereValue.y = thereValue.y + offsets[ 1 ];
      hereValue.z = thereValue.z + offsets[ 2 ];

    }

  }
  GKG_CATCH( "template < class IN > "
             "void gkg::DanielssonDistanceMapper< IN >::updateLocalDistance( "
             "const gkg::Volume< gkg::Vector3d< int > >& components, "
             "int x, int y, int z, "
             "const std::vector< int >& offsets ) const" );

}


template class gkg::DanielssonDistanceMapper< int8_t >;
template class gkg::DanielssonDistanceMapper< uint8_t >;
template class gkg::DanielssonDistanceMapper< int16_t >;
template class gkg::DanielssonDistanceMapper< uint16_t >;
template class gkg::DanielssonDistanceMapper< int32_t >;
template class gkg::DanielssonDistanceMapper< uint32_t >;
template class gkg::DanielssonDistanceMapper< int64_t >;
template class gkg::DanielssonDistanceMapper< uint64_t >;
template class gkg::DanielssonDistanceMapper< float >;
template class gkg::DanielssonDistanceMapper< double >;


#define InstanciateDanielssonDistanceMapper( IN, OUT )                 \
template void gkg::DanielssonDistanceMapper< IN >::getDistance(        \
                   const gkg::Volume< IN >& volume,                    \
                   gkg::Volume< OUT >& distance,                       \
                   bool verbose ) const;                               \
template void gkg::DanielssonDistanceMapper< IN >::getVoronoi(         \
                   const gkg::Volume< IN >& volume,                    \
                   const gkg::TestFunction< IN >& outsideTestFunction, \
                   gkg::Volume< OUT >& voronoi,                        \
                   const OUT& outsideValue,                            \
                   bool verbose ) const


InstanciateDanielssonDistanceMapper( int8_t, uint8_t );
InstanciateDanielssonDistanceMapper( uint8_t, uint8_t );
InstanciateDanielssonDistanceMapper( int16_t, uint8_t );
InstanciateDanielssonDistanceMapper( uint16_t, uint8_t );
InstanciateDanielssonDistanceMapper( int32_t, uint8_t );
InstanciateDanielssonDistanceMapper( uint32_t, uint8_t );
InstanciateDanielssonDistanceMapper( int64_t, uint8_t );
InstanciateDanielssonDistanceMapper( uint64_t, uint8_t );
InstanciateDanielssonDistanceMapper( float, uint8_t );
InstanciateDanielssonDistanceMapper( double, uint8_t );

InstanciateDanielssonDistanceMapper( int8_t, int8_t );
InstanciateDanielssonDistanceMapper( uint8_t, int8_t );
InstanciateDanielssonDistanceMapper( int16_t, int8_t );
InstanciateDanielssonDistanceMapper( uint16_t, int8_t );
InstanciateDanielssonDistanceMapper( int32_t, int8_t );
InstanciateDanielssonDistanceMapper( uint32_t, int8_t );
InstanciateDanielssonDistanceMapper( int64_t, int8_t );
InstanciateDanielssonDistanceMapper( uint64_t, int8_t );
InstanciateDanielssonDistanceMapper( float, int8_t );
InstanciateDanielssonDistanceMapper( double, int8_t );

InstanciateDanielssonDistanceMapper( int8_t, uint16_t );
InstanciateDanielssonDistanceMapper( uint8_t, uint16_t );
InstanciateDanielssonDistanceMapper( int16_t, uint16_t );
InstanciateDanielssonDistanceMapper( uint16_t, uint16_t );
InstanciateDanielssonDistanceMapper( int32_t, uint16_t );
InstanciateDanielssonDistanceMapper( uint32_t, uint16_t );
InstanciateDanielssonDistanceMapper( int64_t, uint16_t );
InstanciateDanielssonDistanceMapper( uint64_t, uint16_t );
InstanciateDanielssonDistanceMapper( float, uint16_t );
InstanciateDanielssonDistanceMapper( double, uint16_t );

InstanciateDanielssonDistanceMapper( int8_t, int16_t );
InstanciateDanielssonDistanceMapper( uint8_t, int16_t );
InstanciateDanielssonDistanceMapper( int16_t, int16_t );
InstanciateDanielssonDistanceMapper( uint16_t, int16_t );
InstanciateDanielssonDistanceMapper( int32_t, int16_t );
InstanciateDanielssonDistanceMapper( uint32_t, int16_t );
InstanciateDanielssonDistanceMapper( int64_t, int16_t );
InstanciateDanielssonDistanceMapper( uint64_t, int16_t );
InstanciateDanielssonDistanceMapper( float, int16_t );
InstanciateDanielssonDistanceMapper( double, int16_t );

InstanciateDanielssonDistanceMapper( int8_t, uint32_t );
InstanciateDanielssonDistanceMapper( uint8_t, uint32_t );
InstanciateDanielssonDistanceMapper( int16_t, uint32_t );
InstanciateDanielssonDistanceMapper( uint16_t, uint32_t );
InstanciateDanielssonDistanceMapper( int32_t, uint32_t );
InstanciateDanielssonDistanceMapper( uint32_t, uint32_t );
InstanciateDanielssonDistanceMapper( int64_t, uint32_t );
InstanciateDanielssonDistanceMapper( uint64_t, uint32_t );
InstanciateDanielssonDistanceMapper( float, uint32_t );
InstanciateDanielssonDistanceMapper( double, uint32_t );

InstanciateDanielssonDistanceMapper( int8_t, int32_t );
InstanciateDanielssonDistanceMapper( uint8_t, int32_t );
InstanciateDanielssonDistanceMapper( int16_t, int32_t );
InstanciateDanielssonDistanceMapper( uint16_t, int32_t );
InstanciateDanielssonDistanceMapper( int32_t, int32_t );
InstanciateDanielssonDistanceMapper( uint32_t, int32_t );
InstanciateDanielssonDistanceMapper( int64_t, int32_t );
InstanciateDanielssonDistanceMapper( uint64_t, int32_t );
InstanciateDanielssonDistanceMapper( float, int32_t );
InstanciateDanielssonDistanceMapper( double, int32_t );

InstanciateDanielssonDistanceMapper( int8_t, uint64_t );
InstanciateDanielssonDistanceMapper( uint8_t, uint64_t );
InstanciateDanielssonDistanceMapper( int16_t, uint64_t );
InstanciateDanielssonDistanceMapper( uint16_t, uint64_t );
InstanciateDanielssonDistanceMapper( int32_t, uint64_t );
InstanciateDanielssonDistanceMapper( uint32_t, uint64_t );
InstanciateDanielssonDistanceMapper( int64_t, uint64_t );
InstanciateDanielssonDistanceMapper( uint64_t, uint64_t );
InstanciateDanielssonDistanceMapper( float, uint64_t );
InstanciateDanielssonDistanceMapper( double, uint64_t );

InstanciateDanielssonDistanceMapper( int8_t, int64_t );
InstanciateDanielssonDistanceMapper( uint8_t, int64_t );
InstanciateDanielssonDistanceMapper( int16_t, int64_t );
InstanciateDanielssonDistanceMapper( uint16_t, int64_t );
InstanciateDanielssonDistanceMapper( int32_t, int64_t );
InstanciateDanielssonDistanceMapper( uint32_t, int64_t );
InstanciateDanielssonDistanceMapper( int64_t, int64_t );
InstanciateDanielssonDistanceMapper( uint64_t, int64_t );
InstanciateDanielssonDistanceMapper( float, int64_t );
InstanciateDanielssonDistanceMapper( double, int64_t );

InstanciateDanielssonDistanceMapper( int8_t, float );
InstanciateDanielssonDistanceMapper( uint8_t, float );
InstanciateDanielssonDistanceMapper( int16_t, float );
InstanciateDanielssonDistanceMapper( uint16_t, float );
InstanciateDanielssonDistanceMapper( int32_t, float );
InstanciateDanielssonDistanceMapper( uint32_t, float );
InstanciateDanielssonDistanceMapper( int64_t, float );
InstanciateDanielssonDistanceMapper( uint64_t, float );
InstanciateDanielssonDistanceMapper( float, float );
InstanciateDanielssonDistanceMapper( double, float );

InstanciateDanielssonDistanceMapper( int8_t, double );
InstanciateDanielssonDistanceMapper( uint8_t, double );
InstanciateDanielssonDistanceMapper( int16_t, double );
InstanciateDanielssonDistanceMapper( uint16_t, double );
InstanciateDanielssonDistanceMapper( int32_t, double );
InstanciateDanielssonDistanceMapper( uint32_t, double );
InstanciateDanielssonDistanceMapper( int64_t, double );
InstanciateDanielssonDistanceMapper( uint64_t, double );
InstanciateDanielssonDistanceMapper( float, double );
InstanciateDanielssonDistanceMapper( double, double );


#undef InstanciateDanielssonDistanceMapper
