#ifndef _SetStandardBasisTypeContext_h_
#define _SetStandardBasisTypeContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include "ProbabilityDensityFunction.h"
//#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <vector>

namespace gkg
{

class SetStandardBasisTypeContext : public LoopContext< int32_t >
{

  public:

    SetStandardBasisTypeContext( 
        std::vector< Vector3d< int32_t > >& sitesVector,
        PdfCartesianField& pdfCartesianfield,
        std::map< float, gkg::OrientationSet >& outputMultipleShellSampling );

    void doIt( int32_t startIndex, int32_t count );

  private:

  std::vector< Vector3d< int32_t > >& _sitesVector;
  PdfCartesianField& _pdfCartesianField;
  std::map< float, gkg::OrientationSet >& _outputMultipleShellSampling;

};


}


#endif

