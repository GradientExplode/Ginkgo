#ifndef _gkg_processing_io_VectorBinaryItemReader_h_
#define _gkg_processing_io_VectorBinaryItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class VectorBinaryItemReader : public ItemReader< Vector >,
                               public Singleton< VectorBinaryItemReader >
{

  public:

    virtual ~VectorBinaryItemReader();

    virtual void read( std::istream& is, Vector* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, Vector* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, Vector* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< VectorBinaryItemReader >;

    VectorBinaryItemReader();

};


}


#endif
