#ifndef _gkg_communication_getopt_SingleOption_h_
#define _gkg_communication_getopt_SingleOption_h_


#include <gkg-communication-getopt/Option.h>


namespace gkg
{


template < class T >
class SingleOption : public Option
{

  public:

    virtual ~SingleOption();

    std::string getInformation() const;
    bool recognize( const std::string& name ) const;
    bool feed( const std::string& value );
    void check() const;

  protected:

    friend class OptionParser;

    SingleOption( const std::string& name,
                  const std::string& information,
                  T& value,
                  bool optional );

    T& _value;
    bool _optional;
    bool _filled;

};


}


#endif
