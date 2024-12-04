#ifndef _gkg_processing_io_SparseMatrixAsciiItemWriter_h_
#define _gkg_processing_io_SparseMatrixAsciiItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>


namespace gkg
{


class SparseMatrixAsciiItemWriter :
                              public ItemWriter< SparseMatrix >,
                              public Singleton< SparseMatrixAsciiItemWriter >
{

  public:

    virtual ~SparseMatrixAsciiItemWriter();

    virtual void write( std::ostream& os, const SparseMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const SparseMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const SparseMatrix* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< SparseMatrixAsciiItemWriter >;

    SparseMatrixAsciiItemWriter();

};


}


#endif
