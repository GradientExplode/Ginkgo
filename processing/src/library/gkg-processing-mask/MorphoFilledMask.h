#ifndef _gkg_processing_mask_MorphoFilledMask_h_
#define _gkg_processing_mask_MorphoFilledMask_h_


#include <gkg-processing-mask/MaskAlgorithm.h>


namespace gkg
{


template < class IN, class OUT >
class MorphoFilledMask : public MaskAlgorithm< IN, OUT >
{

  public:

    MorphoFilledMask( float closingRadius,
                      double noisePosition = 0.9,
                      double noiseRatio = 0.01,
                      int32_t thrown = 20,
                      int32_t kept = 100,
                      const OUT& foreground = 1,
                      const OUT& background = 0 );
    ~MorphoFilledMask();

    std::string getName() const;

    void getMask( const Volume< IN >& volume,
                  Volume< OUT >& mask,
                  bool verbose ) const;

    static std::string getStaticName();

  private:

    float _closingRadius;
    double _noisePosition;
    double _noiseRatio;
    int32_t _thrown;
    int32_t _kept;
    OUT _foreground;
    OUT _background;

};




}


#endif

