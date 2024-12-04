#include <gkg-graphics-X11-core/SelectionManagerImplementation.h>
#include <gkg-graphics-X11-core/SelectionManager.h>
#include <gkg-graphics-X11-core/SelectionHandler.h>
#include <gkg-graphics-X11-core/UnvisibleWindow.h>


gkg::SelectionManagerImplementation::SelectionManagerImplementation(
                                        gkg::SelectionManager* selectionManager,
                                        gkg::Display* display,
                                        const std::string& name )
                                    : _target( selectionManager ),
                                      _name( name ),
                                      _convert( 0 ),
                                      _lose( 0 ),
                                      _done( 0 ),
                                      _ok( 0 ),
                                      _fail( 0 ) 
{

  _owner = new gkg::UnvisibleWindow( display );

}


gkg::SelectionManagerImplementation::~SelectionManagerImplementation()
{

  delete _owner;
  if ( _convert )
  {

    _convert->removeReference();

  }
  if ( _lose )
  {

    _lose->removeReference();

  }
  if ( _done )
  {

    _done->removeReference();

  }
  if ( _ok )
  {

    _ok->removeReference();

  }
  if ( _fail )
  {

    _fail->removeReference();

  }

}


void gkg::SelectionManagerImplementation::retrieve( const std::string&,
                                                    gkg::SelectionHandler*,
                                                    gkg::SelectionHandler* )
{

  // unimplemented

}


void gkg::SelectionManagerImplementation::getValue( std::string&,
                                                    void*&,
                                                    int32_t&,
                                                    int32_t& )
{

  // unimplemented

}


const std::string& gkg::SelectionManagerImplementation::getName() const
{

  return _name;

}


void gkg::SelectionManagerImplementation::doReferencing(
                                                 gkg::SelectionHandler* convert,
                                                 gkg::SelectionHandler* lose,
                                                 gkg::SelectionHandler* done )
{

  if ( convert )
  {

    convert->addReference();

  }
  if ( _convert )
  {

    _convert->removeReference();

  }
  _convert = convert;

  if ( lose )
  {

    lose->addReference();

  }
  if ( _lose )
  {

    _lose->removeReference();

  }
  _lose = lose;

  if ( done )
  {

    done->addReference();

  }
  if ( _done )
  {

    _done->removeReference();

  }
  _done = done;

}
