#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::AcquisitionParameterSetFactory::AcquisitionParameterSetFactory()
{
}


gkg::AcquisitionParameterSetFactory::~AcquisitionParameterSetFactory()
{
}


bool gkg::AcquisitionParameterSetFactory::registerCreator(
           const std::string& name,
           gkg::AcquisitionParameterSetFactory::AcquisitionParameterSetCreator
                                                acquisitionParameterSetCreator )
{
  try
  {

    if ( acquisitionParameterSetCreator )
    {

      std::map< std::string,
                gkg::AcquisitionParameterSetFactory::
                                                  AcquisitionParameterSetCreator
                                                               >::const_iterator
        c = _acquisitionParameterSetCreators.find( name );

      if ( c == _acquisitionParameterSetCreators.end() )
      {

        _acquisitionParameterSetCreators[ name ] =
                                                 acquisitionParameterSetCreator;
        return true;

      }

#ifdef GKG_DEBUG

      else
      {

        std::cerr << "gkg::AcquisitionParameterSetFactory::"
                  << "registerCreator: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

#endif

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AcquisitionParameterSetFactory::"
             "registerCreator( const std::string& name, "
             "gkg::AcquisitionParameterSetFactory::"
             "AcquisitionParameterSetCreator "
             "acquisitionParameterSetCreator )" );

}


gkg::AcquisitionParameterSet*
gkg::AcquisitionParameterSetFactory::create(
                      const std::string& name,
                      const std::vector< int32_t >& measurementCounts,
                      const std::vector< std::string >& stringParameters ) const
{

  try
  {

    std::map< std::string,
              gkg::AcquisitionParameterSetFactory::
                               AcquisitionParameterSetCreator >::const_iterator
      c = _acquisitionParameterSetCreators.find( name );
    if ( c != _acquisitionParameterSetCreators.end() )
    {

      return ( *c->second )( measurementCounts,
                             stringParameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::AcquisitionParameterSet* "
             "gkg::AcquisitionParameterSetFactory::create( "
             "const std::string& name, "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::vector< std::string >& stringParameters ) const" );

}


bool 
gkg::AcquisitionParameterSetFactory::
                     hasAcquisitionParameterSet( const std::string& name ) const
{

  try
  {

    std::map< std::string,
              gkg::AcquisitionParameterSetFactory::
                                AcquisitionParameterSetCreator >::const_iterator
      c = _acquisitionParameterSetCreators.find( name );
    if ( c != _acquisitionParameterSetCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool "
             "gkg::AcquisitionParameterSetFactory::"
             "hasAcquisitionParameterSet( const std::string& name ) const" );

}


std::list< std::string >
gkg::AcquisitionParameterSetFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    std::map< std::string,
              gkg::AcquisitionParameterSetFactory::
                                AcquisitionParameterSetCreator >::const_iterator
      c = _acquisitionParameterSetCreators.begin(),
      ce = _acquisitionParameterSetCreators.end();
    while ( c != ce )
    {

      names.push_back( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::AcquisitionParameterSetFactory::getNames() "
             "const" );

}
