#include <gkg-core-io/DicomServiceClassUserImplementation.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomServiceClassUserImplementation::DicomServiceClassUserImplementation(
                      gkg::DicomServiceClassUser* dicomServiceClassUser,
                      const std::string& callingTitle,
                      const std::string& calledTitle,
		      const std::string& peerHostName,
                      in_port_t port,
                      gkg::DicomServiceClassUser::TransferSyntax transferSyntax,
                      int32_t acseTimeout )
                                         : _target( dicomServiceClassUser ),
                                           _callingTitle( callingTitle ),
                                           _calledTitle( calledTitle ),
					   _peerHostName( peerHostName ),
	                                   _port( port ),
	                                   _transferSyntax( transferSyntax ),
	                                   _acseTimeout( acseTimeout )
{
}


gkg::DicomServiceClassUserImplementation::~DicomServiceClassUserImplementation()
{
}


gkg::DicomServiceClassUser* 
gkg::DicomServiceClassUserImplementation::getTarget() const
{

  try
  {

    return _target;
    
  }
  GKG_CATCH( "gkg::DicomServiceClassUser* "
             "gkg::DicomServiceClassUserImplementation::getTarget() const" );

}


const std::string& 
gkg::DicomServiceClassUserImplementation::getCallingTitle() const
{

  try
  {
  
    return _callingTitle;
  
  }
  GKG_CATCH( "void "
             "gkg::DicomServiceClassUserImplementation::getCallingTitle() "
	     "const" );

}


const std::string& 
gkg::DicomServiceClassUserImplementation::getCalledTitle() const
{

  try
  {
  
    return _calledTitle;
  
  }
  GKG_CATCH( "void "
             "gkg::DicomServiceClassUserImplementation::getCalledTitle() "
	     "const" );

}


const std::string&
gkg::DicomServiceClassUserImplementation::getPeerHostName() const
{

  try
  {
  
    return _peerHostName;
  
  }
  GKG_CATCH( "void "
             "gkg::DicomServiceClassUserImplementation::getPeerHostName() "
	     "const" );

}


in_port_t gkg::DicomServiceClassUserImplementation::getPort() const
{

  try
  {

    return _port; 


  }
  GKG_CATCH( "int32_t "
             "gkg::DicomServiceClassUserImplementation::getPort() const" );

}


gkg::DicomServiceClassUser::TransferSyntax 
gkg::DicomServiceClassUserImplementation::getTransferSyntax() const
{

  try
  {

    return _transferSyntax; 


  }
  GKG_CATCH( "gkg::DicomServiceClassUser::TransferSyntax "
             "gkg::DicomServiceClassUserImplementation::getTransferSyntax() "
	     "const" );

}


int32_t gkg::DicomServiceClassUserImplementation::getACSETimeout() const
{

  try
  {

    return _acseTimeout; 


  }
  GKG_CATCH( "int32_t "
             "gkg::DicomServiceClassUserImplementation::getACSETimeout() "
	     "const" );

}
