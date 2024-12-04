#ifndef _gkg_processing_io_SparseCMatrixAsciiItemWriter_h_
#define _gkg_processing_io_SparseCMatrixAsciiItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>


namespace gkg
{


class SparseCMatrixAsciiItemWriter :
                              public ItemWriter< SparseCMatrix >,
                              public Singleton< SparseCMatrixAsciiItemWriter >
{

  public:

    virtual ~SparseCMatrixAsciiItemWriter();

    virtual void write( std::ostream& os, const SparseCMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const SparseCMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const SparseCMatrix* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< SparseCMatrixAsciiItemWriter >;

    SparseCMatrixAsciiItemWriter();

};


}


#endif
