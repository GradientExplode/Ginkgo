#ifndef _gkg_core_io_TypedDiskFormat_i_h_
#define _gkg_core_io_TypedDiskFormat_i_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-exception/NoReaderException.h>
#include <gkg-core-exception/NoWriterException.h>
#include <gkg-core-exception/NoPartialReaderException.h>
#include <gkg-core-exception/NoPartialWriterException.h>
#include <gkg-core-object/TypedObject_i.h>


template < class O >
inline
gkg::TypedDiskFormat< O >::TypedDiskFormat()
                          : gkg::DiskFormat()
{
}


template < class O >
inline
gkg::TypedDiskFormat< O >::~TypedDiskFormat()
{
}


template < class O >
inline
void gkg::TypedDiskFormat< O >::read( const std::string& name,
                                      O& /*object*/ ) const
{

  throw gkg::NoReaderException( getName(), name );

}


template < class O >
inline
void gkg::TypedDiskFormat< O >::write( const std::string& name,
                                       O& /*object*/,
                                       bool /*ascii*/ ) const
{


  throw gkg::NoWriterException( getName(), name );

}


template < class O >
inline
void gkg::TypedDiskFormat< O >::preparePartialBinaryRead(
                                         const std::string& name,
                                         gkg::HeaderedObject& /*proxy*/ ) const
{

  throw gkg::NoPartialReaderException( getName(), name );

}


template < class O >
inline
void gkg::TypedDiskFormat< O >::partialBinaryRead(
                     const gkg::HeaderedObject& proxy,
                     gkg::GenericObject& /*subpart*/,
                     const gkg::RegionInformation& /*regionInformation*/ ) const
{

  std::string name = "";
  if ( proxy.getHeader().hasAttribute( "proxyName" ) )
  {

    proxy.getHeader().getAttribute( "proxyName", name );

  }
  throw gkg::NoPartialReaderException( getName(), name );

}


template < class O >
inline
void gkg::TypedDiskFormat< O >::preparePartialBinaryWrite(
                                         const std::string& name,
                                         gkg::HeaderedObject& /*proxy*/ ) const
{

  throw gkg::NoPartialWriterException( getName(), name );

}


template < class O >
inline
void gkg::TypedDiskFormat< O >::partialBinaryWrite(
                     const gkg::HeaderedObject& proxy,
                     const gkg::GenericObject& /*subpart*/,
                     const gkg::RegionInformation& /*regionInformation*/ ) const
{

  std::string name = "";
  if ( proxy.getHeader().hasAttribute( "proxyName" ) )
  {

    proxy.getHeader().getAttribute( "proxyName", name );

  }
  throw gkg::NoPartialWriterException( getName(), name );

}


#endif
