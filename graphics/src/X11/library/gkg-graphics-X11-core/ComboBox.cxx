#include <gkg-graphics-X11-core/ComboBox.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Menu.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Stencil.h>
#include <gkg-graphics-X11-core/Color.h>

#include <gkg-graphics-X11-bitmaps/combobox.bm>


//
// class ComboBoxActionCallback
//

namespace gkg
{


class ComboBoxActionCallback : public Action
{

  public:

    ComboBoxActionCallback( ComboBox* comboBox, int32_t index );
    ~ComboBoxActionCallback();

    virtual void execute();

  private:

    ComboBox* _comboBox;
    int32_t _index;

};


}


gkg::ComboBoxActionCallback::ComboBoxActionCallback( gkg::ComboBox* comboBox,
                                                     int32_t index )
                            : gkg::Action(),
                              _comboBox( comboBox ),
                              _index( index )
{
}


gkg::ComboBoxActionCallback::~ComboBoxActionCallback()
{
}


void gkg::ComboBoxActionCallback::execute()
{

  _comboBox->select( _index );

}


//
// class ComboBox
//

gkg::ComboBox::ComboBox()
              : gkg::Patch( 0 ),
                _maxWidth( 0 ),
                _maxHeight( 0 ),
                _menu( 0 ),
                _selectedPatch( 0 ),
                _selected( 0 )
{

  build();

}


gkg::ComboBox::~ComboBox()
{

  for ( int32_t i = 0; i < getItemCount(); i++ )
  {

    if ( _items[ i ] )
    {

      _items[ i ]->removeReference();

    }

  }

}


void gkg::ComboBox::appendItem( gkg::Glyph* item )
{

  if ( item )
  {

    item->addReference();

  }
  _items.push_back( item );
  build();

}


void gkg::ComboBox::prependItem( gkg::Glyph* item )
{

  if ( item )
  {

    item->addReference();

  }
  _items.push_front( item );
  build();

}


void gkg::ComboBox::insertItem( int32_t index, gkg::Glyph* item )
{

  if ( index >= 0 && index < getItemCount() )
  {

    if ( item )
    {

      item->addReference();

    }
    _items.insert( _items.begin() + index, item );
    if ( _selected >= index )
    {

      _selected ++;

    }
    build();

  }

}


void gkg::ComboBox::removeItem( int32_t index )
{

  if ( index >= 0 && index < getItemCount() )
  {

    if ( _items[ index ] )
    {

      _items[ index ]->removeReference();

    }
    _items.erase( _items.begin() + index );
    _selected = 0;
    build();

  }

}


void gkg::ComboBox::replaceItem( int32_t index,
                                    gkg::Glyph* item )
{

  if ( index >= 0 && index < getItemCount() )
  {

    if ( _items[ index ] )
    {

      _items[ index ]->removeReference();

    }
    if ( item )
    {

      item->addReference();

    }
    _items[ index ] = item;
    build();

  }

}


int32_t gkg::ComboBox::getItemCount() const
{

  return ( int32_t )_items.size();

}


gkg::Glyph* gkg::ComboBox::getItem( int32_t index ) const
{

  if ( index >= 0 && index < getItemCount() )
  {

    return _items[ index ];

  }
  return 0;

}


void gkg::ComboBox::select( int32_t index )
{

  if ( index >= 0 && index < getItemCount() )
  {

    _selected = index;
    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

    _selectedPatch->setBody(
      layoutFactory.createHBox( 
                  layoutFactory.createVCenter(
                    layoutFactory.createFixed( _items[ index ],
                                               _maxWidth,
                                               _maxHeight ) ),
                  layoutFactory.createVCenter(
                    layoutFactory.createHMargin(
                      new gkg::Stencil( 
                        new gkg::Bitmap( combobox_bits,
                                            combobox_width,
                                            combobox_height ),
                        new gkg::Color( 0, 0, 0 ) ), 3 ) ) ) );
    _selectedPatch->reallocate();
    _selectedPatch->redraw();

  }

}


int32_t gkg::ComboBox::getSelected() const
{

  return _selected;

}


void gkg::ComboBox::build()
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  int32_t i;
  for ( i = 0; i < getItemCount(); i++ )
  {

    gkg::GeometryRequisition r;
    _items[ i ]->request( r );
    gkg::FloatCoordinate width = r.getRequirementX().getNatural();
    gkg::FloatCoordinate height = r.getRequirementY().getNatural();
    if ( width > _maxWidth )
    {

      _maxWidth = width;

    }
    if ( height > _maxHeight )
    {

      _maxHeight = height;

    }

  }

  _menu = widgetFactory->createMenubar();
  _selectedPatch = new gkg::Patch( 0 );

  gkg::MenuItem* 
    menubarItem = new gkg::MenuItem( _selectedPatch,
                                        new gkg::TellTaleState( 
                                          gkg::TellTaleState::Enabled ) );
  //widgetFactory->createMenubarItem( _selectedPatch );
  menubarItem->setMenu( widgetFactory->createPullright() );

  for ( i = 0; i < getItemCount(); i++ )
  {

    gkg::Action*
      action = new gkg::ComboBoxActionCallback( this, i );
    gkg::MenuItem* 
      menuItem = widgetFactory->createMenuItem(
                   layoutFactory.createOverlay(
                     layoutFactory.createVCenter(
                       layoutFactory.createHSpace( _maxWidth + 16 ) ),
                     layoutFactory.createVCenter(
                       layoutFactory.createVSpace( _maxHeight ) ),
                     layoutFactory.createVCenter( _items[ i ] ) ) );
    menuItem->setAction( action );
    menubarItem->getMenu()->appendItem( menuItem );

  }
  _menu->appendItem( menubarItem );

  setBody( layoutFactory.createFixed( _menu,
                                      _maxWidth + 26,
                                      _maxHeight + 4 ) );
  select( 0 );

}


