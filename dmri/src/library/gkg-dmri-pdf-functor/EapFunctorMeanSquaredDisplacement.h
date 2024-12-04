#ifndef _gkg_dmri_pdf_functor_EapFunctorMeanSquaredDisplacement_h_
#define _gkg_dmri_pdf_functor_EapFunctorMeanSquaredDisplacement_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class EapFunctorMeanSquaredDisplacement :
                           public EapFunctor< Volume< float >, float >,
                           public Singleton< EapFunctorMeanSquaredDisplacement >
{

  public:

    ~EapFunctorMeanSquaredDisplacement();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< float >& output,
                  EapCartesianField& eapCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< EapFunctorMeanSquaredDisplacement >;

    EapFunctorMeanSquaredDisplacement();

};


}


#endif



