#include <gkg-processing-plot/PSPlotImplementation.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <iostream>
#include <cmath>


// standard A4 page size
#define PS_PAGE_WIDTH       ( 21.0 / 2.54 * 72 )
#define PS_PAGE_HEIGHT      ( 29.7 / 2.54 * 72 )
#define PS_XMAX             ( 15.0 / 2.54 * 72 )
#define PS_YMAX             ( 10.0 / 2.54 * 72 )


static const char* ps_prolog = "\
save 20 dict begin\n\
\n\
/sf {   % scale /fontName => -  (set current font)\n\
    {findfont} stopped {pop /Courier findfont} if\n\
    exch scalefont setfont\n\
} def\n\
\n\
/ws {\n\
    4 index 6 4 roll moveto sub\n\
    2 index stringwidth pop sub\n\
    exch div 0 8#40 4 3 roll\n\
    widthshow\n\
} def\n\
\n\
/as {\n\
    4 index 6 4 roll moveto sub\n\
    2 index stringwidth pop sub\n\
    exch div 0 3 2 roll\n\
    ashow\n\
} def\n\
\n\
";


static const char* ps_epilog = "\
end restore\n\
";


RegisterDefaultPlotter( gkg, PSPlotImplementation );


gkg::PSPlotImplementation::PSPlotImplementation()
                           : gkg::CurvePlotterImplementation()
{
}


gkg::PSPlotImplementation::~PSPlotImplementation()
{
}


std::string gkg::PSPlotImplementation::getName() const
{

  return getStaticName();

}


void gkg::PSPlotImplementation::begin( const std::string& name )
{

  _os.open( ( getNameWoSpace( name ) + ".ps" ).c_str() );

  _os << "%!PS-Adobe-2.0\n";
  _os << "%%Creator: " << getNameWoSpace( name ) << "\n";
  _os << "%%Pages: atend\n";
  _os << "%%EndComments\n";
  _os << "%%BeginSetup\n";
  _os << "%%IncludeFeature: *PageSize A4\n";
  _os << "%%EndSetup\n";
  _os << ps_prolog;
  _os << "%%EndProlog\n";

  _os << "%%Page: 1 1" << "\n";
  _os << -( PS_XMAX - PS_PAGE_WIDTH ) / 2 << " "
      << -( PS_YMAX - PS_PAGE_HEIGHT ) / 2 << " translate\n";

}


void gkg::PSPlotImplementation::end()
{

  _os << "showpage\n";
  _os << "%%Trailer\n";
  _os << ps_epilog;
  _os << "%%Pages: 1" << "\n";

  _os.close();

}


double gkg::PSPlotImplementation::getWidth() const
{

  return PS_XMAX;

}


double gkg::PSPlotImplementation::getHeight() const
{

  return PS_YMAX;

}


double gkg::PSPlotImplementation::getCharacterWidth() const
{

  return ( 0.22 / 2.54 * 72 );

}


double gkg::PSPlotImplementation::getCharacterHeight() const
{

  return ( 0.4 / 2.54 * 72 );

}


double gkg::PSPlotImplementation::getTicWidth() const
{

  return ( 0.2 / 2.54 * 72 );

}


double gkg::PSPlotImplementation::getTicHeight() const
{

  return ( 0.2 / 2.54 * 72 );

}


void gkg::PSPlotImplementation::move( double x, double y )
{

  _x = x;
  _y = y;

}


void gkg::PSPlotImplementation::drawVector( double x, double y )
{

  _os << _lineWidth << " setlinewidth" << std::endl;
  _os << "newpath" << std::endl;
  _os << _x << " " << _y << " " << "moveto" << std::endl;
  _os << x << " " << y << " " << "lineto" << std::endl;
  _os << "closepath" << std::endl;
  _os << "gsave "
      << _red << " " << _green << " " << _blue << " "
      << " setrgbcolor stroke grestore" << std::endl;
  _x = x;
  _y = y;

}


void gkg::PSPlotImplementation::drawText( double x, double y,
                                          const std::string& text )
{

  double offset = 0.0;
  switch ( _justifyType )
  {

    case gkg::PSPlotImplementation::LeftJustify:

      offset = 0.0;
      break;

    case gkg::PSPlotImplementation::CenterJustify:

      offset = - ( double )text.length() * getCharacterWidth() / 2.0;
      break;

    case gkg::PSPlotImplementation::RightJustify:

      offset = - ( double )text.length() * getCharacterWidth();
      break;

  }

  x += offset;

  _os << "10 /Fixed sf" << std::endl;
  _os << x << " " << y << " (" << text << ") "
      << text.length() << " "
      << x + ( double )text.length() * getCharacterWidth() << " as"
      << std::endl;

}


void gkg::PSPlotImplementation::setLineType( 
                                 gkg::PSPlotImplementation::LineType lineType )
{

  _lineType = lineType;

  switch ( _lineType )
  {

    case gkg::PSPlotImplementation::BorderLine:

      _lineWidth = 1;
      _red = 0.0;
      _green = 0.0;
      _blue = 0.0;
      break;

    case gkg::PSPlotImplementation::AxisLine:

      _lineWidth = 1;
      _red = 0.0;
      _green = 0.0;
      _blue = 0.0;
      break;

    case gkg::PSPlotImplementation::RunLine1:

      _lineWidth = 1;
      _red = 1.0;
      _green = 0.0;
      _blue = 0.0;
       break;

    case gkg::PSPlotImplementation::RunLine2:

      _lineWidth = 1;
      _red = 0.0;
      _green = 1.0;
      _blue = 0.0;
      break;

    case gkg::PSPlotImplementation::RunLine3:

      _lineWidth = 1;
      _red = 0.0;
      _green = 0.0;
      _blue = 1.0;
      break;

    case gkg::PSPlotImplementation::RunLine4:

      _lineWidth = 1;
      _red = 1.0;
      _green = 1.0;
      _blue = 0.0;
      break;

    case gkg::PSPlotImplementation::RunLine5:

      _lineWidth = 1;
      _red = 1.0;
      _green = 0.0;
      _blue = 1.0;
      break;

    case gkg::PSPlotImplementation::RunLine6:

      _lineWidth = 1;
      _red = 0.0;
      _green = 1.0;
      _blue = 1.0;
      break;

    case gkg::PSPlotImplementation::RunLine7:

      _lineWidth = 1;
      _red = 1.0;
      _green = 0.5;
      _blue = 0.5;
      break;

  }

}


void gkg::PSPlotImplementation::setLeftJustify()
{

  _justifyType = gkg::PSPlotImplementation::LeftJustify;

}


void gkg::PSPlotImplementation::setCenterJustify()
{

  _justifyType = gkg::PSPlotImplementation::CenterJustify;

}


void gkg::PSPlotImplementation::setRightJustify()
{

  _justifyType = gkg::PSPlotImplementation::RightJustify;

}


std::string gkg::PSPlotImplementation::getStaticName()
{

  return "PostscriptPlot";

}


std::string 
gkg::PSPlotImplementation::getNameWoSpace( const std::string& name ) const
{

  std::string result = name;
  for ( size_t c = 0; c < result.length(); c++ )
  {

    if ( result[ c ] == ' ' )
    {

      result[ c ] = '_';

    }

  }
  return result;

}


#undef PS_PAGE_WIDTH
#undef PS_PAGE_HEIGHT
#undef PS_XMAX
#undef PS_YMAX
