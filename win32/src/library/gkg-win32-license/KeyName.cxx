#include <gkg-win32-license/KeyName.h>
#include <gkg-win32-license/KeyNameDef.h>
#include <gkg-core-exception/Exception.h>


gkg::KeyName::KeyName()
            : gkg::Singleton< gkg::KeyName >()
{

  initialize();

}


std::list< std::string > gkg::KeyName::getApplicationNames() const
{

  try
  {
  
    std::list< std::string > applicationNames;
    std::map< std::string, uint64_t >::const_iterator 
      a = _keys.begin(),
      ae = _keys.end();
      
    while ( a != ae )
    {
    
      applicationNames.push_back( a->first );
      ++a;
    
    }
    
    return applicationNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::KeyName::getApplicationNames() const" );

}


uint64_t gkg::KeyName::getId( std::string name )
{

  uint64_t id = 0;
  std::map< std::string, uint64_t >::const_iterator it = _keys.find( name );

  if ( it != _keys.end() )
  {
  
    id = it->second;
    
  }

  return id;
    
}


#include <gkg-win32-license/KeyNameImpl.h>
