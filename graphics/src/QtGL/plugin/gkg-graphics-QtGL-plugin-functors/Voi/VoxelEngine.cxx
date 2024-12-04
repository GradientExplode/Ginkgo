#include <gkg-graphics-QtGL-plugin-functors/Voi/VoxelEngine.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Chunk.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>

#include <cmath>


gkg::VoxelEngine::VoxelEngine()
                : _chunks( 0 ),
                  _sizeX( 0 ),
                  _sizeY( 0 ),
                  _sizeZ( 0 )
{
}


gkg::VoxelEngine::~VoxelEngine()
{

  delete [] _chunks;

}


void gkg::VoxelEngine::initialize( gkg::VoiData& data )
{

  if ( data.labelLoaded() )
  {

    delete [] _chunks;

    _sizeX = int32_t( ::ceil( float( data.label().getSizeX() ) / 
                              float( CHUNK_SIZE ) ) );
    _sizeY = int32_t( ::ceil( float( data.label().getSizeY() ) / 
                              float( CHUNK_SIZE ) ) );
    _sizeZ = int32_t( ::ceil( float( data.label().getSizeZ() ) /
                              float( CHUNK_SIZE ) ) );

    _chunks = new gkg::Chunk[ _sizeX * _sizeY * _sizeZ ];

    if ( _chunks )
    {

      int32_t x, y, z;
      gkg::Chunk* ptr = _chunks;

      _chunkAccessor.resize( _sizeZ );

      for ( z = 0; z < _sizeZ; z++ )
      {

        _chunkAccessor[ z ].resize( _sizeY );

        for ( y = 0; y < _sizeY; y++ )
        {

          _chunkAccessor[ z ][ y ] = ptr;

          for ( x = 0; x < _sizeX; x++, ptr++ )
          {

            ptr->setup( data, x, y, z );

          }

        }

      }

    }

  }

}


void gkg::VoxelEngine::render( gkg::VoiData& data )
{

  if ( _chunks )
  {

    int32_t n = _sizeX * _sizeY * _sizeZ;
    gkg::Chunk* ptr = _chunks;

    while ( n-- )
    {

      ptr->render( data );
      ptr++;

    }

  }

}


void gkg::VoxelEngine::update( gkg::VoiData& data, 
                               std::list< gkg::Vector3d< int32_t > >& points )
{

  if ( _chunks && points.size() )
  {

    int32_t label = data.label()( points.front() );
    std::list< gkg::Vector3d< int32_t > >::iterator
      p = points.begin(),
      pe = points.end();

    while ( p != pe )
    {

      _chunkAccessor[ p->z / CHUNK_SIZE ]
                    [ p->y / CHUNK_SIZE ]
                    [ p->x / CHUNK_SIZE ].updateBlock( p->x % CHUNK_SIZE,
                                                       p->y % CHUNK_SIZE,
                                                       p->z % CHUNK_SIZE,
                                                       label );
      ++p;

    }

  }

}


void gkg::VoxelEngine::updateVisible( int32_t label, bool show )
{

  if ( _chunks )
  {

    int32_t n = _sizeX * _sizeY * _sizeZ;
    gkg::Chunk* ptr = _chunks;

    while ( n-- )
    {

      ptr->updateBlocks( label, show );
      ptr++;

    }

  }

}


void gkg::VoxelEngine::leave()
{

  if ( _chunks )
  {

    int32_t n = _sizeX * _sizeY * _sizeZ;
    gkg::Chunk* ptr = _chunks;

    while ( n-- )
    {

      ptr->leave();
      ptr++;

    }

  }

}
