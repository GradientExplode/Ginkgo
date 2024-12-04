#include <gkg-processing-process/SubVolumeProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>


gkg::SubVolumeProcess::SubVolumeProcess(
                                    const std::string& fileNameOut,
                                    int32_t sx, int32_t sy, 
				    int32_t sz, int32_t st,
                                    int32_t ex, int32_t ey, 
				    int32_t ez, int32_t et,
                                    const std::list< int32_t >& xIndices,
                                    const std::list< int32_t >& yIndices,
                                    const std::list< int32_t >& zIndices,
                                    const std::list< int32_t >& tIndices,
                                    bool ascii,
                                    const std::string& format,
                                    bool verbose )
                      : gkg::Process( "Volume" ),
                        _fileNameOut( fileNameOut ),
                        _sx( sx ), _sy( sy ), _sz( sz ), _st( st ),
                        _ex( ex ), _ey( ey ), _ez( ez ), _et( et ),
                        _xIndices( xIndices ),
                        _yIndices( yIndices ),
                        _zIndices( zIndices ),
                        _tIndices( tIndices ),
                        _ascii( ascii ),
                        _format( format ),
                        _verbose( verbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &SubVolumeProcess::extract< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &SubVolumeProcess::extract< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &SubVolumeProcess::extract< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &SubVolumeProcess::extract< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &SubVolumeProcess::extract< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &SubVolumeProcess::extract< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &SubVolumeProcess::extract< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &SubVolumeProcess::extract< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &SubVolumeProcess::extract< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &SubVolumeProcess::extract< double > );
    registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                     &SubVolumeProcess::extract< std::complex< float > > );
    registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                     &SubVolumeProcess::extract< std::complex< double > > );

  }
  GKG_CATCH( "gkg::SubVolumeProcess::SubVolumeProcess( "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, "
	     "int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, "
	     "int32_t ez, int32_t et, "
             "const std::list< int32_t >& xIndices, "
             "const std::list< int32_t >& yIndices, "
             "const std::list< int32_t >& zIndices, "
             "const std::list< int32_t >& tIndices, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


const std::string& gkg::SubVolumeProcess::getFileNameOut() const
{

  return _fileNameOut;

}


void 
gkg::SubVolumeProcess::getStartValues( int32_t& sx, int32_t& sy, 
                                       int32_t& sz, int32_t& st ) const
{

  sx = _sx;
  sy = _sy;
  sz = _sz;
  st = _st;

}


void 
gkg::SubVolumeProcess::getEndValues( int32_t& ex, int32_t& ey, 
                                     int32_t& ez, int32_t& et ) const
{

  ex = _ex;
  ey = _ey;
  ez = _ez;
  et = _et;

}


const std::list< int32_t >& gkg::SubVolumeProcess::getXIndices() const
{

  return _xIndices;

}


const std::list< int32_t >& gkg::SubVolumeProcess::getYIndices() const
{

  return _yIndices;

}


const std::list< int32_t >& gkg::SubVolumeProcess::getZIndices() const
{

  return _zIndices;

}


const std::list< int32_t >& gkg::SubVolumeProcess::getTIndices() const
{

  return _tIndices;

}


bool gkg::SubVolumeProcess::getAscii() const
{

  return _ascii;

}


const std::string& gkg::SubVolumeProcess::getFormat() const
{

  return _format;

}


bool gkg::SubVolumeProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void gkg::SubVolumeProcess::extract( gkg::Process& process,
                                     const std::string& fileNameIn,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    gkg::SubVolumeProcess&
      subVolumeProcess = static_cast< gkg::SubVolumeProcess& >( process );


    bool verbose = subVolumeProcess.getVerbose();

    // first try to get header information
    if ( verbose )
    {

      std::cout << "reading header information : " << std::flush;

    }
    std::string format;
    gkg::Volume< T > volumeProxy;
    format = gkg::Reader::getInstance().readHeader( fileNameIn, volumeProxy );

    int32_t sizeX = volumeProxy.getSizeX();
    int32_t sizeY = volumeProxy.getSizeY();
    int32_t sizeZ = volumeProxy.getSizeZ();
    int32_t sizeT = volumeProxy.getSizeT();

    if ( verbose )
    {

      std::cout << "( " << sizeX << ", "
                        << sizeY << ", "
                        << sizeZ << ", "
                        << sizeT << " )" << std::endl;

    }

    // putting default for start and end values
    int32_t sx, sy, sz, st;
    subVolumeProcess.getStartValues( sx, sy, sz, st );
    int32_t ex, ey, ez, et;
    subVolumeProcess.getEndValues( ex, ey, ez, et );

    std::vector< int32_t > xIndices( subVolumeProcess.getXIndices().begin(),
                                     subVolumeProcess.getXIndices().end() );
    std::vector< int32_t > yIndices( subVolumeProcess.getYIndices().begin(),
                                     subVolumeProcess.getYIndices().end() );
    std::vector< int32_t > zIndices( subVolumeProcess.getZIndices().begin(),
                                     subVolumeProcess.getZIndices().end() );
    std::vector< int32_t > tIndices( subVolumeProcess.getTIndices().begin(),
                                     subVolumeProcess.getTIndices().end() );

    bool useIndices = false;
    if ( xIndices.size() ||
         yIndices.size() ||
         zIndices.size() ||
         tIndices.size() )
    {

      if ( ( sx != -1 ) || ( ex != -1 ) ||
           ( sy != -1 ) || ( ey != -1 ) ||
           ( sz != -1 ) || ( ez != -1 ) ||
           ( st != -1 ) || ( et != -1 ) )
           
      {

        throw std::runtime_error(
          "setting indices is not compatible with using start and end "
          "coordinates" );

      }
      useIndices = true;

    }
    else
    {

      if ( sx == -1 )
      {

        sx = 0;

      }
      if ( sy == -1 )
      {

        sy = 0;

      }
      if ( sz == -1 )
      {

        sz = 0;

      }
      if ( st == -1 )
      {

        st = 0;

      }
      if ( ex == -1 )
      {

        ex = sizeX - 1;

      }
      if ( ey == -1 )
      {

        ey = sizeY - 1;

      }
      if ( ez == -1 )
      {

        ez = sizeZ - 1;

      }
      if ( et == -1 )
      {

        et = sizeT - 1;

      }

    }
         
    if ( !useIndices )
    {


      // creating bounding box
      gkg::BoundingBox< int32_t > boundingBox( sx, ex, sy, ey, sz, ez, st, et );

      // partial read
      if ( verbose )
      {

        std::cout << "reading input volume \'" << fileNameIn << "\' : "
                  << std::flush;

      }
      gkg::Volume< T > volumeOut( ex - sx + 1,
                                  ey - sy + 1,
                                  ez - sz + 1,
                                  et - st + 1 );

      gkg::VolumeProxy< T > inputVolumeProxy;
      format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                     gkg::Volume< T > >(
                                                              fileNameIn,
                                                              inputVolumeProxy,
                                                              format );

      format = gkg::Reader::getInstance().template partialBinaryRead<
                                                     gkg::Volume< T > >(
                                                              inputVolumeProxy,
                                                              volumeOut,
                                                              boundingBox,
                                                              format );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing result
      if ( verbose )
      {

        std::cout << "writing output volume \'"
                  << subVolumeProcess.getFileNameOut() << "\' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( subVolumeProcess.getFileNameOut(),
                                        volumeOut,
                                        subVolumeProcess.getAscii(),
                                        ( subVolumeProcess.getFormat().empty() ?
                                          format : 
                                          subVolumeProcess.getFormat() ) );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      int32_t t = 0;
      if ( xIndices.empty() )
      {

        xIndices.resize( sizeX );
        for ( x = 0; x < sizeX; x++ )
        {

          xIndices[ x ] = x;

        }

      }

      if ( yIndices.empty() )
      {

        yIndices.resize( sizeY );
        for ( y = 0; y < sizeY; y++ )
        {

          yIndices[ y ] = y;

        }

      }

      if ( zIndices.empty() )
      {

        zIndices.resize( sizeZ );
        for ( z = 0; z < sizeZ; z++ )
        {

          zIndices[ z ] = z;

        }

      }

      if ( tIndices.empty() )
      {

        tIndices.resize( sizeT );
        for ( t = 0; t < sizeT; t++ )
        {

          tIndices[ t ] = t;

        }

      }

      // processing output volume size(s)
      int32_t outputSizeX = ( int32_t )xIndices.size();
      int32_t outputSizeY = ( int32_t )yIndices.size();
      int32_t outputSizeZ = ( int32_t )zIndices.size();
      int32_t outputSizeT = ( int32_t )tIndices.size();

      // reading input volume
      if ( verbose )
      {

        std::cout << "reading input volume \'" << fileNameIn << "\' : "
                  << std::flush;

      }
      gkg::Volume< T > volumeIn;
      gkg::Reader::getInstance().read( fileNameIn,
                                       volumeIn );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // extracting volume
      if ( verbose )
      {

        std::cout << "extracting selected indices : " << std::flush;

      }
      gkg::Volume< T > volumeOut( outputSizeX,
                                  outputSizeY,
                                  outputSizeZ,
                                  outputSizeT );
      volumeOut.getHeader() = volumeIn.getHeader();
      volumeOut.getHeader()[ "sizeX" ] = outputSizeX;
      volumeOut.getHeader()[ "sizeY" ] = outputSizeY;
      volumeOut.getHeader()[ "sizeZ" ] = outputSizeZ;
      volumeOut.getHeader()[ "sizeT" ] = outputSizeT;
                                  
      for ( t = 0; t < outputSizeT; t++ )
      {

        for ( z = 0; z < outputSizeZ; z++ )
        {

          for ( y = 0; y < outputSizeY; y++ )
          {

            for ( x = 0; x < outputSizeX; x++ )
            {


              volumeOut( x, y, z, t ) = volumeIn( xIndices[ x ],
                                                  yIndices[ y ],
                                                  zIndices[ z ],
                                                  tIndices[ t ] );                                                  

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing output volume
      if ( verbose )
      {

        std::cout << "writing output volume \'"
                  << subVolumeProcess.getFileNameOut() << "\' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( subVolumeProcess.getFileNameOut(),
                                        volumeOut,
                                        subVolumeProcess.getAscii(),
                                        ( subVolumeProcess.getFormat().empty() ?
                                          format : 
                                          subVolumeProcess.getFormat() ) );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void gkg::SubVolumeProcess::extract( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}
