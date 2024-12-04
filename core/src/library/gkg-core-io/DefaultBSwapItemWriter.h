#ifndef _gkg_core_io_DefaultBSwapItemWriter_h_
#define _gkg_core_io_DefaultBSwapItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class DefaultBSwapItemWriter : public ItemWriter< T >,
                               public Singleton< DefaultBSwapItemWriter< T > >
{

  public:

    virtual ~DefaultBSwapItemWriter();

    virtual void write( std::ostream& os, const T* pItem, size_t count ) const;
    virtual void write( largefile_ofstream& os, const T* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& os, const T* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultBSwapItemWriter< T > >;

    DefaultBSwapItemWriter();

};


}


#endif
