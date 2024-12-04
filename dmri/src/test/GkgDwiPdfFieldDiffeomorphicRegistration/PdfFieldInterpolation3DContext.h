#ifndef _PdfFieldInterpolation3DContext_h_
#define _PdfFieldInterpolation3DContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include "ProbabilityDensityFunction.h"
//#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <vector>

namespace gkg
{

class PdfFieldInterpolation3DContext : public LoopContext< int32_t >
{

  public:

    PdfFieldInterpolation3DContext( 
                   gkg::PdfCartesianField& warpedField,
                   gkg::PdfCartesianField& inputField,
                   std::set< gkg::Vector3d< int32_t >, 
                             gkg::Vector3dCompare< int32_t > >& hasSiteWarped,
                   std::set< gkg::Vector3d< int32_t >, 
                             gkg::Vector3dCompare< int32_t > >& hasSiteInput,
                   std::vector< gkg::Vector3d< int32_t > >& inputFieldSites,
                   gkg::Volume< float >& flowFieldX, 
                   gkg::Volume< float >& flowFieldY, 
                   gkg::Volume< float >& flowFieldZ, 
                   gkg::Vector3d< int32_t >& inputFieldSize,
                   bool& addIntegerCoordinates,
                   int32_t& index );

    void doIt( int32_t startIndex, int32_t count );

  private:

    gkg::PdfCartesianField& _warpedField;
    gkg::PdfCartesianField& _inputField;
    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > >& _hasSiteWarped;
    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > >& _hasSiteInput;
    std::vector< gkg::Vector3d< int32_t > >& _inputFieldSites;
    gkg::Volume< float >& _flowFieldX; 
    gkg::Volume< float >& _flowFieldY; 
    gkg::Volume< float >& _flowFieldZ; 
    gkg::Vector3d< int32_t >& _inputFieldSize;
    bool& _addIntegerCoordinates;
    int32_t& _index;

};


}


#endif

