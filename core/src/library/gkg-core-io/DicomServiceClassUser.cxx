#include <gkg-core-io/DicomServiceClassUser.h>
#include <gkg-core-io/DicomServiceClassUserImplementation.h>
#include <gkg-core-io/DicomSelector.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomServiceClassUser::DicomServiceClassUser( 
                      const std::string& callingTitle,
                      const std::string& calledTitle,
		      const std::string& peerHostName,
                      in_port_t port,
                      gkg::DicomServiceClassUser::TransferSyntax transferSyntax,
                      int32_t acseTimeout )
{

  try
  {
  
    _dicomServiceClassUserImplementation = 
      gkg::DicomSelector::getInstance().getImplementationFactory()->
                      createDicomServiceClassUserImplementation( this,
	                                                         callingTitle,
	                                                         calledTitle,
								 peerHostName,
						                 port,
						                 transferSyntax,
						                 acseTimeout );
  
  }
  GKG_CATCH( "gkg::DicomServiceClassUser::DicomServiceClassUser( "
             "const std::string& callingTitle, "
             "const std::string& calledTitle, "
	     "const std::string& peerHostName, "
             "int32_t port, "
             "gkg::DicomServiceClassUser::TransferSyntax transferSyntax, "
             "int32_t acseTimeout )" );

}


gkg::DicomServiceClassUser::~DicomServiceClassUser()
{

  delete _dicomServiceClassUserImplementation;

}


void gkg::DicomServiceClassUser::initializeNetwork( 
                           gkg::DicomServiceClassUser::NetworkRole networkRole )
{

  try
  {
  
    _dicomServiceClassUserImplementation->initializeNetwork( networkRole );    
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::initializeNetwork( "
             "gkg::DicomServiceClassUser::NetworkRole networkRole )" );

}


void gkg::DicomServiceClassUser::store( std::list< std::string >& fileNameList, 
                                        bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->store( fileNameList, verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::store( "
             "std::list< std::string >& fileNameList, bool verbose )" );

}


void gkg::DicomServiceClassUser::get( std::list< std::string >& fileNameList, 
                                      bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->get( fileNameList, verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::get( "
             "std::list< std::string >& fileNameList, bool verbose )" );

}


void gkg::DicomServiceClassUser::find( std::list< std::string >& fileNameList, 
                                       bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->find( fileNameList, verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::find( "
             "std::list< std::string >& fileNameList, bool verbose )" );

}


void gkg::DicomServiceClassUser::move( std::list< std::string >& fileNameList, 
                                       bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->move( fileNameList, verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::move( "
             "std::list< std::string >& fileNameList, bool verbose )" );

}


void gkg::DicomServiceClassUser::echo( uint32_t repeatCount, bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->echo( repeatCount, verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::echo( "
             "uint32_t repeatCount, bool verbose )" );

}


void gkg::DicomServiceClassUser::cancel( bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->cancel( verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::cancel( bool verbose )" );

}


void gkg::DicomServiceClassUser::shutdown( bool verbose )
{

  try
  {
  
    _dicomServiceClassUserImplementation->shutdown( verbose );
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::shutdown( bool verbose )" );

}


void gkg::DicomServiceClassUser::dropNetwork()
{

  try
  {
  
    _dicomServiceClassUserImplementation->dropNetwork();
  
  }
  GKG_CATCH( "void gkg::DicomServiceClassUser::dropNetwork()" );

}


const std::string& gkg::DicomServiceClassUser::getCallingTitle() const
{

  try
  {

    return _dicomServiceClassUserImplementation->getCallingTitle(); 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassUser::getCallingTitle() const" );

}


const std::string& gkg::DicomServiceClassUser::getCalledTitle() const
{

  try
  {

    return _dicomServiceClassUserImplementation->getCalledTitle(); 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassUser::getCalledTitle() const" );

}


const std::string& gkg::DicomServiceClassUser::getPeerHostName() const
{

  try
  {

    return _dicomServiceClassUserImplementation->getPeerHostName(); 


  }
  GKG_CATCH( "const std::string& "
             "gkg::DicomServiceClassUser::getPeerHostName() const" );

}


in_port_t gkg::DicomServiceClassUser::getPort() const
{

  try
  {

    return _dicomServiceClassUserImplementation->getPort(); 


  }
  GKG_CATCH( "int32_t gkg::DicomServiceClassUser::getPort() const" );

}


gkg::DicomServiceClassUser::TransferSyntax 
gkg::DicomServiceClassUser::getTransferSyntax() const
{

  try
  {

    return _dicomServiceClassUserImplementation->getTransferSyntax(); 


  }
  GKG_CATCH( "gkg::DicomServiceClassUser::TransferSyntax "
             "gkg::DicomServiceClassUser::getTransferSyntax() const" );

}


int32_t gkg::DicomServiceClassUser::getACSETimeout() const
{

  try
  {

    return _dicomServiceClassUserImplementation->getACSETimeout(); 


  }
  GKG_CATCH( "int32_t gkg::DicomServiceClassUser::getACSETimeout() const" );

}


gkg::DicomServiceClassUserImplementation* 
gkg::DicomServiceClassUser::getImplementation() const
{

  return _dicomServiceClassUserImplementation;

}

