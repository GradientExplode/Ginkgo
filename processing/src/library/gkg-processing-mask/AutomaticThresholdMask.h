#ifndef _gkg_processing_mask_AutoThresholdMask_h_
#define _gkg_processing_mask_AutoThresholdMask_h_


#include <gkg-processing-mask/MaskAlgorithm.h>


namespace gkg
{


template < class IN, class OUT >
class AutomaticThresholdMask : public MaskAlgorithm< IN, OUT >
{

  public:

    AutomaticThresholdMask( double noisePosition = 0.9,
                            double noiseRatio = 0.01,
                            int32_t thrown = 20,
                            int32_t kept = 100,
                            const OUT& foreground = 1,
                            const OUT& background = 0 );
    ~AutomaticThresholdMask();

    std::string getName() const;

    void getMask( const Volume< IN >& volume,
                  Volume< OUT >& mask,
                  bool verbose = false ) const;

    static std::string getStaticName();

  private:

    double _noisePosition;
    double _noiseRatio;
    int32_t _thrown;
    int32_t _kept;
    OUT _foreground;
    OUT _background;

};




}


#endif

