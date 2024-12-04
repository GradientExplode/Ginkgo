#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-core-io/DiskFormat.h>
#include <gkg-core-object/Syntax.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/IOException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <set>

//
// class AnalyzedObject
//

gkg::AnalyzedObject::AnalyzedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "rank_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), false );
  _syntaxSet[ "__generic__" ][ "label_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), false );
  _syntaxSet[ "__generic__" ][ "polygon_size" ] = 
    gkg::Semantic( gkg::TypeOf< uint32_t >::getName(), false );

}


gkg::AnalyzedObject::~AnalyzedObject()
{
}


std::string gkg::AnalyzedObject::getObjectType() const
{

  try
  {

    std::string objectType;
    _header.getAttribute( "object_type", objectType );
    return objectType;

  }
  GKG_CATCH( "std::string gkg::AnalyzedObject::getObjectType() const" );

}


std::string gkg::AnalyzedObject::getItemType() const
{


  try
  {

    std::string itemType;
    _header.getAttribute( "item_type", itemType );
    return itemType;

  }
  GKG_CATCH( "std::string gkg::AnalyzedObject::getItemType() const" );

}


std::string gkg::AnalyzedObject::getRankType() const
{


  try
  {

    std::string rankType;
    _header.getAttribute( "rank_type", rankType );
    return rankType;

  }
  GKG_CATCH( "std::string gkg::AnalyzedObject::getRankType() const" );

}


std::string gkg::AnalyzedObject::getLabelType() const
{


  try
  {

    std::string labelType = "std_string";
    if( _header.hasAttribute( "label_type" ) )
    {

      _header.getAttribute( "label_type", labelType );

    }
    return labelType;

  }
  GKG_CATCH( "std::string gkg::AnalyzedObject::getLabelType() const" );

}


uint32_t gkg::AnalyzedObject::getPolygonSize() const
{


  try
  {

    uint32_t polygonSize;
    _header.getAttribute( "polygon_size", polygonSize );
    return polygonSize;

  }
  GKG_CATCH( "uint32_t gkg::AnalyzedObject::getPolygonSize() const" );

}


//
// class BaseDiskFormatAnalyzer
//


gkg::BaseDiskFormatAnalyzer::BaseDiskFormatAnalyzer()
{
}


gkg::BaseDiskFormatAnalyzer::~BaseDiskFormatAnalyzer()
{
}


//
// class DiskFormatAnalyzer
//


gkg::DiskFormatAnalyzer::DiskFormatAnalyzer()
{
}


gkg::DiskFormatAnalyzer::~DiskFormatAnalyzer()
{
}


void
gkg::DiskFormatAnalyzer::analyze( const std::string& name,
                                  std::string& format,
                                  gkg::AnalyzedObject& analyzedObject,
                                  const std::string& wantedObjectType ) const
{

  int32_t priority = 0;
  int32_t type = -1;
  std::string message;
  std::set< gkg::BaseDiskFormatAnalyzer* > triedBaseDiskFormatAnalyzers;

  // try to find disk format analyzer that matches name extension and that has 
  // not still been tried
  std::multimap< std::string, gkg::BaseDiskFormatAnalyzer* >::const_iterator
    a = _analyzers.begin(), ae = _analyzers.end();
  while ( a != ae )
  {

    gkg::BaseDiskFormatAnalyzer* baseDiskFormatAnalyzer = ( *a ).second;
    if ( ( triedBaseDiskFormatAnalyzers.find( baseDiskFormatAnalyzer ) ==
           triedBaseDiskFormatAnalyzers.end() ) &&
         baseDiskFormatAnalyzer->getDiskFormat().hasMatchingExtension( name ) )
    {

      try
      {

        baseDiskFormatAnalyzer->analyze( name, format, analyzedObject );
        if ( !wantedObjectType.empty() &&
             ( analyzedObject.getObjectType() != wantedObjectType ) )
        {

           throw gkg::ObjectTypeException( name );

        }
        return;

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      if ( baseDiskFormatAnalyzer->getDiskFormat().hasMatchingExtension(
                                                                        name ) )
      {

        triedBaseDiskFormatAnalyzers.insert( baseDiskFormatAnalyzer );

      }

    }
    ++ a;

  }


  // try every analyzer
  a = _analyzers.begin();
  while ( a != ae )
  {

    gkg::BaseDiskFormatAnalyzer* baseDiskFormatAnalyzer = ( *a ).second;
    if ( triedBaseDiskFormatAnalyzers.find( baseDiskFormatAnalyzer ) ==
         triedBaseDiskFormatAnalyzers.end() )
    {

      try
      {

        baseDiskFormatAnalyzer->analyze( name, format, analyzedObject );
        if ( !wantedObjectType.empty() &&
             ( analyzedObject.getObjectType() != wantedObjectType ) )
        {

           throw gkg::ObjectTypeException( name );

        }
        return;

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedBaseDiskFormatAnalyzers.insert( baseDiskFormatAnalyzer );

    }
    ++ a;

  }

  // no disk format analyzer succeeded, so launch an exception
  gkg::IOException::launch(
              type,
              std::string( "void gkg::DiskFormatAnalyzer::analyze: " )+ message,
              std::string( "'" ) + name +
              "' : no matching format" );

}


void gkg::DiskFormatAnalyzer::registerAnalyzer(
                                   const std::string& format,
                                   gkg::BaseDiskFormatAnalyzer* analyzer )
{

  if ( analyzer )
  {

    _analyzers.insert( std::pair< std::string,
                                  gkg::BaseDiskFormatAnalyzer* >( format,
                                                                  analyzer ) );

  }

}
