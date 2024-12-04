#include <gkg-processing-mesh/GridGenerator.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <list>


gkg::GridGenerator::GridGenerator()
{

  try
  {

    _gridTypeNameList.insert( "cubic" );
    _gridTypeNameList.insert( "hexagonal" );

  }
  GKG_CATCH( "gkg::GridGenerator::GridGenerator()" );

}


gkg::GridGenerator::~GridGenerator()
{
}


std::vector< gkg::Vector3d< float > >
gkg::GridGenerator::create( const std::string& gridTypeName,
                            gkg::Vector3d< int32_t > size,
                            gkg::Vector3d< float > spacing,
                            gkg::Vector3d< float > offset ) const
{

  try
  {

    // allocating the ouput grid vector
    std::vector< gkg::Vector3d< float > > grid;

    if ( gridTypeName == "cubic" )
    {

      getCubicGrid( size, spacing, offset, grid );

    }
    else if ( gridTypeName == "hexagonal" )
    {

      getHexagonalGrid( size, spacing, offset, grid );

    }
    else
    {

      throw std::runtime_error( "Unknown grid type" );

    }

    return grid;

  }
  GKG_CATCH( "std::vector< gkg::Vector3d< float > > "
             "gkg::GridGenerator::create( "
             "const std::string& gridTypeName, "
             "gkg::Vector3d< int32_t > size, "
             "gkg::Vector3d< float > spacing, "
             "gkg::Vector3d< float > offset )" );

}


void gkg::GridGenerator::getCubicGrid(
                             gkg::Vector3d< int32_t > size,
                             gkg::Vector3d< float > spacing,
                             gkg::Vector3d< float > offset,
                             std::vector< gkg::Vector3d< float > >& grid ) const
{

  try
  {

    // allocating the output vector
    grid.resize( size.x * size.y * size.z );

    // processing the inital position
    gkg::Vector3d< float > initialPoint;
    initialPoint.x = offset.x - spacing.x * ( ( float )size.x - 1.0 ) / 2.0;
    initialPoint.y = offset.y - spacing.y * ( ( float )size.y - 1.0 ) / 2.0;
    initialPoint.z = offset.z - spacing.z * ( ( float )size.z - 1.0 ) / 2.0;

    gkg::Vector3d< float > currentPoint;
    int32_t x, y, z;
    int32_t index = 0;
    for ( z = 0; z < size.z; z++ )
    {

      currentPoint.z = initialPoint.z + spacing.z * ( float )z;
      for ( y = 0; y < size.y; y++ )
      {

        currentPoint.y = initialPoint.y + spacing.y * ( float )y;
        for ( x = 0; x < size.x; x++ )
        {

          currentPoint.x = initialPoint.x + spacing.x * ( float )x;

          // filling the grid volume
          grid[ index ] = currentPoint;
          ++ index;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::GridGenerator::getCubicGrid( "
             "gkg::Vector3d< int32_t > size, "
             "gkg::Vector3d< float > spacing, "
             "gkg::Vector3d< float > offset, "
             "std::vector< gkg::Vector3d< float > >& grid ) const" );

}


void gkg::GridGenerator::getHexagonalGrid(
                             gkg::Vector3d< int32_t > size,
                             gkg::Vector3d< float > spacing,
                             gkg::Vector3d< float > offset,
                             std::vector< gkg::Vector3d< float > >& grid ) const
{

  try
  {

    // processing the hexagonal spacing in the xy-plane
    gkg::Vector3d< float > hexagonalSpacing( spacing.x,
                                             spacing.y * std::sqrt( 3.0 ),
                                             0.0 );

    // allocating networks
    std::vector< gkg::Vector3d< float > > network1, network2;
    gkg::Vector3d< float > networkShift1, networkShift2;

    // processing network size
    gkg::Vector3d< int32_t > networkSize1( ( int32_t )( ( float )size.x / 2.0 ),
                                           ( int32_t )( ( float )size.y / 2.0 ),
                                           1 );

    gkg::Vector3d< int32_t > networkSize2 = networkSize1;
    if ( size.x % 2 )
    {

      // case : odd point count in x-axis
      ++ networkSize2.x;

    }
    if ( size.y % 2 )
    {

      // case : odd point count in y-axis
      ++ networkSize2.y;

    }

    // prcessing the level along z
    float step = spacing.z * std::sqrt( 3.0 ) / 2.0;
    float initialZ = -step * ( ( float )size.z - 1.0 ) / 2.0;
    networkShift1.z = initialZ;
    networkShift2.z = initialZ;

    std::vector< gkg::Vector3d< float > >::const_iterator
      n, ne;
    int32_t z = 0;
    for ( z = 0; z < size.z; z++ )
    {

      ////// building network1
      if ( z % 2 )
      {

        networkShift1.x -= 0.5 * spacing.x;
        networkShift1.y -= 0.5 * spacing.y;

      }
      getCubicGrid( networkSize1, hexagonalSpacing, offset + networkShift1,
                    network1 );

      ////// building network2
      if ( ( size.x % 2 ) == 0.0 )
      {

        networkShift2.x = 0.5 * hexagonalSpacing.x;

      }
      if ( ( size.y % 2 ) == 0.0 )
      {

        networkShift2.y = 0.5 * hexagonalSpacing.y;

      }
      if ( z % 2 )
      {

        networkShift2.x -= 0.5 * spacing.x;
        networkShift2.y -= 0.5 * spacing.y;

      }
      getCubicGrid( networkSize2, hexagonalSpacing, offset + networkShift2,
                    network2 );

      ////// filling networks
      n = network1.begin(),
      ne = network1.end();
      while ( n != ne )
      {

        grid.push_back( *n );
        ++ n;

      }
      n = network2.begin(),
      ne = network2.end();
      while ( n != ne )
      {

        grid.push_back( *n );
        ++ n;

      }

      // updating z level
      networkShift1.z += step;
      networkShift2.z += step;

      // reset parameters
      networkShift1.x = 0.0;
      networkShift1.y = 0.0;
      networkShift2.x = 0.0;
      networkShift2.y = 0.0;
      network1.clear();
      network2.clear();

    }

  }
  GKG_CATCH( "void gkg::GridGenerator::getHexagonalGrid( "
             "gkg::Vector3d< int32_t > size, "
             "gkg::Vector3d< float > spacing, "
             "gkg::Vector3d< float > offset, "
             "std::vector< gkg::Vector3d< float > >& grid ) const" );

}

