from Viewer import *
from Algorithm import *
from Application import *


class ViewerCallbackManager:

  def __init__( self, algorithm ):

    self._algorithm = algorithm
    
  def setViewFromIndex( self, index ):
  
    if ( self._algorithm.getApplication() is not None ):
    
      viewer = self._algorithm.getApplication().getViewer()
      if ( viewer is not None ):
   
        viewer.setViewFromIndex( index + 1 )

  def setViewFromName( self, name ):

    if ( self._algorithm.getApplication() is not None ):

      viewer = self._algorithm.getApplication().getViewer()
      if ( viewer is not None ):

        viewName = str( name )
        hasView = viewer._views.has_key( viewName )

        if ( viewer.hasViewFromName( viewName ) ):

          viewer.setViewFromName( viewName )

        else:

          viewer.setDefaultView()
