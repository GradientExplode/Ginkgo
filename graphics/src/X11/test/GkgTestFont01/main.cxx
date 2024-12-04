// font

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/FontFamily.h>
#include <iostream>
#include <string>
#include <cstdlib>


static gkg::PropertyData properties[] =
{

  { "*family", "Times" },
  { "*face", "Roman" },
  { "*size", "12" },
  { 0, 0 }

};

static gkg::OptionDescription options[] =
{

  { "-family", "*family", gkg::OptionDescription::OptionValueNext, 0 },
  { "-face", "*face", gkg::OptionDescription::OptionValueNext, 0 },
  { "-size", "*size", gkg::OptionDescription::OptionValueNext, 0 },
  { 0, 0, gkg::OptionDescription::OptionNone, 0 }

};


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestFont01", argc, argv, options, properties );

  gkg::Style* style = session.getStyle();

  std::string family;
  style->findAttribute( "family", family );
  gkg::FontFamily* fontFamily = new gkg::FontFamily( family );

  std::string face;
  style->findAttribute( "face", face );

  int32_t size;
  style->findAttribute( "size", size );

  std::string name;
  float scale;
  if ( fontFamily->getFont( size, face, name, scale ) )
  {

    std::cout << "font '" << name << "', scale " << scale << std::endl;

  }
  else
  {

    std::cout << "no match for " << family << '*' << face << '*' << size
              << std::endl;

  }

  return EXIT_SUCCESS;

}
