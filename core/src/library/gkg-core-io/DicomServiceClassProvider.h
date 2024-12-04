#ifndef _gkg_core_io_DicomServiceClassProvider_h_
#define _gkg_core_io_DicomServiceClassProvider_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <string>


namespace gkg
{


class DicomServiceClassProviderImplementation;


class DicomServiceClassProvider
{

  public:

    typedef void ( *OnReceptionCallback )( const std::string& imageFileName,
                                           bool verbose );
    typedef void ( *OnEndOfStudyCallback )( const std::string& folderName,
                                            bool verbose );


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

    DicomServiceClassProvider( const std::string& title,
                      const std::string& incomingDirectory,
                      in_port_t port,
                      TransferSyntax transferSyntax,
                      int32_t acseTimeout = 30,
                      int32_t eosTimeout = 5 );
    virtual ~DicomServiceClassProvider();

    void registerOnReceptionCallback( OnReceptionCallback onReceptionCallback );
    void registerOnEndOfStudyCallback(
                                    OnEndOfStudyCallback onEndOfStudyCallback );

    void initializeNetwork();
    void run( bool verbose = false );
    void dropNetwork();

    const std::string& getTitle() const;
    const std::string& getIncomingDirectory() const;
    in_port_t getPort() const;
    TransferSyntax getTransferSyntax() const;
    int32_t getACSETimeout() const;
    int32_t getEOSTimeout() const;

    DicomServiceClassProviderImplementation* getImplementation() const;

  protected:

    DicomServiceClassProviderImplementation* 
      _dicomServiceClassProviderImplementation;

};


}


#endif
