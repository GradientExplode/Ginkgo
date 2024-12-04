#ifndef _gkg_core_io_DefaultItemIOFactory_h_
#define _gkg_core_io_DefaultItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


template < class T >
class DefaultItemIOFactory : public Singleton< DefaultItemIOFactory< T > >
{

  public:

    ~DefaultItemIOFactory();

    ItemWriter< T >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< T >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< DefaultItemIOFactory< T > >;

    DefaultItemIOFactory();

};


}


#endif
