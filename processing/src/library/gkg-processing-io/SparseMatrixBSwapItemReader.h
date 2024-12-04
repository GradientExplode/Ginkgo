#ifndef _gkg_processing_io_SparseMatrixBSwapItemReader_h_
#define _gkg_processing_io_SparseMatrixBSwapItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>


namespace gkg
{


class SparseMatrixBSwapItemReader :
                              public ItemReader< SparseMatrix >,
                              public Singleton< SparseMatrixBSwapItemReader >
{

  public:

    virtual ~SparseMatrixBSwapItemReader();

    virtual void read( std::istream& is, SparseMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, SparseMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, SparseMatrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< SparseMatrixBSwapItemReader >;

    SparseMatrixBSwapItemReader();

};


}


#endif
