#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::PlotterFactory::PlotterFactory()
                    : _currentPlotterCreator( 0 ),
                      _defaultPlotterCreator( 0 )
{
}


gkg::PlotterFactory::~PlotterFactory()
{
}


bool gkg::PlotterFactory::registerFactory(
                   const std::string& name,
                   gkg::PlotterFactory::PlotterCreator plotterCreator,
                   bool isDefault )
{

  if ( plotterCreator )
  {

    std::map< std::string,
              gkg::PlotterFactory::PlotterCreator >::const_iterator
      c = _plotterCreators.find( name );

    if ( c == _plotterCreators.end() )
    {

      _plotterCreators[ name ] = plotterCreator;
      if ( isDefault )
      {

        _defaultPlotterCreator = plotterCreator;

      }
      return true;

    }

#ifdef GKG_DEBUG

    else
    {

      std::cerr << "gkg::PlotterFactory::registerFactory: " << name
                << " already exists"
                << std::endl;
      return false;

    }

#endif

  }
  return false;

}


void gkg::PlotterFactory::select( const std::string& name )
{

  try
  {

    std::map< std::string, gkg::PlotterFactory::PlotterCreator >::const_iterator
      c = _plotterCreators.find( name );
    if ( c != _plotterCreators.end() )
    {

      _currentPlotterCreator = c->second;

    }
    else
    {

      throw std::runtime_error( std::string( "'" ) + name + "' not found" );

    }

  }
  GKG_CATCH( "void gkg::PlotterFactory::select( const std::string& name )" );

}


void gkg::PlotterFactory::selectDefault()
{

  try
  {

    if ( _defaultPlotterCreator )
    {

      _currentPlotterCreator = _defaultPlotterCreator;

    }
    else
    {

      throw std::runtime_error( "no default creator" ) ;

    }

  }
  GKG_CATCH( "void gkg::PlotterFactory::selectDefault()" );

}


gkg::CurvePlotterImplementation* 
gkg::PlotterFactory::createCurvePlotterImplementation() const
{

  if ( !_currentPlotterCreator )
  {

    return ( *_defaultPlotterCreator )();

  }
  return ( *_currentPlotterCreator )();

}
