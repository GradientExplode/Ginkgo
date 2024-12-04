#include <gkg-graphics-X11-core/PSFont.h>
#include <gkg-graphics-X11-core/PSFontImplementation.h>
#include <gkg-graphics-X11-core/Font.h>
#include <fstream>


gkg::PSFont::PSFont( const std::string& psName,
                     gkg::FloatCoordinate size,
                     const std::string& name,
                     float scale )
            : gkg::Font( name, scale )
{

  _psFontImplementation = new gkg::PSFontImplementation( psName, size );

}


gkg::PSFont::~PSFont()
{

  delete _psFontImplementation;

}


const std::string& gkg::PSFont::getName() const
{

  return _psFontImplementation->getName();

}


const std::string& gkg::PSFont::getEncoding() const
{

  return _psFontImplementation->getEncoding();

}


gkg::FloatCoordinate gkg::PSFont::getSize() const
{

  return _psFontImplementation->getSize();

}


gkg::FloatCoordinate gkg::PSFont::getWidth( int32_t c ) const
{

  return _psFontImplementation->getWidth( c );

}


gkg::FloatCoordinate gkg::PSFont::getWidth( const std::string& s ) const
{

  return this->gkg::Font::getWidth( s );

}


gkg::PSFontImplementation* gkg::PSFont::getImplementation() const
{

  return _psFontImplementation;

}


bool gkg::PSFont::exists( const std::string& psName )
{

  std::string 
    metricsFile = gkg::PSFontImplementation::getPostscriptFile( psName );
  std::ifstream is( metricsFile.c_str() );
  if ( !is )
  {

    return false;

  }
  is.close();
  return true;

}
