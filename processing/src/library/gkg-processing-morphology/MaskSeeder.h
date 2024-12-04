#ifndef _gkg_processing_morphology_MaskSeeder_h_
#define _gkg_processing_morphology_MaskSeeder_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class TestFunction;
template < class T > class Volume;


template < class IN, class OUT >
class MaskSeeder
{

  public:

    MaskSeeder( const TestFunction< IN >& tesFunction,
                const OUT& foreground,
                const OUT& background,
                const OUT& startLabel = ( OUT )1 );
    virtual ~MaskSeeder();

    int64_t seed( const Volume< IN >& maskVolume,
                  int32_t parcelVoxelCount,
                  int32_t kMeansIterationCount,
                  float ratioOfDistanceVariance,
                  Volume< OUT >& seedVolume ) const;

  protected:

    void reallocate( const Volume< IN >& in, Volume< OUT >& out ) const;

    const TestFunction< IN >& _testFunction;
    OUT _foreground;
    OUT _background;
    OUT _startLabel;

};


}


#endif
