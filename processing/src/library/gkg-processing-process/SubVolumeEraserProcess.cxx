#include <gkg-processing-process/SubVolumeEraserProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>


gkg::SubVolumeEraserProcess::SubVolumeEraserProcess(
                                                 const std::string& fileNameOut,
                                                 int32_t sx, int32_t sy, 
                                                 int32_t sz, int32_t st,
                                                 int32_t ex, int32_t ey, 
                                                 int32_t ez, int32_t et,
                                                 bool ascii,
                                                 const std::string& format,
                                                 bool verbose )
                           : gkg::Process( "Volume" ),
                             _fileNameOut( fileNameOut ),
                             _sx( sx ), _sy( sy ), _sz( sz ), _st( st ),
                             _ex( ex ), _ey( ey ), _ez( ez ), _et( et ),
                             _ascii( ascii ),
                             _format( format ),
                             _verbose( verbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &SubVolumeEraserProcess::erase< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &SubVolumeEraserProcess::erase< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &SubVolumeEraserProcess::erase< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &SubVolumeEraserProcess::erase< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &SubVolumeEraserProcess::erase< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &SubVolumeEraserProcess::erase< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &SubVolumeEraserProcess::erase< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &SubVolumeEraserProcess::erase< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &SubVolumeEraserProcess::erase< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &SubVolumeEraserProcess::erase< double > );
    registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                     &SubVolumeEraserProcess::erase< std::complex< float > > );
    registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                     &SubVolumeEraserProcess::erase< std::complex< double > > );

  }
  GKG_CATCH( "gkg::SubVolumeEraserProcess::SubVolumeEraserProcess( "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, "
             "int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, "
             "int32_t ez, int32_t et, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


const std::string& gkg::SubVolumeEraserProcess::getFileNameOut() const
{

  return _fileNameOut;

}


void 
gkg::SubVolumeEraserProcess::getStartValues( int32_t& sx, int32_t& sy, 
                                             int32_t& sz, int32_t& st ) const
{

  sx = _sx;
  sy = _sy;
  sz = _sz;
  st = _st;

}


void 
gkg::SubVolumeEraserProcess::getEndValues( int32_t& ex, int32_t& ey, 
                                           int32_t& ez, int32_t& et ) const
{

  ex = _ex;
  ey = _ey;
  ez = _ez;
  et = _et;

}


bool gkg::SubVolumeEraserProcess::getAscii() const
{

  return _ascii;

}


const std::string& gkg::SubVolumeEraserProcess::getFormat() const
{

  return _format;

}


bool gkg::SubVolumeEraserProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void gkg::SubVolumeEraserProcess::erase( gkg::Process& process,
                                         const std::string& fileNameIn,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    gkg::SubVolumeEraserProcess&
      subVolumeEraserProcess = static_cast< gkg::SubVolumeEraserProcess& >(
                                                                      process );
    bool verbose = subVolumeEraserProcess.getVerbose();

    // putting default for start and end values
    int32_t sx, sy, sz, st;
    subVolumeEraserProcess.getStartValues( sx, sy, sz, st );
    int32_t ex, ey, ez, et;
    subVolumeEraserProcess.getEndValues( ex, ey, ez, et );

    // sanity check
    if ( ( sx == -1 ) && ( ex == -1 ) &&
         ( sy == -1 ) && ( ey == -1 ) &&
         ( sz == -1 ) && ( ez == -1 ) &&
         ( st == -1 ) && ( et == -1 ) )           
    {

      throw std::runtime_error( "Cannot erase the entire volume" );

    }

    // reading input volume
    if ( verbose )
    {

      std::cout << "reading input volume \'" << fileNameIn << "\' : "
                << std::flush;

    }

    gkg::Volume< T > volumeIn;
    std::string format = gkg::Reader::getInstance().read( fileNameIn, 
                                                          volumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    int32_t sizeT = volumeIn.getSizeT();

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

    // erasing subvolume

    if ( verbose )
    {

      std::cout << "erasing subvolume : " << std::flush;

    }

    int32_t x, y, z, t;
    for ( t = st; t <= et; t++ )
    {

      for ( z = sz; z <= ez; z++ )
      {

        for ( y = sy; y <= ey; y++ )
        {

          for ( x = sx; x <= ex; x++ )
          {

            volumeIn( x, y, z, t ) = T( 0 );

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
                << subVolumeEraserProcess.getFileNameOut() << "\' : "
                << std::flush;

    }

    gkg::Writer::getInstance().write(
                                  subVolumeEraserProcess.getFileNameOut(),
                                  volumeIn,
                                  subVolumeEraserProcess.getAscii(),
                                  ( subVolumeEraserProcess.getFormat().empty() ?
                                    format : 
                                    subVolumeEraserProcess.getFormat() ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void gkg::subVolumeEraserProcess::erase( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}
