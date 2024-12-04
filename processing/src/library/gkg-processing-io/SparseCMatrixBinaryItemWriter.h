#ifndef _gkg_processing_io_SparseCMatrixBinaryItemWriter_h_
#define _gkg_processing_io_SparseCMatrixBinaryItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>


namespace gkg
{


class SparseCMatrixBinaryItemWriter :
                               public ItemWriter< SparseCMatrix >,
                               public Singleton< SparseCMatrixBinaryItemWriter >
{

  public:

    virtual ~SparseCMatrixBinaryItemWriter();

    virtual void write( std::ostream& os, const SparseCMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const SparseCMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const SparseCMatrix* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< SparseCMatrixBinaryItemWriter >;

    SparseCMatrixBinaryItemWriter();

};


}


#endif
