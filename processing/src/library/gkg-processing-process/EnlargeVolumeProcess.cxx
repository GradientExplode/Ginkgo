#include <gkg-processing-process/EnlargeVolumeProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EnlargeVolumeProcess::EnlargeVolumeProcess( const std::string& fileNameOut,
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
                     &EnlargeVolumeProcess::enlarge< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &EnlargeVolumeProcess::enlarge< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &EnlargeVolumeProcess::enlarge< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &EnlargeVolumeProcess::enlarge< double > );
    registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                     &EnlargeVolumeProcess::enlarge< std::complex< float > > );
    registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                     &EnlargeVolumeProcess::enlarge< std::complex< double > > );
    registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                     &EnlargeVolumeProcess::enlarge< gkg::RGBComponent > );

  }
  GKG_CATCH( "gkg::EnlargeVolumeProcess::EnlargeVolumeProcess( "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, "
             "int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, "
             "int32_t ez, int32_t et, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


const std::string& gkg::EnlargeVolumeProcess::getFileNameOut() const
{

  return _fileNameOut;

}


void 
gkg::EnlargeVolumeProcess::getStartValues( int32_t& sx, int32_t& sy, 
                                           int32_t& sz, int32_t& st ) const
{

  sx = _sx;
  sy = _sy;
  sz = _sz;
  st = _st;

}


void 
gkg::EnlargeVolumeProcess::getEndValues( int32_t& ex, int32_t& ey, 
                                         int32_t& ez, int32_t& et ) const
{

  ex = _ex;
  ey = _ey;
  ez = _ez;
  et = _et;

}


bool gkg::EnlargeVolumeProcess::getAscii() const
{

  return _ascii;

}


const std::string& gkg::EnlargeVolumeProcess::getFormat() const
{

  return _format;

}


bool gkg::EnlargeVolumeProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void gkg::EnlargeVolumeProcess::enlarge( gkg::Process& process,
                                         const std::string& fileNameIn,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    gkg::EnlargeVolumeProcess& enlargeVolumeProcess = 
                           static_cast< gkg::EnlargeVolumeProcess& >( process );
    bool verbose = enlargeVolumeProcess.getVerbose();

    // putting default for start and end values
    int32_t sx, sy, sz, st;
    enlargeVolumeProcess.getStartValues( sx, sy, sz, st );
    int32_t ex, ey, ez, et;
    enlargeVolumeProcess.getEndValues( ex, ey, ez, et );

    // sanity checks
    if ( sx < 1 )
    {

      sx = 0;

    }
    if ( sy < 1 )
    {

      sy = 0;

    }
    if ( sz < 1 )
    {

      sz = 0;

    }
    if ( st < 1 )
    {

      st = 0;

    }
    if ( ex < 1 )
    {

      ex = 0;

    }
    if ( ey < 1 )
    {

      ey = 0;

    }
    if ( ez < 1 )
    {

      ez = 0;

    }
    if ( et < 1 )
    {

      et = 0;

    }

    if ( ( sx == 0 ) && ( ex == 0 ) &&
         ( sy == 0 ) && ( ey == 0 ) &&
         ( sz == 0 ) && ( ez == 0 ) &&
         ( st == 0 ) && ( et == 0 ) )           
    {

      throw std::runtime_error( "No additinal slice specified" );

    }

    // reading input volume
    if ( verbose )
    {

      std::cout << "reading input volume \'" << fileNameIn << "\' : "
                << std::flush;

    }
    gkg::Volume< T > volumeIn;
    std::string format = 
      gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t x = 0, ox = 0, sizeX = volumeIn.getSizeX();
    int32_t y = 0, oy = 0, sizeY = volumeIn.getSizeY();
    int32_t z = 0, oz = 0, sizeZ = volumeIn.getSizeZ();
    int32_t t = 0, ot = 0, sizeT = volumeIn.getSizeT();

    // processing output volume size(s)
    int32_t outputSizeX = sizeX + sx + ex;
    int32_t outputSizeY = sizeY + sy + ey;
    int32_t outputSizeZ = sizeZ + sz + ez;
    int32_t outputSizeT = sizeT + st + et;

    // enlarging volume
    if ( verbose )
    {

      std::cout << "enlarging volume : " << std::flush;

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
                                  
    for ( t = 0, ot = st; t < sizeT; t++, ot++ )
    {

      for ( z = 0, oz = sz; z < sizeZ; z++, oz++ )
      {

        for ( y = 0, oy = sy; y < sizeY; y++, oy++ )
        {

          for ( x = 0, ox = sx; x < sizeX; x++, ox++ )
          {


            volumeOut( ox, oy, oz, ot ) = volumeIn( x, y, z, t );

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
                << enlargeVolumeProcess.getFileNameOut() << "\' : "
                << std::flush;

    }

    gkg::Writer::getInstance().write(
                                    enlargeVolumeProcess.getFileNameOut(),
                                    volumeOut,
                                    enlargeVolumeProcess.getAscii(),
                                    ( enlargeVolumeProcess.getFormat().empty() ?
                                      format : 
                                      enlargeVolumeProcess.getFormat() ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::EnlargeVolumeProcess::extract( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}
