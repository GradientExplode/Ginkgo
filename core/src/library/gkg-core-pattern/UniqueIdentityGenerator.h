#ifndef _gkg_core_pattern_UniqueIdentityGenerator_h_
#define _gkg_core_pattern_UniqueIdentityGenerator_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class UniqueIdentityGenerator
{

  public:

    UniqueIdentityGenerator();
    virtual ~UniqueIdentityGenerator();

    uint64_t getIdentity();
    void reset();

  protected:

    uint64_t _count;

};


}


#endif
