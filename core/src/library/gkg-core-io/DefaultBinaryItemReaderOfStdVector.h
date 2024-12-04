#ifndef _gkg_core_io_DefaultBinaryItemReaderOfStdVector_h_
#define _gkg_core_io_DefaultBinaryItemReaderOfStdVector_h_


#include <gkg-core-io/DefaultBinaryItemReader.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultBinaryItemReader< std::vector< T > > :
                 public ItemReader< std::vector< T > >,
                 public Singleton< DefaultBinaryItemReader< std::vector< T > > >
{

  public:

    virtual ~DefaultBinaryItemReader();

    virtual void read(
                std::istream& is, std::vector< T >* pItem, size_t count ) const;
    virtual void read(
          largefile_ifstream& is, std::vector< T >* pItem, size_t count ) const;
    virtual void read(
           largefile_fstream& fs, std::vector< T >* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultBinaryItemReader< std::vector< T > > >;

    DefaultBinaryItemReader();

};


}


#endif