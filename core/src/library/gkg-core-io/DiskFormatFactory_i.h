#ifndef _gkg_core_io_DiskFormatFactory_i_h_
#define _gkg_core_io_DiskFormatFactory_i_h_


#include <gkg-core-io/DiskFormatFactory.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class O >
inline
gkg::DiskFormatFactory< O >::DiskFormatFactory()
{
}


template < class O >
inline
gkg::DiskFormatFactory< O >::~DiskFormatFactory()
{
}


template < class O >
inline
void gkg::DiskFormatFactory< O >::registerDiskFormat(
                                    const std::string& name,
                                    gkg::TypedDiskFormat< O >* typedDiskFormat )
{

  if ( typedDiskFormat )
  {

    typename std::map< std::string, gkg::TypedDiskFormat< O >* >::const_iterator
      c = _diskFormats.find( name );

    if ( c == _diskFormats.end() )
    {

      _diskFormats[ name ] = typedDiskFormat;

    }

#ifdef GKG_DEBUG

    else
    {

      std::cerr << std::string( 
                        "gkg::DiskFormatFactory< O >::registerDiskFormat: " ) +
                   name + " already exists"
                << std::endl;

    }

#endif

  }

}


template < class O >
inline
const std::map< std::string, gkg::TypedDiskFormat< O >* >&
gkg::DiskFormatFactory< O >::getDiskFormats() const
{

  return _diskFormats;

}


template < class O >
inline
gkg::TypedDiskFormat< O >* 
gkg::DiskFormatFactory< O >::getDiskFormat( const std::string& name ) const
{

  typename std::map< std::string,
            gkg::TypedDiskFormat< O >* >::const_iterator
    df = _diskFormats.find( name );
  if ( df != _diskFormats.end() )
  {

    return df->second;

  }
  return 0;

}




#endif
