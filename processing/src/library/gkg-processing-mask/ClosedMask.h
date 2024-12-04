#ifndef _gkg_processing_mask_ClosedMask_h_
#define _gkg_processing_mask_ClosedMask_h_


#include <gkg-processing-mask/MaskAlgorithm.h>


namespace gkg
{


template < class IN, class OUT >
class ClosedMask : public MaskAlgorithm< IN, OUT >
{

  public:

    ClosedMask( float radius,
                float thresholdRatio,
                const OUT& foreground = 1,
                const OUT& background = 0 );
    ~ClosedMask();

    std::string getName() const;

    void getMask( const Volume< IN >& volume,
                  Volume< OUT >& mask,
                  bool verbose ) const;

    static std::string getStaticName();

  private:

    float _radius;
    float _thresholdRatio;
    OUT _foreground;
    OUT _background;

};




}


#endif

