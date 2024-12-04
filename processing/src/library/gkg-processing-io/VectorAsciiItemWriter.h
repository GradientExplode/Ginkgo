#ifndef _gkg_processing_io_VectorAsciiItemWriter_h_
#define _gkg_processing_io_VectorAsciiItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class VectorAsciiItemWriter : public ItemWriter< Vector >,
                              public Singleton< VectorAsciiItemWriter >
{

  public:

    virtual ~VectorAsciiItemWriter();

    virtual void write( std::ostream& os, const Vector* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const Vector* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const Vector* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< VectorAsciiItemWriter >;

    VectorAsciiItemWriter();

};


}


#endif
