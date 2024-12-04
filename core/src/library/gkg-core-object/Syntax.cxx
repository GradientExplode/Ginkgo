#include <gkg-core-object/Syntax.h>


gkg::Semantic::Semantic()
              : needed( false ),
                internal( false )
{
}


gkg::Semantic::Semantic( const std::string& theType,
                         bool theNeeded,
                         bool theInternal )
              : type( theType ),
                needed( theNeeded ),
                internal( theInternal )
{
}
