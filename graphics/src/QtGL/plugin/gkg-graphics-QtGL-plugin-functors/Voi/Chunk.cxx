#include <gkg-graphics-QtGL-plugin-functors/Voi/Chunk.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-container/Volume_i.h>


gkg::Chunk::Chunk()
          : _activeBlockCount( 0 ),
            _nX( CHUNK_SIZE ),
            _nY( CHUNK_SIZE ),
            _nZ( CHUNK_SIZE ),
            _posX( 0 ),
            _posY( 0 ),
            _posZ( 0 ),
            _needUpdate( false )
{

  _buffers[ 0 ] = 0;
  _buffers[ 1 ] = 0;

}


gkg::Chunk::~Chunk()
{

  leave();

}


int32_t gkg::Chunk::getActiveBlockCount()
{

  return _activeBlockCount;

}


void gkg::Chunk::setup( gkg::VoiData& data, int32_t x, int32_t y, int32_t z )
{

  gkg::Volume< int16_t >& label = data.label();
  int32_t dx = label.getSizeX();
  int32_t dy = label.getSizeY();
  int32_t dz = label.getSizeZ();

  int32_t delta = dx - CHUNK_SIZE * ( x + 1 );

  if ( delta < 0 )
  {

    _nX += delta;

  }

  delta = dy - CHUNK_SIZE * ( y + 1 );

  if ( delta < 0 )
  {

    _nY += delta;

  }

  delta = dz - CHUNK_SIZE * ( z + 1 );

  if ( delta < 0 )
  {

    _nZ += delta;

  }

  int32_t x0 = CHUNK_SIZE * x;
  int32_t y0 = CHUNK_SIZE * y;
  int32_t z0 = CHUNK_SIZE * z;
  int32_t i, j, k, u, v, w;

  _activeBlockCount = 0;
  _posX = x;
  _posY = y;
  _posZ = z;

  for ( k = z0, w = 0; w < _nZ; k++, w++ )
  {

    for ( j = y0, v = 0; v < _nY; j++, v++ )
    {

      for ( i = x0, u = 0; u < _nX; i++, u++ )
      {

        int16_t l = label( i, j, k );

        if ( l )
        {

          _blocks[ w ][ v ][ u ].setLabel( l );
          _activeBlockCount++;
          _needUpdate = true;

        }

      }

    }

  }

}


