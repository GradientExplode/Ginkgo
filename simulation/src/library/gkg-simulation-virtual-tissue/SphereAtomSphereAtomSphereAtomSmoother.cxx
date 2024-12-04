#include <gkg-simulation-virtual-tissue/AtomSmootherFactory.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-core-exception/Exception.h>



static void sphereAtomSphereAtomSphereAtomSmoother(
                                      const float* previousParameters,
                                      bool /*previousCompressedRepresentation*/,
                                      float* currentParameters,
                                      bool /*currentCompressedRepresentation*/,
                                      const float* nextParameters,
                                      bool /*nextCompressedRepresentation*/ )
{

  try
  {

    // smoothing operator simply consists in the modification of the current 
    // atom center C to replace it by the middle of the segment 
    //   ->   ->   ->           ->     ->
    // [ C; ( Cp + Cn ) / 2 ],  Cp and Cn corresponding to the previous and next
    // atom centers, thus yielding the equation:
    // ->         ->          ->    ->
    // C' = 0.5 x C + 0.5 x ( Cp + Cn ) / 2.0
    currentParameters[ 0 ] = 0.5 * currentParameters[ 0 ] +
                             0.25 * previousParameters[ 0 ] +
                             0.25 * nextParameters[ 0 ];

    currentParameters[ 1 ] = 0.5 * currentParameters[ 1 ] +
                             0.25 * previousParameters[ 1 ] +
                             0.25 * nextParameters[ 1 ];

    currentParameters[ 2 ] = 0.5 * currentParameters[ 2 ] +
                             0.25 * previousParameters[ 2 ] +
                             0.25 * nextParameters[ 2 ];

  }
  GKG_CATCH( "static void sphereAtomSphereAtomSphereAtomSmoother( "
             "float* previousParameters, "
             "float* currentParameters, "
             "float* nextParameters )" );


}


//
// adding static methods to atom smoother factory
//


static bool registerToAtomSmootherFactory __attribute__((unused)) = 
  gkg::AtomSmootherFactory::getInstance().registerAtomSmoother(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getStaticType(),
    sphereAtomSphereAtomSphereAtomSmoother );
