#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>
#include <vector>
//#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include "ProbabilityDensityFunction.h"
#include "SetStandardBasisTypeContext.h"

gkg::SetStandardBasisTypeContext::SetStandardBasisTypeContext(
         std::vector< gkg::Vector3d< int32_t > >& sitesVector,
         gkg::PdfCartesianField& pdfCartesianField,
         std::map< float, gkg::OrientationSet >& outputMultipleShellSampling )
                    : gkg::LoopContext< int32_t >(),
                    _sitesVector( sitesVector ),
                    _pdfCartesianField( pdfCartesianField ),
                    _outputMultipleShellSampling( outputMultipleShellSampling )
{
}


void gkg::SetStandardBasisTypeContext::doIt( int32_t startIndex, 
                                             int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;

    int32_t o = 0;
    for ( o = startIndex; o < stopIndex; ++o  )
    {

      const gkg::Vector3d< int32_t >& site = _sitesVector[ o ];

      _pdfCartesianField.getItem( site )->setOutputSampling( 
                                               &_outputMultipleShellSampling );
 
      _pdfCartesianField.getItem( site )->setStandardBasisType();


    }

  }
  GKG_CATCH( "void gkg::SetStandardBasisTypeContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}





