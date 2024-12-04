#ifndef _gkg_core_io_DefaultBSwapItemReader_h_
#define _gkg_core_io_DefaultBSwapItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class DefaultBSwapItemReader : public ItemReader< T >,
                               public Singleton< DefaultBSwapItemReader< T > >
{

  public:

    virtual ~DefaultBSwapItemReader();

    virtual void read( std::istream& is, T* pItem, size_t count ) const;
    virtual void read( largefile_ifstream& is, T* pItem, size_t count ) const;
    virtual void read( largefile_fstream& fs, T* pItem, size_t count ) const;

  protected:

    friend class Singleton< DefaultBSwapItemReader< T > >;

    DefaultBSwapItemReader();

};


}


#endif
