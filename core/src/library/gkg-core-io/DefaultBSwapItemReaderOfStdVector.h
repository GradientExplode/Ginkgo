#ifndef _gkg_core_io_DefaultBSwapItemReaderOfStdVector_h_
#define _gkg_core_io_DefaultBSwapItemReaderOfStdVector_h_


#include <gkg-core-io/DefaultBSwapItemReader.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultBSwapItemReader< std::vector< T > > :
                  public ItemReader< std::vector< T > >,
                  public Singleton< DefaultBSwapItemReader< std::vector< T > > >
{

  public:

    virtual ~DefaultBSwapItemReader();

    virtual void read(
                std::istream& is, std::vector< T >* pItem, size_t count ) const;
    virtual void read(
          largefile_ifstream& is, std::vector< T >* pItem, size_t count ) const;
    virtual void read(
           largefile_fstream& fs, std::vector< T >* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultBSwapItemReader< std::vector< T > > >;

    DefaultBSwapItemReader();

};


}


#endif
