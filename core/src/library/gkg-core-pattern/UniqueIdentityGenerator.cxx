#include <gkg-core-pattern/UniqueIdentityGenerator.h>
#include <gkg-core-exception/Exception.h>


gkg::UniqueIdentityGenerator::UniqueIdentityGenerator()
                             : _count( 0U )
{
}


gkg::UniqueIdentityGenerator::~UniqueIdentityGenerator()
{
}


uint64_t gkg::UniqueIdentityGenerator::getIdentity()
{

  try
  {

    uint64_t id = _count;
    ++ _count;
    return id;

  }
  GKG_CATCH( "uint64_t gkg::UniqueIdentityGenerator::getIdentity()" );

}


void gkg::UniqueIdentityGenerator::reset()
{

  try
  {

    _count = 0U;

  }
  GKG_CATCH( "void gkg::UniqueIdentityGenerator::reset()" );

}

