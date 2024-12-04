#ifndef _gkg_graphics_X11_progressive_ProgressiveImage_h_
#define _gkg_graphics_X11_progressive_ProgressiveImage_h_


#include <gkg-core-pattern/Observer.h>
#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-communication-thread/Mutex.h>


namespace gkg
{


class ProgressiveModel;
class Raster;
class Patch;
class ProgressiveImageContext;
class ProgressiveThread;
class IOHandler;

class ProgressiveImage : public MonoGlyph, public Observer
{

  public:

    ProgressiveImage( ProgressiveModel* progressiveModel,
                      int32_t resolutionDivider,
                      bool verbose = false );
    virtual ~ProgressiveImage();

    virtual ProgressiveModel* getProgressiveModel() const;

    virtual int32_t getResolutionDivider() const;
    virtual bool hasFullResolution() const;

    virtual ProgressiveImageContext* getProgressiveImageContext() const;
    virtual void resetProgressiveImageContext();

    virtual void updatePatch();
    virtual void update( const Observable* observable = 0, void* data = 0 );

    void redraw( int64_t, int64_t );

  private:

    ProgressiveModel* _progressiveModel;
    Raster* _raster;
    Patch* _imagePatch;

    Mutex _mutex;
    int32_t _resolutionDivider;
    bool _needsRedraw;

    ProgressiveImageContext* _progressiveImageContext;
    ProgressiveThread* _progressiveThread;
    IOHandler* _handler;

    bool _verbose;

};


}


#endif
