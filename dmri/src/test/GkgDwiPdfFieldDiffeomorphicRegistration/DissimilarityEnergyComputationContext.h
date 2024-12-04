#ifndef _DissimilarityEnergyComputationContext_h_
#define _DissimilarityEnergyComputationContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include "ProbabilityDensityFunction.h"
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <vector>
#include <gkg-processing-container/Volume_i.h>

namespace gkg
{

class DissimilarityEnergyComputationContext : public LoopContext< int32_t >
{

  public:

    DissimilarityEnergyComputationContext(                 
                PdfCartesianField& reference,
                PdfCartesianField& floating,
                std::vector< bool >& referenceHasSiteBoolVector,
                std::vector< Vector3d< int32_t > >& floatingSites,
                float& dissimilarityEnergy,
                int32_t& size );

    void doIt( int32_t startIndex, int32_t count );

  private:

      PdfCartesianField& _reference;
      PdfCartesianField& _floating;
      std::vector< bool >& _referenceHasSiteBoolVector;
      std::vector< Vector3d< int32_t > >& _floatingSites;
      float& _dissimilarityEnergy;
      int32_t& _size;


};


}


#endif