void gkg::Chunk::createMesh( gkg::VoiData& data )
{

  if ( _activeBlockCount && _needUpdate )
  {

    gkg::Slicer& slicer = data.getSlicer();
    int32_t startIndex = 0, nIdx = 0;
    int32_t xStart = CHUNK_SIZE * _posX;
    int32_t yStart = CHUNK_SIZE * _posY;
    int32_t zStart = CHUNK_SIZE * _posZ;
    int32_t i, j, k;
    double x0, y0, z0, x1, y1, z1;

    _colorVertex.resize( 216 * _activeBlockCount ); // = 6 * 4 * 9
    _indice.resize( 24 * _activeBlockCount );

    GLfloat* vptr = &_colorVertex[ 0 ];
    GLuint* iptr = &_indice[ 0 ];
    gkg::RGBComponent c;
    gkg::VoiLabelInfo& labelInfo = data.labelInfo();

    for ( k = 0; k < _nZ; k++ )
    {

      for ( j = 0; j < _nY; j++ )
      {

    	for ( i = 0; i < _nX; i++ )
    	{

          int32_t label = _blocks[ k ][ j ][ i ].getLabel();

    	  if ( _blocks[ k ][ j ][ i ].isActive() && 
               labelInfo.getColor( label, c ) )
    	  {

            slicer.getTexelCoordinates( xStart + i, yStart + j, zStart + k,
                                        x0, y0, z0, x1, y1, z1 );

    	    bool doF = ( ( k < ( _nZ - 1 ) ) && 
    			 ( _blocks[ k + 1 ][ j ][ i ].getLabel() == label ) ) ? 
    			 false : true;
    	    bool doB = ( ( k > 0 ) && 
    			 ( _blocks[ k - 1 ][ j ][ i ].getLabel() == label ) ) ? 
    			 false : true;
    	    bool doR = ( ( i < ( _nX - 1 ) ) && 
    			 ( _blocks[ k  ][ j ][ i + 1 ].getLabel() == label ) ) ?
    			 false : true;
    	    bool doL = ( ( i > 0 ) && 
    			 ( _blocks[ k ][ j ][ i - 1 ].getLabel() == label ) ) ? 
    		 	false : true;
    	    bool doS = ( ( j < ( _nY - 1 ) ) && 
    			 ( _blocks[ k ][ j + 1 ][ i ].getLabel() == label ) ) ? 
    			 false : true;
    	    bool doI = ( ( j > 0 ) && 
    			 ( _blocks[ k  ][ j - 1 ][ i ].getLabel() == label ) ) ?
    			 false : true;
    	    int32_t v0, v1, v2, v3, v4, v5, v6, v7;

    	    float r = float( c.r ) / 255.0f;
    	    float g = float( c.g ) / 255.0f;
    	    float b = float( c.b ) / 255.0f;

    	    // Front
    	    if ( doF )
    	    {

    	      // V0
    	      *vptr++ = r; // color
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;   // coordinates
    	      *vptr++ = y0;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f; // normal
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      v0 = startIndex++;

    	      // V1
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y0;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      v1 = startIndex++;

    	      // V2
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y1;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      v2 = startIndex++;

    	      // V3
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y1;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      v3 = startIndex++;

    	      // indices
    	      *iptr++ = v0;
    	      *iptr++ = v1;
    	      *iptr++ = v2;
    	      *iptr++ = v3;
    	      nIdx++;

    	    }

    	    // Back
    	    if ( doB )
    	    {

    	      // V4
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y0;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      v4 = startIndex++;

    	      // V5
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y0;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      v5 = startIndex++;

    	      // V6
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y1;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      v6 = startIndex++;

    	      // V7
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y1;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      v7 = startIndex++;

    	      // indices
    	      *iptr++ = v5;
    	      *iptr++ = v4;
    	      *iptr++ = v7;
    	      *iptr++ = v6;
    	      nIdx++;

    	    }

    	    // Right
    	    if ( doR )
    	    {

    	      // V1
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y0;
    	      *vptr++ = z1;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v1 = startIndex++;
 
    	      // V2
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y1;
    	      *vptr++ = z1;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v2 = startIndex++;

    	      // V5
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y0;
    	      *vptr++ = z0;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v5 = startIndex++;

    	      // V6
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y1;
    	      *vptr++ = z0;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v6 = startIndex++;

    	      // indices
    	      *iptr++ = v1;
    	      *iptr++ = v5;
    	      *iptr++ = v6;
    	      *iptr++ = v2;
    	      nIdx++;

    	    }

    	    // Left
    	    if ( doL )
    	    {

    	      // V0
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y0;
    	      *vptr++ = z1;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v0 = startIndex++;

    	      // V3
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y1;
    	      *vptr++ = z1;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v3 = startIndex++;

    	      // V4
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y0;
    	      *vptr++ = z0;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v4 = startIndex++;

    	      // V7
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y1;
    	      *vptr++ = z0;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 0.0f;
    	      v7 = startIndex++;

    	      // indices
    	      *iptr++ = v4;
    	      *iptr++ = v0;
    	      *iptr++ = v3;
    	      *iptr++ = v7;
    	      nIdx++;

    	    }

    	    // Top
    	    if ( doS )
    	    {

    	      // V2
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y1;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      v2 = startIndex++;

    	      // V3
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y1;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      v3 = startIndex++;

    	      // V6
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y1;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      v6 = startIndex++;

    	      // V7
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y1;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = 1.0f;
    	      *vptr++ = 0.0f;
    	      v7 = startIndex++;

    	      *iptr++ = v3;
    	      *iptr++ = v2;
    	      *iptr++ = v6;
    	      *iptr++ = v7;
    	      nIdx++;

    	    }

    	    // Bottom
    	    if ( doI )
    	    {

    	      // V0
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y0;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      v0 = startIndex++;

    	      // V1
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y0;
    	      *vptr++ = z1;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      v1 = startIndex++;

    	      // V4
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x0;
    	      *vptr++ = y0;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      v4 = startIndex++;

    	      // V5
    	      *vptr++ = r;
    	      *vptr++ = g;
    	      *vptr++ = b;
    	      *vptr++ = x1;
    	      *vptr++ = y0;
    	      *vptr++ = z0;
    	      *vptr++ = 0.0f;
    	      *vptr++ = -1.0f;
    	      *vptr++ = 0.0f;
    	      v5 = startIndex++;

    	      *iptr++ = v1;
    	      *iptr++ = v0;
    	      *iptr++ = v4;
    	      *iptr++ = v5;
    	      nIdx++;

    	    }

    	  }

    	}

      }

    }

    _colorVertex.resize( startIndex * 9 );
    _indice.resize( nIdx * 4 );

    glBindBuffer( GL_ARRAY_BUFFER, _buffers[ 0 ] );
    glBufferData( GL_ARRAY_BUFFER, _colorVertex.size() * sizeof( GLfloat ),
                  &_colorVertex[ 0 ], GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _buffers[ 1 ] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, _indice.size() * sizeof( GLuint ),
                  &_indice[ 0 ], GL_STATIC_DRAW );

  }

  _needUpdate = false;

}


