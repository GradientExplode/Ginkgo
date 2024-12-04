#include <gkg-processing-plot/GnuPlotImplementation.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-communication-sysinfo/FileFinder.h>
#include <gkg-core-cppext/StdInt.h>
#include <iostream>
#include <cmath>


#define X11_XMAX            4096.0
#define X11_YMAX            4096.0


RegisterDefaultPlotter( gkg, GnuPlotImplementation );


// Class GnuPlotImplementation


gkg::GnuPlotImplementation::GnuPlotImplementation()
                           : gkg::CurvePlotterImplementation()
{

  try
  {

    gkg::FileFinder finder;

    if ( ! finder.locateFromPath( "gnuplot_x11" ) &&
         ! finder.locateFromDirectory( "gnuplot_x11", "/usr/libexec/gnuplot" ) )
    {

      throw std::runtime_error( "could not find gnuplot_x11" );

    }
 
    std::string path = finder.getFullPath() + " -noevents";
    _x11Ipc = popen( path.c_str(), "w" );
    if ( !_x11Ipc )
    {

      std::cerr << "gkg::GnuPlotImplementation::GnuPlotImplementation: "
                << "could not open pipe"
                << std::endl;

    }

  }
  GKG_CATCH("gkg::GnuPlotImplementation::GnuPlotImplementation()" );

}


gkg::GnuPlotImplementation::~GnuPlotImplementation()
{

  fprintf( _x11Ipc, "R\n" );
  fflush( _x11Ipc );
  pclose( _x11Ipc );

}


std::string gkg::GnuPlotImplementation::getName() const
{

  return getStaticName();

}


void gkg::GnuPlotImplementation::begin( const std::string& )
{

  fprintf( _x11Ipc, "G\n" );

}


void gkg::GnuPlotImplementation::end()
{

  fprintf( _x11Ipc, "E\n" );
  fflush( _x11Ipc );

}


double gkg::GnuPlotImplementation::getWidth() const
{

  return X11_XMAX;

}


double gkg::GnuPlotImplementation::getHeight() const
{

  return X11_YMAX;

}


double gkg::GnuPlotImplementation::getCharacterWidth() const
{

  return ( X11_XMAX / 100 );

}


double gkg::GnuPlotImplementation::getCharacterHeight() const
{

  return ( X11_YMAX / 25 );

}


double gkg::GnuPlotImplementation::getTicWidth() const
{

  return ( 2 * X11_XMAX / 150 );

}


double gkg::GnuPlotImplementation::getTicHeight() const
{

  return ( 2 * X11_YMAX / 100 );

}


void gkg::GnuPlotImplementation::move( double x, double y )
{

  fprintf( _x11Ipc, "M%04" PRId32 "%04" PRId32 "\n", ( int32_t )x, 
           ( int32_t )y );

}


void gkg::GnuPlotImplementation::drawVector( double x, double y )
{

  fprintf( _x11Ipc, "V%04" PRId32 "%04" PRId32 "\n", ( int32_t )x, 
           ( int32_t )y );

}


void gkg::GnuPlotImplementation::drawText( double x, double y,
                                           const std::string& text )
{

  fprintf( _x11Ipc, "T%04" PRId32 "%04" PRId32 "%s\n", ( int32_t )x, 
          ( int32_t )y, text.c_str() ); 

}


void gkg::GnuPlotImplementation::setLineType( 
                                 gkg::GnuPlotImplementation::LineType lineType )
{

  _lineType = lineType;

  int32_t type = 0;
  switch ( _lineType )
  {

    case gkg::GnuPlotImplementation::BorderLine:

      type = -2;
      break;

    case gkg::GnuPlotImplementation::AxisLine:

      type = -1;
      break;

    case gkg::GnuPlotImplementation::RunLine1:

      type = 0;
      break;

    case gkg::GnuPlotImplementation::RunLine2:

      type = 1;
      break;

    case gkg::GnuPlotImplementation::RunLine3:

      type = 2;
      break;

    case gkg::GnuPlotImplementation::RunLine4:

      type = 3;
      break;

    case gkg::GnuPlotImplementation::RunLine5:

      type = 4;
      break;

    case gkg::GnuPlotImplementation::RunLine6:

      type = 5;
      break;

    case gkg::GnuPlotImplementation::RunLine7:

      type = 6;
      break;

  }
  fprintf( _x11Ipc, "L%04" PRId32 "\n", type ); 

}


void gkg::GnuPlotImplementation::setLeftJustify()
{

  _justifyType = gkg::GnuPlotImplementation::LeftJustify;
  fprintf( _x11Ipc, "J%04" PRId32 "\n", 0 ); 

}


void gkg::GnuPlotImplementation::setCenterJustify()
{

  _justifyType = gkg::GnuPlotImplementation::CenterJustify;
  fprintf( _x11Ipc, "J%04" PRId32 "\n", 1 ); 

}


void gkg::GnuPlotImplementation::setRightJustify()
{

  _justifyType = gkg::GnuPlotImplementation::RightJustify;
  fprintf( _x11Ipc, "J%04" PRId32 "\n", 2 ); 

}


std::string gkg::GnuPlotImplementation::getStaticName()
{

  return "GnuPlot";

}



#undef X11_XMAX
#undef X11_YMAX
