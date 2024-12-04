#ifndef _gkg_communication_log_LogWriter_h_
#define _gkg_communication_log_LogWriter_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-communication-log/InfoWriter.h>


namespace gkg
{


class LogWriter : public Singleton< LogWriter >,
                  public InfoWriter
{

  public:

    void add( const std::string& text );

  protected:

    friend class Singleton< LogWriter >;

    LogWriter();

};


}


#endif
