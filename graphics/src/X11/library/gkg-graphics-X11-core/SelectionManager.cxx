#include <gkg-graphics-X11-core/SelectionManager.h>
#include <gkg-graphics-X11-core/SelectionManagerImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::SelectionManager::SelectionManager( gkg::Display* display,
                                         const std::string& name )
{

  _selectionManagerImplementation = 
    gkg::getGUIImplementationFactory()->createSelectionManagerImplementation(
                                                         this, display, name );

}


gkg::SelectionManager::~SelectionManager()
{

  delete _selectionManagerImplementation;

}


void gkg::SelectionManager::own( gkg::SelectionHandler* convert,
                                 gkg::SelectionHandler* lose,
                                 gkg::SelectionHandler* done )
{

  _selectionManagerImplementation->own();
  _selectionManagerImplementation->doReferencing( convert, lose, done );

}


void gkg::SelectionManager::putValue( const void* data,
                                      int32_t length,
                                      int32_t format )
{

  _selectionManagerImplementation->putValue( data, length, format );

}


void gkg::SelectionManager::retrieve( const std::string& type,
                                      gkg::SelectionHandler* ok,
                                      gkg::SelectionHandler* fail )
{

  _selectionManagerImplementation->retrieve( type, ok, fail );

}


void gkg::SelectionManager::getValue( std::string& type,
                                      void*& data,
                                      int32_t& length,
                                      int32_t& format )
{

  _selectionManagerImplementation->getValue( type, data, length, format );

}


gkg::SelectionManagerImplementation* 
gkg::SelectionManager::getImplementation() const
{

  return _selectionManagerImplementation;

}
