#include <gkg-graphics-X11-core/PrinterImplementation.h>
#include <gkg-graphics-X11-core/Printer.h>


//
// class PrinterImplementation::Info
//
gkg::PrinterImplementation::Info::Info()
                           : color( 0 ),
                             brush( 0 ),
                             font( 0 )
{
}


gkg::PrinterImplementation::Info::Info( const gkg::Color* theColor,
                                        const gkg::Brush* theBrush,
                                        const gkg::Font* theFont )
                           : color( theColor ),
                             brush( theBrush ),
                             font( theFont )
{
}


//
// class PrinterImplementation
//

gkg::PrinterImplementation::PrinterImplementation( gkg::Printer* printer,
                                                   std::ostream* out )
                           : _target( printer ),
                             _out( out ),
                             _page( 1 ),
                             _x( 0 ),
                             _y( 0 ),
                             _textCurrentX( 0 ),
                             _textCurrentY( 0 ),
                             _textChars( 0 ),
                             _textSpaces( 0 )
{
}


gkg::PrinterImplementation::~PrinterImplementation()
{

  _infos.clear();

}


gkg::Printer* gkg::PrinterImplementation::getTarget() const
{

  return _target;

}
