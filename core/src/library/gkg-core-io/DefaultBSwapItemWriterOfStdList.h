#ifndef _gkg_core_io_DefaultBSwapItemWriterOfStdList_h_
#define _gkg_core_io_DefaultBSwapItemWriterOfStdList_h_


#include <gkg-core-io/DefaultBSwapItemWriter.h>
#include <list>


namespace gkg
{


template < class T >
class DefaultBSwapItemWriter< std::list< T > > :
                    public ItemWriter< std::list< T > >,
                    public Singleton< DefaultBSwapItemWriter< std::list< T > > >
{

  public:

    virtual ~DefaultBSwapItemWriter();

    virtual void write( std::ostream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::list< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultBSwapItemWriter< std::list< T > > >;

    DefaultBSwapItemWriter();

};


}


#endif
