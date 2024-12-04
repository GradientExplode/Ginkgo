#ifndef _gkg_dmri_odf_functor_OdfFunctorDecompositionSH_h_
#define _gkg_dmri_odf_functor_OdfFunctorDecompositionSH_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
//#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class OdfFunctorDecompositionSH :
                           public OdfFunctor< Volume< float >, float >,
                           public Singleton< OdfFunctorDecompositionSH >
{

  public:

    ~OdfFunctorDecompositionSH();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< float >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorDecompositionSH >;

    OdfFunctorDecompositionSH();

    Matrix getReconstructionMatrix( const OrientationSet& orientationSet );
};


}


#endif
