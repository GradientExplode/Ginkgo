#include <gkg-processing-process/Process.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-core-exception/DataTypeFormatException.h>
#include <gkg-core-exception/Exception.h>


gkg::Process::Process( const std::string& wantedObjectType )
             : _wantedObjectType( wantedObjectType )
{
}


gkg::Process::~Process()
{
}


void gkg::Process::execute( const std::string& name )
{

  try
  {

    gkg::AnalyzedObject analyzedObject;
    std::string format;
    gkg::DiskFormatAnalyzer::getInstance().analyze( name,
                                                    format,
                                                    analyzedObject,
                                                    _wantedObjectType );
    execute( name, analyzedObject, format );

  }
  GKG_CATCH_WITH_COMMENT(
    "void gkg::Process::execute( const std::string& name )",
    "cannot analyze '" + name + "' file; " );

}


void gkg::Process::execute( const std::string& name,
                            const gkg::AnalyzedObject& analyzedObject,
                            const std::string& format )
{

  try
  {

    std::string objectType = analyzedObject.getObjectType();
    std::string searchType = "";

    if ( objectType == "MeshMap" )
    {

      try
      {

        searchType = analyzedObject.getRankType();

      }
      catch( std::exception& )
      {

        throw std::runtime_error( "no rank type found" );

      }

    }
    else
    {

      try
      {

        searchType = analyzedObject.getItemType();

      }
      catch( std::exception& )
      {

        try
        {

          searchType = analyzedObject.getLabelType();

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "no item or label type found" );

        }

      }

    }

    std::map< std::string,
              std::map< std::string, gkg::Process::Function > >::const_iterator
      m = _functions.find( objectType );
    if ( m != _functions.end() )
    {

      std::map< std::string, gkg::Process::Function >::const_iterator
        f = ( *m ).second.find( searchType );
      if ( f != ( *m ).second.end() )
      {

        ( *f ).second( *this, name, analyzedObject, format );
        return;

      }
      else
      {

        throw gkg::DataTypeFormatException(
              std::string( "unsupported item, label or rank type '" ) +
              searchType +
              "' with object type '" + objectType +
              "' and format '" + format + "'", name );

      }

    }
    else
    {

      throw gkg::DataTypeFormatException(
              std::string( "unsupported object type '" ) +
              objectType +
              "' with format '" + format + "'", name );

    }

  }
  GKG_CATCH_WITH_COMMENT(
    "void gkg::Process::execute( const std::string& name, "
    "const gkg::AnalyzedObject& analyzedObject, "
    "const std::string& format )",
    "cannot analyze '" + name + "' file; " );

}


void gkg::Process::registerProcess( const std::string& objectType,
                                    const std::string& itemType,
                                    gkg::Process::Function function )
{

  _functions[ objectType ][ itemType ] = function;

}
