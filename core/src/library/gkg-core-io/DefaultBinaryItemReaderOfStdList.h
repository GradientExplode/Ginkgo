#ifndef _gkg_core_io_DefaultBinaryItemReaderOfStdList_h_
#define _gkg_core_io_DefaultBinaryItemReaderOfStdList_h_


#include <gkg-core-io/DefaultBinaryItemReader.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultBinaryItemReader< std::list< T > > :
                   public ItemReader< std::list< T > >,
                   public Singleton< DefaultBinaryItemReader< std::list< T > > >
{

  public:

    virtual ~DefaultBinaryItemReader();

    virtual void read(
                  std::istream& is, std::list< T >* pItem, size_t count ) const;
    virtual void read(
            largefile_ifstream& is, std::list< T >* pItem, size_t count ) const;
    virtual void read(
             largefile_fstream& fs, std::list< T >* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultBinaryItemReader< std::list< T > > >;

    DefaultBinaryItemReader();

};


}


#endif
