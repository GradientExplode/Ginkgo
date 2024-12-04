#ifndef _gkg_dmri_fiber_selector_FiberSelectionChecker_h_
#define _gkg_dmri_fiber_selector_FiberSelectionChecker_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>
#include <set>
#include <map>
#include <string>


namespace gkg
{



class FiberSelectionChecker : public Singleton< FiberSelectionChecker >
{

  public:

    typedef void ( *SanityCheckFunctor )( const std::vector< std::string >&,
                                          const std::vector< double >& );

    ~FiberSelectionChecker();

    std::set< std::string > getNames() const;
    std::string getNameListHelp() const;
      
    const std::string& getStringParameterHelp( const std::string& name ) const;
    const std::string& getScalarParameterHelp( const std::string& name ) const;
    int32_t getStringParameterCount( const std::string& name ) const;
    int32_t getScalarParameterCount( const std::string& name ) const;
    void doSanityChecks( const std::string& name,
                         const std::vector< std::string >& stringParameters,
                         const std::vector< double >& scalarParameters ) const;


    void addChecker( const std::string& name,
                     const std::string& stringParameterHelp,
                     const std::string& scalarParameterHelp,
                     int32_t stringParameterCount,
                     int32_t scalarParameterCount,
                     SanityCheckFunctor sanityCheckFunctor );

  protected:

    friend class Singleton< FiberSelectionChecker >;

    FiberSelectionChecker();

    std::map< std::string, std::string > _stringParameterHelps;
    std::map< std::string, std::string > _scalarParameterHelps;
    std::map< std::string, int32_t > _stringParameterCounts;
    std::map< std::string, int32_t > _scalarParameterCounts;
    std::map< std::string, SanityCheckFunctor > _sanityCheckFunctors;

};


}


#endif
