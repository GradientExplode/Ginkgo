#ifndef _gkg_processing_io_MatrixItemIOFactory_h_
#define _gkg_processing_io_MatrixItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


class MatrixItemIOFactory : public Singleton< MatrixItemIOFactory >
{

  public:

    ~MatrixItemIOFactory();

    ItemWriter< Matrix >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< Matrix >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< MatrixItemIOFactory >;

    MatrixItemIOFactory();

};


}


#endif
