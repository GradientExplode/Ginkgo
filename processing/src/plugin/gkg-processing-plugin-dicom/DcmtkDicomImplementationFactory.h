#ifndef _gkg_processing_plugin_dicom_DcmtkDicomImplementationFactory_h_
#define _gkg_processing_plugin_dicom_DcmtkDicomImplementationFactory_h_


#include <gkg-core-io/DicomImplementationFactory.h>
#include <gkg-core-pattern/SingletonAccessor.h>



namespace gkg
{


class DcmtkDicomImplementationFactory : 
                      public DicomImplementationFactory,
                      public SingletonAccessor< DcmtkDicomImplementationFactory,
                                                DicomImplementationFactory >
{

  public:

    ~DcmtkDicomImplementationFactory();

    // dicom file
    DicomFileImplementation* 
      createDicomFileImplementation(
        DicomFile* dicomFile,
        const std::string& fileName ) const;

    // dicom dump
    DicomDumpImplementation* 
      createDicomDumpImplementation(
        DicomDump* dicomDump,
        const std::string& fileName ) const;

    // dicom storage unit
    DicomServiceClassProviderImplementation*
      createDicomServiceClassProviderImplementation(
        DicomServiceClassProvider* dicomServiceClassProvider,
        const std::string& title,
        const std::string& incomingDirectory,
        int32_t port,
        DicomServiceClassProvider::TransferSyntax transferSyntax,
        int32_t acseTimeout,
        int32_t eosTimeout ) const;
	
    // dicom service class user
    DicomServiceClassUserImplementation*
      createDicomServiceClassUserImplementation(
        DicomServiceClassUser* dicomServiceClassUser,
	const std::string& callingTitle,
	const std::string& calledTitle,
	const std::string& peerHostName,
	int32_t port,
	DicomServiceClassUser::TransferSyntax transferSyntax,
	int32_t acseTimeout ) const;

  protected:

    friend class Singleton< DcmtkDicomImplementationFactory >;

    DcmtkDicomImplementationFactory();

};


}


#endif
