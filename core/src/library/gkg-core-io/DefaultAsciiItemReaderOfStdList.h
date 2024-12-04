#ifndef _gkg_core_io_DefaultAsciiItemReaderOfStdList_h_
#define _gkg_core_io_DefaultAsciiItemReaderOfStdList_h_


#include <gkg-core-io/DefaultAsciiItemReader.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultAsciiItemReader< std::list< T > > :
                    public ItemReader< std::list< T > >,
                    public Singleton< DefaultAsciiItemReader< std::list< T > > >
{

  public:

    virtual ~DefaultAsciiItemReader();

    virtual void read(
                  std::istream& is, std::list< T >* pItem, size_t count ) const;
    virtual void read(
            largefile_ifstream& is, std::list< T >* pItem, size_t count ) const;
    virtual void read(
             largefile_fstream& fs, std::list< T >* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultAsciiItemReader< std::list< T > > >;

    DefaultAsciiItemReader();

};


}


#endif
