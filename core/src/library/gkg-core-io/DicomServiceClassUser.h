#ifndef _gkg_core_io_DicomServiceClassUser_h_
#define _gkg_core_io_DicomServiceClassUser_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <string>
#include <list>


namespace gkg
{


class DicomServiceClassUserImplementation;


class DicomServiceClassUser
{

  public:
  
    enum TransferSyntax
    {

      Uncompressed,
      Implicit,
      Little,
      Big,
      Lossless,
      Jpeg8,
      Jpeg12,
      J2kLossless,
      J2kLossy,
      Rle,
      Deflated

    };
    
    enum NetworkRole
    {
    
      NetRequestor,
      NetAcceptorRequestor
    
    };

    DicomServiceClassUser( const std::string& callingTitle,
                           const std::string& calledTitle,
			   const std::string& peerHostName,
                           in_port_t port,
			   TransferSyntax transferSyntax,
			   int32_t acseTimeout = 30 );
    virtual ~DicomServiceClassUser();
    
    void initializeNetwork( NetworkRole networkRole = NetRequestor );
    void store( std::list< std::string >& fileNameList, bool verbose = false );
    void get( std::list< std::string >& fileNameList, bool verbose = false );
    void find( std::list< std::string >& fileNameList, bool verbose = false );
    void move( std::list< std::string >& fileNameList, bool verbose = false );
    void echo( uint32_t repeatCount, bool verbose = false );
    void cancel( bool verbose = false );
    void shutdown( bool verbose = false );
    void dropNetwork();
    
    const std::string& getCallingTitle() const;
    const std::string& getCalledTitle() const;
    const std::string& getPeerHostName() const;
    in_port_t getPort() const;
    TransferSyntax getTransferSyntax() const;
    int32_t getACSETimeout() const;
    
    DicomServiceClassUserImplementation* getImplementation() const;
    
  protected:
  
    DicomServiceClassUserImplementation* _dicomServiceClassUserImplementation;

};


}


#endif
