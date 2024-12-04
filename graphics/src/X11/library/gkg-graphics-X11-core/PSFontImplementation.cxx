#include <gkg-graphics-X11-core/PSFontImplementation.h>
#include <gkg-graphics-X11-core/PSFont.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


gkg::PSFontImplementation::PSFontImplementation( const std::string& psName,
                                                 gkg::FloatCoordinate size )
                          : _name( "" ),
                            _encoding( "" ),
                            _size( size )
{

  std::string metricsFile = getPostscriptFile( psName );
  FILE* file = fopen( metricsFile.c_str(), "r" );
  if ( file )
  {

    char line[ 256 ];
    char name[ 256 ];
    char encoding[ 256 ];
    int32_t c;
    int32_t w;
    while ( fgets( line, 255, file ) )
    {

      if ( sscanf( line, "FullName %[a-zA-Z ]", name ) == 1 )
      {

        _name = ( std::string )name;

      }
      else if ( sscanf( line, "EncodingScheme %s", encoding ) == 1 )
      {

        _encoding = ( std::string )encoding;

      }
      else if ( sscanf( line, "C %d ; WX %d ;", &c, &w ) == 2 )
      {

        if ( c != -1 )
        {

          _widths[ c ] = ( float )w / 1000 * _size;

        }

      }

    }
    fclose( file );

  }

}


gkg::PSFontImplementation::~PSFontImplementation()
{
}


const std::string& gkg::PSFontImplementation::getName() const
{

  return _name;

}


const std::string& gkg::PSFontImplementation::getEncoding() const
{

  return _encoding;

}


gkg::FloatCoordinate gkg::PSFontImplementation::getSize() const
{

  return _size;

}


gkg::FloatCoordinate gkg::PSFontImplementation::getWidth( int32_t c ) const
{

  return _widths[ c ];

}


// users can also override the default Postscript directory by setting the
// PSFONTDIR environment variable
std::string 
gkg::PSFontImplementation::getPostscriptFile( const std::string& psName )
{

  std::string directory;
  const char* dir = getenv( "PSFONTDIR" );
  if ( dir )
  {

    directory = ( std::string )dir;

  }
  else
  {

    directory = gkg::getDefaultPSMetricsDirectory();

  }
  std::string metricsFile = directory +
                            gkg::getDirectorySeparator() +
                            psName + ".afm";
  return metricsFile;

}
