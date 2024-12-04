#ifndef _gkg_processing_bias_BiasCorrection3d_h_
#define _gkg_processing_bias_BiasCorrection3d_h_


#include <gkg-processing-bias/BiasCorrection.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{


template < class T >
class BiasCorrection3d : public BiasCorrection< T >,
                         public Creator2Arg< BiasCorrection3d< T >,
                                             BiasCorrection< T >,
                                             const std::vector< double >&,
                                             const std::vector< std::string >& >
{

  public:

    virtual ~BiasCorrection3d();

    std::string getName() const;

    static std::string getStaticName();

    void correct( const Volume< T >& in,
                  const Volume< int16_t >& mask,
                  const Volume< float >& weight,
                  Volume< T >& out,
                  Volume< float >& bias,
                  bool verbose ) const;

  protected:

    friend class Creator2Arg< BiasCorrection3d< T >,
                              BiasCorrection< T >,
                              const std::vector< double >&,
                              const std::vector< std::string >& >;

    // scalarParameters[  0 ] -> piecewise bias volume X size
    // scalarParameters[  1 ] -> piecewise bias volume Y size
    // scalarParameters[  2 ] -> piecewise bias volume Z size
    // scalarParameters[  3 ] -> entropy factor
    // scalarParameters[  4 ] -> data attachment factor
    // scalarParameters[  5 ] -> regularization factor
    // scalarParameters[  6 ] -> simulated annealing gain k
    // scalarParameters[  7 ] -> simulated annealing Boltzmann mu parameter
    // scalarParameters[  8 ] -> simulated annealing initial temperature
    // scalarParameters[  9 ] -> simulated annealing minimum temperature
    // scalarParameters[ 10 ] -> trial count per step
    // scalarParameters[ 11 ] -> iteration count per temperature
    // scalarParameters[ 12 ] -> simulated annealing maximum step size
    // scalarParameters[ 13 ] -> simulated annealing verbosity
    // scalarParameters[ 14 ] -> level count in entropy processing
    // scalarParameters[ 15 ] -> smooth pdf in entropy processing

    BiasCorrection3d( const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& stringParameters );

    Vector3d< int32_t > _piecewiseBiasSize;
    double _entropyFactor;
    double _dataAttachmentFactor;
    double _regularizationFactor;
    double _k;
    double _mu;
    double _initialTemperature;
    double _minimumTemperature;
    int32_t _trialCountPerStep;
    int32_t _iterationCountPerTemperature;
    double _maximumStepSize;
    bool _verboseSimulatedAnnealing;
    int32_t _entropyLevelCount;
    bool _entropyIsSmoothingApplied;

};


}


#endif
