from Algorithm import *
from AlgorithmGUI import *
from Viewer import *
from MainThreadActionManager import *
from PyQt5 import *

class Application( QtWidgets.QWidget ):

  def __init__( self,
                algorithmGUI,
                viewerCreator = None,
                preferedSizes = None,
                parentToolBox = None, quitCbk = None ):
  
    self._parentToolBox = parentToolBox
    
    # storing algoritgm GUI and viewer
    self._algorithmGUI = algorithmGUI
    self._viewerCreator = viewerCreator
    self._viewer = None
    self._viewerOpened = False
    self._preferedSizes = preferedSizes

    if( self._algorithmGUI._uifile != None ):

      # change the app's look and feel
      # one of "windows", "motif", "cde",
      # "plastique", "windowsxp", or "macintosh"
      QtWidgets.QApplication.setStyle( QtWidgets.QStyleFactory.create( "plastique" ) )

      # and the corresponding palette
      QtWidgets.QApplication.setPalette( 
                                  QtWidgets.QApplication.style().standardPalette() )

      # initializing widget
      QtWidgets.QWidget.__init__( self, self._parentToolBox )

      # mother horizontal layout
      self._layout = QtWidgets.QHBoxLayout()
      self.setLayout( self._layout )
    
      # adding control panel and viewer to layout
      self._scrollArea = QtWidgets.QScrollArea()
      self._scrollArea.setMinimumWidth(
                              self._algorithmGUI.getPanelWidget().width() + 30 )
      self._scrollArea.setFrameShape( QtWidgets.QFrame.NoFrame )
      self._scrollArea.setWidget( self._algorithmGUI.getPanelWidget() )
    
      qSizePolicy = QtWidgets.QSizePolicy( QtWidgets.QSizePolicy.Fixed,
                                       QtWidgets.QSizePolicy.MinimumExpanding )
      self._scrollArea.setSizePolicy( qSizePolicy )
      self._scrollArea.updateGeometry()

      self._layout.addWidget( self._scrollArea )
      
      self._defaultWidget = QtWidgets.QPushButton()
      #self._defaultWidget.setStyleSheet( 'background-color:lightGray' )
      self._defaultWidget.setFlat( True )
      self._defaultWidget.setAutoFillBackground( True )
      self._defaultWidget.setEnabled( False )
      self._defaultWidget.setMinimumSize( self._preferedSizes[ 0 ],
                                          self._preferedSizes[ 1 ] )
      self._layout.addWidget( self._defaultWidget )

      # adding main thread action manager
      self._mainThreadActionManager = MainThreadActionManager(
                                                      threading.currentThread(),
                                                      self )
                                      
      # managing batch mode
      self._batchMode = False

    else:

      self._batchMode = True

    # adding viewer to algorithm
    self._algorithmGUI.getAlgorithm().addApplication( self )


  def openViewer( self, defaultViewIndex ):


    # BUG_PYANATOMIST
    return

    if ( self._viewerCreator is not None ):
    
      if ( self._viewerOpened == False ):
      
        self._layout.removeWidget( self._defaultWidget )

        self._viewer = self._viewerCreator( self._preferedSizes, self )
        self._layout.addLayout( self._viewer.getViewStackedLayout() )
        self._viewer.setDefaultViewIndex( defaultViewIndex )
        self._viewer.setDefaultView()
        self._viewerOpened = True


  def closeViewer( self ):
  
    if ( self._viewerCreator is not None ):

      print 1
      if ( self._viewerOpened == True ):

        print 2
     
        self._layout.removeItem( self._viewer.getViewStackedLayout() )
        del self._viewer
        self._viewer = None
        self._viewerOpened = False
        self._defaultWidget.setParent( None )
        self._layout.addWidget( self._defaultWidget )
        self._defaultWidget.repaint()
   

  def getParentToolBox( self ):
  
    return self._parentToolBox

  def getAlgorithmGUI( self ):
  
    return self._algorithmGUI
                                      
  def getViewer( self ):
  
    return self._viewer
                                      
  def getMainThreadActionManager( self ):

    return self._mainThreadActionManager
  
  def setBatchMode( self, value ):
  
    self._batchMode = value
    
  def getBatchMode( self ):
  
    return self._batchMode
    
  def addLog( self, logText ):
  
    self._parentToolBox.addLog( logText )

