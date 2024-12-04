#include <gkg-core-io/DicomServiceClassProvider.h>
#include <gkg-core-io/DicomServiceClassProviderImplementation.h>
#include <gkg-core-io/DicomSelector.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomServiceClassProvider::DicomServiceClassProvider(
                  const std::string& title,
                  const std::string& incomingDirectory,
                  in_port_t port,
                  gkg::DicomServiceClassProvider::TransferSyntax transferSyntax,
                  int32_t acseTimeout,
                  int32_t eosTimeout )
{

  try
  {

    _dicomServiceClassProviderImplementation =
    gkg::DicomSelector::getInstance().getImplementationFactory()->
               createDicomServiceClassProviderImplementation( this,
                                                              title,
                                                              incomingDirectory,
                                                              port,
                                                              transferSyntax,
                                                              acseTimeout,
                                                              eosTimeout );

  }
  GKG_CATCH( "gkg::DicomServiceClassProvider::DicomServiceClassProvider( "
             "const std::string& title, "
             "const std::string& incomingDirectory, "
             "int32_t port, "
             "gkg::DicomServiceClassProvider::TransferSyntax transferSyntax, "
             "int32_t acseTimeout, "
             "int32_t eosTimeout )" );

}


gkg::DicomServiceClassProvider::~DicomServiceClassProvider()
{

  delete _dicomServiceClassProviderImplementation;

}


void gkg::DicomServiceClassProvider::registerOnReceptionCallback(
       gkg::DicomServiceClassProvider::OnReceptionCallback onReceptionCallback )
{

  try
  {

    _dicomServiceClassProviderImplementation->registerOnReceptionCallback(
                                                          onReceptionCallback );

  }
  GKG_CATCH( "void gkg::DicomServiceClassProvider::addOnReceptionCallback( "
             "gkg::DicomServiceClassProvider::OnReceptionCallback "
             "onReceptionCallBack )" );

}


void gkg::DicomServiceClassProvider::registerOnEndOfStudyCallback(
     gkg::DicomServiceClassProvider::OnEndOfStudyCallback onEndOfStudyCallback )
{

  try
  {

    _dicomServiceClassProviderImplementation->registerOnEndOfStudyCallback(
                                                         onEndOfStudyCallback );

  }
  GKG_CATCH( "void gkg::DicomServiceClassProvider::addOnEndOfStudyCallback( "
             "gkg::DicomServiceClassProvider::OnEndOfStudyCallback "
             "onEndOfStudyCallback )" );

}


void gkg::DicomServiceClassProvider::initializeNetwork()
{

  try
  {

    _dicomServiceClassProviderImplementation->initializeNetwork(); 

  }
  GKG_CATCH( "void gkg::DicomServiceClassProvider::initializeNetwork()" );

}


void gkg::DicomServiceClassProvider::run( bool verbose )
{

  try
  {

    _dicomServiceClassProviderImplementation->run( verbose ); 

  }
  GKG_CATCH( "void gkg::DicomServiceClassProvider::run( bool verbose )" );

}


void gkg::DicomServiceClassProvider::dropNetwork()
{

  try
  {

    _dicomServiceClassProviderImplementation->dropNetwork(); 

  }
  GKG_CATCH( "void gkg::DicomServiceClassProvider::dropNetwork()" );

}


const std::string& gkg::DicomServiceClassProvider::getTitle() const
{

  try
  {

    return _dicomServiceClassProviderImplementation->getTitle(); 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassProvider::getTitle() const" );

}


const std::string& gkg::DicomServiceClassProvider::getIncomingDirectory() const
{

  try
  {

    return _dicomServiceClassProviderImplementation->getIncomingDirectory(); 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassProvider::getIncomingDirectory() const" );

}


in_port_t gkg::DicomServiceClassProvider::getPort() const
{

  try
  {

    return _dicomServiceClassProviderImplementation->getPort(); 


  }
  GKG_CATCH( "int32_t gkg::DicomServiceClassProvider::getPort() const" );

}


gkg::DicomServiceClassProvider::TransferSyntax 
gkg::DicomServiceClassProvider::getTransferSyntax() const
{

  try
  {

    return _dicomServiceClassProviderImplementation->getTransferSyntax(); 


  }
  GKG_CATCH( "gkg::DicomServiceClassProvider::TransferSyntax "
             "gkg::DicomServiceClassProvider::getTransferSyntax() const" );

}


int32_t gkg::DicomServiceClassProvider::getACSETimeout() const
{

  try
  {

    return _dicomServiceClassProviderImplementation->getACSETimeout(); 


  }
  GKG_CATCH( "int32_t gkg::DicomServiceClassProvider::getACSETimeout() const" );

}


int32_t gkg::DicomServiceClassProvider::getEOSTimeout() const
{

  try
  {

    return _dicomServiceClassProviderImplementation->getEOSTimeout(); 


  }
  GKG_CATCH( "int32_t gkg::DicomServiceClassProvider::getEOSTimeout() const" );

}


gkg::DicomServiceClassProviderImplementation* 
gkg::DicomServiceClassProvider::getImplementation() const
{

  return _dicomServiceClassProviderImplementation;

}

