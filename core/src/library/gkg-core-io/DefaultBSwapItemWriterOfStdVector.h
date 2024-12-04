#ifndef _gkg_core_io_DefaultBSwapItemWriterOfStdVector_h_
#define _gkg_core_io_DefaultBSwapItemWriterOfStdVector_h_


#include <gkg-core-io/DefaultBSwapItemWriter.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultBSwapItemWriter< std::vector< T > > :
                  public ItemWriter< std::vector< T > >,
                  public Singleton< DefaultBSwapItemWriter< std::vector< T > > >
{

  public:

    virtual ~DefaultBSwapItemWriter();

    virtual void write( std::ostream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::vector< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultBSwapItemWriter< std::vector< T > > >;

    DefaultBSwapItemWriter();

};


}


#endif
