#ifndef _gkg_dmri_odf_functor_OdfFunctorStationaryFraction_h_
#define _gkg_dmri_odf_functor_OdfFunctorStationaryFraction_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorStationaryFraction :
                           public OdfFunctor< Volume< float >, float >,
                           public Singleton< OdfFunctorStationaryFraction >
{

  public:

    ~OdfFunctorStationaryFraction();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< float >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& /*scalarParameters*/,
                  const std::vector< std::string >& /*stringParameters*/,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorStationaryFraction >;

    OdfFunctorStationaryFraction();

};


}


#endif
