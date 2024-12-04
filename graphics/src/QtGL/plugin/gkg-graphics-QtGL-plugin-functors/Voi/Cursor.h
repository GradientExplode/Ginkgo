#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Cursor_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Cursor_h_


#include <gkg-core-cppext/StdInt.h>

#include <QBitmap>
#include <QCursor>


namespace gkg
{


class VoiCursor
{

  public:

    enum VoiShape
    {
    
      VoiBucketCursor = 100,
      VoiGraspingHandCursor,
      VoiGumCursor,
      VoiMagicWandCursor,
      VoiMagnifyingGlassCursor,
      VoiPenCursor,
      VoiPipetteCursor,
      VoiBitmapCursor,
      VoiQtCursor

    };

    VoiCursor();
    VoiCursor( VoiShape shape );
    VoiCursor( const QBitmap &b, const QBitmap &m, 
               int32_t hotX=-1, int32_t hotY=-1 );

    void setShape( VoiShape );
    void setBitmap( const QBitmap &, const QBitmap &, int32_t, int32_t );

    VoiShape shape() { return theShape; }

    QCursor cursor() { return theCursor; }

  private:

    void fillBucketBitmap();
    void fillGraspingHandBitmap();
    void fillGumBitmap();
    void fillMagicWandBitmap();
    void fillMagnifyingGlassBitmap();
    void fillPenBitmap();
    void fillPipetteBitmap();

    VoiShape theShape;

    QBitmap curs;
    QBitmap mask;

    QCursor theCursor;

};


}


#endif
