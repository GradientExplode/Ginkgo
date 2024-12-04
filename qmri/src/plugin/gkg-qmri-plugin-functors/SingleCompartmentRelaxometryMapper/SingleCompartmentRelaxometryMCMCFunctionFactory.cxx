#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                               SingleCompartmentRelaxometryMCMCFunctionFactory()
{
}


gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                              ~SingleCompartmentRelaxometryMCMCFunctionFactory()
{
}


bool gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::registerCreator(
  const std::string& name,
  gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                 RelaxometryMCMCFunctionCreator relaxometryMCMCFunctionCreator )
{
  try
  {

    if ( relaxometryMCMCFunctionCreator )
    {

      std::map< std::string,
                gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                                RelaxometryMCMCFunctionCreator >::const_iterator
        c = _relaxometryMCMCFunctionCreators.find( name );

      if ( c == _relaxometryMCMCFunctionCreators.end() )
      {

        _relaxometryMCMCFunctionCreators[ name ] =
                                                 relaxometryMCMCFunctionCreator;
        return true;

      }

#ifdef GKG_DEBUG

      else
      {

        std::cerr << "gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::"
                  << "registerCreator: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

#endif

    }
    return false;

  }
  GKG_CATCH( "bool gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::"
             "registerCreator( const std::string& name, "
             "gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::"
             "RelaxometryMCMCFunctionCreator "
             "relaxometryMCMCFunctionCreator )" );

}


gkg::SingleCompartmentRelaxometryMCMCFunction*
gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::create(
          const std::string& name,
          const std::vector< int32_t >& measurementCounts,
          const std::vector< double >& noiseStandardDeviations,
          const gkg::RCPointer< gkg::AcquisitionParameterSet >&
                                                 acquisitionParameterSet ) const
{

  try
  {

    std::map< std::string,
              gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                               RelaxometryMCMCFunctionCreator >::const_iterator
      c = _relaxometryMCMCFunctionCreators.find( name );
    if ( c != _relaxometryMCMCFunctionCreators.end() )
    {

      return ( *c->second )( measurementCounts,
                             noiseStandardDeviations,
                             acquisitionParameterSet );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::SingleCompartmentRelaxometryMCMCFunction* "
             "gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::create( "
             "const std::string& name, "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::vector< double >& noiseStandardDeviations, "
             "const gkg::RCPointer< gkg::AcquisitionParameterSet >& "
             "acquisitionParameterSet ) const" );

}


bool 
gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                     hasRelaxometryMCMCFunction( const std::string& name ) const
{

  try
  {

    std::map< std::string,
              gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                                RelaxometryMCMCFunctionCreator >::const_iterator
      c = _relaxometryMCMCFunctionCreators.find( name );
    if ( c != _relaxometryMCMCFunctionCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool "
             "gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::"
             "hasRelaxometryMCMCFunction( const std::string& name ) const" );

}


std::list< std::string >
gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    std::map< std::string,
              gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::
                                RelaxometryMCMCFunctionCreator >::const_iterator
      c = _relaxometryMCMCFunctionCreators.begin(),
      ce = _relaxometryMCMCFunctionCreators.end();
    while ( c != ce )
    {

      names.push_back( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::getNames() "
             "const" );

}
