from PyQt5 import QtCore, QtGui, QtWidgets, uic
from ResourceManager import *
from ProcessListWidgetItem import *


class ProcessListWidget:

  def __init__( self,
                processListParameter,
                algorithmGUI,
                addButtonLabelSuffixName,
                uiWidgetName,
                defaultProcessName ):

    self._processListParameter = processListParameter
    self._algorithmGUI = algorithmGUI
    self._defaultProcessName = defaultProcessName
    
    self._processListWidgetItems = []
    self._processListWidgetItemCount = 0

    self._currentIndex = 0
    self._widgetIndex = 0

    # getting pointer to top widget
    self._topWidget = uic.loadUi( ResourceManager().getUIFileName( \
                                                         'core',
                                                         'ProcessListGUI.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x,
                                                             QtCore.QObject,
                                                             y )

    # setting process widget and layout
    chainWidget = self._findChild( self._algorithmGUI.getPanelWidget(),
                                   uiWidgetName )
    chainLayout = self._findChild( self._topWidget,
                                   'gridLayout_ProcessListGUI' )
    chainLayout.setParent( None )
    chainWidget.setLayout( chainLayout )

    # getting pointer to scroll area
    self._scrollAreaWidget = self._findChild( \
                                           self._algorithmGUI.getPanelWidget(),
                                           'scrollArea_ProcessListGUI' )
    self._verticalLayout = self._findChild( \
                                           self._algorithmGUI.getPanelWidget(),
                                           'verticalLayout_ProcessListWidgets' )

    # collecting tool buttons and changing label of 'Add' tool button
    self._toolButtonAdd = self._findChild( self._algorithmGUI.getPanelWidget(),
                                           'toolButton_Add' )
    self._toolButtonAdd.setText( 'Add ' + addButtonLabelSuffixName )

    self._toolButtonGoUp = self._findChild( \
                                            self._algorithmGUI.getPanelWidget(),
                                            'toolButton_GoUp' )
    self._toolButtonGoDown = self._findChild( \
                                            self._algorithmGUI.getPanelWidget(),
                                            'toolButton_GoDown' )
    self._toolButtonTrash = self._findChild( \
                                            self._algorithmGUI.getPanelWidget(),
                                            'toolButton_Trash' )
    self._toolButtonShowDetails = self._findChild( \
                                            self._algorithmGUI.getPanelWidget(),
                                            'toolButton_ShowDetails' )
    self._toolButtonHideDetails = self._findChild( \
                                            self._algorithmGUI.getPanelWidget(),
                                            'toolButton_HideDetails' )

    # setting icons of tool buttons
    self._toolButtonGoUp.setIcon( \
     QtGui.QIcon( ResourceManager().getIconFileName( 'icon-go-up.png' ) ) )
    self._toolButtonGoDown.setIcon( \
     QtGui.QIcon( ResourceManager().getIconFileName( 'icon-go-down.png' ) ) )
    self._toolButtonTrash.setIcon( 
     QtGui.QIcon( ResourceManager().getIconFileName( 'icon-user-trash.png' ) ) )

    # connecting tool buttons to callback(s)
    self._toolButtonAdd.clicked.connect( self.addItem )
    self._toolButtonGoUp.clicked.connect( self.moveItemUp )
    self._toolButtonGoDown.clicked.connect( self.moveItemDown )
    self._toolButtonTrash.clicked.connect( self.removeItem )
    self._toolButtonShowDetails.clicked.connect( self.showDetails )
    self._toolButtonHideDetails.clicked.connect( self.hideDetails )


  def getProcessListParameter( self ):

    return self._processListParameter


  def getAlgorithmGUI( self ):

    return self._algorithmGUI


  def getChild( self, widgetName ):

    return self._findChild( self._algorithmGUI.getPanelWidget(), widgetName )


  def addItem( self, processName = None ):

    # if no process name is provided, setting it to default one
    if ( processName == None ):

      processName = self._defaultProcessName

    # creating new process item
    index = self._processListWidgetItemCount
    widgetIndex = self._widgetIndex
    processListWidgetItem = ProcessListWidgetItem( self,
                                                   index,
                                                   processName,
                                                   widgetIndex )

    # adding it to the list of item(s)
    self._processListWidgetItems.append( processListWidgetItem )

    # setting current index to newly created item index
    self.setCurrentIndex( index )

    self._processListWidgetItemCount += 1
    self._widgetIndex += 1


  def moveItemUp( self ):

    index = self._currentIndex
    if ( self._currentIndex > 0 ):


      self.inverseItems( index, index - 1 )
      self.updateSelectedItem( index, index - 1 )

    else:

      self.updateSelectedItem( index, index )


  def moveItemDown( self ):

    index = self._currentIndex
    if ( index < self._processListWidgetItemCount - 1 ):

      self.inverseItems( index, index + 1 )
      self.updateSelectedItem( index, index + 1 )

    else:

      self.updateSelectedItem( index, index )


  def removeItem( self ):

    if ( self._processListWidgetItemCount > 0 ):

      self._processListWidgetItemCount -= 1

      currentIndex = self._currentIndex
      for itemIndex in xrange( currentIndex, self._processListWidgetItemCount ):

        self.inverseItems( itemIndex, itemIndex + 1 )

      self.reconnectParameters( self._processListWidgetItemCount )
      self.removeItemWidget( self._processListWidgetItemCount )

      if ( currentIndex == self._processListWidgetItemCount ):

        self._currentIndex = currentIndex - 1

      else:

        self._currentIndex = currentIndex 

      self.updateSelectedItem( self._currentIndex, self._currentIndex )


  def showDetails( self ):

    for processListWidgetItem in self._processListWidgetItems:

      processListWidgetItem.setStackedWidgetAsDisplayed( True )


  def hideDetails( self ):

    for processListWidgetItem in self._processListWidgetItems:

      processListWidgetItem.setStackedWidgetAsDisplayed( False )


  def setCurrentIndex( self, currentIndex ):

    oldIndex = self._currentIndex
    self._currentIndex = currentIndex
    self.updateSelectedItem( oldIndex, currentIndex )


  def setValues( self, values ):

    newValues = list()
    for value in values:

      newValues.append( value.copy() )

    self.clear()
    index = 0
    # get process index for every process name

    for value in newValues:

      if not ( value.has_key( 'processName' ) ):

        message = 'parameters have no key \'processName\''
        raise RuntimeError, message

      self.addItem( value[ 'processName' ]  )

      for parameterName in value.keys():

        if not ( parameterName == 'processName' ):

          if ( type( value[ parameterName ] ) == int \
            or type( value[ parameterName ] ) == float \
            or type( value[ parameterName ] ) == bool \
            or type( value[ parameterName ] ) == str \
            or type( value[ parameterName ] ) == list() ):

            self._processListParameter.getParameter( \
               index,
               parameterName ).updateWidget( value[ parameterName ] )
          else:

            self._processListParameter.getParameter( \
               index,
               parameterName ).updateWidget( value[ parameterName ].getValue() )

      index += 1


  def getLayout( self ):

    return self._verticalLayout


  def inverseItems( self, firstIndex, secondIndex ):

    # collecting parameter
    saveFirstParameterValues = self._processListWidgetItems[ \
                                              firstIndex ].getParameterValues()
    saveSecondParameterValues = self._processListWidgetItems[ \
                                              secondIndex ].getParameterValues()

    # collecting display status
    firstWidgetDisplayStatus = self._processListWidgetItems[ \
                                              firstIndex ].isDisplayed()
    secondWidgetDisplayStatus = self._processListWidgetItems[ \
                                              secondIndex ].isDisplayed()

    # swapping first and second indives
    tmpCurrentIndex = \
                    self._processListWidgetItems[ firstIndex ].getCurrentIndex()
    self._processListWidgetItems[ firstIndex ].setProcessListWidgetItemIndex( \
                 self._processListWidgetItems[ secondIndex ].getCurrentIndex() )
    self._processListWidgetItems[ secondIndex ].setProcessListWidgetItemIndex( \
                                                               tmpCurrentIndex )

    # swapping parameter value(s)
    self._processListWidgetItems[ firstIndex ].setParameterValues( \
                                                      saveSecondParameterValues,
                                                      secondIndex )
    self._processListWidgetItems[ secondIndex ].setParameterValues( \
                                                      saveFirstParameterValues,
                                                      firstIndex )

    # swapping display status
    if ( firstWidgetDisplayStatus ):

      self._processListWidgetItems[ secondIndex ].setStackedWidgetAsDisplayed( \
                                                                          True )

    else:

      self._processListWidgetItems[ secondIndex ].setStackedWidgetAsHidden()

    if ( secondWidgetDisplayStatus ):

      self._processListWidgetItems[ firstIndex ].setStackedWidgetAsDisplayed( \
                                                                          True )

    else:

      self._processListWidgetItems[ firstIndex ].setStackedWidgetAsHidden()

    # setting current index to second index
    self._currentIndex = secondIndex


  def updateSelectedItem( self, oldIndex, newIndex ):

    if ( self._processListWidgetItemCount != 0 ):

      oldFrame = self._processListWidgetItems[ oldIndex ].getFrame()
      colorMap = oldFrame.palette()
      colorMap.setColor( QtGui.QPalette.WindowText,
                         QtGui.QColor( 105, 117, 129 ) )
      oldFrame.setPalette( colorMap )
      oldFrame.setLineWidth( 1 )

      newFrame = self._processListWidgetItems[ newIndex ].getFrame()
      colorMap = newFrame.palette()
      colorMap.setColor( QtGui.QPalette.WindowText,
                         QtGui.QColor( 54, 106, 129 ) )
      newFrame.setPalette( colorMap )
      newFrame.setLineWidth ( 2 )


  def reconnectParameters( self, oldIndex, newIndex = -1 ):

    self._processListWidgetItems[ oldIndex ].disconnectWidgetsFromParameters()
    self._processListParameter.removeParameters( oldIndex )
    if ( newIndex != -1 ):

      self._processListParameter.addParameters( \
                  self._processListWidgetItems[ oldIndex ]._currentProcessName,
                  newIndex )

      self._processListParameter.updateParameters( newIndex )
      self._processListWidgetItems[ newIndex ].connectWidgetsAndParameters( \
                  self._processListWidgetItems[ oldIndex ]._currentProcessName )


  def removeItemWidget( self, index ):

    self._verticalLayout.removeWidget( \
                              self._processListWidgetItems[ index ].getFrame() )
    self._processListWidgetItems[ index ].getFrame().hide()
    for child in self._processListWidgetItems[ \
                               index ].getFrame().findChildren( QtWidgets.QWidget ):

      child.deleteLater()

    self._processListWidgetItems[ index ].getFrame().deleteLater()

    del self._processListWidgetItems[ index ]


  def clear( self ):

    index = self._processListWidgetItemCount - 1
    while ( index >= 0 ):

      self.reconnectParameters( index )
      self.removeItemWidget( index )
      index -= 1

    self._processListWidgetItemCount = 0
    self._currentIndex = - 1
