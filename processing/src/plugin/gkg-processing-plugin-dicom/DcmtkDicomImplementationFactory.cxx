#include <gkg-processing-plugin-dicom/DcmtkDicomImplementationFactory.h>
#include <gkg-processing-plugin-dicom/DcmtkDicomFileImplementation.h>
#include <gkg-processing-plugin-dicom/DcmtkDicomDumpImplementation.h>
#include <gkg-processing-plugin-dicom/DcmtkDicomSCPImplementation.h>
#include <gkg-processing-plugin-dicom/DcmtkDicomSCUImplementation.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/DicomSelector.h>
#include <gkg-core-io/DicomFile.h>


//
// DcmtkDicomImplementationFactory methods
//


ForceSingletonInstanciation( gkg::DcmtkDicomImplementationFactory,
                             dcmtk );

gkg::DcmtkDicomImplementationFactory::DcmtkDicomImplementationFactory()
                                     : gkg::DicomImplementationFactory()
{

  try
  {

    gkg::DicomSelector::getInstance().registerFactory(
                           "dcmtk",
                           &gkg::DcmtkDicomImplementationFactory::getAccessor );

  }
  GKG_CATCH( "gkg::DcmtkDicomImplementationFactory::"
             "DcmtkDicomImplementationFactory()" );

}


gkg::DcmtkDicomImplementationFactory::
                                    ~DcmtkDicomImplementationFactory()
{
}


/////////////////////////////////// dicom file /////////////////////////////////


gkg::DicomFileImplementation* 
gkg::DcmtkDicomImplementationFactory::createDicomFileImplementation(
                                             gkg::DicomFile* dicomFile,
                                             const std::string& fileName ) const
{

  try
  {

    return new gkg::DcmtkDicomFileImplementation( dicomFile, fileName );

  }
  GKG_CATCH( "gkg::DicomFileImplementation* "
             "gkg::DcmtkDicomImplementationFactory::"
             "createDicomFileImplementation( "
             "gkg::DicomFile* dicomFile, "
             "const std::string& fileName ) const" );

}


/////////////////////////////////// dicom dump /////////////////////////////////


gkg::DicomDumpImplementation* 
gkg::DcmtkDicomImplementationFactory::createDicomDumpImplementation(
                                             gkg::DicomDump* dicomDump,
                                             const std::string& fileName ) const
{

  try
  {

    return new gkg::DcmtkDicomDumpImplementation( dicomDump, fileName );

  }
  GKG_CATCH( "gkg::DicomDumpImplementation* "
             "gkg::DcmtkDicomImplementationFactory::"
             "createDicomDumpImplementation( "
             "gkg::DicomDump* dicomDump, "
             "const std::string& fileName ) const" );

}


///////////////////////// dicom service class provider /////////////////////////


gkg::DicomServiceClassProviderImplementation* 
gkg::DcmtkDicomImplementationFactory::
                                  createDicomServiceClassProviderImplementation(
                  gkg::DicomServiceClassProvider* dicomServiceClassProvider,
                  const std::string& title,
                  const std::string& incomingDirectory,
                  int32_t port,
                  gkg::DicomServiceClassProvider::TransferSyntax transferSyntax,
                  int32_t acseTimeout,
                  int32_t eosTimeout ) const
{

  try
  {

    return new gkg::DcmtkDicomSCPImplementation( dicomServiceClassProvider,
                                                 title,
                                                 incomingDirectory,
                                                 port,
                                                 transferSyntax,
                                                 acseTimeout,
                                                 eosTimeout );

  }
  GKG_CATCH( "gkg::DicomServiceClassProviderImplementation* "
             "gkg::DcmtkDicomImplementationFactory::"
             "createDicomServiceClassProviderImplementation( "
             "gkg::DicomServiceClassProvider* dicomServiceClassProvider, "
             "const std::string& title, "
             "const std::string& incomingDirectory, "
             "int32_t port, "
             "gkg::DicomServiceClassProvider::TransferSyntax transferSyntax, "
             "int32_t acseTimeout, "
             "int32_t eosTimeout ) const" );

}


//////////////////////////// dicom service class user //////////////////////////


gkg::DicomServiceClassUserImplementation*
gkg::DcmtkDicomImplementationFactory::createDicomServiceClassUserImplementation(
                      gkg::DicomServiceClassUser* dicomServiceClassUser,
                      const std::string& callingTitle,
		      const std::string& calledTitle,
		      const std::string& peerHostName,
                      int32_t port,
                      gkg::DicomServiceClassUser::TransferSyntax transferSyntax,
                      int32_t acseTimeout ) const
{

  try
  {

    return new gkg::DcmtkDicomSCUImplementation( dicomServiceClassUser,
                                                 callingTitle,
						 calledTitle,
						 peerHostName,
                                                 port,
                                                 transferSyntax,
                                                 acseTimeout );

  }
  GKG_CATCH( "gkg::DicomServiceClassUserImplementation* " 
             "gkg::DcmtkDicomImplementationFactory::"
	     "createDicomServiceClassUserImplementation( "
             "gkg::DicomServiceClassUser* dicomStorageUnit, "
             "const std::string& title, "
             "int32_t port, "
             "gkg::DicomServiceClassUser::TransferSyntax transferSyntax, "
             "int32_t acseTimeout ) const" );

}

