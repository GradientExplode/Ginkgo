#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>
#include <vector>
#include "ProbabilityDensityFunction.h"
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include "ReorientationContext.h"

gkg::ReorientationContext::ReorientationContext(
         std::string& reorientationType,
         std::vector< gkg::Vector3d< int32_t > >& sitesVector,
         gkg::PdfCartesianField& pdfCartesianField,
         gkg::PdfCartesianField& otherPdfCartesianField,
         std::vector< bool >& otherFieldHasSiteBoolVector,
         gkg::Volume< float >& J11,
         gkg::Volume< float >& J12,
         gkg::Volume< float >& J13,
         gkg::Volume< float >& J21,
         gkg::Volume< float >& J22,
         gkg::Volume< float >& J23,
         gkg::Volume< float >& J31,
         gkg::Volume< float >& J32,
         gkg::Volume< float >& J33 )
                    : gkg::LoopContext< int32_t >(),
                    _reorientationType( reorientationType ),
                    _sitesVector( sitesVector ),
                    _pdfCartesianField( pdfCartesianField ),
                    _otherPdfCartesianField( otherPdfCartesianField ),
                    _otherFieldHasSiteBoolVector( otherFieldHasSiteBoolVector ),
                    _J11( J11 ),
                    _J12( J12 ),
                    _J13( J13 ),
                    _J21( J21 ),
                    _J22( J22 ),
                    _J23( J23 ),
                    _J31( J31 ),
                    _J32( J32 ),
                    _J33( J33 )
{
}


void gkg::ReorientationContext::doIt( int32_t startIndex, 
                                      int32_t count )
{

  try
  {

    lock();

    int32_t stopIndex = startIndex + count;

    gkg::Matrix jacobianMatrix( 3, 3 );

    int32_t o = 0;
    for ( o = startIndex; o < stopIndex; ++o  )
    {

      const gkg::Vector3d< int32_t >& site = _sitesVector[ o ];

      jacobianMatrix( 0, 0 ) = _J11( site );
      jacobianMatrix( 0, 1 ) = _J12( site );
      jacobianMatrix( 0, 2 ) = _J13( site );
      jacobianMatrix( 1, 0 ) = _J21( site );
      jacobianMatrix( 1, 1 ) = _J22( site );
      jacobianMatrix( 1, 2 ) = _J23( site );
      jacobianMatrix( 2, 0 ) = _J31( site );
      jacobianMatrix( 2, 1 ) = _J32( site );
      jacobianMatrix( 2, 2 ) = _J33( site );

      if ( _otherFieldHasSiteBoolVector[ o ] )
      {

        if ( _reorientationType == "affine" )
        {

          _pdfCartesianField.getItem( site )->affineReorientation(
                                   jacobianMatrix,
                                 *( _otherPdfCartesianField.getItem( site ) ) );
 
        }
        else if ( _reorientationType == "finite-strain" )
        {

          _pdfCartesianField.getItem( site )->finiteStrainReorientation(
                                   jacobianMatrix,
                                 *( _otherPdfCartesianField.getItem( site ) ) );          

        }

      }

    }

    unlock();

  }
  GKG_CATCH( "void gkg::ReorientationContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}





