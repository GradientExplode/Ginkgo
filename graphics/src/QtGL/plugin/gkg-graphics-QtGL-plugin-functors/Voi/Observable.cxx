#include <gkg-graphics-QtGL-plugin-functors/Voi/Observable.h>


gkg::VoiObservable::~VoiObservable()
{

  std::set< gkg::VoiObserver * >::iterator it;

  if ( !views.empty() )
    {
    
      for ( it = views.begin(); it != views.end(); ++it )
      {
      
        (*it)->unregisterData( this );
	
      }

      views.erase( views.begin(), views.end() );

    }

}


void gkg::VoiObservable::registerView( gkg::VoiObserver *view )
{

  views.insert( view );

}


void gkg::VoiObservable::unregisterView( gkg::VoiObserver *view )
{

  views.erase( view );

}


void gkg::VoiObservable::notifyDataLoaded()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateDataLoaded();

  }
  
}


void gkg::VoiObservable::notifyLabelLoaded()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateLabelLoaded();

  }
  
}


void gkg::VoiObservable::notifyData()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateData();

  }
  
}


void gkg::VoiObservable::notifyPalette()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updatePalette();

  }
  
}


void 
gkg::VoiObservable::notifyLabel( std::list< gkg::Vector3d< int32_t > >& bck )
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateLabel( bck );
    
  }
  
}


void gkg::VoiObservable::notifyScrollBar()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateScrollBar();
    
  }

}


void gkg::VoiObservable::notifyZoom()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateZoom();
    
  }

}


void gkg::VoiObservable::notifySlice( gkg::Vector3d< int32_t >& point, 
                                      int32_t t )
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateSlice( point, t );
    
  }
  
}


void gkg::VoiObservable::notifyFrame( int32_t frame )
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateFrame( frame );
    
  }
  
}


void gkg::VoiObservable::notifyRedraw()
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->redrawView();
    
  }
  
}


void gkg::VoiObservable::notifyMode( gkg::VoiModeStrategy *strgy )
{

  std::set< gkg::VoiObserver * >::iterator it;

  for ( it = views.begin(); it != views.end(); ++it )
  {
  
    (*it)->updateMode( strgy );
    
  }
  
}
