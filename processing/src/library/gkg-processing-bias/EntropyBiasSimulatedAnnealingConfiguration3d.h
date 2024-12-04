#ifndef _gkg_processing_bias_EntropyBiasSimulatedAnnealingConfiguration3d_h_
#define _gkg_processing_bias_EntropyBiasSimulatedAnnealingConfiguration3d_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SimulatedAnnealingConfiguration.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>


namespace gkg
{


template < class T >
class EntropyBiasSimulatedAnnealingConfiguration3d :
                                          public SimulatedAnnealingConfiguration
{

  public:

    EntropyBiasSimulatedAnnealingConfiguration3d(
                                   const Volume< T >& in,
                                   const Volume< int16_t >& mask,
                                   const Volume< float >& weight,
                                   Volume< T >& out,
                                   Volume< float >& bias,
                                   const Vector3d< int32_t >& piecewiseBiasSize,
                                   double entropyFactor,
                                   double dataAttachmentFactor,
                                   double regularizationFactor,
                                   int32_t levelCount,
                                   bool isSmoothingApplied );
    EntropyBiasSimulatedAnnealingConfiguration3d(
               const EntropyBiasSimulatedAnnealingConfiguration3d< T >& other );
    ~EntropyBiasSimulatedAnnealingConfiguration3d();

    void setTime( int32_t time );

    const Volume< float >& getPiecewiseBias() const;

    double getEnergy() const;
    void step( double uniformRandomNumber, double stepSize );

    double getDistance( const SimulatedAnnealingConfiguration& other ) const;
    void display() const;
    void copy( const SimulatedAnnealingConfiguration& other );
    SimulatedAnnealingConfiguration* copyConstruct() const;

    void applyBiasCorrection();

  protected:

    const Volume< T >& _in;
    const Volume< int16_t >& _mask;
    const Volume< float >& _weight;
    Volume< T >& _out;
    Volume< float >& _bias;
    int32_t _time;
    double _mean;
    double _minimum;
    double _maximum;
    Volume< float > _piecewiseBias;
    double _entropyFactor;
    double _dataAttachmentFactor;
    double _regularizationFactor;
    int32_t _levelCount;
    bool _isSmoothingApplied;
    RandomGenerator _randomGenerator;
    DericheGaussianCoefficients< double >* _dericheGaussianCoefficients;

};


}


#endif

