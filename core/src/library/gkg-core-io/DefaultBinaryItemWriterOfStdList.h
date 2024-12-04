#ifndef _gkg_core_io_DefaultBinaryItemWriterOfStdList_h_
#define _gkg_core_io_DefaultBinaryItemWriterOfStdList_h_


#include <gkg-core-io/DefaultBinaryItemWriter.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultBinaryItemWriter< std::list< T > > :
                   public ItemWriter< std::list< T > >,
                   public Singleton< DefaultBinaryItemWriter< std::list< T > > >
{

  public:

    virtual ~DefaultBinaryItemWriter();

    virtual void write( std::ostream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::list< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultBinaryItemWriter< std::list< T > > >;

    DefaultBinaryItemWriter();

};


}


#endif
