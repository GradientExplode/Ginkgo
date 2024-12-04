#ifndef _gkg_processing_process_Process_h_
#define _gkg_processing_process_Process_h_


#include <string>
#include <map>


namespace gkg
{


class AnalyzedObject;

class Process
{

  public:

    typedef void ( *Function )( Process&,
                                const std::string&,
                                const AnalyzedObject&,
                                const std::string& );

    virtual ~Process();

    void execute( const std::string& name );
    void execute( const std::string& name,
                  const AnalyzedObject& analyzedObject,
                  const std::string& format );

    void registerProcess( const std::string& objectType,
                          const std::string& itemType,
                          Function function );

  protected:

    Process( const std::string& wantedObjectType = "" );

    std::string _wantedObjectType;
    std::map< std::string, std::map< std::string, Function > > _functions;

};


}


#endif
