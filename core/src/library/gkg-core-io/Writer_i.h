#ifndef _gkg_core_io_Writer_i_h_
#define _gkg_core_io_Writer_i_h_


#include <gkg-core-io/Writer.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/IOException.h>
#include <gkg-core-exception/Exception.h>
#include <set>


template < class O >
inline
std::string gkg::Writer::writeHeader( const std::string& name,
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

        diskFormat->writeHeader( name, object );
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

        diskFormat->writeHeader( name, object );
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

        diskFormat->writeHeader( name, object );
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
    std::string( "template < class O > std::string gkg::Writer::writeHeader( "
                 "const std::string& name, O& object, const std::string& format"
                 " ) const: " ) + message,
    std::string( "'" ) + name + "' : no matching format" );
  return "";

}


template < class O >
inline
std::string gkg::Writer::write( const std::string& name,
                                O& object,
                                bool ascii,
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

        diskFormat->write( name, object, ascii );
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

        diskFormat->write( name, object, ascii );
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

        diskFormat->write( name, object, ascii );
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
    std::string( "template < class O > std::string gkg::Writer::write( const "
                 "std::string& name, O& object, bool ascii, const std::string& "
                 "format ) const: " ) + message,
    std::string( "'" ) + name + "' : no matching format" );
  return "";

}


template < class O >
inline
std::string
gkg::Writer::preparePartialBinaryWrite( const std::string& name,
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

        diskFormat->preparePartialBinaryWrite( name, proxy );
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

        diskFormat->preparePartialBinaryWrite( name, proxy );
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

        diskFormat->preparePartialBinaryWrite( name, proxy );
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
    std::string( "template < class O > std::string gkg::Writer::"
                 "preparePartialBinaryWrite( const std::string& name, "
                 "gkg::HeaderedObject& proxy, const std::string& format ) "
                 "const: " ) + message,
    std::string( "'" ) + name + "' : no matching format" );
  return "";

}


template < class O >
inline
std::string gkg::Writer::partialBinaryWrite(
                                const gkg::HeaderedObject& proxy,
                                const gkg::GenericObject& subpart,
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

        diskFormat->partialBinaryWrite( proxy, subpart, regionInformation );
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

        diskFormat->partialBinaryWrite( proxy, subpart, regionInformation );
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

        diskFormat->partialBinaryWrite( proxy, subpart, regionInformation );
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
    std::string( "template < class O > std::string gkg::Writer::"
                 "partialBinaryWrite( "
                 "const gkg::HeaderedObejct& proxy, "
                 "const gkg::GenericObject& subpart, "
                 "const gkg::RegionInformation& regionInformation, "
                 "const std::string& format ) const: " ) + message,
    std::string( "'" ) + proxyFileName + "' : no matching format" );
  return "";

}


#endif
