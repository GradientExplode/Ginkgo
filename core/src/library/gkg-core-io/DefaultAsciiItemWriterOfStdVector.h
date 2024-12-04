#ifndef _gkg_core_io_DefaultAsciiItemWriterOfStdVector_h_
#define _gkg_core_io_DefaultAsciiItemWriterOfStdVector_h_


#include <gkg-core-io/DefaultAsciiItemWriter.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultAsciiItemWriter< std::vector< T > > :
                  public ItemWriter< std::vector< T > >,
                  public Singleton< DefaultAsciiItemWriter< std::vector< T > > >
{

  public:

    virtual ~DefaultAsciiItemWriter();

    virtual void write( std::ostream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::vector< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultAsciiItemWriter< std::vector< T > > >;

    DefaultAsciiItemWriter();

};


}


#endif
