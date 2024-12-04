#ifndef _gkg_core_io_DicomServiceClassProviderImplementation_h_
#define _gkg_core_io_DicomServiceClassProviderImplementation_h_


#include <gkg-core-io/DicomServiceClassProvider.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <list>


namespace gkg
{


class DicomServiceClassProviderImplementation
{

  public:

    virtual ~DicomServiceClassProviderImplementation();

    virtual DicomServiceClassProvider* getTarget() const;

    virtual void registerOnReceptionCallback(
           DicomServiceClassProvider::OnReceptionCallback onReceptionCallback );
    virtual void registerOnEndOfStudyCallback(
         DicomServiceClassProvider::OnEndOfStudyCallback onEndOfStudyCallback );

    virtual void initializeNetwork() = 0;
    virtual void run( bool verbose ) = 0;
    virtual void dropNetwork() = 0;

    virtual const std::string& getTitle() const;
    virtual const std::string& getIncomingDirectory() const;
    virtual in_port_t getPort() const;
    virtual DicomServiceClassProvider::TransferSyntax getTransferSyntax() const;
    virtual int32_t getACSETimeout() const;
    virtual int32_t getEOSTimeout() const;

  protected:

    DicomServiceClassProviderImplementation(
                       DicomServiceClassProvider* dicomServiceClassProvider,
                       const std::string& title,
                       const std::string& incomingDirectory,
                       in_port_t port,
                       DicomServiceClassProvider::TransferSyntax transferSyntax,
                       int32_t acseTimeout,
                       int32_t eosTimeout );

    DicomServiceClassProvider* _target;

    std::string _title;
    std::string _incomingDirectory;
    in_port_t _port;
    DicomServiceClassProvider::TransferSyntax _transferSyntax;
    int32_t _acseTimeout;
    int32_t _eosTimeout;

    std::list< DicomServiceClassProvider::OnReceptionCallback >
      _onReceptionCallbacks;
    std::list< DicomServiceClassProvider::OnEndOfStudyCallback >
      _onEndOfStudyCallbacks;

};


}


#endif
