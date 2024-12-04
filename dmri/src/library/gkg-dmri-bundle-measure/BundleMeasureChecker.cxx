#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-core-exception/Exception.h>


gkg::BundleMeasureChecker::BundleMeasureChecker()
{
}


gkg::BundleMeasureChecker::~BundleMeasureChecker()
{
}


std::string gkg::BundleMeasureChecker::getNameListHelp() const
{

  try
  {

    std::string nameListHelp;
    std::map< std::string, int32_t >::const_iterator
      c = _stringParameterCounts.begin(),
      ce = _stringParameterCounts.end();
    while ( c != ce )
    {

      nameListHelp += "- " + c->first + "\n";
      ++ c;

    }
    return nameListHelp;

  }
  GKG_CATCH( "std::string "
             "gkg::BundleMeasureChecker::getNameListHelp() const" );

}


std::set< std::string > gkg::BundleMeasureChecker::getNames() const
{

  try
  {

    std::set< std::string > names;
    std::map< std::string, int32_t >::const_iterator
      c = _stringParameterCounts.begin(),
      ce = _stringParameterCounts.end();
    while ( c != ce )
    {

      names.insert( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::BundleMeasureChecker::getNames() const" );

}


const std::string& gkg::BundleMeasureChecker::getStringParameterHelp(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, std::string >::const_iterator
      h = _stringParameterHelps.find( name );

    if ( h == _stringParameterHelps.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return h->second;

  }
  GKG_CATCH( "int32_t gkg::BundleMeasureChecker::getStringParameterHelp( "
             "const std::string& name ) const" );

}


const std::string& gkg::BundleMeasureChecker::getScalarParameterHelp(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, std::string >::const_iterator
      h = _scalarParameterHelps.find( name );

    if ( h == _scalarParameterHelps.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return h->second;

  }
  GKG_CATCH( "int32_t gkg::BundleMeasureChecker::getScalarParameterHelp( "
             "const std::string& name ) const" );

}


int32_t gkg::BundleMeasureChecker::getStringParameterCount(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, int32_t >::const_iterator
      c = _stringParameterCounts.find( name );

    if ( c == _stringParameterCounts.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return c->second;

  }
  GKG_CATCH( "int32_t gkg::BundleMeasureChecker::getStringParameterCount( "
             "const std::string& name ) const" );

}


int32_t gkg::BundleMeasureChecker::getScalarParameterCount(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, int32_t >::const_iterator
      c = _scalarParameterCounts.find( name );

    if ( c == _scalarParameterCounts.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return c->second;

  }
  GKG_CATCH( "int32_t gkg::BundleMeasureChecker::getScalarParameterCount( "
             "const std::string& name ) const" );

}


void gkg::BundleMeasureChecker::doSanityChecks(
                           const std::string& name,
                           const std::vector< std::string >& stringParameters,
                           const std::vector< double >& scalarParameters ) const
{

  try
  {

    std::map< std::string,
              gkg::BundleMeasureChecker::SanityCheckFunctor >::const_iterator
      f = _sanityCheckFunctors.find( name );

    if ( f == _sanityCheckFunctors.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    // in case a null sanity check functor has been registered
    if ( !f->second )
    {

      return;

    }
    ( f->second )( stringParameters, scalarParameters );

  }
  GKG_CATCH( "void gkg::BundleMeasureChecker::doSanityChecks( "
             "const std::string& name, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters ) const" );

}


void gkg::BundleMeasureChecker::addChecker(
              const std::string& name,
              const std::string& stringParameterHelp,
              const std::string& scalarParameterHelp,
              int32_t stringParameterCount,
              int32_t scalarParameterCount,
              gkg::BundleMeasureChecker::SanityCheckFunctor sanityCheckFunctor )
{

  try
  {

    std::map< std::string, int32_t >::const_iterator
      c = _stringParameterCounts.find( name );
    if ( c != _stringParameterCounts.end() )
    {

      throw std::runtime_error( "measure name already present" );

    }

    _stringParameterHelps[ name ] = stringParameterHelp;
    _scalarParameterHelps[ name ] = scalarParameterHelp;
    _stringParameterCounts[ name ] = stringParameterCount;
    _scalarParameterCounts[ name ] = scalarParameterCount;
    _sanityCheckFunctors[ name ] = sanityCheckFunctor;

  }
  GKG_CATCH( "void gkg::BundleMeasureChecker::addChecker( "
             "const std::string& name, "
             "const std::string& stringParameterHelp, "
             "const std::string& scalarParameterHelp, "
             "int32_t stringParameterCount, "
             "int32_t scalarParameterCount, "
             "gkg::BundleMeasureChecker::SanityCheckFunctor "
             "sanityCheckFunctor )" );

}

