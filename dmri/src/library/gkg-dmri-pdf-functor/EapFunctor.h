#ifndef _gkg_dmri_pdf_functor_EapFunctor_h_
#define _gkg_dmri_pdf_functor_EapFunctor_h_


#include <gkg-dmri-container/EapCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>
#include <vector>


namespace gkg
{


template < class C, class I >
class EapFunctor
{

  public:

    virtual ~EapFunctor();

    virtual std::string getName() const = 0;
    virtual bool isStandardBasisRequired() const = 0;

    virtual void process( C& output,
                          EapCartesianField& eapCartesianField,
                          const std::vector< double >& scalarParameters,
                          const std::vector< std::string >& stringParameters,
                          bool verbose ) const;

  protected:

    EapFunctor();

    // RGB  scale must be stored in scalarParameters[ 0 ]
    // mesh scale must be stored in scalarParameters[ 1 ]
    // ascii is stored in scalarParameters[ 2 ]
    // mesh opacity is stored in scalarParameters[ 3 ]
    // radii of EAP mesh must be stored in scalarParameters[ 4 + r ]

    virtual void processItem(
                I& item,
                const OrientationDistributionFunction& eap,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif
