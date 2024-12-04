#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


#define DEFAULT_NUMERICAL_ANALYSIS_IMPLEMENTATION_FACTORY  "gsl"


gkg::NumericalAnalysisSelector::NumericalAnalysisSelector()
                               : _implementationFactory( NULL ),
                                 _ready( false )
{
}


gkg::NumericalAnalysisSelector::~NumericalAnalysisSelector()
{

  _accessors.clear();

}


gkg::NumericalAnalysisImplementationFactory*
gkg::NumericalAnalysisSelector::getImplementationFactory()
{

  try
  {

    if ( !_ready )
    {

      select( DEFAULT_NUMERICAL_ANALYSIS_IMPLEMENTATION_FACTORY );

    }
    return _implementationFactory;

  }
  GKG_CATCH( "gkg::NumericalAnalysisImplementationFactory* "
             "gkg::NumericalAnalysisSelector::getImplementationFactory()" );

}


void gkg::NumericalAnalysisSelector::registerFactory(
                             const std::string& name,
                             gkg::NumericalAnalysisSelector::Accessor accessor )
{

  try
  {

    if ( accessor )
    {

      std::map< std::string,
                gkg::NumericalAnalysisSelector::Accessor >:: const_iterator
        a = _accessors.find( name );
      if ( a == _accessors.end() )
      {

        _accessors[ name ] = accessor;

      }
      else
      {

        throw std::runtime_error( 
                std::string( "numerical analysis implementation factory" ) +
                " '" + name + " already registered" );

      }

    }

  }
  GKG_CATCH( "void gkg::NumericalAnalysisSelector::registerFactory( "
             "const std::string& name, "
             "gkg::NumericalAnalysisSelector::Accessor accessor )" );

}


void gkg::NumericalAnalysisSelector::select( const std::string& name )
{

  try
  {

    std::map< std::string,
              gkg::NumericalAnalysisSelector::Accessor >::const_iterator
      a = _accessors.find( name );

    if ( a != _accessors.end() )
    {

      _implementationFactory = &( *a->second )();

    }
    else
    {

      throw std::runtime_error(
             std::string( "unable to find numerical analysis implementation" ) +
             " factory '" + name + "'" );

    }

  }
  GKG_CATCH( "void gkg::NumericalAnalysisSelector::select( "
             "const std::string& name )" );

}


#undef DEFAULT_NUMERICAL_ANALYSIS_IMPLEMENTATION_FACTORY
