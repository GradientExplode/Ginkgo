#ifndef _gkg_core_io_DefaultBSwapItemReaderOfStdList_h_
#define _gkg_core_io_DefaultBSwapItemReaderOfStdList_h_


#include <gkg-core-io/DefaultBSwapItemReader.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultBSwapItemReader< std::list< T > > :
                    public ItemReader< std::list< T > >,
                    public Singleton< DefaultBSwapItemReader< std::list< T > > >
{

  public:

    virtual ~DefaultBSwapItemReader();

    virtual void read(
                  std::istream& is, std::list< T >* pItem, size_t count ) const;
    virtual void read(
            largefile_ifstream& is, std::list< T >* pItem, size_t count ) const;
    virtual void read(
             largefile_fstream& fs, std::list< T >* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultBSwapItemReader< std::list< T > > >;

    DefaultBSwapItemReader();

};


}


#endif
