#ifndef _gkg_processing_io_SparseCMatrixItemIOFactory_h_
#define _gkg_processing_io_SparseCMatrixItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


class SparseCMatrixItemIOFactory : public Singleton< SparseCMatrixItemIOFactory >
{

  public:

    ~SparseCMatrixItemIOFactory();

    ItemWriter< SparseCMatrix >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< SparseCMatrix >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< SparseCMatrixItemIOFactory >;

    SparseCMatrixItemIOFactory();

};


}


#endif
