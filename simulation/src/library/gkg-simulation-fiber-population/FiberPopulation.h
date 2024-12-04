#ifndef _gkg_simulation_fiber_population_FiberPopulation_h_
#define _gkg_simulation_fiber_population_FiberPopulation_h_


#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-core-pattern/RCPointer.h>



namespace gkg
{


class Fiber;


class FiberPopulation : public Population,
                        public Creator5Arg< FiberPopulation,
                                            Population,
                                            int32_t,
                                            const VirtualTissue*,
                                            const Dictionary&,
                                            bool,
                                            std::ofstream* >
{

  public:

    FiberPopulation( const FiberPopulation& other );
    virtual ~FiberPopulation();

    FiberPopulation& operator=( const FiberPopulation& other );

    std::string getTypeName() const; 

    template < class L >
    void getBundleMap( gkg::BundleMap< L >& bundleMap, const L& label ) const;

    static std::string getStaticName();

  protected:

    friend struct Creator5Arg< FiberPopulation,
                               Population,
                               int32_t,
                               const VirtualTissue*,
                               const Dictionary&,
                               bool,
                               std::ofstream* >;

    FiberPopulation( int32_t id,
                     const VirtualTissue* parent,
                     const Dictionary& parameters,
                     bool verbose,
                     std::ofstream* osLog );


    float _atomOversamplingRatio;
    RCPointer< ParameterDistribution > _axonDiameterDistribution;
    float _globalAngularDispersionInDegrees;
    bool _hasTortuosity;
    float _tortuosityMagnitude;
    float _tortuosityAngularDispersionInDegrees;
    float _tortuosityWaveLength;
    bool _hasBeading;
    RCPointer< ParameterDistribution > _interbeadingLengthDistribution;
    RCPointer< ParameterDistribution > _beadingWidthDistribution;
    RCPointer< ParameterDistribution > _beadingMagnitudeRatioDistribution;
    Vector3d< float > _meanOrientation;
    bool _hasMyelinSheath;
    RCPointer< ParameterDistribution > _gRatioDistribution;
    bool _hasRanvierNodes;
    RCPointer< ParameterDistribution > _internodalLengthToNodeWidthDistribution;
    float _volumeFraction;


    std::vector< RCPointer< Fiber > > _fibers;


};


}


#endif
