#ifndef _gkg_communication_log_HistoryWriter_h_
#define _gkg_communication_log_HistoryWriter_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-communication-log/InfoWriter.h>


namespace gkg
{


class HistoryWriter : public Singleton< HistoryWriter >,
                      public InfoWriter
{

  public:

    void initialize( const std::string& applicationName );

    void activate( bool active );
    bool isActive();

    void add( const std::string& text );
    void add( const std::string& text,
              const std::string& fileName, 
              char* data, 
              size_t size );

    template < class T >
    void add( const std::string& text, T value, bool newLine = true );

  protected:

    friend class Singleton< HistoryWriter >;

    HistoryWriter();

    void clearOldDataDirectories();
    void addDate( std::ofstream& stream );

    bool _first;
    bool _active;
    std::string _dataDirectoryName;

};


}


#endif
