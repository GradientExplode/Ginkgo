#ifndef _gkg_core_io_DefaultBinaryItemWriterOfStdVector_h_
#define _gkg_core_io_DefaultBinaryItemWriterOfStdVector_h_


#include <gkg-core-io/DefaultBinaryItemWriter.h>
#include <vector>


namespace gkg
{


template < class T >
class DefaultBinaryItemWriter< std::vector< T > > :
                 public ItemWriter< std::vector< T > >,
                 public Singleton< DefaultBinaryItemWriter< std::vector< T > > >
{

  public:

    virtual ~DefaultBinaryItemWriter();

    virtual void write( std::ostream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const std::vector< T >* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const std::vector< T >* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< DefaultBinaryItemWriter< std::vector< T > > >;

    DefaultBinaryItemWriter();

};


}


#endif
