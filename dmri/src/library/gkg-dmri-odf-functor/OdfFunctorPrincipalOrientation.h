#ifndef _gkg_dmri_odf_functor_OdfFunctorPrincipalOrientation_h_
#define _gkg_dmri_odf_functor_OdfFunctorPrincipalOrientation_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorPrincipalOrientation :
                                public OdfFunctor< Volume< Vector3d< float > >,
                                                   Vector3d< float > >,
                                public Singleton< OdfFunctorPrincipalOrientation >
{

  public:

    ~OdfFunctorPrincipalOrientation();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorPrincipalOrientation >;

    OdfFunctorPrincipalOrientation();

    void processItem(
                 Vector3d< float >& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif
