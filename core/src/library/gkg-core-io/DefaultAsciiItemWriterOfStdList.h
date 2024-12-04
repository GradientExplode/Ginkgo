#ifndef _gkg_core_io_DefaultAsciiItemWriterOfStdList_h_
#define _gkg_core_io_DefaultAsciiItemWriterOfStdList_h_


#include <gkg-core-io/DefaultAsciiItemWriter.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultAsciiItemWriter< std::list< T > > :
                    public ItemWriter< std::list< T > >,
                    public Singleton< DefaultAsciiItemWriter< std::list< T > > >
{

  public:

    virtual ~DefaultAsciiItemWriter();

    virtual void write( std::ostream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::list< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultAsciiItemWriter< std::list< T > > >;

    DefaultAsciiItemWriter();

};


}


#endif
