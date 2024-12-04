#ifndef _gkg_processing_io_CMatrixItemIOFactory_h_
#define _gkg_processing_io_CMatrixItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


class CMatrixItemIOFactory : public Singleton< CMatrixItemIOFactory >
{

  public:

    ~CMatrixItemIOFactory();

    ItemWriter< CMatrix >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< CMatrix >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< CMatrixItemIOFactory >;

    CMatrixItemIOFactory();

};


}


#endif
