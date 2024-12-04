#ifndef _gkg_dmri_odf_functor_OdfFunctor_h_
#define _gkg_dmri_odf_functor_OdfFunctor_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>
#include <vector>


namespace gkg
{


template < class C, class I >
class OdfFunctor
{

  public:

    virtual ~OdfFunctor();

    virtual std::string getName() const = 0;
    virtual bool isStandardBasisRequired() const = 0;

    virtual void process( C& output,
                          OdfCartesianField& odfCartesianField,
                          const std::vector< double >& scalarParameters,
                          const std::vector< std::string >& stringParameters,
                          bool verbose ) const;

  protected:

    OdfFunctor();

    // RGB scale must be stored in scalarParameters[ 0 ]
    // mesh scale must be stored in scalarParameters[ 1 ]
    // lower GFA threshold must be stored in scalarParameters[ 2 ]
    // upper GFA threshold must be stored in scalarParameters[ 3 ]

    virtual void processItem(
                I& item,
                const OrientationDistributionFunction& odf,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif
