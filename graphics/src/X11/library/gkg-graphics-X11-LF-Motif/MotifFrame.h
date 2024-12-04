#ifndef _gkg_graphics_X11_LF_Motif_MotifFrame_h_
#define _gkg_graphics_X11_LF_Motif_MotifFrame_h_


#include <gkg-graphics-X11-LF-Motif/MotifInfo.h>
#include <gkg-graphics-X11-core/BevelFrame.h>
#include <gkg-graphics-X11-core/TellTale.h>


namespace gkg
{


class MotifFrame : public BevelFrame
{

  public:

    MotifFrame( Glyph* glyph,
                TellTaleState* tellTaleState,
                MotifInfo* info,
                FloatCoordinate thickness,
                bool target,
                bool choosable );
    ~MotifFrame();

    const MotifInfo& getMotifInfo() const;

    void draw( Canvas* canvas,
               const GeometryAllocation& geometryAllocation ) const;
    void pick( Canvas* canvas,
               const GeometryAllocation& geometryAllocation,
               int32_t depth,
               Hit& hit );

    void drawFrame( Canvas* canvas,
                    const GeometryAllocation& geometryAllocation,
                    FloatCoordinate thickness ) const;

  private:

    TellTaleState* _tellTaleState;
    MotifInfo* _motifInfo;
    bool _target;
    bool _choosable;

};


}


#endif
