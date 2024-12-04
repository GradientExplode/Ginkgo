#ifndef _gkg_core_io_DefaultBinaryItemWriter_h_
#define _gkg_core_io_DefaultBinaryItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class DefaultBinaryItemWriter : public ItemWriter< T >,
                                public Singleton< DefaultBinaryItemWriter< T > >
{

  public:

    virtual ~DefaultBinaryItemWriter();

    virtual void write( std::ostream& os, const T* pItem, size_t count ) const;
    virtual void write( largefile_ofstream& os, const T* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const T* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultBinaryItemWriter< T > >;

    DefaultBinaryItemWriter();

};


}


#endif
