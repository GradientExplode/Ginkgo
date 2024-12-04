#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>


gkg::StrategyFactory::StrategyFactory()
                    : gkg::Singleton< gkg::StrategyFactory >()
{
}


gkg::StrategyFactory::~StrategyFactory()
{

  std::map< gkg::VoiStrategy::StgyType, gkg::VoiStrategy* >::iterator
    s = _strategies.begin(),
    se = _strategies.end();

  while ( s != se )
  {

    delete s->second;
    ++s;

  }

}


bool gkg::StrategyFactory::add( gkg::VoiStrategy* strategy )
{

  if ( strategy )
  {

    gkg::VoiStrategy::StgyType type = strategy->type();
    std::map< gkg::VoiStrategy::StgyType, gkg::VoiStrategy* >::iterator
      s = _strategies.find( type );

    if ( s == _strategies.end() )
    {

      _strategies.insert( std::make_pair( type, strategy ) );

      return true;

    }

  }

  return false;

}


gkg::VoiStrategy* gkg::StrategyFactory::getStrategy( 
                                               gkg::VoiStrategy::StgyType type )
{

  std::map< gkg::VoiStrategy::StgyType, gkg::VoiStrategy* >::iterator
    s = _strategies.find( type );

  if ( s != _strategies.end() )
  {

    return s->second;

  }

  return 0;

}
