#ifndef _gkg_dmri_qspace_sampling_QSpaceSamplingFactory_h_
#define _gkg_dmri_qspace_sampling_QSpaceSamplingFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-dmri-qspace-sampling/QSpaceSampling.h>
#include <gkg-core-object/Dictionary.h>
#include <vector>
#include <string>


namespace gkg
{


class QSpaceSamplingFactory : public Singleton< QSpaceSamplingFactory >
{

  public:

    ~QSpaceSamplingFactory();

    QSpaceSampling* 
      create( const std::vector< std::string >& stringParameters,
              const std::vector< double >& scalarParameters,
              const HomogeneousTransform3d< float >& transform3d,
              const Dictionary& gradientCharacteristics ) const;

    QSpaceSampling* create( HeaderedObject& headeredObject ) const;

  protected:

    friend class Singleton< QSpaceSamplingFactory >;

    QSpaceSamplingFactory();

};


}


#endif
