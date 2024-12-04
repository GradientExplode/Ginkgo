#ifndef _gkg_core_io_Reader_i_h_
#define _gkg_core_io_Reader_i_h_


#include <gkg-core-io/Reader.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/IOException.h>
#include <gkg-core-exception/Exception.h>
#include <set>


template < class O >
inline
std::string gkg::Reader::readHeader( const std::string& name,
                                     O& object,
                                     const std::string& format ) const
{

  int32_t priority = 0;
  int32_t type = -1;
  std::string message;
  std::set< std::string > triedDiskFormats;

  // if format is given, try to get it
  if ( !format.empty() )
  {

    gkg::TypedDiskFormat< O >* diskFormat =
      gkg::DiskFormatFactory< O >::getInstance().getDiskFormat( format );
    if ( diskFormat )
    {

      try
      {

        diskFormat->readHeader( name, object );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message, 5 );

      }
      triedDiskFormats.insert( format );

    }

  }

  // try to find disk format that matches name extension and that has not
  // still been tried
  typename std::map< std::string, gkg::TypedDiskFormat< O >* >::const_iterator
    f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin(),
    fe = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().end();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() ) &&
         ( diskFormat->hasMatchingExtension( name ) ) )
    {

      try
      {

        diskFormat->readHeader( name, object );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // try every format that has not still been tried
  f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() )
    {

      try
      {

        diskFormat->readHeader( name, object );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // no disk format succeeded, so launch an exception
  gkg::IOException::launch(
    type,
    std::string( "template < class O > std::string gkg::Reader::readHeader( "
                 "const std::string& name, O& object, const std::string& "
                 "format ) const: " ) + message,
    std::string( "'" ) + name + "' : no matching format" );
  return "";

}


template < class O >
inline
std::string gkg::Reader::read( const std::string& name,
                               O& object,
                               const std::string& format ) const
{

  int32_t priority = 0;
  int32_t type = -1;
  std::string message;
  std::set< std::string > triedDiskFormats;

  // if format is given, try to get it
  if ( !format.empty() )
  {

    gkg::TypedDiskFormat< O >* diskFormat =
      gkg::DiskFormatFactory< O >::getInstance().getDiskFormat( format );
    if ( diskFormat )
    {

      try
      {

        diskFormat->read( name, object );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message, 5 );

      }
      triedDiskFormats.insert( format );

    }

  }

  // try to find disk format that matches name extension and that has not
  // still been tried
  typename std::map< std::string, gkg::TypedDiskFormat< O >* >::const_iterator
    f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin(),
    fe = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().end();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() ) &&
         ( diskFormat->hasMatchingExtension( name ) ) )
    {

      try
      {

        diskFormat->read( name, object );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // try every format that has not still been tried
  f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() )
    {

      try
      {

        diskFormat->read( name, object );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // no disk format succeeded, so launch an exception
  gkg::IOException::launch(
    type,
    std::string( "template < class O > std::string gkg::Reader::read( "
                 "const std::string& name, O& object, const std::string& format"
                 " ) const: " ) + message,
    std::string( "'" ) + name + "' : no matching format" );
  return "";

}


template < class O >
inline
std::string
gkg::Reader::preparePartialBinaryRead( const std::string& name,
                                       gkg::HeaderedObject& proxy,
                                       const std::string& format ) const
{

  int32_t priority = 0;
  int32_t type = -1;
  std::string message;
  std::set< std::string > triedDiskFormats;

  // if format is given, try to get it
  if ( !format.empty() )
  {

    gkg::TypedDiskFormat< O >* diskFormat =
      gkg::DiskFormatFactory< O >::getInstance().getDiskFormat( format );
    if ( diskFormat )
    {

      try
      {

        diskFormat->preparePartialBinaryRead( name, proxy );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message, 5 );

      }
      triedDiskFormats.insert( format );

    }

  }

  // try to find disk format that matches name extension and that has not
  // still been tried
  typename std::map< std::string, gkg::TypedDiskFormat< O >* >::const_iterator
    f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin(),
    fe = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().end();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() ) &&
         ( diskFormat->hasMatchingExtension( name ) ) )
    {

      try
      {

        diskFormat->preparePartialBinaryRead( name, proxy );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // try every format that has not still been tried
  f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() )
    {

      try
      {

        diskFormat->preparePartialBinaryRead( name, proxy );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // no disk format succeeded, so launch an exception
  gkg::IOException::launch(
    type,
    std::string( "template < class O > std::string gkg::Reader::"
                 "preparePartialBinaryRead( const std::string& name, "
                 "gkg::HeaderedObject& proxy, const std::string& format ) "
                 "const: " ) + message,
    std::string( "'" ) + name + "' : no matching format" );
  return "";

}


template < class O >
inline
std::string gkg::Reader::partialBinaryRead(
                                const gkg::HeaderedObject& proxy,
                                gkg::GenericObject& subpart,
                                const gkg::RegionInformation& regionInformation,
                                const std::string& format ) const
{

  int32_t priority = 0;
  int32_t type = -1;
  std::string message;
  std::set< std::string > triedDiskFormats;

  // checking that the proxy has got a file name
  std::string proxyFileName;
  try
  {

    proxy.getHeader().getAttribute( "proxyFileName", proxyFileName );

  }
  catch ( std::exception& e )
  {

    gkg::IOException::keepPriority( e, priority, type, message );

  }

  // if format is given, try to get it
  if ( !format.empty() )
  {

    gkg::TypedDiskFormat< O >* diskFormat =
      gkg::DiskFormatFactory< O >::getInstance().getDiskFormat( format );
    if ( diskFormat )
    {

      try
      {

        diskFormat->partialBinaryRead( proxy, subpart, regionInformation );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message, 5 );

      }
      triedDiskFormats.insert( format );

    }

  }

  // try to find disk format that matches name extension and that has not
  // still been tried
  typename std::map< std::string, gkg::TypedDiskFormat< O >* >::const_iterator
    f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin(),
    fe = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().end();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() ) &&
         ( diskFormat->hasMatchingExtension( proxyFileName ) ) )
    {

      try
      {

        diskFormat->partialBinaryRead( proxy, subpart, regionInformation );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // try every format that has not still been tried
  f = gkg::DiskFormatFactory< O >::getInstance().getDiskFormats().begin();
  while ( f != fe )
  {

    gkg::TypedDiskFormat< O >* diskFormat = ( *f ).second;
    if ( triedDiskFormats.find( ( *f ).first ) == triedDiskFormats.end() )
    {

      try
      {

        diskFormat->partialBinaryRead( proxy, subpart, regionInformation );
        return diskFormat->getName();

      }
      catch ( std::exception& e )
      {

        gkg::IOException::keepPriority( e, priority, type, message );

      }
      triedDiskFormats.insert( (*f).first );

    }
    ++ f;

  }

  // no disk format succeeded, so launch an exception
  gkg::IOException::launch(
    type,
    std::string( "template < class O > std::string gkg::Reader::"
                 "partialBinaryRead( "
                 "const gkg::HeaderedObject& proxy, "
                 "gkg::GenericObject& subpart, "
                 "const gkg::RegionInformation& regionInformation, "
                 "const std::string& format ) const: " ) + message,
    std::string( "'" ) + proxyFileName + "' : no matching format" );
  return "";

}


#endif
