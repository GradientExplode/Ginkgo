#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctor_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctor_h_


#include <gkg-dmri-container/DiffusoidCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>
#include <vector>


namespace gkg
{


template < class C, class I >
class DiffusoidFunctor
{

  public:

    virtual ~DiffusoidFunctor();

    virtual std::string getName() const = 0;

    virtual void process( C& output,
                          DiffusoidCartesianField& diffusoidCartesianField,
                          const std::vector< double >& scalarParameters,
                          const std::vector< std::string >& stringParameters,
                          bool verbose ) const;

  protected:

    DiffusoidFunctor();

    // RGB scale must be stored in scalarParameters[ 0 ]
    // mesh scale must be stored in scalarParameters[ 1 ]

    virtual void processItem(
                I& item,
                const Diffusoid& diffusoid,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif
