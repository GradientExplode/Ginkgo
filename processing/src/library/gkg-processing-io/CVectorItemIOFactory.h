#ifndef _gkg_processing_io_CVectorItemIOFactory_h_
#define _gkg_processing_io_CVectorItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


class CVectorItemIOFactory : public Singleton< CVectorItemIOFactory >
{

  public:

    ~CVectorItemIOFactory();

    ItemWriter< CVector >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< CVector >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< CVectorItemIOFactory >;

    CVectorItemIOFactory();

};


}


#endif
