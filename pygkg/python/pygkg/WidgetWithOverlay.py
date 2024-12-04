import PyQt5
from PyQt5 import QtCore, QtGui, QtWidgets, uic

from Singleton import *
import anatomist.direct.api as ana


def newEventAutoSubscription( self, pool ):

  NoModifier = QtCore.Qt.NoModifier
  ShiftModifier = QtCore.Qt.ShiftModifier
  ControlModifier = QtCore.Qt.ControlModifier
  AltModifier = QtCore.Qt.AltModifier

  self.__class__.__base__.eventAutoSubscription( self, pool )

  self.mouseMoveEventSubscribe( QtCore.Qt.NoButton, NoModifier,
                                pool.action( "OverlayAction" ).mouseMoved )


class OverlayActionActivation( object, metaclass = Singleton ):

  def __init__( self ):

    ad = ana.cpp.ActionDictionary.instance()
    ad.addAction( 'OverlayAction', lambda: OverlayAction() )
    cd = ana.cpp.ControlDictionary.instance()
    c = cd.getControlInstance( 'Default 3D control' )
    cl = c.__class__
    cl._initial_eventAutoSubscription = cl.eventAutoSubscription
    setattr( cl, 'eventAutoSubscription', newEventAutoSubscription )
    cd.addControl( c.name(), cl, c.priority(), True )


class OverlayAction( ana.cpp.Action ):

  def name( self ):

    return 'OverlayAction'

  def mouseMoved( self, x, y, xx, yy ):

    win = self.view().aWindow()
    win.emit(QtCore.SIGNAL( "mouseMoved(int,int)" ), x, y )


class WidgetWithOverlay( QtWidgets.QFrame ):

  def __init__( self, mainWidget,
                      buttonWidgets,
                      label,
                      canvas = None,
                      parent = None,
                      backgroundColor = None ):
  
    QtWidgets.QFrame.__init__( self, parent )

    self._mainWidget = mainWidget

    #add main window
    self._mainWidget.setParent( self )
    self._layout = QtWidgets.QHBoxLayout( self )
    self._layout.addWidget( self._mainWidget )
    
    # add menu widgets
    self._menuWidget = QtWidgets.QWidget( self )

    if ( buttonWidgets != None ):

      self._menuWidget.setMaximumHeight( 22 )
      self._horizontalLayout = QtWidgets.QHBoxLayout( self._menuWidget )
      self._horizontalLayout.setContentsMargins( 0, 0, 0, 0 )
      self._menuWidget.setParent( self )    
      for buttonWidget in buttonWidgets:

        self._horizontalLayout.addWidget( buttonWidget )
      
      self._horizontalLayout.addStretch()


      self._menuWidget.move( 5, 5 )
      self._menuWidget.setProperty( 'isMenuWidget', True )
      if( backgroundColor == None ):

        self._menuWidget.setStyleSheet( '*[isMenuWidget="true"]{ ' + \
                                         ' background-color : black; }' )

      else:

        self._menuWidget.setStyleSheet( '*[isMenuWidget="true"]{ ' + \
                                         ' background-color : white; }' )


      #QObject.connect( self._mainWidget,
      #                         QtCore.SIGNAL( 'mouseMoved(int,int)' ),
      #                         self.mouseMoved )
                               
      self._mainWidget.mouseMoved.connect( self.mouseMoved )

      # set property as being an overlayWidget
      if( backgroundColor == None ):

        self.setStyleSheet( 'QFrame{ background-color : black; ' + \
                                   ' border: 10px; ' + \
                                   ' border-radius: 10px; }' )

      else:

        self.setStyleSheet( 'QFrame{ background-color : white; ' + \
                                   ' border: 10px; ' + \
                                   ' border-radius: 10px; }' )

    #add label window
    self._labelWidget = QtWidgets.QLabel( label )
    if ( label is not None ):

      if( backgroundColor == None ):

        self._labelWidget.setStyleSheet( \
                         'QLabel { background-color : black; color : white; }' )

      else:

        self._labelWidget.setStyleSheet( \
                         'QLabel { background-color : white; color : black; }' )

      self._labelWidget.setAlignment( QtCore.Qt.AlignHCenter |
                                      QtCore.Qt.AlignBottom )
      self._labelWidget.setFont( QtGui.QFont( 'Arial', 8, 1, True ) )
      self._labelWidget.setMaximumHeight( 13 )
      self._labelWidget.setParent( self )    

    self._labelWidget.hide()
    self._menuWidget.hide()

    self._canvas = canvas


  def mouseMoveEvent( self, event ):

    self._labelWidget.show()
    if ( event.y < 20 ):
         
      self._menuWidget.show()
      
    else:
    
      self._menuWidget.hide()

  def mouseMoved( self, x, y ):
 
    self._labelWidget.show()
    if ( y < 20 ):
         
      self._menuWidget.show()
      
    else:
    
      self._menuWidget.hide()

  def mouseMovedInGraph( self, event ):

    if( self._canvas != None ):

      height = self._canvas.height()

    else:

      height = self._mainWidget.height()

    self._labelWidget.show()
    if ( ( height - event.y ) < 20 ):

      self._menuWidget.show()

    else:

      self._menuWidget.hide()

    
  def actionEvent( self, event ):

    QtWidgets.QWidget.actionEvent( self, event )


  def focusOutEvent( self, event ):

    self._labelWidget.hide()
    self._menuWidget.hide()
    QtWidgets.QWidget.focusOutEvent( self, event )
    

  def leaveEvent( self, event ):

    self._labelWidget.hide()
    self._menuWidget.hide()
    QtWidgets.QWidget.leaveEvent( self, event )
    

  def resizeEvent( self, event ):

    self._menuWidget.resize( event.size() )
    self._labelWidget.resize( event.size() )
    self._mainWidget.setGeometry( 5, 5, self.width() - 10, self.height() - 10 )
    self._menuWidget.setGeometry( 5, 
                                  5,
                                  self._mainWidget.width() - 4,
                                  self._menuWidget.height() )
    self._labelWidget.setGeometry( 5, 
                                   5,
                                   self._mainWidget.width() - 4,
                                   self._labelWidget.height() )
    if ( self._canvas != None ):

       self._canvas.setGeometry( 5, 5, self.width() - 30, self.height() - 10 )

    self._labelWidget.move( 5, self.height() - 18 )
    event.accept()


  def zoomIn( self ):

    self._mainWidget.setGeometry( 5, 5, self.width() - 10, self.height() - 10 )
    self._menuWidget.setGeometry( 5, 
                             5,
                             self._mainWidget.width() - 4,
                             self._menuWidget.height() )
    self._labelWidget.setGeometry( 5, 
                                   5,
                                   self._mainWidget.width() - 4,
                                   self._labelWidget.height() )
    if ( self._canvas != None ):

       self._canvas.setGeometry( 5, 5, self.width() - 30, self.height() - 10 )

    self._labelWidget.move( 5, self.height() - 18 )


  def zoomOut( self ):

    self._mainWidget.setGeometry( 5, 5, self.width() - 10, self.height() - 10 )
    self._menuWidget.setGeometry( 5, 
                                  5,
                                  self._mainWidget.width() - 4,
                                  self._menuWidget.height() )
    self._labelWidget.setGeometry( 5, 
                                   5,
                                   self._mainWidget.width() - 4,
                                   self._labelWidget.height() )
    if ( self._canvas != None ):

       self._canvas.setGeometry( 5, 5, self.width() - 30, self.height() - 10 )
    self._labelWidget.move( 5, self.height() - 18 )

