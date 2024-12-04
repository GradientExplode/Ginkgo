#ifndef _gkg_processing_io_SparseMatrixItemIOFactory_h_
#define _gkg_processing_io_SparseMatrixItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


class SparseMatrixItemIOFactory : public Singleton< SparseMatrixItemIOFactory >
{

  public:

    ~SparseMatrixItemIOFactory();

    ItemWriter< SparseMatrix >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< SparseMatrix >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< SparseMatrixItemIOFactory >;

    SparseMatrixItemIOFactory();

};


}


#endif
