import tempfile
import math
import PyQt5
from PyQt5 import QtCore, QtGui, QtWidgets, uic

import anatomist.direct.api as ana
from anatomist.cpp.palettecontrastaction import PaletteContrastAction
from soma import aims
from soma.aims import colormaphints

import matplotlib

matplotlib.use( 'Qt5Agg' )
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure
from matplotlib.lines import Line2D
from matplotlib.widgets import Slider

from ResourceManager import *
from ColorMapDialog import *
from WidgetWithOverlay import *
from VolumeInformationHandler import *
from PdfViewerWidget import *


################################################################################
# class WindowCallbackManager
################################################################################

class WindowCallbackManager:

  def __init__( self,
                viewer,
                viewName,
                viewLayout,
                zoomViewLayout,
                windowLayout,
                windowName,
                windowType,
                zoomToolButton,
                muteToolButton,
                row,
                column,
                rowSpan,
                columnSpan,
                comments = None,
                widgetWithOverlay = None ):
  
    self._viewer = viewer
    self._viewName = viewName
    self._viewLayout = viewLayout
    self._zoomViewLayout = zoomViewLayout
    self._windowLayout = windowLayout
    self._windowName = windowName
    self._windowType = windowType
    self._zoomToolButton = zoomToolButton
    self._muteToolButton = muteToolButton
    self._row = row
    self._column = column
    self._rowSpan = rowSpan
    self._columnSpan = columnSpan
    self._state = 0
    self._comments = comments
    self._widgetWithOverlay = widgetWithOverlay
    

  def zoomInOrOut( self ):
  
    if ( self._state == 0 ):
    
      self._viewLayout.removeItem( self._windowLayout )
      self._zoomToolButton.setIcon( QtGui.QIcon(
                    ResourceManager().getIconFileName( 'icon-zoom-out.png' ) ) )
      self._windowLayout.setParent( None )
      self._zoomViewLayout.addLayout( self._windowLayout, 
                                      0,
				      0,
				      self._viewLayout.rowCount(),
				      self._viewLayout.columnCount() )
      self._viewer.setViewFromIndex( 0 )
      self._state = 1
      if( self._widgetWithOverlay != None ):

        self._widgetWithOverlay.zoomIn()
      
    else:

      self._zoomViewLayout.removeItem( self._windowLayout )
      self._zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
      self._windowLayout.setParent( None )
      self._viewLayout.addLayout( self._windowLayout,
                                  self._row, self._column,
                                  self._rowSpan, self._columnSpan )
      self._viewer.setViewFromName( self._viewName )
      self._state = 0  
      if( self._widgetWithOverlay != None ):

        self._widgetWithOverlay.zoomOut()


  def zoomOut( self ):
  
    if ( self._state == 1 ):
    
      self._zoomViewLayout.removeItem( self._windowLayout )
      self._zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
      self._windowLayout.setParent( None )
      self._viewLayout.addLayout( self._windowLayout,
                                  self._row, self._column,
                                  self._rowSpan, self._columnSpan )
      self._viewer.setViewFromName( self._viewName )
  
      self._state = 0  
      if( self._widgetWithOverlay != None ):

        self._widgetWithOverlay.zoomOut()

  def mute( self, value = -1 ):

    if ( value == -1 ):

      self._windowType = ( self._windowType + 1 ) % 3

    else:

      self._windowType = value

    if ( self._windowType == 0 ):
    
      self._viewer.getWindow( self._viewName, self._windowName ).muteAxial()
      self._muteToolButton.setIcon( QtGui.QIcon(
       ResourceManager().getIconFileName( 'icon-window-axial-small.png' ) ) )
      
    elif ( self._windowType == 1 ):
    
      self._viewer.getWindow( self._viewName, self._windowName ).muteCoronal()
      self._muteToolButton.setIcon( QtGui.QIcon(
       ResourceManager().getIconFileName( 'icon-window-coronal-small.png' ) ) )
      
    elif ( self._windowType == 2 ):
    
      self._viewer.getWindow( self._viewName, self._windowName ).muteSagittal()
      self._muteToolButton.setIcon( QtGui.QIcon(
       ResourceManager().getIconFileName( 'icon-window-sagittal-small.png' ) ) )
    
  def chooseColorMap( self ):

    try:
    
      objectNames = self._viewer.getObjectsForAWindow( self._viewName,
                                                       self._windowName )
    except:
    
      return                                             

    terminalObjects = []                                             
    for objectName in objectNames:
    
      object = self._viewer.getObject( objectName )
      terminalObjects += self.getChildren( object )

    if ( len( terminalObjects ) ):

      terminalObjectNames = []
      for terminalObject in terminalObjects:
    
        terminalObjectName = self._viewer.getObjectName( terminalObject )
        terminalObjectNames.append( terminalObjectName )

      colorMapNames = self._viewer.getColorMapNames()   
      colorMapDialog = ColorMapDialog( self._viewer,
                                       terminalObjectNames,
                                       colorMapNames,
                                       None )
      result = colorMapDialog.exec_()
      if result:
    
        print 'color map changed'

      del colorMapDialog


  def getChildren( self, object ):
  
    terminalElements = []
    if ( object.objectType == 'FUSION2D' ):

      for child in object.children:
    
        terminalElements += self.getChildren( child )

    else:

      terminalElements.append( object )
    
    return terminalElements

  def snapshot( self ):
  
    fileDialog = QtWidgets.QFileDialog()
    fileDialog.setFileMode( QtWidgets.QFileDialog.AnyFile )
    fileDialog.setAcceptMode( QtWidgets.QFileDialog.AcceptSave )
    result = fileDialog.exec_()
    fileName = None
    if result:
    
      fileName = unicode( fileDialog.selectedFiles()[ 0 ] )

    if fileName is not None:

      self._viewer.getAnatomist().execute(
                         'WindowConfig',
                         windows=[ self._viewer.getWindow( self._viewName,
                                                           self._windowName ) ],
                         snapshot=fileName )


  def comment( self ):
      
    commentDialog = QtWidgets.QDialog()
    oldText = ''
    if self._comments[ self._viewName ].has_key( self._windowName ):
    
      oldText = self._comments[ self._viewName ][ self._windowName ][ 0 ]
      print oldText

    text, ok = QtWidgets.QInputDialog().getText( commentDialog, 'Comment addition', 
                                             'Enter your comment:',
                                             QtWidgets.QLineEdit.Normal,
                                             oldText )

    if ( ok and ( text != '' ) ):

      print 'Your comment : ', text
      oldState = self._state
      
      if ( self._state == 0 ):
      
        self.zoomInOrOut()

      applicationDirectory = ResourceManager().getApplicationTempDirectory(
                    self._viewer.getApplication().getParentToolBox().getName() )
      algorithmName = self._viewer.getApplication().getAlgorithmGUI().\
                                   getAlgorithm().getName()
      tempFileName = tempfile.mktemp( suffix = '.png',
                                      prefix = 'snapshot-' +
                                                algorithmName + '-' +
                                                self._viewName + '-' +
                                                self._windowName + '-',
                                      dir = applicationDirectory )

      print 'tempFileName=', tempFileName
        
      self._viewer.getAnatomist().execute(
                       'WindowConfig',
                       windows = [ self._viewer.getWindow( self._viewName,
                                                           self._windowName ) ],
                       snapshot = tempFileName )
      
      if ( self._state != oldState ):
      
        self.zoomInOrOut()
                         
      self._comments[ self._viewName ][ self._windowName ] = unicode( text ), \
                                                             tempFileName

################################################################################
# class Window3DCallbackManager
################################################################################

class Window3DCallbackManager:

  def __init__( self,
                viewer,
                viewName,
                window,
                showAnatomyButton = None,
                object2DName = None,
                windowName = None,
                windowsSynchronized3d = [] ):

    self._viewer = viewer
    self._viewName = viewName
    self._window = window
    self._displayPushButton = False
    self._showAnatomyButton = showAnatomyButton
    self._object2DName = object2DName
    self._windowName = windowName
    self._windowsSynchronized3d = windowsSynchronized3d

  def display( self ):

    view = self._viewer.getViewFromName( self._viewName )
    if ( self._displayPushButton == True ):

      self._displayPushButton = False
      self._showAnatomyButton.setIcon( QtGui.QIcon( \
       ResourceManager().getIconFileName( 'icon-show-anatomy.png' ) ) )

      if ( self._viewer._objects.has_key( self._object2DName ) ):

        self._viewer.removeObjectFromWindow( self._object2DName,
                                             self._viewName,
                                             self._windowName )

    else:

      self._displayPushButton = True
      self._showAnatomyButton.setIcon( QtGui.QIcon( \
       ResourceManager().getIconFileName( 'icon-hide-anatomy.png' ) ) )
      if ( self._viewer._objects.has_key( self._object2DName ) ):

        self._viewer.addObjectToWindow( self._object2DName,
                                        self._viewName,
                                        self._windowName )

  def synchronize3d( self ):

    wconf = self._window.getInfos()
    for windowSynchronized3d in self._windowsSynchronized3d:

      if ( wconf[ 'windowType' ] == '3D' ):

        self._viewer._windows[ self._viewName ]\
                             [ windowSynchronized3d ].mute3D()

      if ( wconf[ 'windowType' ] == 'Axial' ):

        self._viewer._windows[ self._viewName ]\
                             [ windowSynchronized3d ].muteAxial()

      if ( wconf[ 'windowType' ] == 'Coronal' ):

        self._viewer._windows[ self._viewName ]\
                             [ windowSynchronized3d ].muteCoronal()

      if ( wconf[ 'windowType' ] == 'Sagittal' ):

        self._viewer._windows[ self._viewName ]\
                             [ windowSynchronized3d ].muteSagittal()

      self._viewer._windows[ self._viewName ][ windowSynchronized3d ].camera( \
                         observer_position = wconf[ 'observer_position' ],
                         cursor_position = wconf[ 'position' ],
                         zoom = wconf[ 'zoom' ],
                         view_quaternion = wconf[ 'view_quaternion' ] )

  def setClippingDistance( self, value ):

    self._viewer.getAnatomist().execute(
                         'WindowConfig',
                         windows=[ self._viewer.getWindow( self._viewName,
                                                           self._windowName ) ],
                         clip_distance = value ) 

