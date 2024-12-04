#ifndef _gkg_graphics_X11_core_ComboBox_h_
#define _gkg_graphics_X11_core_ComboBox_h_


#include <gkg-graphics-X11-core/Patch.h>
#include <deque>


namespace gkg
{


class Menu;


class ComboBox : public Patch
{

  public:

    ComboBox();
    virtual ~ComboBox();

    virtual void appendItem( Glyph* item );
    virtual void prependItem( Glyph* item );
    virtual void insertItem( int32_t index, Glyph* item );
    virtual void removeItem( int32_t index );
    virtual void replaceItem( int32_t index, Glyph* item );

    virtual int32_t getItemCount() const;
    virtual Glyph* getItem( int32_t index ) const;

    virtual void select( int32_t index );
    virtual int32_t getSelected() const;

  private:

    void build();

    std::deque< Glyph* > _items;
    FloatCoordinate _maxWidth;
    FloatCoordinate _maxHeight;
    Menu* _menu;
    Patch* _selectedPatch;
    int32_t _selected;

};


}


#endif
