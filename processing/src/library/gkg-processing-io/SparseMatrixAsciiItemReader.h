#ifndef _gkg_processing_io_SparseMatrixAsciiItemReader_h_
#define _gkg_processing_io_SparseMatrixAsciiItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>


namespace gkg
{


class SparseMatrixAsciiItemReader :
                              public ItemReader< SparseMatrix >,
                              public Singleton< SparseMatrixAsciiItemReader >
{

  public:

    virtual ~SparseMatrixAsciiItemReader();

    virtual void read( std::istream& is, SparseMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, SparseMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, SparseMatrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< SparseMatrixAsciiItemReader >;

    SparseMatrixAsciiItemReader();

};


}


#endif
