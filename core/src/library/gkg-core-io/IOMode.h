#ifndef _gkg_core_io_IOMode_h_
#define _gkg_core_io_IOMode_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class IOMode : public Singleton< IOMode >
{

  public:

    enum Type
    {

      Ascii,
      Binary

    };

    ~IOMode();

    Type getTypeFromName( const std::string& name ) const;
    std::string getNameFromType( Type type ) const;

  private:

    friend class Singleton< IOMode >;

    IOMode();

};



}


#endif
