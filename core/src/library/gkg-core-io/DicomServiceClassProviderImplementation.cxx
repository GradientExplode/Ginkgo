#include <gkg-core-io/DicomServiceClassProviderImplementation.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomServiceClassProviderImplementation::
                                        DicomServiceClassProviderImplementation( 
                      gkg::DicomServiceClassProvider* dicomServiceClassProvider,
                      const std::string& title,
                      const std::string& incomingDirectory,
                      in_port_t port,
                      gkg::DicomServiceClassProvider::TransferSyntax transferSyntax,
                      int32_t acseTimeout,
                      int32_t eosTimeout )
                    : _target( dicomServiceClassProvider ),
                      _title( title ),
                      _incomingDirectory( incomingDirectory ),
                      _port( port ),
                      _transferSyntax( transferSyntax ),
                      _acseTimeout( acseTimeout ),
                      _eosTimeout( eosTimeout )
{
}


gkg::DicomServiceClassProviderImplementation::
                                      ~DicomServiceClassProviderImplementation()
{
}


gkg::DicomServiceClassProvider* 
gkg::DicomServiceClassProviderImplementation::getTarget() const
{

  return _target;

}


void gkg::DicomServiceClassProviderImplementation::registerOnReceptionCallback(
       gkg::DicomServiceClassProvider::OnReceptionCallback onReceptionCallback )
{

  try
  {

    _onReceptionCallbacks.push_back( onReceptionCallback );

  }
  GKG_CATCH( "void gkg::DicomServiceClassProviderImplementation::"
             "registerOnReceptionCallback( "
             "gkg::DicomServiceClassProvider::OnReceptionCallback "
             "onReceptionCallback )" );
}


void gkg::DicomServiceClassProviderImplementation::registerOnEndOfStudyCallback(
     gkg::DicomServiceClassProvider::OnEndOfStudyCallback onEndOfStudyCallback )
{

  try
  {

    _onEndOfStudyCallbacks.push_back( onEndOfStudyCallback );

  }
  GKG_CATCH( "void gkg::DicomServiceClassProviderImplementation::"
             "registeOnEndOfStudyCallback( "
             "gkg::DicomServiceClassProvider::OnEndOfStudyCallback "
             "onEndOfStudyCallback )" );

}


const std::string& 
gkg::DicomServiceClassProviderImplementation::getTitle() const
{

  try
  {

    return _title; 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassProviderImplementation::getTitle() const" );

}


const std::string& 
gkg::DicomServiceClassProviderImplementation::getIncomingDirectory() const
{

  try
  {

    return _incomingDirectory; 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassProviderImplementation::"
             "getIncomingDirectory() const" );

}


in_port_t gkg::DicomServiceClassProviderImplementation::getPort() const
{

  try
  {

    return _port; 


  }
  GKG_CATCH( "int32_t "
             "gkg::DicomServiceClassProviderImplementation::getPort() const" );

}


gkg::DicomServiceClassProvider::TransferSyntax 
gkg::DicomServiceClassProviderImplementation::getTransferSyntax() const
{

  try
  {

    return _transferSyntax; 


  }
  GKG_CATCH( "gkg::DicomServiceClassProvider::TransferSyntax "
             "gkg::DicomServiceClassProviderImplementation::"
             "getTransferSyntax() const" );

}


int32_t gkg::DicomServiceClassProviderImplementation::getACSETimeout() const
{

  try
  {

    return _acseTimeout; 


  }
  GKG_CATCH( "int32_t "
             "gkg::DicomServiceClassProviderImplementation::"
             "getACSETimeout() const" );

}


int32_t gkg::DicomServiceClassProviderImplementation::getEOSTimeout() const
{

  try
  {

    return _eosTimeout; 


  }
  GKG_CATCH( "int32_t "
             "gkg::DicomServiceClassProviderImplementation::"
             "getEOSTimeout() const" );

}

