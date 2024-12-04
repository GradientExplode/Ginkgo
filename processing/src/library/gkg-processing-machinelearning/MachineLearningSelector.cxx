#include <gkg-processing-machinelearning/MachineLearningSelector.h>
#include <gkg-core-exception/Exception.h>


#define DEFAULT_MACHINE_LEARNING_IMPLEMENTATION_FACTORY  "libsvm"


gkg::MachineLearningSelector::MachineLearningSelector()
                             : gkg::Singleton< gkg::MachineLearningSelector >(),
                               _implementationFactory( 0 ),
                               _ready( false )
{
}


gkg::MachineLearningSelector::~MachineLearningSelector()
{

  _accessors.clear();

}


gkg::MachineLearningImplementationFactory* 
gkg::MachineLearningSelector::getImplementationFactory()
{

  try
  {

    if ( !_ready )
    {

      select( DEFAULT_MACHINE_LEARNING_IMPLEMENTATION_FACTORY );

    }

    return _implementationFactory;

  }
  GKG_CATCH( "gkg::MachineLearningImplementationFactory* "
             "gkg::MachineLearningSelector::getImplementationFactory()" );

}


void gkg::MachineLearningSelector::registerFactory(
                               const std::string& name,
                               gkg::MachineLearningSelector::Accessor accessor )
{

  try
  {

    if ( accessor )
    {

      std::map< std::string, 
                gkg::MachineLearningSelector::Accessor >::const_iterator
        a = _accessors.find( name );

      if ( a == _accessors.end() )
      {

        _accessors[ name ] = accessor;

      }
      else
      {

        throw std::runtime_error(
                std::string( "machine learning implementation factory" ) +
                             " '" + name + " already registered" );

      }

    }

  }
  GKG_CATCH( "void gkg::MachineLearningSelector::registerFactory( "
             "const std::string& name, "
             "gkg::MachineLearningSelector::Accessor accessor )" );

}


void gkg::MachineLearningSelector::select( const std::string& name )
{

  try
  {

    std::map< std::string,
              gkg::MachineLearningSelector::Accessor >::const_iterator
      a = _accessors.find( name );

    if ( a != _accessors.end() )
    {

      _implementationFactory = &( *a->second )();
      _ready = true;

    }
    else
    {

      throw std::runtime_error(
             std::string( "unable to find machine learning implementation" ) +
             " factory '" + name + "'" );

    }

  }
  GKG_CATCH( "void gkg::MachineLearningSelector::select( "
             "const std::string& name )" );

}


#undef DEFAULT_MACHINE_LEARNING_IMPLEMENTATION_FACTORY
