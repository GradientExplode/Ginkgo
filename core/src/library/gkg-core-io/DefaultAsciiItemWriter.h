#ifndef _gkg_core_io_DefaultAsciiItemWriter_h_
#define _gkg_core_io_DefaultAsciiItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class DefaultAsciiItemWriter : public ItemWriter< T >,
                               public Singleton< DefaultAsciiItemWriter< T > >
{

  public:

    virtual ~DefaultAsciiItemWriter();

    virtual void write( std::ostream& os, const T* pItem, size_t count ) const;
    virtual void write( largefile_ofstream& os, const T* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const T* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultAsciiItemWriter< T > >;

    DefaultAsciiItemWriter();

};


}


#endif
