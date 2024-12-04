#ifndef _gkg_core_io_DicomImplementationFactory_h_
#define _gkg_core_io_DicomImplementationFactory_h_


#include <string>
#include <gkg-core-io/DicomDump.h>
#include <gkg-core-io/DicomServiceClassProvider.h>
#include <gkg-core-io/DicomServiceClassUser.h>


namespace gkg
{


class DicomFile;
class DicomFileImplementation;


class DicomImplementationFactory
{

  public:

    virtual ~DicomImplementationFactory();

    // dicom file
    virtual DicomFileImplementation*
      createDicomFileImplementation(
        DicomFile* dicomFile,
        const std::string& fileName ) const = 0;

    // dicom dump
    virtual DicomDumpImplementation*
      createDicomDumpImplementation(
        DicomDump* dicomDump,
        const std::string& fileName ) const = 0;

    // dicom storage unit
    virtual DicomServiceClassProviderImplementation*
      createDicomServiceClassProviderImplementation(
        DicomServiceClassProvider* dicomServiceClassProvider,
        const std::string& title,
        const std::string& incomingDirectory,
        int32_t port,
        DicomServiceClassProvider::TransferSyntax transferSyntax,
        int32_t acseTimeout,
        int32_t eosTimeout ) const = 0;
	
    // dicom service class user
    virtual DicomServiceClassUserImplementation*
      createDicomServiceClassUserImplementation(
        DicomServiceClassUser* dicomServiceClassUser,
	const std::string& callingTitle,
	const std::string& calledTitle,
	const std::string& peerHostName,
	int32_t port,
	DicomServiceClassUser::TransferSyntax transferSyntax,
	int32_t acseTimeout ) const = 0;

  protected:

    DicomImplementationFactory();

};


}


#endif
