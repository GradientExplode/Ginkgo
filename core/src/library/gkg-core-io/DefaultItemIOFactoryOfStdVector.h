#ifndef _gkg_core_io_DefaultItemIOFactoryOfStdVector_h_
#define _gkg_core_io_DefaultItemIOFactoryOfStdVector_h_


#include <gkg-core-io/DefaultItemIOFactory.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultItemIOFactory< std::vector< T > > :
                    public Singleton< DefaultItemIOFactory< std::vector< T > > >
{

  public:

    ~DefaultItemIOFactory();

    ItemWriter< std::vector< T > >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< std::vector< T > >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< DefaultItemIOFactory< std::vector< T > > >;

    DefaultItemIOFactory();

};


}


#endif
