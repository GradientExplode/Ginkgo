#ifndef _gkg_processing_io_TypedVolumeReaderProcess_h_
#define _gkg_processing_io_TypedVolumeReaderProcess_h_


#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume.h>



namespace gkg
{


template < class O >
class TypedVolumeReaderProcess : public Process
{

  public:

    TypedVolumeReaderProcess( Volume< O >& theVolume );

    Volume< O >& volume;

  private:

    template < class T >
    static void read( Process& process,
                      const std::string& fileNameIn,
                      const AnalyzedObject&,
                      const std::string& );

};


}


#endif
