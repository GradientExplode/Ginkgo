#ifndef _gkg_processing_process_SubVolumeProcess_h_
#define _gkg_processing_process_SubVolumeProcess_h_


#include <gkg-processing-process/Process.h>
#include <string>
#include <list>


namespace gkg
{


class SubVolumeProcess : public gkg::Process
{

  public:

    SubVolumeProcess( const std::string& fileNameOut,
                      int32_t sx, int32_t sy, int32_t sz, int32_t st,
                      int32_t ex, int32_t ey, int32_t ez, int32_t et,
                      const std::list< int32_t >& xIndices,
                      const std::list< int32_t >& yIndices,
                      const std::list< int32_t >& zIndices,
                      const std::list< int32_t >& tIndices,
                      bool ascii,
                      const std::string& format,
                      bool verbose );

    const std::string& getFileNameOut() const;
    void getStartValues( int32_t& sx, int32_t& sy, 
                         int32_t& sz, int32_t& st ) const;
    void getEndValues( int32_t& ex, int32_t& ey, 
                       int32_t& ez, int32_t& et ) const;
    const std::list< int32_t >& getXIndices() const;
    const std::list< int32_t >& getYIndices() const;
    const std::list< int32_t >& getZIndices() const;
    const std::list< int32_t >& getTIndices() const;
    bool getAscii() const;
    const std::string& getFormat() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void extract( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameOut;
    int32_t _sx, _sy, _sz, _st;
    int32_t _ex, _ey, _ez, _et;
    const std::list< int32_t >& _xIndices;
    const std::list< int32_t >& _yIndices;
    const std::list< int32_t >& _zIndices;
    const std::list< int32_t >& _tIndices;
    bool _ascii;
    std::string _format;
    bool _verbose;

};


}



#endif
