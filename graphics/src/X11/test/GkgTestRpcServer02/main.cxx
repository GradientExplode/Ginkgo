// RPC inter-process communication channel : receiver
// This example is a plotter that receive some instructions from 
// GkgTestRpcClient02;
// Workflow:
// - open a terminal, and launch GkgTestRpcServer02
// - open a new terminal, and launch GkgTestRpcClient02, and click on <Enter>
//   to see new data plotted.


#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-dispatcher/IOCallback.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include "Graph.h"
#include "Rpc.h"
#include <cstring>


static gkg::PropertyData properties[] =
{

  { "*plot_color", "black" },
  { "*mark_color", "black" },
  { "*axis_color", "black" },
  { "*tick_color", "black" },
  { "*background", "wheat" },
  { 0, 0 }

};


static gkg::OptionDescription options[] =
{

  { "-plot", "*plot_color", gkg::OptionDescription::OptionValueNext, 0 },
  { "-mark", "*mark_color", gkg::OptionDescription::OptionValueNext, 0 },
  { "-axis", "*axis_color", gkg::OptionDescription::OptionValueNext, 0 },
  { "-tick", "*tick_color", gkg::OptionDescription::OptionValueNext, 0 },
  { 0, 0, gkg::OptionDescription::OptionNone, 0 }

};


Graph* graph = 0;


void Callback( char* msg )
{

  float x, y;
  char header[ 80 ];
  if ( strcmp( msg, "exit" ) == 0 )
  {

    gkg::Session::getInstance().quit();

  }
  else
  {

    sscanf( msg, "%s %f %f ", header, &x, &y );
    graph->addPoint( x, y );

  } 

}


int main( int argc, char* argv[] ) 
{

  std::string symbol = "FOO";
  Service* service = new Service( 8001 );

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestRpcServer02", argc, argv, options, properties );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  graph = new Graph( 300.0, 300.0, 0.0, 6.0, 92.0, 94.0,
                     widgetFactory->getBackground(), symbol );
  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( graph );

  int result = session.runWindow( aWindow );

  delete service;

  return result;

}
