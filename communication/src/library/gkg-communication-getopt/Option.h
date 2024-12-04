#ifndef _gkg_communication_getopt_Option_h_
#define _gkg_communication_getopt_Option_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>
#include <deque>


namespace gkg
{


class Option
{

  public:

    virtual ~Option();

    const std::string& getName() const;
    std::string getNames() const;
    bool belongToNames( const std::string& name ) const;

    virtual std::string getInformation() const = 0;
    virtual bool recognize( const std::string& name ) const = 0;
    virtual bool feed( const std::string& value ) = 0;
    virtual void check() const = 0;


  protected:

    friend class OptionParser;

    Option( const std::string& name, const std::string& information );

    void addName( const std::string& name );

    std::deque< std::string > _names;
    std::string _information;

};


}



#endif
