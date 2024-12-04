#ifndef _gkg_processing_io_VectorBinaryItemWriter_h_
#define _gkg_processing_io_VectorBinaryItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class VectorBinaryItemWriter : public ItemWriter< Vector >,
                               public Singleton< VectorBinaryItemWriter >
{

  public:

    virtual ~VectorBinaryItemWriter();

    virtual void write( std::ostream& os, const Vector* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const Vector* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const Vector* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< VectorBinaryItemWriter >;

    VectorBinaryItemWriter();

};


}


#endif