################################################################################
# class GraphCallbackManager
################################################################################

class GraphCallbackManager:

  def __init__( self,
                viewer,
                viewName,
                viewLayout,
                zoomViewLayout,
                graphLayout,
                graphName,
                zoomToolButton,
                row,
                column,
                rowSpan,
                columnSpan,
                slider = None,
                comments = None,
                widgetWithOverlay = None ):
  
    self._viewer = viewer
    self._viewName = viewName
    self._viewLayout = viewLayout
    self._zoomViewLayout = zoomViewLayout
    self._graphLayout = graphLayout
    self._graphName = graphName
    self._zoomToolButton = zoomToolButton
    self._row = row
    self._column = column
    self._rowSpan = rowSpan
    self._columnSpan = columnSpan
    self._state = 0
    self._slider = slider
    self._comments = comments
    self._widgetWithOverlay = widgetWithOverlay


  def zoomInOrOut( self ):
  
    if ( self._state == 0 ):
    
      self._viewLayout.removeItem( self._graphLayout )
      self._zoomToolButton.setIcon( QtGui.QIcon(
                    ResourceManager().getIconFileName( 'icon-zoom-out.png' ) ) )
      self._graphLayout.setParent( None )
      self._zoomViewLayout.addLayout( self._graphLayout, 0, 0, 1, 1 )
      self._viewer.setViewFromIndex( 0 )
      
      self._state = 1
      if( self._widgetWithOverlay != None ):

        self._widgetWithOverlay.zoomIn()

    else:

      self._zoomViewLayout.removeItem( self._graphLayout )
      self._zoomToolButton.setIcon( QtGui.QIcon( 
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
      self._graphLayout.setParent( None )
      self._viewLayout.addLayout( self._graphLayout,
                                  self._row, self._column,
                                  self._rowSpan, self._columnSpan )
      self._viewer.setViewFromName( self._viewName )
      self._state = 0  
      if( self._widgetWithOverlay != None ):

        self._widgetWithOverlay.zoomOut()


  def zoomOut( self ):
  
    if ( self._state == 1 ):
    
      self._zoomViewLayout.removeItem( self._graphLayout )
      self._zoomToolButton.setIcon( QtGui.QIcon( 
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
      self._graphLayout.setParent( None )
      self._viewLayout.addLayout( self._graphLayout,
                                  self._row, self._column,
                                  self._rowSpan, self._columnSpan )
      self._viewer.setViewFromName( self._viewName )
  
      self._state = 0
      if( self._widgetWithOverlay != None ):

        self._widgetWithOverlay.zoomOut()

  def snapshot( self ):
  
    fileDialog = QtWidgets.QFileDialog()
    fileDialog.setFileMode( QtWidgets.QFileDialog.AnyFile )
    fileDialog.setAcceptMode( QtWidgets.QFileDialog.AcceptSave )
    result = fileDialog.exec_()
    fileName = None
    if result:
    
      fileName = unicode( fileDialog.selectedFiles()[ 0 ] )

    if fileName is not None:

      self._viewer.getFigure( self._viewName, self._graphName ).savefig(
                                                                      fileName )

  def sliderMatrixIntensity( self, value ):
    
    if self._slider is not None:
      
      # Sanity check
      if ( self._viewer._colorBars.has_key( self._viewName ) ):
      
        if ( self._viewer._colorBars[ self._viewName ].has_key( 
                                                            self._graphName ) ):
        
          
          # colorbar = \
          #         self._viewer._colorBars[ self._viewName ][ self._graphName ]
          
          # sanity check
          if ( value != 0 ):
            
            self._viewer._axesImage[ self._viewName ] \
                                   [ self._graphName ].set_clim( 0, value )
            #colorbar.set_clim( 0, value )
            #colorbar.draw_all()
            self._viewer._figures[ self._viewName ] \
                                 [ self._graphName ].canvas.draw()


  def comment( self ):

    commentDialog = QtWidgets.QDialog()
    oldText = ''
    if self._comments[ self._viewName ].has_key( self._graphName ):

      oldText = self._comments[ self._viewName ][ self._graphName ][ 0 ]
      print oldText

    text, ok = QtWidgets.QInputDialog().getText( commentDialog, 'Comment addition', 
                                             'Enter your comment:',
                                             QtWidgets.QLineEdit.Normal,
                                             oldText )

    if ( ok and ( text != '' ) ):

      print 'Your comment : ', text
      oldState = self._state

      if ( self._state == 0 ):

        self.zoomInOrOut()

      applicationDirectory = ResourceManager().getApplicationTempDirectory(
                    self._viewer.getApplication().getParentToolBox().getName() )
      algorithmName = self._viewer.getApplication().getAlgorithmGUI().\
                                   getAlgorithm().getName()

      tempFileName = tempfile.mktemp( suffix = '.png',
                                      prefix = 'snapshot-' +
                                               algorithmName + '-' +
                                               self._viewName + '-' +
                                               self._graphName + '-',
                                      dir = applicationDirectory )

      print 'tempFileName=', tempFileName

      self._viewer.getFigure( self._viewName, self._graphName ).savefig(
                                                                  tempFileName )

      if ( self._state != oldState ):

        self.zoomInOrOut()

      self._comments[ self._viewName ][ self._graphName ] = unicode( text ), \
                                                            tempFileName



################################################################################
# class Viewer
################################################################################

class Viewer:

  def __init__( self, minimumSize, parent ):


    self._parent = parent    
    self._minimumSize = minimumSize
    self._anatomist = None
    self._colorMapNames = []

    # main widget
    self._viewStackedLayout = None
    self._defaultViewIndex = int( 1 )

    # creating structures for zoom view
    self._zoomView = None
    self._zoomLayout = None
     
    # creating structures to manage other views
    self._views = {}
    self._zoomView = None
    self._viewIndices = {}
    self._viewNames = {}

    self._comments = {}

    self._layouts = {}
    self._zoomLayout = None

    self._windows = {}
    self._graphs = {}
    self._pdfViewers = {}

    self._canvas = {}
    self._figures = {}
    self._plots = {}
    self._colorBars = {}
    self._axesImage = {}
    self._sliders = {}

    self._windowLayouts = {}
    self._graphLayouts = {}
    self._pdfViewerLayouts = {}
    self._widgetLayouts = {}

    self._callbackManagers = {}
    self._threeDCallbackManagers = {}

    self._objects = {}
    self._referentials = {}
    self._widgets = {}

    self._objectsForAWindow = {}
    self._windowsForAnObject = {}

    self._objectNames = {}

    self._isActivated = False

    self.open()
    

  def __del__( self ):
  
    self.close()


  def open( self ):
  
    # start the Anatomist engine, in batch mode (no main window)
    self._anatomist = ana.Anatomist( '-b' )

    # detecting palettes
    self._colorMapNames = []
    palettes = self._anatomist.getPalettes()
    for p in palettes:
    
      self._colorMapNames.append( p.name )
    
    # creating view stacked layout
    self._viewStackedLayout = QtWidgets.QStackedLayout()

    self._referentials = { 'centralRef' : self._anatomist.centralRef }

    # creating zoom view
    self.createZoomView()
    
    # boolean to manager display activation and cancelation
    self._isActivated = True

    # initializing overlay action
    OverlayActionActivation()


  def close( self ):
  
    # probably to be further developed
    if ( self._anatomist is not None ):

      self._anatomist.close()


  def getApplication( self ):

    return self._parent


  def getAnatomist( self ):
  
    return self._anatomist
    
  def setActivated( self, value ):
  
    self._isActivated = value
  
  def createZoomView( self ):
  
    # setting zoom view index
    viewIndex = 0

    # creating view widget
    self._zoomView = QtWidgets.QWidget( self._parent )
    self._zoomView.setMinimumSize( self._minimumSize[ 0 ],
                                   self._minimumSize[ 1 ] )
    self._zoomView.hide()

    # creating layout for zoom view
    self._zoomLayout = QtWidgets.QGridLayout()    
    self._zoomLayout.setSpacing( 2 )
    self._zoomView.setLayout( self._zoomLayout )
  
    # adding the view to the stacked layout
    self._viewStackedLayout.addWidget( self._zoomView )
    self._zoomView.hide()

  def createView( self, viewName ):
  
    # sanity check
    if ( self._views.has_key( viewName ) ):
    
      message = '\'' + viewName + '\' view already present'
      raise RuntimeError, message

    # collecting view index
    viewIndex = self.getViewCount()

    # creating view widget
    self._views[ viewName ] = QtWidgets.QWidget( self._parent )
    self._views[ viewName ].setMinimumSize( self._minimumSize[ 0 ],
                                            self._minimumSize[ 1 ] )
    self._views[ viewName ].hide()

    # filling look-up tables
    self._viewIndices[ viewName ] = int( viewIndex )
    self._viewNames[ int( viewIndex ) ] = str( viewName )

    # creating layout for view
    self._layouts[ viewName ] = QtWidgets.QGridLayout()    
    self._layouts[ viewName ].setSpacing( 2 )
    self._views[ viewName ].setLayout( self._layouts[ viewName ] )

    # initializing window, widget and graph look-up tables
    self._windows[ viewName ] = {}
    self._graphs[ viewName ] = {}
    self._pdfViewers[ viewName ] = {}
    self._widgets[ viewName ] = {}

    # initializing graph canvas and figures
    self._canvas[ viewName ] = {}
    self._figures[ viewName ] = {}
    self._plots[ viewName ] = {}
    self._colorBars[ viewName ] = {}
    self._axesImage[ viewName ] = {}
    self._sliders[ viewName ] = {}

    # initializing window, widget and graph look-up tables
    self._windowLayouts[ viewName ] = {}
    self._graphLayouts[ viewName ] = {}
    self._pdfViewerLayouts[ viewName ] = {}
    self._widgetLayouts[ viewName ] = {}

    # initializing callback manager look-up table
    self._callbackManagers[ viewName ] = {}
    self._threeDCallbackManagers[ viewName ] = {}

    # initializing object look-up tables
    self._objectsForAWindow[ viewName ] = {}
    self._windowsForAnObject[ viewName ] = {}

    # adding the view to the stacked layout
    self._viewStackedLayout.addWidget( self._views[ viewName ] )
    self._views[ viewName ].hide()

    # comments
    self._comments[ viewName ] = {}


  def getViewStackedLayout( self ):
  
    return self._viewStackedLayout


  def getViewCount( self ):
  
    return len( self._viewIndices ) + 1


  def hasViewFromName( self, viewName ):
  
    if ( self._views.has_key( str( viewName ) ) ):
    
      return True
  
    return False


  def getViewFromName( self, viewName ):
  
    if ( not self.hasViewFromName( viewName ) ):
    
      message = '\'' + viewName + '\' view not found'
      raise RuntimeError, message
  
    return self._views[ str( viewName ) ]


  def hasViewFromIndex( self, viewIndex ):
  
    if ( self._viewNames.has_key( int( viewIndex ) ) ):
    
      return True
  
    return False


  def getViewFromIndex( self, viewIndex ):
  
    if ( not self.hasViewFromIndex( viewIndex ) ):
    
      message = 'view of index' + viewIndex + ' not found'
      raise RuntimeError, message
  
    return self._views[ self._viewNames[ int( viewIndex ) ] ]


  def getZoomView( self ):
  
    return self._zoomView
    

  def getZoomViewIndex( self ):
  
    return O
    

  def getDefaultView( self ):
  
    return getViewFromIndex( self.getDefaultViewIndex() )
    

  def setDefaultViewIndex( self, defaultViewIndex ):
  
    self._defaultViewIndex = int( defaultViewIndex )
    

  def getDefaultViewIndex( self ):
  
    return self._defaultViewIndex


  def getComments( self ):
  
    return self._comments


  def zoomOutAllWindowsAndGraphs( self ):
  
    for viewName in self._callbackManagers.keys():
    
      for name in self._callbackManagers[ viewName ].keys():
    
        self._callbackManagers[ viewName ][ name ].zoomOut()


  def setViewFromName( self, viewName ):
  
    if ( not self._viewIndices.has_key( str( viewName ) ) ):
    
      message = 'view name ' + viewName + ' not found'
      raise RuntimeError, message
  
    self._viewStackedLayout.setCurrentIndex( self._viewIndices[ viewName ] )


  def setViewFromIndex( self, viewIndex ):

    if ( ( int( viewIndex ) < 0 ) and \
         ( int( viewIndex ) >= self.getViewCount() ) ):
         
      message = 'view index ' + viewIndex + ' invalid'
      raise RuntimeError, message

    self._viewStackedLayout.setCurrentIndex( viewIndex )


  def setDefaultView( self ):
  
    self._viewStackedLayout.setCurrentIndex( self.getDefaultViewIndex() )


  def getLayoutFromName( self, viewName ):
  
    if ( not self._layouts.has_key( viewName ) ):
    
      message = '\'' + viewName + '\' layout not found'
      raise RuntimeError, message
  
    return self._layouts[ viewName ]


  def getLayoutFromIndex( self, viewIndex ):
  
    if ( not self._viewNames.has_key( int( viewIndex ) ) ):
    
      message = 'view of index' + viewIndex + ' not found'
      raise RuntimeError, message
  
    return self._layouts[ self._viewNames[ int( viewIndex ) ] ]


  def addAxialWindow( self, viewName,
                      row, column, rowSpan, columnSpan,
                      name, label,
                      backgroundColor = [ 0.0, 0.0, 0.0, 1.0 ],
                      isMuteToolButtonVisible = True,
                      isColorMapToolButtonVisible = True,
                      isSnapshotToolButtonVisible = True,
                      isZoomToolButtonVisible = True,
                      isCommentToolButtonVisible = True,
                      layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):
 
      layout = self.getLayoutFromName( viewName )
    
    command = ana.cpp.CreateWindowCommand( 'Axial',
                                           -1,
                                           None,
                                           [],
                                           1,
                                           view, 1, 0,
                                           { '__syntax__' : 'dictionary',
                                             'no_decoration' : 1,
                                             'hidden' : True } )
    self._anatomist.execute( command )
    axialWindow = self._anatomist.AWindow( self._anatomist,
                                           command.createdWindow() )
    command.createdWindow().setAcceptDrops( False )
    axialWindow.setParent( None )
    
    windowLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( windowLayout, row, column, rowSpan, columnSpan )

    print 'je passe par la a'

    buttons = list()

    zoomToolButton = QtWidgets.QToolButton( view )
    zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
    buttons.append( zoomToolButton )

    muteToolButton = QtWidgets.QToolButton( view )
    muteToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-window-axial-small.png' ) ) )
    buttons.append( muteToolButton )

    colorMapToolButton = QtWidgets.QToolButton( view )
    colorMapToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-color-map-small.png' ) ) )
    buttons.append( colorMapToolButton )

    snapshotToolButton = QtWidgets.QToolButton( view )
    snapshotToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-snapshot.png' ) ) )
    buttons.append( snapshotToolButton )

    commentToolButton = QtWidgets.QToolButton( view )
    commentToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-comment.png' ) ) )
    buttons.append( commentToolButton )


    print 'je passe par la b'

    widgetWithOverlay = WidgetWithOverlay( axialWindow.getInternalRep(),
                                           buttons,
                                           label )


    print 'je passe par la c'

    callbackManager = WindowCallbackManager( self,
                                             viewName,
                                             layout,
                                             self._zoomLayout,
                                             windowLayout,
                                             name,
                                             0,
                                             zoomToolButton,
                                             muteToolButton,
                                             row,
                                             column,
                                             rowSpan,
                                             columnSpan,
                                             self._comments,
                                             widgetWithOverlay )

    self._callbackManagers[ viewName ][ name ] = callbackManager

    zoomToolButton.clicked.connect( callbackManager.zoomInOrOut )
    muteToolButton.clicked.connect( callbackManager.mute )
    colorMapToolButton.clicked.connect( callbackManager.chooseColorMap )
    snapshotToolButton.clicked.connect( callbackManager.snapshot )
    commentToolButton.clicked.connect( callbackManager.comment )

    windowLayout.addWidget( widgetWithOverlay )

    axialWindow.releaseAppRef()
    axialWindow.muteAxial()
    self._anatomist.execute( 'WindowConfig',
                             windows = [ axialWindow ],
                             light = { 'background' : backgroundColor } )

    qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                                     QtWidgets.QSizePolicy.MinimumExpanding )
    qSizePolicy.setHeightForWidth( True )
    axialWindow.getInternalRep().setSizePolicy( qSizePolicy )
    axialWindow.getInternalRep().updateGeometry()

    self._windows[ viewName ][ name ] = axialWindow
    self._windowLayouts[ viewName ][ name ] = windowLayout

    if ( isMuteToolButtonVisible == False ):
    
      muteToolButton.hide()

    if ( isColorMapToolButtonVisible == False ):
    
      colorMapToolButton.hide()

    if ( isSnapshotToolButtonVisible == False ):
    
      snapshotToolButton.hide()

    if ( isZoomToolButtonVisible == False ):
    
      zoomToolButton.hide()

    if ( isCommentToolButtonVisible == False ):
    
      commentToolButton.hide()      


  def addCoronalWindow( self, viewName,
                        row, column, rowSpan, columnSpan,
                        name, label,
                        backgroundColor = [ 0.0, 0.0, 0.0, 1.0 ],
                        isMuteToolButtonVisible = True,
                        isColorMapToolButtonVisible = True,
                        isSnapshotToolButtonVisible = True,
                        isZoomToolButtonVisible = True,
                        isCommentToolButtonVisible = True,
                        layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):
 
      layout = self.getLayoutFromName( viewName )
    
    command = ana.cpp.CreateWindowCommand( 'Coronal',
                                           -1,
                                           None,
                                           [],
                                           1,
                                           view, 1, 0,
                                           { '__syntax__' : 'dictionary',
                                             'no_decoration' : 1,
                                             'hidden' : True } )
    self._anatomist.execute( command )
    coronalWindow = self._anatomist.AWindow( self._anatomist,
                                           command.createdWindow() )
    command.createdWindow().setAcceptDrops( False )
    coronalWindow.setParent( None )
    
    windowLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( windowLayout, row, column, rowSpan, columnSpan )

    buttons = list()

    zoomToolButton = QtWidgets.QToolButton( view )
    zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
    buttons.append( zoomToolButton )

    muteToolButton = QtWidgets.QToolButton( view )
    muteToolButton.setIcon( QtGui.QIcon(
        ResourceManager().getIconFileName( 'icon-window-coronal-small.png' ) ) )
    buttons.append( muteToolButton )

    colorMapToolButton = QtWidgets.QToolButton( view )
    colorMapToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-color-map-small.png' ) ) )
    buttons.append( colorMapToolButton )

    snapshotToolButton = QtWidgets.QToolButton( view )
    snapshotToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-snapshot.png' ) ) )
    buttons.append( snapshotToolButton )

    commentToolButton = QtWidgets.QToolButton( view )
    commentToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-comment.png' ) ) )
    buttons.append( commentToolButton )

    widgetWithOverlay = WidgetWithOverlay( coronalWindow.getInternalRep(),
                                           buttons,
                                           label )

    callbackManager = WindowCallbackManager( self,
                                             viewName,
                                             layout,
                                             self._zoomLayout,
                                             windowLayout,
                                             name,
                                             0,
                                             zoomToolButton,
                                             muteToolButton,
                                             row,
                                             column,
                                             rowSpan,
                                             columnSpan,
                                             self._comments,
                                             widgetWithOverlay )

    self._callbackManagers[ viewName ][ name ] = callbackManager

    zoomToolButton.clicked.connect( callbackManager.zoomInOrOut )
    muteToolButton.clicked.connect( callbackManager.mute )
    colorMapToolButton.clicked.connect( callbackManager.chooseColorMap )
    snapshotToolButton.clicked.connect( callbackManager.snapshot )
    commentToolButton.clicked.connect( callbackManager.comment )

    windowLayout.addWidget( widgetWithOverlay )

    coronalWindow.releaseAppRef()
    coronalWindow.muteCoronal()
    self._anatomist.execute( 'WindowConfig',
                             windows = [ coronalWindow ],
                             light = { 'background' : backgroundColor } )

    qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                                     QtWidgets.QSizePolicy.MinimumExpanding )
    qSizePolicy.setHeightForWidth( True )
    coronalWindow.getInternalRep().setSizePolicy( qSizePolicy )
    coronalWindow.getInternalRep().updateGeometry()

    self._windows[ viewName ][ name ] = coronalWindow
    self._windowLayouts[ viewName ][ name ] = windowLayout

    if ( isMuteToolButtonVisible == False ):
    
      muteToolButton.hide()

    if ( isColorMapToolButtonVisible == False ):
    
      colorMapToolButton.hide()

    if ( isSnapshotToolButtonVisible == False ):
    
      snapshotToolButton.hide()

    if ( isZoomToolButtonVisible == False ):
    
      zoomToolButton.hide()

    if ( isCommentToolButtonVisible == False ):
    
      commentToolButton.hide()      

  def addSagittalWindow( self, viewName,
                        row, column, rowSpan, columnSpan,
                        name, label,
                        backgroundColor = [ 0.0, 0.0, 0.0, 1.0 ],
                        isMuteToolButtonVisible = True,
                        isColorMapToolButtonVisible = True,
                        isSnapshotToolButtonVisible = True,
                        isZoomToolButtonVisible = True,
                        isCommentToolButtonVisible = True,
                        layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):
 
      layout = self.getLayoutFromName( viewName )
    
    command = ana.cpp.CreateWindowCommand( 'Sagittal',
                                           -1,
                                           None,
                                           [],
                                           1,
                                           view, 1, 0,
                                           { '__syntax__' : 'dictionary',
                                             'no_decoration' : 1,
                                             'hidden' : True } )
    self._anatomist.execute( command )
    sagittalWindow = self._anatomist.AWindow( self._anatomist,
                                           command.createdWindow() )
    command.createdWindow().setAcceptDrops( False )
    sagittalWindow.setParent( None )
    
    windowLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( windowLayout, row, column, rowSpan, columnSpan )

    buttons = list()

    zoomToolButton = QtWidgets.QToolButton( view )
    zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
    buttons.append( zoomToolButton )

    muteToolButton = QtWidgets.QToolButton( view )
    muteToolButton.setIcon( QtGui.QIcon(
       ResourceManager().getIconFileName( 'icon-window-sagittal-small.png' ) ) )
    buttons.append( muteToolButton )

    colorMapToolButton = QtWidgets.QToolButton( view )
    colorMapToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-color-map-small.png' ) ) )
    buttons.append( colorMapToolButton )

    snapshotToolButton = QtWidgets.QToolButton( view )
    snapshotToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-snapshot.png' ) ) )
    buttons.append( snapshotToolButton )

    commentToolButton = QtWidgets.QToolButton( view )
    commentToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-comment.png' ) ) )
    buttons.append( commentToolButton )


    widgetWithOverlay = WidgetWithOverlay( sagittalWindow.getInternalRep(),
                                           buttons,
                                           label )

    callbackManager = WindowCallbackManager( self,
                                             viewName,
                                             layout,
                                             self._zoomLayout,
                                             windowLayout,
                                             name,
                                             0,
                                             zoomToolButton,
                                             muteToolButton,
                                             row,
                                             column,
                                             rowSpan,
                                             columnSpan,
                                             self._comments,
                                             widgetWithOverlay )

    self._callbackManagers[ viewName ][ name ] = callbackManager

    zoomToolButton.clicked.connect( callbackManager.zoomInOrOut )
    muteToolButton.clicked.connect( callbackManager.mute )
    colorMapToolButton.clicked.connect( callbackManager.chooseColorMap )
    snapshotToolButton.clicked.connect( callbackManager.snapshot )
    commentToolButton.clicked.connect( callbackManager.comment )

    windowLayout.addWidget( widgetWithOverlay )

    sagittalWindow.releaseAppRef()
    sagittalWindow.muteSagittal()
    self._anatomist.execute( 'WindowConfig',
                             windows = [ sagittalWindow ],
                             light = { 'background' : backgroundColor } )

    qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                                     QtWidgets.QSizePolicy.MinimumExpanding )
    qSizePolicy.setHeightForWidth( True )
    sagittalWindow.getInternalRep().setSizePolicy( qSizePolicy )
    sagittalWindow.getInternalRep().updateGeometry()

    self._windows[ viewName ][ name ] = sagittalWindow
    self._windowLayouts[ viewName ][ name ] = windowLayout

    if ( isMuteToolButtonVisible == False ):
    
      muteToolButton.hide()

    if ( isColorMapToolButtonVisible == False ):
    
      colorMapToolButton.hide()

    if ( isSnapshotToolButtonVisible == False ):
    
      snapshotToolButton.hide()

    if ( isZoomToolButtonVisible == False ):
    
      zoomToolButton.hide()

    if ( isCommentToolButtonVisible == False ):
    
      commentToolButton.hide() 
   

  def add3DWindow( self, viewName,
                   row, column, rowSpan, columnSpan, 
                   name, label,
                   backgroundColor = [ 0.0, 0.0, 0.0, 1.0 ],
                   isMuteToolButtonVisible = True,
                   isColorMapToolButtonVisible = True,
                   isSnapshotToolButtonVisible = True,
                   isZoomToolButtonVisible = True,
                   isClippingSpinBoxVisible = False,
                   isCommentToolButtonVisible = True,
                   clippingOptions = [ 2, 0.01, 1000.0, 2.0, 0.1, 2 ],
                   object2DName = None,
                   windowsSynchronized3d = [],
                   layout = None ):

    if ( self._isActivated == False ):

      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):

      layout = self.getLayoutFromName( viewName )

    command = ana.cpp.CreateWindowCommand( '3D',
                                           -1,
                                           None,
                                           [],
                                           1,
                                           view, 1, 0,
                                           { '__syntax__' : 'dictionary',
                                             'no_decoration' : 1,
                                             'hidden' : True } )
    self._anatomist.execute( command )
    threeDWindow = self._anatomist.AWindow( self._anatomist,
                                            command.createdWindow() )
    command.createdWindow().setAcceptDrops( False )
    threeDWindow.setParent( None )

    windowLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( windowLayout, row, column, rowSpan, columnSpan )

    buttons = list()
    # 2D object buttons
    showAnatomyButton = None
    if ( object2DName != None ):

      showAnatomyButton = QtWidgets.QToolButton( view )
      buttons.append( showAnatomyButton )

    # 3D synchronization button
    if ( len( windowsSynchronized3d ) ):

      synchronization3dToolButton = QtWidgets.QToolButton( view )
      synchronization3dToolButton.setIcon( QtGui.QIcon( \
       ResourceManager().getIconFileName( 'icon-synchronization-3d.png' ) ) )
      buttons.append( synchronization3dToolButton )

    zoomToolButton = QtWidgets.QToolButton( view )
    zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
    buttons.append( zoomToolButton )

    muteToolButton = QtWidgets.QToolButton( view )
    muteToolButton.setIcon( QtGui.QIcon(
       ResourceManager().getIconFileName( 'icon-window-3d-small.png' ) ) )
    buttons.append( muteToolButton )

    colorMapToolButton = QtWidgets.QToolButton( view )
    colorMapToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-color-map-small.png' ) ) )
    buttons.append( colorMapToolButton )

    snapshotToolButton = QtWidgets.QToolButton( view )
    snapshotToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-snapshot.png' ) ) )
    buttons.append( snapshotToolButton )

    commentToolButton = QtWidgets.QToolButton( view )
    commentToolButton.setIcon( QtGui.QIcon(
          ResourceManager().getIconFileName( 'icon-comment.png' ) ) )
    buttons.append( commentToolButton )

    widgetWithOverlay = WidgetWithOverlay( threeDWindow.getInternalRep(),
                                           buttons,
                                           label )

    callbackManager = WindowCallbackManager( self,
                                             viewName,
                                             layout,
                                             self._zoomLayout,
                                             windowLayout,
                                             name,
                                             0,
                                             zoomToolButton,
                                             muteToolButton,
                                             row,
                                             column,
                                             rowSpan,
                                             columnSpan,
                                             self._comments,
                                             widgetWithOverlay )

    threeDCallbackManager = None
    threeDCallbackManager = Window3DCallbackManager( self,
                                                     viewName,
                                                     threeDWindow,
                                                     showAnatomyButton,
                                                     object2DName,
                                                     name,
                                                     windowsSynchronized3d )

    self._callbackManagers[ viewName ][ name ] = callbackManager
    self._threeDCallbackManagers[ viewName ][ name ] = threeDCallbackManager

    zoomToolButton.clicked.connect( callbackManager.zoomInOrOut )
    muteToolButton.clicked.connect( callbackManager.mute )
    colorMapToolButton.clicked.connect( callbackManager.chooseColorMap )
    snapshotToolButton.clicked.connect( callbackManager.snapshot )

    if ( object2DName != None ):

      showAnatomyButton.clicked.connect( threeDCallbackManager.display )

    if ( len( windowsSynchronized3d ) ):

      synchronization3dToolButton.clicked.connect(
                                           threeDCallbackManager.synchronize3d )

    windowLayout.addWidget( widgetWithOverlay )

    threeDWindow.releaseAppRef()
    self._anatomist.execute( 'WindowConfig',
                             windows = [ threeDWindow ],
                             light = { 'background' : backgroundColor } )
  
    qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                                     QtWidgets.QSizePolicy.MinimumExpanding )
    qSizePolicy.setHeightForWidth( True )
    threeDWindow.getInternalRep().setSizePolicy( qSizePolicy )
    threeDWindow.getInternalRep().updateGeometry()

    self._windows[ viewName ][ name ] = threeDWindow
    self._windowLayouts[ viewName ][ name ] = windowLayout

    if( isClippingSpinBoxVisible ):

      spinBoxLayout = QtWidgets.QHBoxLayout()
      self._anatomist.execute( 'WindowConfig', 
                               windows = [ threeDWindow ],
                               clipping = clippingOptions[ 0 ] )

      clippingLabel = QtWidgets.QLabel( 'Clipping distance: ' )
      clippingSpinBox = QtWidgets.QDoubleSpinBox( view )
      clippingSpinBoxvalueChanged.connect(
                                     threeDCallbackManager.setClippingDistance )
      clippingSpinBox.setMinimum( clippingOptions[ 1 ] )
      clippingSpinBox.setMaximum( clippingOptions[ 2 ] )
      clippingSpinBox.setValue( clippingOptions[ 3 ] )
      clippingSpinBox.setSingleStep( clippingOptions[ 4 ] )
      clippingSpinBox.setDecimals( clippingOptions[ 5 ] )
      clippingSpinBox.setReadOnly( False )

      spinBoxLayout.addWidget( clippingLabel )
      spinBoxLayout.addWidget( clippingSpinBox )

      windowLayout.addLayout( spinBoxLayout )

    if ( isMuteToolButtonVisible == False ):
    
      muteToolButton.hide()

    else:

      callbackManager.mute( 0 )

    if ( isColorMapToolButtonVisible == False ):
    
      colorMapToolButton.hide()

    if ( isSnapshotToolButtonVisible == False ):
    
      snapshotToolButton.hide()

    if ( isZoomToolButtonVisible == False ):
    
      zoomToolButton.hide()

    if ( isCommentToolButtonVisible == False ):

      commentToolButton.hide()


  def addPdfViewer( self, viewName,
                    row, column, rowSpan, columnSpan,
                    name, label,
                    layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):

      layout = self.getLayoutFromName( viewName )

    pdfViewerLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( pdfViewerLayout, row, column, rowSpan, columnSpan )
    
    pdfViewer = PdfViewerWidget( view )
    pdfViewerLayout.addWidget( pdfViewer )
    
    self._pdfViewers[ viewName ][ name ] = pdfViewer
    self._pdfViewerLayouts[ viewName ][ name ] = pdfViewerLayout


  def addGraph( self, viewName,
                row, column, rowSpan, columnSpan,
                name, label,
                xLabel, yLabel, callBack,
                isSnapshotToolButtonVisible = True,
                isZoomToolButtonVisible = True,
                isCommentToolButtonVisible = True,
                layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):

      layout = self.getLayoutFromName( viewName )

    graphLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( graphLayout, row, column, rowSpan, columnSpan )
    
    buttons = list()

    zoomToolButton = QtWidgets.QToolButton( view )
    zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
    buttons.append( zoomToolButton )
    snapshotToolButton = QtWidgets.QToolButton( view )
    snapshotToolButton.setIcon( QtGui.QIcon(
                    ResourceManager().getIconFileName( 'icon-snapshot.png' ) ) )
    buttons.append( snapshotToolButton )
    commentToolButton = QtWidgets.QToolButton( view )
    commentToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-comment.png' ) ) )
    buttons.append( commentToolButton )

    figure = Figure( ( 1.2, 0.6 ), 60 )
    figure.set_facecolor( 'w' )
    figure.set_frameon( True )
    axes = figure.add_subplot( 111, alpha=0.1 )
    axes.hold( False )
    axes.set_xlabel( xLabel )
    axes.set_ylabel( yLabel )

    canvas = FigureCanvasQTAgg( figure )
    canvas.setSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                          QtWidgets.QSizePolicy.MinimumExpanding )
    canvas.updateGeometry()

    widgetWithOverlay = WidgetWithOverlay( canvas,
                                           buttons,
                                           label,
                                           None,
                                           None,
                                           1.0 )

    if ( callBack is not None ):
    
      canvas.mpl_connect( 'pick_event', callBack )

    callbackManager = GraphCallbackManager( self,
                                            viewName,
                                            layout,
                                            self._zoomLayout,
                                            graphLayout,
                                            name,
                                            zoomToolButton,
                                            row,
                                            column,
                                            rowSpan,
                                            columnSpan,
                                            None,
                                            self._comments,
                                            widgetWithOverlay )



    self._callbackManagers[ viewName ][ name ] = callbackManager

    zoomToolButton.clicked.connect( callbackManager.zoomInOrOut )
    snapshotToolButton.clicked.connect( callbackManager.snapshot )
    commentToolButton.clicked.connect( callbackManager.comment )

    graphLayout.addWidget( widgetWithOverlay )
    canvas.mpl_connect( 'motion_notify_event',
                        widgetWithOverlay.mouseMovedInGraph )

    self._graphs[ viewName ][ name ] = axes
    self._canvas[ viewName ][ name ] = canvas
    self._figures[ viewName ][ name ] = figure
    self._graphLayouts[ viewName ][ name ] = graphLayout
    
    if ( isSnapshotToolButtonVisible == False ):
    
      snapshotToolButton.hide()

    if ( isZoomToolButtonVisible == False ):
    
      zoomToolButton.hide()

    if ( isCommentToolButtonVisible == False ):

      commentToolButton.hide()      



  def addMatrixGraph( self, viewName,
                      row, column, rowSpan, columnSpan,
                      name, label,
                      xLabel, yLabel, callBack,
                      backgroundColor = [ 1.0, 1.0, 1.0, 1.0 ],
                      isSnapshotToolButtonVisible = True,
                      isZoomToolButtonVisible = True,
                      isCommentToolButtonVisible = True,
                      layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]

    if ( layout == None ):

      layout = self.getLayoutFromName( viewName )

    graphLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( graphLayout, row, column, rowSpan, columnSpan )
    
    buttons = list()

    zoomToolButton = QtWidgets.QToolButton( view )
    zoomToolButton.setIcon( QtGui.QIcon(
                     ResourceManager().getIconFileName( 'icon-zoom-in.png' ) ) )
    buttons.append( zoomToolButton )
    snapshotToolButton = QtWidgets.QToolButton( view )
    snapshotToolButton.setIcon( QtGui.QIcon(
                    ResourceManager().getIconFileName( 'icon-snapshot.png' ) ) )
    buttons.append( snapshotToolButton )
    commentToolButton = QtWidgets.QToolButton( view )
    commentToolButton.setIcon( QtGui.QIcon(
                    ResourceManager().getIconFileName( 'icon-comment.png' ) ) )

    buttons.append( commentToolButton )

    figure = Figure()
    figure.set_facecolor( 'w' )
    figure.set_frameon( True )
    axes = figure.add_subplot( 111 )
    axes.hold( False )
    defaultLabels = [ 'label0', 'label1', 'label2', 'label3' ]
    axes.set_xlabel( xLabel )
    axes.set_ylabel( yLabel )
    
    canvas = FigureCanvasQTAgg( figure )
    canvas.setSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                          QtWidgets.QSizePolicy.MinimumExpanding )
    canvas.updateGeometry()
    if ( callBack is not None ):
    
      canvas.mpl_connect( 'pick_event', callBack )
    
    # Add slider
    slider = QtWidgets.QSlider( QtCore.Qt.Vertical, view )
    slider.setRange( 0, 100 )
    slider.setValue( 100 )

    mainWidget = QtWidgets.QWidget()
    horizontalLayoutMatrix = QtWidgets.QHBoxLayout()
    horizontalLayoutMatrix.addWidget( canvas )
    horizontalLayoutMatrix.addWidget( slider )
    mainWidget.setLayout( horizontalLayoutMatrix )

    widgetWithOverlay = WidgetWithOverlay( mainWidget,
                                           buttons,
                                           label,
                                           canvas = canvas,
                                           backgroundColor = backgroundColor )

    callbackManager = GraphCallbackManager( self,
                                            viewName,
                                            layout,
                                            self._zoomLayout,
                                            graphLayout,
                                            name,
                                            zoomToolButton,
                                            row,
                                            column,
                                            rowSpan,
                                            columnSpan,
                                            slider,
                                            self._comments,
                                            widgetWithOverlay )


    self._callbackManagers[ viewName ][ name ] = callbackManager

    graphLayout.addWidget( widgetWithOverlay )

    canvas.mpl_connect( 'motion_notify_event',
                        widgetWithOverlay.mouseMovedInGraph )

    zoomToolButton.clicked.connect( callbackManager.zoomInOrOut )
    snapshotToolButton.clicked.connect( callbackManager.snapshot )
    slider.valueChanged.connect( callbackManager.sliderMatrixIntensity )
    commentToolButton.clicked.connect( callbackManager.comment )

    self._graphs[ viewName ][ name ] = axes
    self._canvas[ viewName ][ name ] = canvas
    self._figures[ viewName ][ name ] = figure
    self._sliders[ viewName ][ name ] = slider
    self._graphLayouts[ viewName ][ name ] = graphLayout
    
    if ( isSnapshotToolButtonVisible == False ):
    
      snapshotToolButton.hide()

    if ( isZoomToolButtonVisible == False ):
    
      zoomToolButton.hide()

    if ( isCommentToolButtonVisible == False ):

      commentToolButton.hide()      



  def addBrowserWindow( self, viewName,
                        row, column, rowSpan, columnSpan,
                        name, label,
                        layout = None ):

    if ( self._isActivated == False ):
    
      return

    view = self.getViewFromName( viewName )
    viewIndex = self._viewIndices[ viewName ]
    if ( layout == None ):

      layout = self.getLayoutFromName( viewName )

    command = ana.cpp.CreateWindowCommand( 'Browser',
                                           -1,
                                           None,
                                           [],
                                           1,
                                           view, 1, 0,
                                           { '__syntax__' : 'dictionary',
                                             'no_decoration' : 1,
                                             'hidden' : True } )
    self._anatomist.execute( command )
    browserWindow = self._anatomist.AWindow( self._anatomist,
                                            command.createdWindow() )
    command.createdWindow().setAcceptDrops( False )
    browserWindow.setParent( None )

    windowLayout = QtWidgets.QVBoxLayout()
    layout.addLayout( windowLayout, row, column, rowSpan, columnSpan )

    labelWidget = None
    if ( label is not None ):

      labelWidget = QtWidgets.QLabel( label )
      labelWidget.setAlignment( QtCore.Qt.AlignHCenter |
                                QtCore.Qt.AlignBottom )
      labelWidget.setFont( QtGui.QFont( 'Arial', 8, 1, True ) )

    mainWidget = QtWidgets.QWidget()
    verticalLayoutBrowser = QtWidgets.QVBoxLayout()
    verticalLayoutBrowser.addWidget( browserWindow.getInternalRep() )
    if ( labelWidget is not None ):

      verticalLayoutBrowser.addWidget( labelWidget )
    mainWidget.setLayout( verticalLayoutBrowser )
    widgetWithOverlay = WidgetWithOverlay( mainWidget, None, None )
    windowLayout.addWidget( widgetWithOverlay )

    qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                                     QtWidgets.QSizePolicy.MinimumExpanding )
    qSizePolicy.setHeightForWidth( True )
    browserWindow.getInternalRep().setSizePolicy( qSizePolicy )
    browserWindow.getInternalRep().updateGeometry()

    self._windows[ viewName ][ name ] = browserWindow
    self._windowLayouts[ viewName ][ name ] = windowLayout


  def addListWidget( self, viewName,
                     row, column, rowSpan, columnSpan,
                     name, label,
                     selectionMode = QtWidgets.QAbstractItemView.SingleSelection,
                     layout = None ):

    if ( self._isActivated == False ):
    
      return

    widgetLayout = QtWidgets.QVBoxLayout()
    if ( layout == None ):

      layout = self.getLayoutFromName( viewName )

    layout.addLayout( widgetLayout, row, column, rowSpan, columnSpan )

    listWidget = QtWidgets.QListWidget()
    listWidget.setSelectionMode( selectionMode )

    if ( label is not None ):

      labelWidget = QtWidgets.QLabel( label )
      labelWidget.setAlignment( QtCore.Qt.AlignHCenter |
                                QtCore.Qt.AlignBottom )
      labelWidget.setFont( QtGui.QFont( 'Arial', 8, 1, True ) )

    mainWidget = QtWidgets.QWidget()
    verticalLayoutListWidget = QtWidgets.QVBoxLayout()
    verticalLayoutListWidget.addWidget( labelWidget )
    verticalLayoutListWidget.addWidget( listWidget )
    mainWidget.setLayout( verticalLayoutListWidget )

    widgetWithOverlay = WidgetWithOverlay( mainWidget, None, None )
    widgetLayout.addWidget( widgetWithOverlay )

    qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.MinimumExpanding,
                                     QtWidgets.QSizePolicy.MinimumExpanding )
    qSizePolicy.setHeightForWidth( True )

    self._widgets[ viewName ][ name ] = listWidget
    self._widgetLayouts[ viewName ][ name ] = widgetLayout


  def reset( self ):

    # erasing objects
    for key in self._objects.keys():

      self.deleteObject( key )
    
    for viewName in self._threeDCallbackManagers.keys():

      for name in self._threeDCallbackManagers[ viewName ].keys():

        if( self._threeDCallbackManagers[ viewName ][ name ] != None ):

          self._threeDCallbackManagers[ viewName ]\
                                             [ name ]._displayPushButton = False
          if( self._threeDCallbackManagers[ viewName ]\
                                          [ name ]._showAnatomyButton != None ):

            self._threeDCallbackManagers[ viewName ]\
                [ name ]._showAnatomyButton.setIcon( QtGui.QIcon( \
                ResourceManager().getIconFileName( 'icon-show-anatomy.png' ) ) )
    
    # erasing referentials
    for key in self._referentials.keys():
    
      if ( key != 'centralRef' ):
      
        self.deleteReferential( key )

    # clearing graphs
    for viewName in self._graphs.keys():
    
      for key in self._plots[ viewName ].keys():
      
        for plot in self._plots[ viewName ][ key ]:
        
          plot.remove()
          del plot
        
        del self._plots[ viewName ][ key ]

      for key in self._graphs[ viewName ].keys():
      
        self._graphs[ viewName ][ key ].cla()
        self._graphs[ viewName ][ key ].set_xlim( 0.0, 1.0 )
        self._graphs[ viewName ][ key ].set_ylim( 0.0, 1.0 )
        self._canvas[ viewName ][ key ].draw()

    # clearing widget
    for viewName in self._widgets.keys():

      for key in self._widgets[ viewName ].keys():

        self._widgets[ viewName ][ key ].clear()


  def loadObject( self, fileName, objectName ):
  
    if ( self._isActivated == False ):
    
      return

    if ( self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' already present'
      raise RuntimeError, message

    anatomistObject = None
    if ( fileName == '' ):

      print fileName, 'not found'

    else:

      anatomistObject = self._anatomist.loadObject( fileName )

    self._objects[ objectName ] = anatomistObject
    self._objectNames[ anatomistObject ] = objectName
    if ( anatomistObject != None ):

      print '\'' + objectName + '\' -> ' + fileName + \
            ' of type ' + anatomistObject.objectType + ' read'

  def deleteObject( self, objectName ):

    # sanity check
    if ( not self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' not found'
      raise RuntimeError, message

    # first removing object from all windows in all views
    for viewName in self._windowsForAnObject.keys():
    
      self.removeObjectFromAllWindows( objectName, viewName )

    # removing object name from object names dictionary
    if( self._objects[ objectName ] != None ):

      del self._objectNames[ self._objects[ objectName ] ]

      # then destroying the object itself
      self._anatomist.deleteObjects( [ self._objects[ objectName ] ] )
    
    # removing the object from the object dictionary
    del self._objects[ objectName ]
    
    print 'object ' + objectName + ' deleted'
    

  def reloadObject( self, objectName ):

    if ( self._isActivated == False ):
    
      return

    # sanity check
    if ( not self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' not found'
      raise RuntimeError, message

    # then destroying the object itself
    if( self._objects[ objectName ] != None ):

      self._anatomist.reloadObjects( [ self._objects[ objectName ] ] )


  def fusionObjects( self, objectNames, fusionName, method = None ):
  
    if ( self._isActivated == False ):
    
      return

    if ( self._objects.has_key( fusionName ) ):
    
      message = 'object \'' + fusionName + '\' already present'
      raise RuntimeError, message
    
    objects = []
    for objectName in objectNames:
    
      objects.append( self.getObject( objectName ) )
    
    anatomistObject = self._anatomist.fusionObjects( objects, method )
    
    if ( anatomistObject is not None ):
    
      self._objects[ fusionName ] = anatomistObject
      self._objectNames[ anatomistObject ] = fusionName
  

  def setFusion2dParameters( self, fusionName, fusionRate, method = 'linear' ):
  
    if ( self._isActivated == False ):
    
      return

    if not ( self._objects.has_key( fusionName ) ):
    
      message = 'object \'' + fusionName + '\' not registered'
      raise RuntimeError, message
    
    fusionObject = self.getObject( fusionName )
    
    self._anatomist.execute( 'Fusion2DParams', 
                             object = fusionObject,
                             mode = method,
                             rate = fusionRate )


  def setColormap( self, objectName, colormapName,
                   lowerValue = None, upperValue = None ):
  
    if ( self._isActivated == False ):
    
      return

    if ( not self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' not found'
      raise RuntimeError, message
    print 'objectName=', objectName, ' lower=', lowerValue, ' upperValue=', \
          upperValue

    if( self._objects[ objectName ] != None ):

      self._objects[ objectName ].setPalette( colormapName,
                                            lowerValue, upperValue,
                                            None, None, None, None,
                                            None, None, True )


  def getColorMapNames( self ):
  
    return self._colorMapNames
    

  def setDiffuse( self, objectName, diffuse ):
  
    if ( self._isActivated == False ):
    
      return

    if ( not self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' not found'
      raise RuntimeError, message

    if( self._objects[ objectName ] != None ):

      self._objects[ objectName ].setMaterial( None, True, None, diffuse )


  def addObjectToWindow( self, objectName, viewName, windowName,
                         addChildren = False,
                         addGraphNodes = True,
                         addGraphRelations = False,
                         addWholeGraphObject = False ):
  
    if ( self._isActivated == False ):
    
      return

    # sanity check
    if ( not self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' not found'
      raise RuntimeError, message

    # updating object list for a given window
    if ( not self._objectsForAWindow[ viewName ].has_key( windowName ) ):
    
      self._objectsForAWindow[ viewName ][ windowName ] = []
      
    self._objectsForAWindow[ viewName ][ windowName ].append( objectName )

    # updating window list for a given object
    if ( not self._windowsForAnObject[ viewName ].has_key( objectName ) ):
    
      self._windowsForAnObject[ viewName ][ objectName ] = []

    self._windowsForAnObject[ viewName ][ objectName ].append( windowName )

    if( self._objects[ objectName ] != None ):

      if ( ( self.getObject( objectName ).objectType == 'GRAPH' ) \
                                             and not ( addWholeGraphObject )  ):

        self._anatomist.addObjects( self._objects[ objectName ].children,
                                  [ self._windows[ viewName ][ windowName ] ],
                                  add_children = addChildren,
                                  add_graph_nodes = addGraphNodes,
                                  add_graph_relations = addGraphRelations )
      else:

        self._anatomist.addObjects( [ self._objects[ objectName ] ],
                                [ self._windows[ viewName ][ windowName ] ],
                                add_children = addChildren,
                                add_graph_nodes = addGraphNodes,
                                add_graph_relations = addGraphRelations )
 

  def removeObjectFromWindow( self,
                              objectName,
                              viewName,
                              windowName ):

    if ( self._isActivated == False ):

      return

    if( self._objects[ objectName ] != None ):

      self._anatomist.removeObjects( self.getObject( objectName ).children,
                                   [ self.getWindow( viewName, windowName ) ] )
      self._anatomist.removeObjects( [ self.getObject( objectName ) ],
                                   [ self.getWindow( viewName, windowName ) ] )

    self._windowsForAnObject[ viewName ][ objectName ].remove( windowName )
    self._objectsForAWindow[ viewName ][ windowName ].remove( objectName )


  def removeObjectFromAllWindows( self, objectName, viewName ):

    if ( self._isActivated == False ):
    
      return

    if ( self._windowsForAnObject[ viewName ].has_key( objectName ) ):
    
      windowNames = self._windowsForAnObject[ viewName ][ objectName ]
      for w in windowNames:

        self.removeObjectFromWindow( objectName, viewName, w )


  def createReferential( self, referentialName ):

    if ( self._isActivated == False ):
    
      return

    if ( self._referentials.has_key( referentialName ) ):
    
      message = 'referential \'' + referentialName + '\' already present'
      raise RuntimeError, message
      
    self._referentials[ referentialName ] = self._anatomist.createReferential()


  def deleteReferential( self, referentialName ):

    # sanity check
    if ( not self._referentials.has_key( referentialName ) ):
    
      message = 'referential \'' + referentialName + '\' not found'
      raise RuntimeError, message

    # removing the object from the object dictionary
    del self._referentials[ referentialName ]
    

  def assignReferentialToObject( self, referentialName, objectName ):
  
    if ( self._isActivated == False ):
    
      return

    self._anatomist.assignReferential( self.getReferential( referentialName ),
                                       self.getObject( objectName ) )


  def assignReferentialToWindow( self, referentialName, viewName, windowName ):
  
    if ( self._isActivated == False ):
    
      return

    self._anatomist.assignReferential( self.getReferential( referentialName ),
                                       self.getWindow( viewName, windowName ) )


  def loadTransformation( self, transformation,
                          originReferentialName, destinationReferentialName ):
                          
    if ( self._isActivated == False ):
    
      return
    
    if type( transformation ) == str:
    
      # transform is a filename
      self._anatomist.loadTransformation(
                             transformation,
                             self.getReferential( originReferentialName ),
                             self.getReferential( destinationReferentialName ) )

    elif type( transformation ) == list:
      
      # transform is a vector of 12 parameters (get from 4x3 anatomist matrix)
      
      if len( transformation ) != 12:
      
        message = 'expecting vector of size 12!'
        raise RuntimeError, message
      
      command = ana.cpp.LoadTransformationCommand( 
                             transformation,
                             self.getReferential( originReferentialName ),
                             self.getReferential( destinationReferentialName ) )
      
      self._anatomist.execute( command )
      
    else:
      
      message = 'bad transformation type: expecting filename or vector'
      raise RuntimeError, message
    

  def assignObjectToGraph( self, objectName, graphName ):
  
    if ( self._isActivated == False ):
    
      return

    # Convert anatomist objects to aims ones
    aimsGraph = self._anatomist.toAimsObject( self.getObject( graphName ) )
    v = aimsGraph.addVertex( 'roi' )
    
    aimsObject = self._anatomist.toAimsObject( self.getObject( objectName ) )
    aims.GraphManip.storeAims( aimsGraph, v, 'aims_mesh', aimsObject )
    v[ 'name' ] = objectName
    
    
    if ( self._objects.has_key( graphName ) ):
    
      if ( self._objects[ graphName ] is not None ):
        
        # Trick to avoid pb of reference counter: remove old graph before
        del self._objectNames[ self._objects[ graphName ] ]
        if( self._objects[ graphName ] != None ):

          self._objects[ graphName ].releaseAppRef()

        del self._objects[ graphName ]

    aGraph = self._anatomist.toAObject( aimsGraph )
    
    self._objects[ graphName ] = aGraph
    self._objectNames[ aGraph ] = graphName
    if( self._objects[ graphName ] != None ):

      self._objects[ graphName ].releaseAppRef()
          
    
  def createRoiGraph( self, objectName ):
  
    if ( self._isActivated == False ):
    
      return

    if ( self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' already present'
      raise RuntimeError, message
    
    # Create graph structure
    aimsObject = aims.Graph( 'RoiArg' )
    
    # These are needed for internal conversions
    aimsObject[ 'type.global.tri' ] = 'roi.global.tri'
    aimsObject[ 'roi.global.tri' ] = 'roi'
    
    anatomistObject = self._anatomist.toAObject( aimsObject )
    
    if ( anatomistObject is not None ):

      self._objects[ objectName ] = anatomistObject
      self._objectNames[ anatomistObject ] = objectName
      print '\'' + objectName + '\'' + 'created'
    
    
  def createHierarchy( self, hierarchyName, graphSyntaxName, 
                       listOfNames, listOfRGBColors, fileNameToSave ):
  
    if ( self._isActivated == False ):
    
      return

    if ( self._objects.has_key( hierarchyName ) ):
    
      message = 'object \'' + hierarchyName + '\' already present'
      raise RuntimeError, message

    hierarchy = aims.Hierarchy()
    hierarchy.setSyntax( 'hierarchy' )
    hierarchy[ 'graph_syntax' ] = 'RoiArg'
    
    itemCount = len( listOfNames )
   
    for i in range ( 0, itemCount ):
    
      node = aims.Tree( True, 'fold_name' )
      node[ 'name' ] = listOfNames[ i ]
      R = listOfRGBColors[ i ][ 0 ]
      G = listOfRGBColors[ i ][ 1 ]
      B = listOfRGBColors[ i ][ 2 ]
      node[ 'color' ] = aims.vector_S32( [ R, G, B ] )
      hierarchy.insert( node )
      
    anatomistObject = self._anatomist.toAObject( hierarchy )
      
    if ( anatomistObject is not None ):

      self._objects[ hierarchyName ] = anatomistObject
      self._objectNames[ anatomistObject ] = hierarchyName
      print '\'' + hierarchyName + '\'' + 'created'
    
    if ( len( fileNameToSave )  ):
      
      aims.write( hierarchy, fileNameToSave )
      
    
  def adjustCamera( self, viewName, windowName, 
                    cursorPosition = None,
                    viewQuaternion = None,
                    zoom = None ):

    if ( self._isActivated == False ):
    
      return

    # if default value : try to auto adjust cursor              
    if ( cursorPosition is None and viewQuaternion is None ):
    
      try:

        objectNames = self.getObjectsForAWindow( viewName, windowName )
    
      except:
    
        objectNames = []
        return                                             

      for objectName in objectNames:
      
        try:

          object = self.getObject( objectName )
          if ( object.objectType == 'VOLUME' ):
            
            volAims = self._anatomist.toAimsObject( object )

            voxelSize = volAims.header()[ 'voxel_size' ]
            tmpVolDimensions = volAims.header()[ 'volume_dimension' ]
            volumeSize = [ int( i ) for i in tmpVolDimensions ]
            volumeCenter = [ v * s / 2 for v, s in zip( volumeSize,
                                                        voxelSize ) ]
            cursorPosition = volumeCenter
            break

          elif( object.objectType == 'GRAPH' ):

            cursorPosition = object.boundingbox()[ 1 ].list()
            cursorPosition[ 0 ] = cursorPosition[ 0 ] / 2
            cursorPosition[ 1 ] = cursorPosition[ 1 ] / 2
            cursorPosition[ 2 ] = cursorPosition[ 2 ] / 2

          elif( object.objectType == 'FUSION2D' ):

            children = object.children
            for child in children:

              if ( child.objectType == 'VOLUME' ):

                volAims = self._anatomist.toAimsObject( child )

                voxelSize = volAims.header()[ 'voxel_size' ]
                tmpVolDimensions = volAims.header()[ 'volume_dimension' ]
                volumeSize = [ int( i ) for i in tmpVolDimensions ]
                volumeCenter = [ v * s / 2 for v, s in zip( volumeSize,
                                                        voxelSize ) ]
                cursorPosition = volumeCenter
                break


        except:
          
          print objectName, ' not found...'

    self._anatomist.execute( 
                           'Camera', 
                           windows = [ self.getWindow( viewName, windowName ) ],
                           zoom = zoom, 
                           observer_position = None, 
                           view_quaternion = viewQuaternion, 
                           slice_quaternion = None, 
                           force_redraw = False, 
                           cursor_position = cursorPosition, 
                           boundingbox_min = None, 
                           boundingbox_max = None )

  def setScene( self,
                viewName,
                windowName, 
                cursor_visibility, 
                show_toolbars, 
                view_size, 
                backGroundColor ):

    if ( self._isActivated == False ):
    
      return

    self._anatomist.execute( 
                           'WindowConfig', 
                           windows = [ self.getWindow( viewName, windowName ) ],
                           show_toolbars = show_toolbars,
                           cursor_visibility = cursor_visibility, 
                           view_size = view_size,
                           light = { 'background' : backGroundColor } )


  def snapshot( self, viewName, windowName, fileName ):

    if ( self._isActivated == False ):
    
      return

    self._anatomist.execute( 
                           'WindowConfig', 
                           windows = [ self.getWindow( viewName, windowName ) ],
                           snapshot = fileName )


  def muteWindowToAxial( self, viewName, windowName ):
  
    self.getWindow( viewName, windowName ).muteAxial()


  def muteWindowToCoronal( self, viewName, windowName ):
  
    self.getWindow( viewName, windowName ).muteCoronal()


  def muteWindowToSagittal( self, viewName, windowName ):
  
    self.getWindow( viewName, windowName ).muteSagittal()


  def setGraphParams( self, useNomenclature ):
                          
    if ( self._isActivated == False ):
    
      return

    self._anatomist.setGraphParams( useNomenclature  )


  def addObjectToGraph( self, x, y,
                        xLabel, yLabel,
                        xMinimum, xMaximum,
                        yMinimim, yMaximum,
                        viewName, graphName ):

    if ( self._isActivated == False ):
    
      return

    self._plots[ viewName ][ graphName ] = \
      self._graphs[ viewName ][ graphName ].plot( x, y, 'o',
                                                picker=5, alpha=0.5,
                                                label='Outliers'  )
    self._graphs[ viewName ][ graphName ].set_xlabel( xLabel )
    self._graphs[ viewName ][ graphName ].set_ylabel( yLabel )
    self._graphs[ viewName ][ graphName ].set_xlim( xMinimum, xMaximum )
    self._graphs[ viewName ][ graphName ].set_ylim( yMinimim, yMaximum )
    self._graphs[ viewName ][ graphName ].get_xaxis().grid( True )
    self._graphs[ viewName ][ graphName ].get_yaxis().grid( True )
    self._graphs[ viewName ][ graphName ].set_autoscale_on( False )
    self._graphs[ viewName ][ graphName ].legend()
    self._canvas[ viewName ][ graphName ].draw()

 
  def addObjectToMatrixGraph( self,
                              viewName, graphName, 
                              dataMatrix,
                              xLabel, yLabel,
                              xAxisLabels, yAxisLabels ):

    if ( self._isActivated == False ):
    
      return

    # Sanity check
    if not len( dataMatrix ):
      
      print 'empty data matrix, cannot do not display it'
      return

    cax = self._graphs[ viewName ][ graphName ].matshow( 
                                                     dataMatrix,
                                                     aspect = 'auto',
                                                     interpolation = 'nearest' )
    
    self._axesImage[ viewName ][ graphName ] = cax
    
    # checking whether colorbar already exists before creating it
    if ( self._colorBars.has_key( viewName ) ):
    
      if ( self._colorBars[ viewName ].has_key( graphName ) ):
    
        self._colorBars[ viewName ][ graphName ].draw_all()
                          
      else:
      
        self._colorBars[ viewName ][ graphName ] = \
                          self._figures[ viewName ][ graphName ].colorbar( cax )
        
    else:
      
      self._colorBars[ viewName ][ graphName ] = \
                          self._figures[ viewName ][ graphName ].colorbar( cax )
    
    
    
    # sanity check
    if ( ( len( yAxisLabels ) != 0 ) and \
         ( len( yAxisLabels ) != len( dataMatrix ) ) ):
      
      message = 'bad yAxisLabels size!'
      raise RuntimeError, message
    
    if ( ( len( xAxisLabels ) != 0 ) and 
         ( len( xAxisLabels ) != len( dataMatrix[ 0 ] ) ) ):
      
      message = 'bad xAxisLabels size!'
      raise RuntimeError, message
    
    
    # retrieving ticks for each axis
    xTicks = self._graphs[ viewName ][ graphName ].get_xticks()
    yTicks = self._graphs[ viewName ][ graphName ].get_yticks()
    
    # sublist of labels which will be displayed in axis (not all label may be 
    # displayed if there are too many...)
    subXTicksLabels = []
    subYTicksLabels = []
    
    if ( len( xTicks ) > 0 ): 
      
      for i in range ( 1, len( xTicks ) ):
        
        if ( ( int( xTicks[ i ] ) ) < len( xAxisLabels ) ):
          
          subXTicksLabels.append( xAxisLabels[ int( xTicks[ i ] ) ] )
        
    
    if ( len( yTicks ) > 0 ):
      
      for i in range ( 1, len( yTicks ) ):
        
        if ( ( int( yTicks[ i ] ) ) < len( yAxisLabels ) ):
          
          subYTicksLabels.append( yAxisLabels[ int( yTicks[ i ] ) ] )
    
    
    # setting new tick    
    if ( len( xAxisLabels ) ) :
      
      
      #Info : rotation parameter has only an effect with matplotlib > v1.00
      self._graphs[ viewName ][ graphName ].set_xticklabels(
                                         [''] + subXTicksLabels, rotation = 45 )
    
    if ( len( xAxisLabels ) ) :
      
      self._graphs[ viewName ][ graphName ].set_yticklabels( 
                                                        [''] + subYTicksLabels )
      
    # Find matrix max value
    matrixDimX = len( dataMatrix[ 0 ] )
    matrixDimY = len( dataMatrix )
    matrixMaxValue = 0
    
    for i in range( 0, matrixDimX ):
      
      for j in range( 0, matrixDimY ):
        
        if ( dataMatrix[ j ][ i ] > matrixMaxValue ):
          
           matrixMaxValue = dataMatrix[ j ][ i ]      
      
    if ( self._sliders.has_key( viewName ) ):
      
      if ( self._sliders[ viewName ].has_key( graphName ) ) :
        
        self._sliders[ viewName ][ graphName ].setRange( 0, matrixMaxValue )
        self._sliders[ viewName ][ graphName ].setValue( matrixMaxValue )
      
    self._graphs[ viewName ][ graphName ].set_xlabel( xLabel )
    self._graphs[ viewName ][ graphName ].set_ylabel( yLabel )
    self._canvas[ viewName ][ graphName ].draw()

 
  def addPdfToPdfViewer( self, viewName, pdfViewerName, fileNamePdf ):

    if ( self._isActivated == False ):
    
      return

    self._pdfViewers[ viewName ][ pdfViewerName ].load( fileNamePdf )

 
  def getObject( self, objectName ):

    if ( not self._objects.has_key( objectName ) ):
    
      message = 'object \'' + objectName + '\' not found'
      raise RuntimeError, message
    
    return self._objects[ objectName ]


  def getObjectName( self, object ):

    for k in self._objectNames.keys():
    
      if object == k:
      
        return self._objectNames[ k ]
        
    message = 'object not found'
    raise RuntimeError, message


  def getObjectsForAWindow( self, viewName, windowName ):

    if ( not self._objectsForAWindow.has_key( viewName ) ):
    
      message = 'view \'' + viewName + '\' not found'
      raise RuntimeError, message
    

    if ( not self._objectsForAWindow[ viewName ].has_key( windowName ) ):

      message = 'window \'' + windowName + '\' not found'
      raise RuntimeError, message

    return self._objectsForAWindow[ viewName ][ windowName ]
    

  def getReferential( self, referentialName ):

    if ( not self._referentials.has_key( referentialName ) ):
    
      message = 'referential \'' + referentialName + '\' not found'
      raise RuntimeError, message
    
    return self._referentials[ referentialName ]


  def getWindow( self, viewName, windowName ):
  
    if ( not self._windows.has_key( viewName ) ):
    
      message = '\'' + viewName + '\' view not found'
      raise RuntimeError, message
  
    if ( not self._windows[ viewName ].has_key( windowName ) ):
    
      message = '\'' + windowName + '\' window not found for \'' + viewName + \
                '\' view'
      raise RuntimeError, message
  
    return self._windows[ viewName ][ windowName ]


  def getGraph( self, viewName, graphName ):
  
    if ( not self._graphs.has_key( viewName ) ):
    
      message = '\'' + viewName + '\' view not found'
      raise RuntimeError, message
  
    if ( not self._graphs[ viewName ].has_key( graphName ) ):
    
      message = '\'' + graphName + '\' graph not found for \'' + viewName + \
                '\' view'
      raise RuntimeError, message
  
    return self._graphs[ viewName ][ graphName ]

  def getCanvas( self, viewName, graphName ):
  
    if ( not self._canvas.has_key( viewName ) ):
    
      message = '\'' + viewName + '\' view not found'
      raise RuntimeError, message
  
    if ( not self._canvas[ viewName ].has_key( graphName ) ):
    
      message = '\'' + graphName + '\' canvas not found for \'' + viewName + \
                '\' view'
      raise RuntimeError, message
  
    return self._canvas[ viewName ][ graphName ]


  def getFigure( self, viewName, graphName ):
  
    if ( not self._figures.has_key( viewName ) ):
    
      message = '\'' + viewName + '\' view not found'
      raise RuntimeError, message
  
    if ( not self._figures[ viewName ].has_key( graphName ) ):
    
      message = '\'' + graphName + '\' figure not found for \'' + viewName + \
                '\' view'
      raise RuntimeError, message
  
    return self._figures[ viewName ][ graphName ]


  def getWidget( self, viewName, windowName ):

    return self._widgets[ viewName ][ windowName ]
 

  def insertItems( self, viewName, widgetName, items ) :

    listWidget = self.getWidget( viewName, widgetName )
    itemCount = listWidget.count()

    for item in items:

      listWidget.insertItem( itemCount, item )
      itemCount += 1


  def displayHideObject2D( self, viewName, name ):

    self._threeDCallbackManagers[ viewName ][ name ].display()


  def synchronize3DWindows( self, viewName, name ):

    self._threeDCallbackManagers[ viewName ][ name ].synchronize3d()


