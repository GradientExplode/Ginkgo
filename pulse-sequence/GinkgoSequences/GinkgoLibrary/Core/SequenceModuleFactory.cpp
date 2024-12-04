#include <Core/SequenceModuleFactory.h>



gkg::SequenceModuleFactory::SequenceModuleFactory()
{
}


gkg::SequenceModuleFactory::~SequenceModuleFactory()
{
}


bool gkg::SequenceModuleFactory::registerSequenceModuleCreator(
                                   const std::string& sequenceModuleTypeName,
                                   gkg::SequenceModuleFactory::Creator creator )
{

  if ( creator )
  {

    std::map< std::string, gkg::SequenceModuleFactory::Creator >::const_iterator
      c = _creators.find( sequenceModuleTypeName );

    if ( c == _creators.end() )
    {

      _creators[ sequenceModuleTypeName ] = creator;
      return true;

    }
    else
    {

      std::cerr << "gkg::SequenceModuleFactory::"
                << "registerSequenceModuleCreator: " << sequenceModuleTypeName
                << " already exists"
                << std::endl;
      return false;

    }

  }
  return false;

}


gkg::RCPointer< gkg::SequenceModule >
gkg::SequenceModuleFactory::create(
           const std::string& sequenceModuleTypeName,
           const std::vector< double >& doubleParameters,
           const std::vector< std::string >& stringParameters,
           const std::map< std::string, gkg::RCPointer< gkg::SequenceModule > >&
                                                   otherSequenceModules ) const
{

  std::map< std::string, Creator >::const_iterator
    c = _creators.find( sequenceModuleTypeName );

  gkg::RCPointer< gkg::SequenceModule > sequenceModulePtr;
  if ( c != _creators.end() )
  {

    sequenceModulePtr.reset( ( *c->second )( doubleParameters,
                                             stringParameters,
                                             otherSequenceModules ) );

  }

  return sequenceModulePtr;

}


std::set< std::string > 
gkg::SequenceModuleFactory::getSequenceModuleTypeNames() const
{

  std::set< std::string > sequenceModuleTypeNames;

  std::map< std::string, gkg::SequenceModuleFactory::Creator >::const_iterator
    c = _creators.begin(),
    ce = _creators.end();

  while ( c != ce )
  {

    sequenceModuleTypeNames.insert( c->first );
    ++ c;

  }

  return sequenceModuleTypeNames;

}
