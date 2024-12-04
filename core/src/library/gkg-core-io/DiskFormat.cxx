#include <gkg-core-io/DiskFormat.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/NotOpenException.h>


gkg::DiskFormat::DiskFormat()
{
}


gkg::DiskFormat::~DiskFormat()
{
}


bool gkg::DiskFormat::hasReader() const
{

  return false;

}


bool gkg::DiskFormat::hasWriter() const
{

  return false;

}


std::string 
gkg::DiskFormat::getNameWithoutExtension( const std::string& name ) const
{

  std::set< std::string >::const_iterator e = _extensions.begin(),
                                            ee = _extensions.end();
  while ( e != ee )
  {

    if ( name.length() > e->length() )
    {

       if ( name.substr( name.length() - e->length(), e->length()) == *e )
       {

         return name.substr( 0, name.length() - e->length() );

       }

    }
    ++ e;

  }
  return name;

}


bool 
gkg::DiskFormat::hasMatchingExtension( const std::string& name ) const
{

  if ( _extensions.empty() )
  {

    return true;

  }
  
  std::set< std::string >::const_iterator e = _extensions.begin(),
                                            ee = _extensions.end();
  while ( e != ee )
  {

    if ( name.length() > e->length() )
    {

       if ( name.substr( name.length() - e->length(), e->length()) == *e )
       {

         return true;

       }

    }
    ++ e;

  }
  return false;

}


std::string gkg::DiskFormat::getHeaderExtension() const
{

  return "";

}

void gkg::DiskFormat::readHeader( const std::string& name,
                                  gkg::HeaderedObject& object ) const
{

  gkg::UncommentCounterInputFileStream
    ucis( ( getNameWithoutExtension( name ) + getHeaderExtension() ).c_str() );

  if ( !ucis )
  {

    throw gkg::FileNotFoundException( getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

  }

  try
  {

    gkg::BaseObjectReader baseObjectReader( object.getSyntaxSet(),
                                            object.getReaderHelpers() );
    baseObjectReader.read( ucis, object.getHeader() );
    ucis.close();

  }
  catch ( std::exception& e )
  {

    ucis.close();
    throw std::runtime_error( std::string( "gkg::DiskFormat::readHeader: " ) +
                              e.what() );

  }

}


void gkg::DiskFormat::writeHeader( const std::string& name,
                                   const gkg::HeaderedObject& object ) const
{

  std::ofstream os( ( getNameWithoutExtension( name ) +
                      getHeaderExtension() ).c_str() );

  if ( ! os )
  {

    throw gkg::NotOpenException( getNameWithoutExtension( name ) +
                                 getHeaderExtension() );

  }

  try
  {

    gkg::BaseObjectWriter baseObjectWriter( object.getSyntaxSet(),
                                            object.getWriterHelpers() );
    baseObjectWriter.write( os, object.getHeader() );
    os.close();

  }
  catch ( std::exception& e )
  {

    os.close();
    throw std::runtime_error( std::string( "gkg::DiskFormat::writeHeader: " ) +
                              e.what() );

  }

}


