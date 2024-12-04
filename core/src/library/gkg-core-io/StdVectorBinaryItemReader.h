#ifndef _gkg_core_io_StdVectorBinaryItemReader_h_
#define _gkg_core_io_StdVectorBinaryItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


template < class T >
class StdVectorBinaryItemReader : public ItemReader< std::vector< T > >,
                               public Singleton< StdVectorBinaryItemReader< T > >
{

  public:

    virtual ~StdVectorBinaryItemReader();

    virtual void read( std::istream& is, std::vector< T >* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, std::vector< T >* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, std::vector< T >* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< StdVectorBinaryItemReader< T > >;

    StdVectorBinaryItemReader();

};


}


#endif
