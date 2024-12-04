#ifndef _ReorientationContext_h_
#define _ReorientationContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include "ProbabilityDensityFunction.h"
//#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <vector>

namespace gkg
{

class ReorientationContext : public LoopContext< int32_t >
{

  public:

    ReorientationContext(
         std::string& reorientationType,
         std::vector< gkg::Vector3d< int32_t > >& sitesVector,
         PdfCartesianField& pdfCartesianField,
         PdfCartesianField& otherPdfCartesianField,
         std::vector< bool >& otherFieldHasSiteBoolVector,
         Volume< float >& J11,
         Volume< float >& J12,
         Volume< float >& J13,
         Volume< float >& J21,
         Volume< float >& J22,
         Volume< float >& J23,
         Volume< float >& J31,
         Volume< float >& J32,
         Volume< float >& J33 );

    void doIt( int32_t startIndex, int32_t count );

  private:

  std::string& _reorientationType;
  std::vector< Vector3d< int32_t > >& _sitesVector;
  PdfCartesianField& _pdfCartesianField;
  PdfCartesianField& _otherPdfCartesianField;
  std::vector< bool >& _otherFieldHasSiteBoolVector;
  Volume< float >& _J11;
  Volume< float >& _J12;
  Volume< float >& _J13;
  Volume< float >& _J21;
  Volume< float >& _J22;
  Volume< float >& _J23;
  Volume< float >& _J31;
  Volume< float >& _J32;
  Volume< float >& _J33;

};


}


#endif

