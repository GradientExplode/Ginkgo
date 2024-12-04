#include <gkg-graphics-X11-core/SpinBox.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Stencil.h>
#include <gkg-graphics-X11-core/Style.h>

#include <gkg-graphics-X11-bitmaps/spinboxup.bm>
#include <gkg-graphics-X11-bitmaps/spinboxdown.bm>


//
// class SpinBoxActionCallback
//

namespace gkg
{


class SpinBoxActionCallback : public Action
{

  public:

    SpinBoxActionCallback( SpinBox* spinBox, int32_t increment );
    ~SpinBoxActionCallback();

    virtual void execute();

  private:

    SpinBox* _spinBox;
    int32_t _increment;

};


}


gkg::SpinBoxActionCallback::SpinBoxActionCallback( gkg::SpinBox* spinBox,
                                                   int32_t increment )
                           : gkg::Action(),
                             _spinBox( spinBox ),
                             _increment( increment )
{
}


gkg::SpinBoxActionCallback::~SpinBoxActionCallback()
{
}


void gkg::SpinBoxActionCallback::execute()
{

  int32_t newIndex = _spinBox->getSelected() + _increment;
  if ( ( newIndex >= 0 ) && ( newIndex < _spinBox->getItemCount() ) )
  {

    _spinBox->select( newIndex );

  }

}


//
// class SpinBox
//

gkg::SpinBox::SpinBox()
             : gkg::MonoGlyph(),
               _maxWidth( 0 ),
               _maxHeight( 0 ),
               _selectedPatch( 0 ),
               _selected( 0 ),
               _downAction( 0 ),
               _upAction( 0 )
{

  build();

}


gkg::SpinBox::~SpinBox()
{

  for ( int32_t i = 0; i < getItemCount(); i++ )
  {

    if ( _items[ i ] )
    {

      _items[ i ]->removeReference();

    }

  }

}


void gkg::SpinBox::appendItem( gkg::Glyph* item )
{

  if ( item )
  {

    item->addReference();

  }
  _items.push_back( item );
  build();

}


void gkg::SpinBox::prependItem( gkg::Glyph* item )
{

  if ( item )
  {

    item->addReference();

  }
  _items.push_front( item );
  build();

}


void gkg::SpinBox::insertItem( int32_t index, gkg::Glyph* item )
{

  if ( ( index >= 0 ) && ( index < getItemCount() ) )
  {

    if ( item )
    {

      item->addReference();

    }
    _items.insert( _items.begin() + index, item );
    if ( _selected >= index )
    {

      ++ _selected;

    }
    build();

  }

}


void gkg::SpinBox::removeItem( int index )
{

  if ( ( index >= 0 ) && ( index < getItemCount() ) )
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


void gkg::SpinBox::replaceItem( int32_t index, gkg::Glyph* item )
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


int32_t gkg::SpinBox::getItemCount() const
{

  return ( int32_t )_items.size();

}


gkg::Glyph* gkg::SpinBox::getItem( int32_t index ) const
{

  if ( ( index >= 0 ) && ( index < getItemCount() ) )
  {

    return _items[ index ];

  }
  return 0;

}


void gkg::SpinBox::select( int32_t index )
{

  if ( ( index >= 0 ) && ( index < getItemCount() ) )
  {

    _selected = index;

    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

    _selectedPatch->setBody( layoutFactory.createVCenter(
                               layoutFactory.createFixed( _items[ index ],
                                                          _maxWidth,
                                                          _maxHeight ) ) );
    _selectedPatch->reallocate();
    _selectedPatch->redraw();

  }

}


int32_t gkg::SpinBox::getSelected() const
{

  return _selected;

}


void gkg::SpinBox::down()
{

  _downAction->execute();

}


void gkg::SpinBox::up()
{

  _upAction->execute();

}


void gkg::SpinBox::build()
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
  _selectedPatch = new gkg::Patch( 0 );

  _downAction = new gkg::SpinBoxActionCallback( this, -1 );
  gkg::Button* 
    downButton = widgetFactory->createNoMarginPushButton(
                   layoutFactory.createCenter(
                     new gkg::Stencil(
                       new gkg::Bitmap( spinboxdown_bits,
                                           spinboxdown_width,
                                           spinboxdown_height ),
                       widgetFactory->getForeground() ) ),
                   _downAction );

  _upAction = new gkg::SpinBoxActionCallback( this, +1 );
  gkg::Button* 
    upButton = widgetFactory->createNoMarginPushButton(
                 layoutFactory.createCenter(
                   new gkg::Stencil(
                     new gkg::Bitmap( spinboxup_bits,
                                         spinboxup_width,
                                         spinboxup_height ),
                     widgetFactory->getForeground() ) ),
                 _upAction );

  setBody( layoutFactory.createHBox(
             widgetFactory->createInsetFrame(
               layoutFactory.createFixed( _selectedPatch,
                                          _maxWidth,
                                          _maxHeight ) ),
             layoutFactory.createVCenter( 
               layoutFactory.createVBox( 
                 layoutFactory.createVFlexible( upButton, fil, 0.0 ),
                 layoutFactory.createVFlexible( downButton, fil, 0.0 ) ) ) ) );
  select( 0 );

}


