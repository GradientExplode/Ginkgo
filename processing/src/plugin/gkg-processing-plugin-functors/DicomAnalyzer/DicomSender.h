#ifndef _gkg_processing_plugin_functors_DicomAnalyzer_DicomSender_h_
#define _gkg_processing_plugin_functors_DicomAnalyzer_DicomSender_h_


#include <string>


namespace gkg
{


class DicomSeries;


class DicomSender
{

  public:

    DicomSender( const DicomSeries& series );
    virtual ~DicomSender();

    void send( const std::string& outputDirectory ) const;

  private:

    const DicomSeries& _series;

};


}


#endif

