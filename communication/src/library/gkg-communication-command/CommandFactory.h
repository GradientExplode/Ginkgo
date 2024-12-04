#ifndef _gkg_communication_command_CommandFactory_h_
#define _gkg_communication_command_CommandFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-communication-command/Command.h>
#include <gkg-core-object/Dictionary.h>

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>


struct noCaseCompLess /*: std::binary_function< std::string, std::string, bool >
                        deprecated in c++17 */
{

  typedef std::string first_argument_type;
  typedef std::string second_argument_type;
  typedef bool result_type;

  struct noCaseComp /*: public std::binary_function< unsigned char,
                                                   unsigned char,
                                                   bool >
                      deprecated in c++17 */
  {

    typedef unsigned char first_argument_type;
    typedef unsigned char second_argument_type;
    typedef bool result_type;

    bool operator () ( const unsigned char& c1, const unsigned char& c2 ) const
    {

      return tolower( c1 ) < tolower( c2 );

    }

  };

  bool operator () ( const std::string& s1, const std::string& s2 ) const
  {

    return std::lexicographical_compare( s1.begin(), s1.end(),
                                         s2.begin(), s2.end(),
                                         noCaseComp() );

  }

};


namespace gkg
{


class CommandFactory : public Singleton< CommandFactory >
{

  public:

    typedef Command* ( *CommandCreatorFromArgcArgv )( int32_t, char** );
    typedef Command* ( *CommandCreatorFromDictionary )( const Dictionary& );
    typedef std::string ( *CommandHelp )();

    bool registerCommand(
                    const std::string& name, 
                    CommandCreatorFromArgcArgv commandCreatorFromArgcArgv,
                    CommandCreatorFromDictionary commandCreatorFromDictionary,
                    CommandHelp commandHelp );

    Command* create( int32_t argc, char** argv ) const;
    Command* create( const std::string& name,
                     const Dictionary& parameters ) const;
    std::string getHelp( const std::string& name ) const;

    std::vector< std::string > getNames() const;

  protected:

    friend class Singleton< CommandFactory >;

    CommandFactory();

    std::map< std::string, CommandCreatorFromArgcArgv, noCaseCompLess >
      _commandCreatorsFromArgcArgv;
    std::map< std::string, CommandCreatorFromDictionary, noCaseCompLess >
      _commandCreatorsFromDictionary;
    std::map< std::string, CommandHelp, noCaseCompLess >
      _commandHelps;

};


}


#define RegisterCommandCreator( Name, Help )                                   \
static std::string help##Name()                                                \
{                                                                              \
                                                                               \
  return Help;                                                                 \
                                                                               \
}                                                                              \
                                                                               \
static bool registeredCommandCreator_##Name __attribute((unused)) =            \
  gkg::CommandFactory::getInstance().registerCommand(                          \
    gkg::Name::getStaticName(),                                                \
    gkg::Creator2Arg< gkg::Name, gkg::Command,                                 \
                      int32_t, char** >::createInstance,                       \
    gkg::Creator1Arg< gkg::Name, gkg::Command,                                 \
                    const gkg::Dictionary& >::createInstance,                  \
    help##Name  )


#endif
