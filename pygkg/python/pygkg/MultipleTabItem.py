import PyQt5
from PyQt5 import QtCore, QtWidgets

import anatomist.direct.api as ana

class MultipleTabItem( QtWidgets.QWidget ):

  def __init__( self,
                anatomist,
                index,
                parent = None ):

    QtWidgets.QWidget.__init__( self, parent )

    self._anatomist = anatomist
    self._index = index

    gridLayout = QtWidgets.QGridLayout()
    self.setLayout( gridLayout )

    self._addPushButton = QtWidgets.QPushButton()
    self._addPushButton.setMaximumWidth( 45 )
    gridLayout.addWidget( self._addPushButton, 0, 0 )

    self._upPushButton = QtWidgets.QPushButton()
    self._upPushButton.setMaximumWidth( 45 )
    gridLayout.addWidget( self._upPushButton, 1, 0 )

    self._downPushButton = QtWidgets.QPushButton()
    self._downPushButton.setMaximumWidth( 45 )
    gridLayout.addWidget( self._downPushButton, 2, 0 )

    self._leftPushButton = QtWidgets.QPushButton()
    self._leftPushButton.setMaximumWidth( 45 )
    gridLayout.addWidget( self._leftPushButton, 3, 0 )

    self._rightPushButton = QtWidgets.QPushButton()
    self._rightPushButton.setMaximumWidth( 45 )
    gridLayout.addWidget( self._rightPushButton, 4, 0 )

    self._rowSpanLabel = QtWidgets.QLabel( 'Row\nspan' )
    self._rowSpanLabel.setMaximumHeight( 40 )
    gridLayout.addWidget( self._rowSpanLabel, 5, 0 )

    self._rowSpanSpinBow = QtWidgets.QSpinBox()
    self._rowSpanSpinBow.setMaximumWidth( 45 )
    self._rowSpanSpinBow.setMinimum( 1 )
    gridLayout.addWidget( self._rowSpanSpinBow, 6, 0 )

    self._columnSpanLabel = QtWidgets.QLabel( 'Column\nspan' )
    self._columnSpanLabel.setMaximumHeight( 40 )
    gridLayout.addWidget( self._columnSpanLabel, 7, 0 )

    self._columnSpanSpinBow = QtWidgets.QSpinBox()
    self._columnSpanSpinBow.setMaximumWidth( 45 )
    self._columnSpanSpinBow.setMinimum( 1 )
    gridLayout.addWidget( self._columnSpanSpinBow, 8, 0 )

    self._windowType = ''
    self._resultFileName = ''

    self.connect( self._addPushButton,
                  QtCore.SIGNAL( "clicked()" ),
                  self.createRemoveWindow )
    self._window = None
    self._aWindow = None
    self._anatomistObject = None
    self._isDisplayed = False

  def getRowSpanSpinBow( self ):

    return self._rowSpanSpinBow


  def getColumnSpanSpinBow( self ):

    return self._columnSpanSpinBow


  def getAddPushButton( self ):

    return self._addPushButton


  def setResultFileName( self, resultFileName ):

    self._resultFileName = resultFileName


  def getResultFileName( self ):

    return self._resultFileName


  def setWindowType( self, windowType ):

    self._windowType = windowType


  def getWindowType( self ):

    return self._windowType


  def clear( self ):

    if ( self._anatomistObject ):

      self._anatomist.deleteObjects( self._anatomistObject )

    if ( self._window ):

      self._window.close()
      self._window = None
      

  def createRemoveWindow( self ):

    if ( self._isDisplayed ):

      self._isDisplayed = False
      self.emit( QtCore.SIGNAL( "removePushButtonClicked(int)" ), self._index )

    else:

      self._isDisplayed = True
      if ( self._windowType == 'bundles' or self._windowType == 'anatomy' ):

        command = None
        if ( self._windowType == 'bundles' ):

          command = ana.cpp.CreateWindowCommand( '3D',
                                                 -1,
                                                 None,
                                                 [],
                                                 1,
                                                 self, 1, 0,
                                                 { '__syntax__' : 'dictionary',
                                                   'no_decoration' : 1,
                                                   'hidden' : True } )
        elif ( self._windowType == 'anatomy' ):

          command = ana.cpp.CreateWindowCommand( 'Axial',
                                                 -1,
                                                 None,
                                                 [],
                                                 1,
                                                 self, 1, 0,
                                                 { '__syntax__' : 'dictionary',
                                                   'no_decoration' : 1,
                                                   'hidden' : True } )
        self._anatomist.execute( command )
        self._aWindow = self._anatomist.AWindow( self._anatomist,
                                                 command.createdWindow() )
        command.createdWindow().setAcceptDrops( False )
        self._aWindow.setParent( None )
        self._anatomist.execute( 
                             'WindowConfig',
                             windows = [ self._aWindow ],
                             light = { 'background' : [ 0.0, 0.0, 0.0, 1.0 ] } )

        self._anatomistObject = self._anatomist.loadObject( 
                                                          self._resultFileName )

        if ( self._windowType == 'bundles' ):

          self._anatomistObject.setMaterial( None,
                                             True,
                                             None,
                                             [ 0.1, 1.0, 1.0, 0.1 ] )

        if ( self._anatomistObject is not None ):

          print '\'' + self._resultFileName + \
                ' of type ' + self._anatomistObject.objectType + ' read'

        if ( self._windowType == 'bundles' ):

          self._anatomist.addObjects( self._anatomistObject.children,
                                      [ self._aWindow ],
                                      add_children = False,
                                      add_graph_nodes = True,
                                      add_graph_relations = False )

        else:

          self._anatomist.addObjects( self._anatomistObject,
                                      [ self._aWindow ],
                                      add_children = False,
                                      add_graph_nodes = True,
                                      add_graph_relations = False )
      self._window = self._aWindow.getInternalRep()
      self.emit( QtCore.SIGNAL( "addPushButtonClicked(int)" ), self._index )


  def getWindow( self ):

    return self._window


  def getAWindow( self ):

    return self._aWindow


  def getRowSpan( self ):

    return self._rowSpanSpinBow.value()


  def getColumnSpan( self ):

    return self._columnSpanSpinBow.value()

