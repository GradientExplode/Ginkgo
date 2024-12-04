#ifndef _gkg_core_io_DicomServiceClassUserImplementation_h_
#define _gkg_core_io_DicomServiceClassUserImplementation_h_


#include <gkg-core-io/DicomServiceClassUser.h>
#include <gkg-core-cppext/NetInetIn.h>


namespace gkg
{


class DicomServiceClassUserImplementation
{

  public:
  
    virtual ~DicomServiceClassUserImplementation();
    
    virtual DicomServiceClassUser* getTarget() const;
    
    virtual void initializeNetwork( DicomServiceClassUser::NetworkRole 
				                              networkRole ) = 0;
    virtual void store( std::list< std::string >& fileNameList, 
                        bool verbose ) = 0;
    virtual void get( std::list< std::string >& fileNameList, 
                      bool verbose ) = 0;
    virtual void find( std::list< std::string >& fileNameList, 
                       bool verbose ) = 0;
    virtual void move( std::list< std::string >& fileNameList, 
                       bool verbose ) = 0;
    virtual void echo( uint32_t repeatCount, bool verbose ) = 0;
    virtual void cancel( bool verbose ) = 0;
    virtual void shutdown( bool verbose ) = 0;
    virtual void dropNetwork() = 0;
    
    virtual const std::string& getCallingTitle() const;
    virtual const std::string& getCalledTitle() const;
    virtual const std::string& getPeerHostName() const;
    virtual in_port_t getPort() const;
    virtual DicomServiceClassUser::TransferSyntax getTransferSyntax() const;
    virtual int32_t getACSETimeout() const;
    
  protected:
  
    DicomServiceClassUserImplementation(
                           DicomServiceClassUser* dicomServiceClassUser,
	                   const std::string& callingTitle,
	                   const std::string& calledTitle,
			   const std::string& peerHostName,
	                   in_port_t port,
	                   DicomServiceClassUser::TransferSyntax transferSyntax,
	                   int32_t acseTimeout );
			   
    DicomServiceClassUser* _target;
    
    std::string _callingTitle;
    std::string _calledTitle;
    std::string _peerHostName;
    in_port_t _port;
    DicomServiceClassUser::TransferSyntax _transferSyntax;
    int32_t _acseTimeout;

};


}


#endif
