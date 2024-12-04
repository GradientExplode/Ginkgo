#ifndef _gkg_processing_io_SparseMatrixBinaryItemReader_h_
#define _gkg_processing_io_SparseMatrixBinaryItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>


namespace gkg
{


class SparseMatrixBinaryItemReader :
                               public ItemReader< SparseMatrix >,
                               public Singleton< SparseMatrixBinaryItemReader >
{

  public:

    virtual ~SparseMatrixBinaryItemReader();

    virtual void read( std::istream& is, SparseMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, SparseMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, SparseMatrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< SparseMatrixBinaryItemReader >;

    SparseMatrixBinaryItemReader();

};


}


#endif
