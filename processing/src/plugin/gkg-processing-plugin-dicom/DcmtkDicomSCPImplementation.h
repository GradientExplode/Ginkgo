#ifndef _gkg_processing_plugin_dicom_DcmtkDicomSCPImplementation_h_
#define _gkg_processing_plugin_dicom_DcmtkDicomSCPImplementation_h_


#include <gkg-core-io/DicomServiceClassProviderImplementation.h>
#include <gkg-core-io/DicomServiceClassProvider.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>


#include <dcmtk/config/osconfig.h>

#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#define INCLUDE_CSTDARG
#define INCLUDE_CCTYPE
#define INCLUDE_CSIGNAL



#include <dcmtk/ofstd/ofstdinc.h>
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmnet/cond.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>    // for DCM_StudyInstanceUID
#include <dcmtk/ofstd/ofdatime.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcuid.h>


namespace gkg
{


class DcmtkDicomSCPImplementation : 
                                  public DicomServiceClassProviderImplementation
{

  public:

    struct StoreCallbackData
    {

      DcmtkDicomSCPImplementation* dcmtkDicomSCPImplementation;
      std::string imageFileName;
      DcmFileFormat* dcmFileFormat;
      T_ASC_Association* association;
      bool verbose;

    };


    DcmtkDicomSCPImplementation(
                       DicomServiceClassProvider* dicomServiceClassProvider,
                       const std::string& title,
                       const std::string& incomingDirectory,
                       int32_t port,
                       DicomServiceClassProvider::TransferSyntax transferSyntax,
                       int32_t acseTimeout,
                       int32_t eosTimeout );
    ~DcmtkDicomSCPImplementation();

    void initializeNetwork();
    void run( bool verbose );
    void dropNetwork();

    static DcmtkDicomSCPImplementation* 
      getImplementation( DicomServiceClassProvider* dicomServiceClassProvider );

  private:

    OFCondition acceptAssociation( bool verbose );
    OFCondition processCommands( T_ASC_Association* association,
                                 bool verbose );
    OFCondition echoSCP( T_ASC_Association* association,
                         T_DIMSE_Message* message,
                         T_ASC_PresentationContextID presentationContextID,
                         bool verbose );
    OFCondition storeSCP( T_ASC_Association* association,
                          T_DIMSE_Message* message,
                          T_ASC_PresentationContextID presentationContextID,
                          bool verbose );
    void executeOnReception( const std::string& imageFileName, bool verbose );
    void executeEndOfStudyEvents( bool verbose );

    static void storeSCPCallback( void* cbData,
                                  T_DIMSE_StoreProgress* progress,
                                  T_DIMSE_C_StoreRQ* request,
                                  char* ,
                                  DcmDataset** imageDataSet,
                                  T_DIMSE_C_StoreRSP* rsp,
                                  DcmDataset** statusDetail );

    const std::string& getLastStudyInstanceUID() const;
    void setLastStudyInstanceUID( const std::string& lastStudyInstanceUID );

    const std::string& getSubDirectoryPathAndName() const;
    void setSubDirectoryPathAndName(
                                   const std::string& subDirectoryPathAndName );

    void storeLastStudySubDirectoryPathAndName();
    OFCondition refuseAssociation( T_ASC_Association* association );

    T_ASC_Network* _associationNetwork;
    std::string _lastStudyInstanceUID;
    bool _endOfStudyThroughTimeoutEvent;
    std::string _lastStudySubDirectoryPathAndName;
    std::string _subDirectoryPathAndName;

    OFString _callingAETitle;
    OFString _calledAETitle;

};


}


#endif
