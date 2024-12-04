from PyQt5 import QtCore, QtGui, QtWidgets, uic


class AdvancedParameterWidget:

  def __init__( self, algorithm, pushButton, parameterName, uiName ):

    self._algorithm = algorithm
    self._uiName = uiName
    self._parameterName = parameterName
    pushButton.clicked.connect( self.launchAdvancedParameterWidgetDialog )

  def launchAdvancedParameterWidgetDialog( self ):

    parameter = self._algorithm.getParameter( self._parameterName )
    advancedParameterWidgetDialog = AdvancedParameterWidgetDialog( self._uiName,
                                                                   parameter )

    result = advancedParameterWidgetDialog.exec_()
    if result:

      value = advancedParameterWidgetDialog.getAdvancedParameters( parameter )
      parameter.setValue( value )


class AdvancedParameterWidgetDialog( QtWidgets.QDialog ):

  def __init__( self, uiName, parameter, parent = None ):

    QtWidgets.QDialog.__init__( self, parent )
    self._awinAdvancedParameter = uic.loadUi( uiName )

    layout = QtWidgets.QVBoxLayout( self )
    layout.addWidget( self._awinAdvancedParameter )

    self._findChild = lambda x, y: QtCore.QObject.findChild( x,
                                                             QtCore.QObject,
                                                             y )
    self._values = parameter.getValue()
    self._dependencies, self._dependencyValues = parameter.getDependencies()
    self._widgetNames = parameter.getWidgets()
    self._widgetTypes = parameter.getWidgetTypes()
    self._choices = parameter.getChoices()
    self._spinboxRangeValues = parameter.getSpinboxRangeValues()

    buttonBoxOkCancel = self._findChild( self._awinAdvancedParameter,
                                         'buttonBox_OkCancel' )

    buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                             ).clicked.connect( self.accept )
    buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                             ).clicked.connect( self.reject )

    self.setWindowModality( QtCore.Qt.WindowModal )
    self.setWindowTitle( parameter.getName() )

    dependencyWidgetNames = list()
    for values in self._dependencies.values():

      for value in values:

        dependencyWidgetNames.append( value )

    dependencyWidgetNames = set( dependencyWidgetNames )
    
    for key in self._values.keys():

      widget = self._findChild( self._awinAdvancedParameter,
                                self._widgetNames[ key ] )

      if ( self._widgetTypes[ key ] == 'lineEdit' ):

        widget.setText( self._values[ key ] )

      if ( self._widgetTypes[ key ] == 'checkBox' ):

        widget.setChecked( self._values[ key ] )

      elif ( self._widgetTypes[ key ] == 'spinBox' ):

        widget.setMinimum( self._spinboxRangeValues[ key ][ 0 ] )
        widget.setMaximum( self._spinboxRangeValues[ key ][ 1 ] )
        widget.setValue( self._values[ key ] )
        widget.setSingleStep( self._spinboxRangeValues[ key ][ 2 ] )

      elif ( self._widgetTypes[ key ] == 'comboBox' ):

        for choice in self._choices[ key ]:

          widget.addItem( choice )

        widget.setCurrentIndex( self._values[ key ] )


      if( self._dependencies.has_key( key ) ):

        enable = True
        for index in xrange( len( self._dependencies[ key ] ) ):

          if not(  self._dependencyValues[ key ][ index ]\
                       [ self._values[ self._dependencies[ key ][ index ] ] ] ):

            enable = False

        widget.setEnabled( enable )

    for dependencyKey in dependencyWidgetNames:

      dependencyWidget = self._findChild( self._awinAdvancedParameter,
                                           self._widgetNames[ dependencyKey ] )
      dependencyWidgetType = self._widgetTypes[ dependencyKey ]
      if ( dependencyWidgetType == 'stackedWidget' ):

        pass


      elif ( dependencyWidgetType == 'comboBox' ):

        dependencyWidget.currentIndexChanged.connect( self.checkDependencies )

      elif ( dependencyWidgetType == 'checkBox' ):

        dependencyWidget.stateChanged.connect( self.checkDependencies )
 
  def checkDependencies( self, value ):

    for key in self._dependencies:

      widget = self._findChild( self._awinAdvancedParameter,
                                                      self._widgetNames[ key ] )

      val = True
      for dependencyIndex in xrange( len( self._dependencies[ key ] ) ):

        dependencyWidget = self._findChild( self._awinAdvancedParameter,
             self._widgetNames[ self._dependencies[ key ][ dependencyIndex ] ] )
        dependencyWidgetType = \
             self._widgetTypes[ self._dependencies[ key ][ dependencyIndex ] ]
        if ( val == True ):

          if ( dependencyWidgetType == 'comboBox' ):

            val = self._dependencyValues[ key ][ dependencyIndex ]\
                                             [ dependencyWidget.currentIndex() ]

          if ( dependencyWidgetType == 'checkBox' ):

            val = self._dependencyValues[ key ][ dependencyIndex ]\
                                                [ dependencyWidget.isChecked() ]

      widget.setEnabled( val )
    

  def getAdvancedParameters( self, parameter ):

    for key in parameter.getValue().keys():

      widget = self._findChild( self._awinAdvancedParameter,
                                self._widgetNames[ key ] )

      if ( self._widgetTypes[ key ] == 'lineEdit' ):

        parameter.setValueFromKey( key, str( widget.text() ) )

      elif ( self._widgetTypes[ key ] == 'checkBox' ):

        parameter.setValueFromKey( key, widget.isChecked() )

      elif ( self._widgetTypes[ key ] == 'spinBox' ):

        parameter.setValueFromKey( key, widget.value() )

      elif ( self._widgetTypes[ key ] == 'comboBox' ):

        parameter.setValueFromKey( key, widget.currentIndex() )

    return parameter.getValue() 

  
