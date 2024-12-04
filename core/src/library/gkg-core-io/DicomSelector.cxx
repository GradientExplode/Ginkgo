#include <gkg-core-io/DicomSelector.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


#define DEFAULT_DICOM_IMPLEMENTATION_FACTORY  "dcmtk"


gkg::DicomSelector::DicomSelector()
                   : _implementationFactory( 0 ),
                     _ready( false )
{
}


gkg::DicomSelector::~DicomSelector()
{

  _accessors.clear();

}


gkg::DicomImplementationFactory*
gkg::DicomSelector::getImplementationFactory()
{

  try
  {

    if ( !_ready )
    {

      select( DEFAULT_DICOM_IMPLEMENTATION_FACTORY );

    }
    return _implementationFactory;

  }
  GKG_CATCH( "gkg::DicomImplementationFactory* "
             "gkg::DicomSelector::getImplementationFactory()" );

}


void gkg::DicomSelector::registerFactory(
                                         const std::string& name,
                                         gkg::DicomSelector::Accessor accessor )
{

  try
  {

    if ( accessor )
    {

      std::map< std::string,
                gkg::DicomSelector::Accessor >:: const_iterator
        a = _accessors.find( name );
      if ( a == _accessors.end() )
      {

        _accessors[ name ] = accessor;

      }
      else
      {

        throw std::runtime_error( 
                                 std::string( "dicom implementation factory" ) +
                                 " '" + name + " already registered" );

      }

    }

  }
  GKG_CATCH( "void gkg::DicomSelector::registerFactory( "
             "const std::string& name, "
             "gkg::DicomSelector::Accessor accessor )" );

}


void gkg::DicomSelector::select( const std::string& name )
{

  try
  {

    std::map< std::string, gkg::DicomSelector::Accessor >::const_iterator
      a = _accessors.find( name );

    if ( a != _accessors.end() )
    {

      _implementationFactory = &( *a->second )();

    }
    else
    {

      throw std::runtime_error(
                          std::string( "unable to find dicom implementation" ) +
                          " factory '" + name + "'" );

    }

  }
  GKG_CATCH( "void gkg::DicomSelector::select( "
             "const std::string& name )" );

}


#undef DEFAULT_DICOM_IMPLEMENTATION_FACTORY
