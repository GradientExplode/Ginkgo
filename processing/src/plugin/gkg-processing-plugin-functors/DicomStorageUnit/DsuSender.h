#ifndef _gkg_processing_plugin_functors_DicomStorageUnit_DsuSender_h_
#define _gkg_processing_plugin_functors_DicomStorageUnit_DsuSender_h_


#include <string>


namespace gkg
{


class DsuSeries;


class DsuSender
{

  public:

    DsuSender( const DsuSeries& series,
               const std::string& tmpDirectory );
    virtual ~DsuSender();

    void send( const std::string& hostname,
               const std::string& login,
               const std::string& password,
               const std::string& destinationPath ) const;

  private:

    void prepare();
    void clean();

    const DsuSeries& _series;
    std::string _tmpDirectory;
    std::string _gtarCommand;
    std::string _institutionModelDateExamDsuSeriesPidDirectory;
    std::string _institutionModelDateExamDsuSeriesPidSubDirectory;
    std::string _examDsuSeriesDirectory;
    std::string _examDsuSeriesSubDirectory;

    std::string _fullPathTarFileName;
    std::string _tarFileName;

};


}


#endif

