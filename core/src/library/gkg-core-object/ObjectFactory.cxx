#include <gkg-core-object/ObjectFactory_i.h>


gkg::ObjectFactory::ObjectFactory()
{
}


void gkg::ObjectFactory::setId( const std::string& name )
{

  _id = name;

}


void gkg::ObjectFactory::registerCreatorIds(
                                           const std::string& outType,
                                           const std::string& inType,
                                           gkg::ObjectFactory::Creator creator )
{

  getCreators()[ outType ][ inType ] = creator;

}


std::map< std::string, std::map< std::string, gkg::ObjectFactory::Creator > >&
gkg::ObjectFactory::getCreators()
{

  static std::map< std::string,
                  std::map< std::string,
                            gkg::ObjectFactory::Creator > > creators;
  return creators;

}
