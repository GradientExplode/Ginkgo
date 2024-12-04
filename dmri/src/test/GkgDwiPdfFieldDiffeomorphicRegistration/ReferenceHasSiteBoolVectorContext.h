#ifndef _ReferenceHasSiteBoolVectorContext_h_
#define _ReferenceHasSiteBoolVectorContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include "ProbabilityDensityFunction.h"
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <vector>

namespace gkg
{

class ReferenceHasSiteBoolVectorContext : public LoopContext< int32_t >
{

  public:

    ReferenceHasSiteBoolVectorContext( 
                     std::set< Vector3d< int32_t >, 
                               Vector3dCompare< int32_t > > hasSiteReference,
                     std::vector< bool >& referenceHasSiteBoolVector,
                     const std::vector< Vector3d< int32_t > >& floatingSites );

    void doIt( int32_t startIndex, int32_t count );

  private:

    std::set< Vector3d< int32_t >, 
              Vector3dCompare< int32_t > > _hasSiteReference;
    std::vector< bool >& _referenceHasSiteBoolVector;
    const std::vector< Vector3d< int32_t > >& _floatingSites;

};


}


#endif

