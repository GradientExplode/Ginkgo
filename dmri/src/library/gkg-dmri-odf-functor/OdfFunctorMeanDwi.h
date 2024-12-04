#ifndef _gkg_dmri_odf_functor_OdfFunctorMeanDwi_h_
#define _gkg_dmri_odf_functor_OdfFunctorMeanDwi_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class OdfFunctorMeanDwi : public OdfFunctor< Volume< float >, float >,
                          public Singleton< OdfFunctorMeanDwi >
{

  public:

    ~OdfFunctorMeanDwi();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< float >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorMeanDwi >;

    OdfFunctorMeanDwi();

    void processItem(
                 float& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif
