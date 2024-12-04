#ifndef _gkg_core_io_ByteOrder_h_
#define _gkg_core_io_ByteOrder_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class ByteOrder : public Singleton< ByteOrder >
{

  public:

    enum Type
    {

      LittleEndian,
      BigEndian

    };

    ~ByteOrder();

    Type getCurrentType() const;
    std::string getCurrentName() const;
    std::string getCurrentABCDCode() const;

    Type getTypeFromName( const std::string& name ) const;
    std::string getNameFromType( Type type ) const;

    Type getTypeFromABCDCode( const std::string& code ) const;
    std::string getABCDCodeFromType( Type type ) const;

  private:

    friend class Singleton< ByteOrder >;

    ByteOrder();

};



}


#endif
