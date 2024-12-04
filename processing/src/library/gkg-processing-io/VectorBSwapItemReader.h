#ifndef _gkg_processing_io_VectorBSwapItemReader_h_
#define _gkg_processing_io_VectorBSwapItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class VectorBSwapItemReader : public ItemReader< Vector >,
                              public Singleton< VectorBSwapItemReader >
{

  public:

    virtual ~VectorBSwapItemReader();

    virtual void read( std::istream& is, Vector* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, Vector* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, Vector* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< VectorBSwapItemReader >;

    VectorBSwapItemReader();

};


}


#endif
