#include <gkg-graphics-QtGL-plugin-functors/Voi/Observer.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Observable.h>


gkg::VoiObserver::~VoiObserver()
{

  std::set< gkg::VoiObservable * >::iterator 
    it = datas.begin(),
    ie = datas.end();

  while ( it != ie )
  {
  
    (*it)->unregisterView( this );
    ++it;
    
  }
  
  datas.erase( datas.begin(), datas.end() );

}


void gkg::VoiObserver::registerData( gkg::VoiObservable *data )
{

  datas.insert( data );

}


void gkg::VoiObserver::unregisterData( gkg::VoiObservable *data )
{

  datas.erase( data );

}


void gkg::VoiObserver::updateDataLoaded()
{
}


void gkg::VoiObserver::updateLabelLoaded()
{
}


void gkg::VoiObserver::updateData()
{
}


void gkg::VoiObserver::updatePalette()
{
}


void gkg::VoiObserver::updateLabel( std::list< gkg::Vector3d< int32_t > >& )
{
}


void gkg::VoiObserver::updateScrollBar()
{
}


void gkg::VoiObserver::updateZoom()
{
}


void gkg::VoiObserver::updateSlice( gkg::Vector3d< int32_t >&, int32_t )
{
}


void gkg::VoiObserver::updateFrame( int32_t )
{
}


void gkg::VoiObserver::redrawView()
{
}


void gkg::VoiObserver::updateMode( gkg::VoiModeStrategy * )
{
}
