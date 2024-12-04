#include <gkg-graphics-video-avi/AviWriter.h>
#include <gkg-graphics-video-avi/FourCC.h>
#include <gkg-core-exception/Exception.h>

#include <cstring>
#include <cstdio>


template < class S >
inline
gkg::AviWriter< S >::AviWriter( int32_t width, 
                           	int32_t height,
				int32_t bitsPerPixel,
                           	int32_t nFrames, 
			   	int32_t frameRate,
                           	int32_t flags )
                    : _moviSizeOffset( 0 ),
                      _moviSize( 4 )
{

  try
  {
  
    memset( &_mainHeader, 0, sizeof( gkg::AviMainHeader ) );
    memset( &_streamHeader, 0, sizeof( gkg::AviStreamHeader ) );
    memset( &_bih, 0, sizeof( gkg::BitmapInfoHeader ) );
    initialize( width, height, bitsPerPixel, nFrames, frameRate, flags );

  }
  GKG_CATCH( "template < class S > inline "
             "gkg::AviWriter< S >::AviWriter( int32_t width, int32_t height, "
	     "int32_t bitsPerPixel, int32_t nFrames, int32_t frameRate, "
             "int32_t flags )" );

}


template < class S >
inline
gkg::AviWriter< S >::~AviWriter()
{
}


template < class S >
inline
void gkg::AviWriter< S >::addFrame( const void* dib, uint32_t size )
{

  try
  {

    if ( dib )
    {

      int32_t ckid = MAKEAVICKID( cktypeDIBbits, 0 );

      if ( _index.empty() )
      {

    	_bih.biSizeImage = size;
    	_streamHeader.dwSuggestedBufferSize = size;
    	_mainHeader.dwSuggestedBufferSize = size;
    	writeHeaders();

      }

      uint32_t offset = _aviBuffer.lseek( 0, SEEK_CUR );

      _aviBuffer.write_le32( ckid );
      _aviBuffer.write_le32( size );
      _aviBuffer.write( dib, size );

      if ( size & 1 )
      {

    	_aviBuffer.write( dib, 1 );

      }

      gkg::AviIndexEntry entry;
      entry.ckid = ckid;
      entry.dwFlags = AVIIF_KEYFRAME;
      entry.dwChunkOffset = offset - _moviSizeOffset - 4;
      entry.dwChunkLength = size;
      _index.push_back( entry );

      _moviSize += 8 + size + ( size & 1 );

    }
  
  }
  GKG_CATCH( "template < class S > inline "
             "void gkg::AviWriter< S >::addFrame( const void* dib, "
	     "uint32_t size )" );

}


template < class S >
inline
void gkg::AviWriter< S >::write( std::string fileName )
{

  try
  {
  
    finalize();
    _aviBuffer.write( fileName );

  }
  GKG_CATCH( "template < class S > inline "
             "void gkg::AviWriter< S >::write( std::string fileName )" );

}


template < class S >
inline
void gkg::AviWriter< S >::initialize( int32_t width, 
                                      int32_t height,
				      int32_t bitsPerPixel,
                                      int32_t nFrames, 
				      int32_t frameRate,
                                      int32_t flags )
{

  try
  {

    _mainHeader.dwWidth = width;
    _mainHeader.dwHeight = height;
    _mainHeader.dwTotalFrames = nFrames;
    _mainHeader.dwFlags = flags;
    _mainHeader.dwFlags |= AVIF_HASINDEX | AVIF_TRUSTCKTYPE;
    _mainHeader.dwPaddingGranularity = 0;
    _mainHeader.dwStreams = 1;
    _mainHeader.dwMicroSecPerFrame = 1000000 / frameRate;

    _streamHeader.dwRate = 1000;
    _streamHeader.dwScale = 1000 / frameRate;
    _streamHeader.fccType = streamtypeVIDEO;
    _streamHeader.fccHandler = 0;
    _streamHeader.dwFlags = flags;
    _streamHeader.dwLength = nFrames;
    _streamHeader.rcFrame.right = width;
    _streamHeader.rcFrame.bottom = height;

    _bih.biSize = 40;
    _bih.biPlanes = 1;
    _bih.biBitCount = bitsPerPixel;
    _bih.biWidth = width;
    _bih.biHeight = height;

    _index.clear();

    _aviBuffer.initialize( width, height, bitsPerPixel, nFrames );
    
  }
  GKG_CATCH( "template < class S > inline "
             "void gkg::AviWriter< S >::initialize( int32_t width, "
             "int32_t height, int32_t bitsPerPixel, int32_t nFrames, "
	     "int32_t frameRate, int32_t flags )" );

}


template < class S >
inline
void gkg::AviWriter< S >::writeHeaders()
{

  try
  {
  
    int32_t hdr_size;

    _aviBuffer.write_le32( FOURCC_RIFF );
    _aviBuffer.write_le32( 0 );
    _aviBuffer.write_le32( formtypeAVI );

    _aviBuffer.write_le32( FOURCC_LIST );
    hdr_size = 12 + sizeof( gkg::AviMainHeader );
    hdr_size += 20 + sizeof( gkg::AviStreamHeader );
    hdr_size += 8 + _bih.biSize;
    _aviBuffer.write_le32( hdr_size );
    _aviBuffer.write_le32( listtypeAVIHEADER );

    _aviBuffer.write_le32( ckidAVIMAINHDR );
    _aviBuffer.write_le32( (int32_t)sizeof( gkg::AviMainHeader ) );
    _aviBuffer.write( &_mainHeader, sizeof( gkg::AviMainHeader ) );

    _aviBuffer.write_le32( FOURCC_LIST );
    hdr_size = 12 + sizeof( gkg::AviStreamHeader );
    hdr_size += 8 + _bih.biSize;
    _aviBuffer.write_le32( hdr_size );
    _aviBuffer.write_le32( listtypeSTREAMHEADER );

    _aviBuffer.write_le32( ckidSTREAMHEADER );
    _aviBuffer.write_le32( (int32_t)sizeof( gkg::AviStreamHeader ) );
    _aviBuffer.write( &_streamHeader, sizeof( gkg::AviStreamHeader ) );

    _aviBuffer.write_le32( ckidSTREAMFORMAT );
    _aviBuffer.write_le32( _bih.biSize );
    _aviBuffer.write( &_bih, _bih.biSize );

    _aviBuffer.write_le32( FOURCC_LIST );
    _moviSizeOffset = _aviBuffer.lseek( 0, SEEK_CUR );
    _aviBuffer.write_le32( 0 );
    _aviBuffer.write_le32( listtypeAVIMOVIE );
    
  }
  GKG_CATCH( "template < class S > inline "
             "void gkg::AviWriter< S >::writeHeaders()" );

}


template < class S >
inline
void gkg::AviWriter< S >::finalize()
{

  try
  {
  
    _aviBuffer.write_le32( ckidAVINEWINDEX );
    _aviBuffer.write_le32( (int32_t)( _index.size() * 
    				      sizeof( gkg::AviIndexEntry ) ) );
    _aviBuffer.write( &_index[ 0 ], 
                      (int32_t)( _index.size() * 
		                 sizeof( gkg::AviIndexEntry ) ) );
    _aviBuffer.lseek( _moviSizeOffset, SEEK_SET );
    _aviBuffer.write_le32( _moviSize );

    uint32_t endpos = _aviBuffer.lseek( 0, SEEK_END );

    _aviBuffer.lseek( 4, SEEK_SET );
    _aviBuffer.write_le32( endpos - 8 );

  }
  GKG_CATCH( "template < class S > inline"
             "void gkg::AviWriter< S >::finalize()" );
  
}
