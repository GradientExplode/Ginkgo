#ifndef _gkg_core_io_StdVectorItemIOFactory_h_
#define _gkg_core_io_StdVectorItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


template < class T > class ItemWriter;
template < class T > class ItemReader;


template < class T >
class StdVectorItemIOFactory : public Singleton< StdVectorItemIOFactory< T > >
{

  public:

    ~StdVectorItemIOFactory();

    ItemWriter< std::vector< T > >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< std::vector< T > >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< StdVectorItemIOFactory< T > >;

    StdVectorItemIOFactory();

};


}


#endif
