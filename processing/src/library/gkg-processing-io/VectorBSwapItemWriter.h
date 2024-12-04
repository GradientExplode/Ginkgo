#ifndef _gkg_processing_io_VectorBSwapItemWriter_h_
#define _gkg_processing_io_VectorBSwapItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class VectorBSwapItemWriter : public ItemWriter< Vector >,
                              public Singleton< VectorBSwapItemWriter >
{

  public:

    virtual ~VectorBSwapItemWriter();

    virtual void write( std::ostream& os, const Vector* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const Vector* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const Vector* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< VectorBSwapItemWriter >;

    VectorBSwapItemWriter();

};


}


#endif
