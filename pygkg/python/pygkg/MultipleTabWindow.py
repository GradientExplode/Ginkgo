import PyQt5
from PyQt5 import QtCore, QtWidgets, uic

from ResourceManager import *
from MultipleTabItem import *

class MultipleTabWindow( QtWidgets.QWidget ):

  def __init__( self, parent = None ):

    QtWidgets.QWidget.__init__( self, parent )

    self._window = uic.loadUi( ResourceManager().getUIFileName( \
                                                 'core',
                                                 'MultipleViewTab.ui' ) )

    self._stackedWidget = self._window.findChild( QtWidgets.QStackedWidget,
                                                  'stackedWidget' )
    self._tabWidget = self._window.findChild( QtWidgets.QTabWidget, 'tabWidget' )
    scrollArea = self._window.findChild( QtWidgets.QScrollArea, 'scrollArea' )
    self._scrollArealayout = QtWidgets.QGridLayout()
    scrollArea.setLayout( self._scrollArealayout )

    self._spinBox_RowCount = self._window.findChild( QtWidgets.QSpinBox,
                                                     'spinBox_RowCount' )
    self._spinBox_ColumnCount = self._window.findChild( QtWidgets.QSpinBox,
                                                        'spinBox_ColumnCount' )

    self._window.connect( self._spinBox_RowCount,
                          QtCore.SIGNAL( "valueChanged(int)" ),
                          self.reorganizeWindows )
    self._window.connect( self._spinBox_ColumnCount,
                          QtCore.SIGNAL( "valueChanged(int)" ),
                          self.reorganizeWindows )

    self._tabWidget.clear()
    self._pushButton_Maximize = self._window.findChild( QtWidgets.QPushButton,
                                                        'pushButton_Maximize' )
    self._pushButton_Minimize = self._window.findChild( QtWidgets.QPushButton,
                                                        'pushButton_Minimize' )
 
    self.connect( self._pushButton_Maximize,
                  QtCore.SIGNAL( "clicked()" ),
                  self.showMultipleTabWindowOptions )
    self.connect( self._pushButton_Minimize,
                  QtCore.SIGNAL( "clicked()" ),
                  self.hideMultipleTabWindowOptions )

    self._preferedWidth = 0

    self._multipleTabItems = list()
    self._displayedMultipleTabItemIndices = list()
    self._spacers = list()


  def clear( self ):

    print -1
    self._tabWidget.clear()
    print -2
    self._displayedMultipleTabItemIndices = list()
    print -3
    for multipleTabItem in self._multipleTabItems:

      print -4
      multipleTabItem.clear()
      print -5
      del multipleTabItem
      print -6

  def getMultipleTabItem( self, index ):

    return self._multipleTabItems[ index ]


  def addMultiTabItem( self, anatomist ):

    print -10
    multipleTabItem = MultipleTabItem( anatomist,
                                       len( self._multipleTabItems ) )
    print -11
    self._multipleTabItems.append( multipleTabItem )
    self.connect( multipleTabItem,
                  QtCore.SIGNAL( "addPushButtonClicked(int)" ),
                  self.addWindow )
    print -12
    self.connect( multipleTabItem,
                  QtCore.SIGNAL( "removePushButtonClicked(int)" ),
                  self.removeWindow )
    print -13
    self._window.connect( multipleTabItem.getRowSpanSpinBow(),
                          QtCore.SIGNAL( "valueChanged(int)" ),
                          self.reorganizeWindows )
    self._window.connect( multipleTabItem.getColumnSpanSpinBow(),
                          QtCore.SIGNAL( "valueChanged(int)" ),
                          self.reorganizeWindows )

  def getWindow( self ):

    return self._window


  def getTabWidget( self ):

    return self._tabWidget


  def getScrollAreaLayout( self ):

    return self._scrollArealayout


  def showMultipleTabWindowOptions( self ):

    self._stackedWidget.setCurrentIndex( 1 )
    if ( self._preferedWidth > 360 ):

      self._stackedWidget.setMaximumWidth( 16777215 )

    elif ( self._preferedWidth < 200 ):

      self._stackedWidget.setMaximumWidth( 200 )

    else:

      self._stackedWidget.setMaximumWidth( self._preferedWidth )

   
  def hideMultipleTabWindowOptions( self ):

    self._stackedWidget.setCurrentIndex( 0 )
    self._stackedWidget.setMaximumWidth( 20 )

 
  def setPreferedWidth( self, width ):

    self._preferedWidth = width


  def getRowCount( self ):

    return self._spinBox_RowCount.value()


  def getColumnCount( self ):

    return self._spinBox_ColumnCount.value()


  def getWindowCount( self ):

    return len( self._multipleTabItems )


  def addMultipleTabItem( self, multipleTabItem ):

    self._multipleTabItems.append( multipleTabItem )


  def reorganizeWindows( self, value = None ):

    for multipleTabItem in self._multipleTabItems:

      self._scrollArealayout.removeWidget( multipleTabItem.getWindow() )

    for spacer in self._spacers:

      self._scrollArealayout.removeWidget( spacer )

    self._spacers = list()

    for index in self._displayedMultipleTabItemIndices :

      self.addWindow( index )


  def addWindow( self, index ):
 
    if not( self._displayedMultipleTabItemIndices.count( index ) ):

      self._displayedMultipleTabItemIndices.append( index )

    columnCount = self._spinBox_ColumnCount.value()
    availablePositionFound = False
    row = 0
    column = 0
    while( availablePositionFound == False ):

      if( self._scrollArealayout.itemAtPosition( row, column ) == None ):

        availablePositionFound = True
        for rowSpan in xrange( self._multipleTabItems[ index ].getRowSpan() ):

          for columnSpan in xrange( 
                              self._multipleTabItems[ index ].getColumnSpan() ):

            print 'colm', column + columnSpan, columnCount
            if( self._scrollArealayout.itemAtPosition( 
                  row + rowSpan, column + columnSpan )!= None \
                or ( column + columnSpan ) >= columnCount ):

              availablePositionFound = False
      if not( availablePositionFound ):

        if( column + 1 >= columnCount ):

          print 'chnage row', column, columnCount
          row += 1
          column = 0

        else:

          column += 1

    print row, column
    self._scrollArealayout.addWidget( 
                               self._multipleTabItems[ index ].getWindow(),
                               row,
                               column,
                               self._multipleTabItems[ index ].getRowSpan(),
                               self._multipleTabItems[ index ].getColumnSpan() )

  def removeWindow( self, index ):

    self._displayedMultipleTabItemIndices.remove( index )
    print 'layoutcount avant', self._scrollArealayout.count()
    print self._multipleTabItems[ index ], self._multipleTabItems[ index ].getWindow()
    self._scrollArealayout.removeWidget( self._multipleTabItems[ index ].getWindow() )
    print 'layoutcount apres', self._scrollArealayout.count()
    self._multipleTabItems[ index ].clear()

    self.reorganizeWindows()

