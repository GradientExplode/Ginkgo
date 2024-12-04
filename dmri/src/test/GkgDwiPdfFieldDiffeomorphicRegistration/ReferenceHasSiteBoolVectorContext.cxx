#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <vector>
#include "ReferenceHasSiteBoolVectorContext.h"





gkg::ReferenceHasSiteBoolVectorContext::ReferenceHasSiteBoolVectorContext( 
                 std::set< gkg::Vector3d< int32_t >, 
                           gkg::Vector3dCompare< int32_t > > hasSiteReference,
                 std::vector< bool >& referenceHasSiteBoolVector,
                 const std::vector< gkg::Vector3d< int32_t > >& floatingSites )
                     :gkg::LoopContext< int32_t >(),
                      _hasSiteReference( hasSiteReference ),
                      _referenceHasSiteBoolVector( referenceHasSiteBoolVector ),
                      _floatingSites( floatingSites )
{
}


void gkg::ReferenceHasSiteBoolVectorContext::doIt( int32_t startIndex, 
                                                   int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;

    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > >::iterator h;

    int32_t o = 0;
    for( o = startIndex; o < stopIndex; ++o  )
    {

      h = _hasSiteReference.find( _floatingSites[ o ] );

      if( h != _hasSiteReference.end() )
      {

        _referenceHasSiteBoolVector[ o ] = true;

      }      

    }

  }
  GKG_CATCH( "void gkg::ReferenceHasSiteBoolVectorContext::"
             "doIt( int32_t startIndex," 
             "int32_t count )" );

}




