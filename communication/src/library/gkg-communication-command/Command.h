#ifndef _gkg_communication_command_Command_h_
#define _gkg_communication_command_Command_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-object/Dictionary.h>


namespace gkg
{


class Command
{

  public:

    virtual ~Command();

  protected:

    Command();
    Command( int32_t argc, char* argv[], 
             bool loadPlugin = false, bool removeFirst = true );
    Command( const Dictionary& parameters );

    void clear();
    void filter( int32_t argc, char* argv[], bool removeFirst );
    void initialize();

    virtual void parse() = 0;

    void getString( const Dictionary& parameters,
                    const std::string& parameterName,
                    std::string& result ) const;
    void getBoolean( const Dictionary& parameters,
                     const std::string& parameterName,
                     bool& result ) const;
    template < class T >
    void getInteger( const Dictionary& parameters,
                     const std::string& parameterName,
                     T& result ) const;
    template < class T >
    void getFloating( const Dictionary& parameters,
                      const std::string& parameterName,
                      T& result ) const;
    void getVectorOfStrings( const Dictionary& parameters,
                             const std::string& parameterName,
                             std::vector< std::string >& result ) const;
    template < class T >
    void getVectorOfIntegers( const Dictionary& parameters,
                              const std::string& parameterName,
                              std::vector< T >& result ) const;
    template < class T >
    void getVectorOfFloatings( const Dictionary& parameters,
                               const std::string& parameterName,
                               std::vector< T >& result ) const;
    void getListOfStrings( const Dictionary& parameters,
                           const std::string& parameterName,
                           std::list< std::string >& result ) const;
    template < class T >
    void getListOfIntegers( const Dictionary& parameters,
                            const std::string& parameterName,
                            std::list< T >& result ) const;
    template < class T >
    void getListOfFloatings( const Dictionary& parameters,
                             const std::string& parameterName,
                             std::list< T >& result ) const;

    int32_t _argc;
    char** _argv;
    bool _loadPlugin;

};


}


#define DECLARE_STRING_PARAMETER( dictionary, type, name ) \
type name;                                                 \
this->getString( dictionary, #name, name )

#define DECLARE_BOOLEAN_PARAMETER( dictionary, type, name ) \
type name;                                                  \
this->getBoolean( dictionary, #name, name )

#define DECLARE_INTEGER_PARAMETER( dictionary, type, name ) \
type name;                                                  \
this->getInteger( dictionary, #name, name )

#define DECLARE_FLOATING_PARAMETER( dictionary, type, name ) \
type name;                                                   \
this->getFloating( dictionary, #name, name )

#define DECLARE_VECTOR_OF_STRINGS_PARAMETER( dictionary, type, name ) \
type name;                                                            \
this->getVectorOfStrings( dictionary, #name, name )

#define DECLARE_VECTOR_OF_INTEGERS_PARAMETER( dictionary, type, name ) \
type name;                                                             \
this->getVectorOfIntegers( dictionary, #name, name )

#define DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( dictionary, type, name ) \
type name;                                                              \
this->getVectorOfFloatings( dictionary, #name, name )

#define DECLARE_LIST_OF_STRINGS_PARAMETER( dictionary, type, name ) \
type name;                                                          \
this->getListOfStrings( dictionary, #name, name )

#define DECLARE_LIST_OF_INTEGERS_PARAMETER( dictionary, type, name ) \
type name;                                                           \
this->getListOfIntegers( dictionary, #name, name )

#define DECLARE_LIST_OF_FLOATINGS_PARAMETER( dictionary, type, name ) \
type name;                                                            \
this->getListOfFloatings( dictionary, #name, name )


#define DECLARE_NO_PARAMETER_HELP \
std::string( "no parameter" ) + "\n"

#define DECLARE_STRING_PARAMETER_HELP( name ) \
std::string( #name ) + " -> str\n"

#define DECLARE_BOOLEAN_PARAMETER_HELP( name ) \
std::string( #name ) + " -> bool\n"

#define DECLARE_INTEGER_PARAMETER_HELP( name ) \
std::string( #name ) + " -> int\n"

#define DECLARE_FLOATING_PARAMETER_HELP( name ) \
std::string( #name ) + " -> float\n"

#define DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( name ) \
std::string( #name ) + " -> tuple of str\n"

#define DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( name ) \
std::string( #name ) + " -> tuple of int\n"

#define DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( name ) \
std::string( #name ) + " -> tuple of float\n"

#define DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( name ) \
std::string( #name ) + " -> tuple of str\n"

#define DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( name ) \
std::string( #name ) + " -> tuple of int\n"

#define DECLARE_LIST_OF_FLOATINGS_PARAMETER_HELP( name ) \
std::string( #name ) + " -> tuple of float\n"

#endif
