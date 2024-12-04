#ifndef _gkg_core_io_StdListBSwapItemWriter_h_
#define _gkg_core_io_StdListBSwapItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <list>


namespace gkg
{


template < class T >
class StdListBSwapItemWriter : public ItemWriter< std::list< T > >,
                            public Singleton< StdListBSwapItemWriter< T > >
{

  public:

    virtual ~StdListBSwapItemWriter();

    virtual void write( std::ostream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::list< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::list< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< StdListBSwapItemWriter< T > >;

    StdListBSwapItemWriter();

};


}


#endif
