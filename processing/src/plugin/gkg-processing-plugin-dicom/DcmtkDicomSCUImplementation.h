#ifndef _gkg_processing_plugin_dicom_DcmtkDicomSCUImplementation_h_
#define _gkg_processing_plugin_dicom_DcmtkDicomSCUImplementation_h_


// SCU stands for Service Class User


#include <gkg-core-io/DicomServiceClassUserImplementation.h>
#include <gkg-core-io/DicomServiceClassUser.h>
#include <gkg-core-exception/Exception.h>


#define MAX_TRANSFERSYNTAX_COUNT 4

#include <dcmtk/config/osconfig.h>

#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#define INCLUDE_CSTDARG
#define INCLUDE_CCTYPE
#define INCLUDE_CSIGNAL

#include <dcmtk/ofstd/ofstdinc.h>
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmdata/dcdict.h>


namespace gkg
{


class DcmtkDicomSCUImplementation : public DicomServiceClassUserImplementation
{

  public:
  
    DcmtkDicomSCUImplementation( 
                           DicomServiceClassUser* dicomServiceClassUser,
	                   const std::string& callingTitle,
	                   const std::string& calledTitle,
			   const std::string& peerHostName,
	                   int32_t port,
	                   DicomServiceClassUser::TransferSyntax transferSyntax,
	                   int32_t acseTimeout );
    ~DcmtkDicomSCUImplementation();
    
    void initializeNetwork( DicomServiceClassUser::NetworkRole networkRole );
    void store( std::list< std::string >& fileNameList, bool verbose );
    void get( std::list< std::string >& fileNameList, bool verbose );
    void find( std::list< std::string >& fileNameList, bool verbose );
    void move( std::list< std::string >& fileNameList, bool verbose );
    void echo( uint32_t repeatCount, bool verbose );
    void cancel( bool verbose );
    void shutdown( bool verbose );
    void dropNetwork();
       
    static DcmtkDicomSCUImplementation*
              getImplementation( DicomServiceClassUser* dicomServiceClassUser );

  private:
  
    void initializeTransferSyntaxes();
    OFCondition initializeAssociation( bool verbose );
    bool requestAssociation( bool verbose );
    bool releaseAssociation( OFCondition cond, bool verbose );
  
    T_ASC_Network* _associationNetwork;
    T_ASC_Parameters* _associationParameters;
    T_ASC_Association* _association;
    T_ASC_PresentationContextID _currentPresentationContextID;
    DIC_US _currentMessageID;
    
    const char* _transferSyntaxes[ MAX_TRANSFERSYNTAX_COUNT ];
    int32_t _transferSyntaxesCount;

};


}


#endif
