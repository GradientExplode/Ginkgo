#ifndef _gkg_core_io_StdListBSwapItemReader_h_
#define _gkg_core_io_StdListBSwapItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <list>


namespace gkg
{


template < class T >
class StdListBSwapItemReader : public ItemReader< std::list< T > >,
                            public Singleton< StdListBSwapItemReader< T > >
{

  public:

    virtual ~StdListBSwapItemReader();

    virtual void read( std::istream& is, std::list< T >* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, std::list< T >* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, std::list< T >* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< StdListBSwapItemReader< T > >;

    StdListBSwapItemReader();

};


}


#endif
