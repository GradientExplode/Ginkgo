#ifndef _gkg_processing_io_VectorItemIOFactory_h_
#define _gkg_processing_io_VectorItemIOFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>


namespace gkg
{


class VectorItemIOFactory : public Singleton< VectorItemIOFactory >
{

  public:

    ~VectorItemIOFactory();

    ItemWriter< Vector >& getWriter( bool ascii, bool bswap ) const;
    ItemReader< Vector >& getReader( bool ascii, bool bswap ) const;

  private:

    friend class Singleton< VectorItemIOFactory >;

    VectorItemIOFactory();

};


}


#endif