void gkg::Chunk::updateBlock( int32_t x, int32_t y, int32_t z, int32_t label )
{

  if ( _blocks[ z ][ y ][ x ].getLabel() != label )
  {

    _blocks[ z ][ y ][ x ].setLabel( label );
    _activeBlockCount += label ? 1 : -1;
    _needUpdate = true;

  }

}


void gkg::Chunk::updateBlocks( int32_t label, bool show )
{

  if ( _activeBlockCount )
  {

    int32_t i, j, k;

    for ( k = 0; k < _nZ; k++ )
    {

      for ( j = 0; j < _nY; j++ )
      {

        for ( i = 0; i < _nX; i++ )
        {

          if ( _blocks[ k ][ j ][ i ].getLabel() == label )
          {

            _blocks[ k ][ j ][ i ].setActive( show );
            _needUpdate = true;

          }

        }

      }

    }

  }

}


void gkg::Chunk::render( gkg::VoiData& data )
{

  if ( _activeBlockCount )
  {

    if ( glIsBuffer( _buffers[ 0 ] ) == GL_FALSE )
    {

      glGenBuffers( 2, _buffers );
      _needUpdate = true;

    }

    createMesh( data );

    glBindBuffer( GL_ARRAY_BUFFER, _buffers[ 0 ] );
    glNormalPointer( GL_FLOAT, 9 * sizeof( GLfloat ),
                     ( (GLfloat*)NULL + ( 6 ) ) );
    glVertexPointer( 3, GL_FLOAT, 9 * sizeof( GLfloat ),
                     ( (GLfloat*)NULL + ( 3 ) ) );
    glColorPointer( 3, GL_FLOAT, 9 * sizeof( GLfloat ), 0 );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _buffers[ 1 ] );

    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    glDrawElements( GL_QUADS, _indice.size(), GL_UNSIGNED_INT, 0 );

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

  }

}


void gkg::Chunk::leave()
{

  if ( glIsBuffer( _buffers[ 0 ] ) == GL_TRUE )
  {

    glDeleteBuffers( 2, _buffers );
    _buffers[ 0 ] = 0;
    _buffers[ 1 ] = 0;

  }

}
