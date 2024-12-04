#ifndef _gkg_core_io_StdListItemIOFactory_h_
#define _gkg_core_io_StdListItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <list>


namespace gkg
{


template < class T > class ItemWriter;
template < class T > class ItemReader;


template < class T >
class StdListItemIOFactory : public Singleton< StdListItemIOFactory< T > >
{

  public:

    ~StdListItemIOFactory();

    ItemWriter< std::list< T > >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< std::list< T > >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< StdListItemIOFactory< T > >;

    StdListItemIOFactory();

};


}


#endif
