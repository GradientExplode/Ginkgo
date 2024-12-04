#ifndef _gkg_core_io_DefaultAsciiItemReaderOfStdVector_h_
#define _gkg_core_io_DefaultAsciiItemReaderOfStdVector_h_


#include <gkg-core-io/DefaultAsciiItemReader.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultAsciiItemReader< std::vector< T > > :
                    public ItemReader< std::vector< T > >,
                    public Singleton< DefaultAsciiItemReader< std::vector< T > > >
{

  public:

    virtual ~DefaultAsciiItemReader();

    virtual void read(
                std::istream& is, std::vector< T >* pItem, size_t count ) const;
    virtual void read(
          largefile_ifstream& is, std::vector< T >* pItem, size_t count ) const;
    virtual void read(
           largefile_fstream& fs, std::vector< T >* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultAsciiItemReader< std::vector< T > > >;

    DefaultAsciiItemReader();

};


}


#endif
