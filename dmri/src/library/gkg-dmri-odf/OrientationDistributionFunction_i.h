#ifndef _gkg_dmri_odf_OrientationDistributionFunction_i_h_
#define _gkg_dmri_odf_OrientationDistributionFunction_i_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class Compare >
inline
void gkg::OrientationDistributionFunction::addSymmetricalMesh(
                          const gkg::Transform3d< float >& transform,
                          const R& rank,
                          gkg::MeshMap< R, float, 3U, Compare >& meshMap )const
{

  try
  {

    _orientationSet->addSymmetricalMesh( transform,
                                         rank,
                                         getOrientationProbabilities(),
                                         meshMap );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationDistributionFunction::addSymmetricalMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}





#endif
