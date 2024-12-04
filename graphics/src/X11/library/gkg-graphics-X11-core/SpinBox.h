#ifndef _gkg_graphics_X11_core_SpinBox_h_
#define _gkg_graphics_X11_core_SpinBox_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <deque>


namespace gkg
{


class Action;
class Patch;


class SpinBox : public MonoGlyph
{

  public:

    SpinBox();
    virtual ~SpinBox();

    virtual void appendItem( Glyph* item );
    virtual void prependItem( Glyph* item );
    virtual void insertItem( int32_t index, Glyph* item );
    virtual void removeItem( int32_t index );
    virtual void replaceItem( int32_t index, Glyph* item );

    virtual int32_t getItemCount() const;
    virtual Glyph* getItem( int32_t index ) const;

    virtual void select( int32_t index );
    virtual int32_t getSelected() const;

    virtual void up();
    virtual void down();

  private:

    void build();

    std::deque< Glyph* > _items;
    FloatCoordinate _maxWidth;
    FloatCoordinate _maxHeight;
    Patch* _selectedPatch;
    int32_t _selected;
    Action* _downAction;
    Action* _upAction;

};


}


#endif
