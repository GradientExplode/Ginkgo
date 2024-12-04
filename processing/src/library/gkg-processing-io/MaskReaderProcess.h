#ifndef _gkg_processing_io_MaskReaderProcess_h_
#define _gkg_processing_io_MaskReaderProcess_h_


#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume.h>
#include <string>


namespace gkg
{


class MaskReaderProcess : public Process
{

  public:

    MaskReaderProcess( Volume< int8_t >& theMask,
                       const std::string& theMode,
                       const double& theThreshold1,
                       const double& theThreshold2,
                       bool theVerbose );

    Volume< int8_t >& mask;
    const std::string& mode;
    const double& threshold1;
    const double& threshold2;
    const bool& verbose;

  private:

    template < class M >
    static void read( Process& process,
                      const std::string& fileNameMask,
                      const gkg::AnalyzedObject&,
                      const std::string& );

};


}


#endif
