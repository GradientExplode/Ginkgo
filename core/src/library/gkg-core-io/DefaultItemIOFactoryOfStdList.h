#ifndef _gkg_core_io_DefaultItemIOFactoryOfStdList_h_
#define _gkg_core_io_DefaultItemIOFactoryOfStdList_h_


#include <gkg-core-io/DefaultItemIOFactory.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultItemIOFactory< std::list< T > > :
                      public Singleton< DefaultItemIOFactory< std::list< T > > >
{

  public:

    ~DefaultItemIOFactory();

    ItemWriter< std::list< T > >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< std::list< T > >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< DefaultItemIOFactory< std::list< T > > >;

    DefaultItemIOFactory();

};


}


#endif
