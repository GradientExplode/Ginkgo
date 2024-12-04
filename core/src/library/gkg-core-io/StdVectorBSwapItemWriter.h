#ifndef _gkg_core_io_StdVectorBSwapItemWriter_h_
#define _gkg_core_io_StdVectorBSwapItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


template < class T >
class StdVectorBSwapItemWriter : public ItemWriter< std::vector< T > >,
                              public Singleton< StdVectorBSwapItemWriter< T > >
{

  public:

    virtual ~StdVectorBSwapItemWriter();

    virtual void write( std::ostream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::vector< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< StdVectorBSwapItemWriter< T > >;

    StdVectorBSwapItemWriter();

};


}


#endif
