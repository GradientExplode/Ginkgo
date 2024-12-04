from PyQt5 import QtCore, QtGui, QtWidgets
from ResourceManager import *
from Frame import *
from Parameter import *
from ProcessListParameter import *


################################################################################
# Modified widgets for ProcessListWidgetItem
################################################################################


class ProcessListBaseWidget( QtWidgets.QWidget ):


  def __init__( self, parent = None ):

    QtWidgets.QWidget.__init__( self, parent )


  def mousePressEvent( self, event ):

    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListLabel( QtWidgets.QLabel ):


  def __init__( self, parent = None ):

    QtWidgets.QLabel.__init__( self, parent )


  def mousePressEvent( self, event ):

    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListStackedWidget( QtWidgets.QStackedWidget ):


  def __init__( self, parent = None ):

    QtWidgets.QStackedWidget.__init__( self, parent )


  def mousePressEvent( self, event ):

    QtWidgets.QStackedWidget.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListLineEdit( QtWidgets.QLineEdit, ProcessListBaseWidget ):


  def __init__( self, parent = None ):

    QtWidgets.QLineEdit.__init__( self, parent )


  def mousePressEvent( self, event ):

    QtWidgets.QLineEdit.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListCheckBox( QtWidgets.QCheckBox ):


  def __init__( self, parent = None ):

    QtWidgets.QCheckBox.__init__( self, parent )


  def mousePressEvent( self, event ):

    QtWidgets.QCheckBox.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListComboBox( QtWidgets.QComboBox ):


  def __init__( self, parent = None ):

    QtWidgets.QComboBox.__init__( self, parent )


  def mousePressEvent( self, event ):

    QtWidgets.QComboBox.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListSpinBox( QtWidgets.QSpinBox, ProcessListBaseWidget ):


  def __init__( self, panelWidget, parent = None ):

    QtWidgets.QSpinBox.__init__( self, parent )
    self._lineEdit = self.findChildren( QtWidgets.QLineEdit )[ 0 ]
    self._lineEdit.cursorPositionChanged.connect( self.setHasClicked )
    self._lineEdit.editingFinished.connect( self.setCursorPosition )


  def setHasClicked( self, oldValue, newValue ):

    self.emit( QtCore.SIGNAL( "clicked()" ) )


  def setCursorPosition( self ):

    self._lineEdit.setCursorPosition( -1 )


  def mousePressEvent( self, event ):

    QtWidgets.QSpinBox.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListDoubleSpinBox( QtWidgets.QDoubleSpinBox, ProcessListBaseWidget ):


  def __init__( self, panelWidget, parent = None ):

    QtWidgets.QDoubleSpinBox.__init__( self, parent )
    self._lineEdit = self.findChildren(QtWidgets.QLineEdit )[ 0 ]
    panelWidget.connect( self._lineEdit,
                         QtCore.SIGNAL( "cursorPositionChanged(int,int)" ),
                         self.setHasClicked )
    panelWidget.connect( self._lineEdit,
                         QtCore.SIGNAL( "editingFinished()" ),
                         self.setCursorPosition )


  def setHasClicked( self, oldValue, newValue ):

    self.emit( QtCore.SIGNAL( "clicked()" ) )


  def setCursorPosition( self ):

    self._lineEdit.setCursorPosition( -1 )


  def mousePressEvent( self, event ):

    QtWidgets.QSpinBox.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


class ProcessListComboBox( QtWidgets.QComboBox, ProcessListBaseWidget ):


  def __init__( self, parent = None ):

    QtWidgets.QComboBox.__init__( self, parent )


  def mousePressEvent( self, event ):

    QtWidgets.QComboBox.mousePressEvent( self, event )
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.emit( QtCore.SIGNAL( "clicked()" ) )


################################################################################
# class ProcessListWidgetItem
################################################################################

class ProcessListWidgetItem:

  def __init__( self,
                processListWidget,
                index,
                defaultProcessName,
                widgetIndex ):

    self._processListWidget = processListWidget
    self._index = index
    self._currentProcessName = defaultProcessName
    self._widgetIndex = widgetIndex

    self._isSelected = True
    self._isDisplayed = True

    self._widgetNameToParameterNameLut = {}

    self._stackedWidgetSizes = {}
    self._frameHeightVariation = 0
    self._parameterFrames = {}

    # creating a frame
    self._frame = Frame()
    self._frame.setFrameShape( QtWidgets.QFrame.Panel )
    self._frame.setFrameShadow( QtWidgets.QFrame.Plain )
    colorMap = self._frame.palette()
    colorMap.setColor( QtGui.QPalette.WindowText,
                       QtGui.QColor( 105, 117, 129 ) )
    self._frame.setPalette( colorMap )

    # adding frame to process list widget layout
    self._processListWidget.getLayout().addWidget( self._frame,
                                                   self._index )
    self._frame.clicked.connect( self.setCurrentIndex )

    # creating a grid layout to put parameter widgets within itself
    self._gridLayout = QtWidgets.QGridLayout()
    self._frame.setLayout( self._gridLayout )
    self._gridLayout.setHorizontalSpacing( 2 )
    self._gridLayout.setVerticalSpacing( 6 )

    # adding a index name label
    self._labelIndex = ProcessListLabel( str( self._index + 1 ) )
    self._labelIndex.setMaximumWidth( 25 )
    font = self._labelIndex.font();
    font.setBold( True )
    font.setPointSize( 10 )
    # setting color to black else it would take the frame color
    colorMap = self._labelIndex.palette()
    colorMap.setColor( QtGui.QPalette.WindowText, QtGui.QColor( 0, 0, 0 ) )
    self._labelIndex.setPalette( colorMap )
    self._labelIndex.setFont( font )
    self._gridLayout.addWidget( self._labelIndex, 0, 0 )
    self._labelIndex.clicked.connect(  self.setCurrentIndex )

    # adding a combo box
    self._comboBox = ProcessListComboBox()
    font = self._comboBox.font();
    font.setBold( True )
    self._comboBox.setFont( font )
    self._gridLayout.addWidget( self._comboBox, 0, 1, 1, 2 )
    self._comboBox.clicked.connect(  self.setCurrentIndex )

    # adding a stacked widget
    self._stackedWidget = ProcessListStackedWidget()
    self._stackedWidget.setFrameShape( QtWidgets.QFrame.Panel )
    self._stackedWidget.setFrameShadow( QtWidgets.QFrame.Plain )
    colorMap = self._stackedWidget.palette()
    colorMap.setColor( QtGui.QPalette.WindowText,
                       QtGui.QColor( 119, 120, 138 ) )
    self._stackedWidget.setPalette( colorMap )
    self._gridLayout.addWidget( self._stackedWidget, 1, 2, 2, 1 )
    self._stackedWidget.clicked.connect( self.setCurrentIndex )

    # preparing the combo box and the process list widget(s)
    for processName in self._processListWidget.getProcessListParameter().\
                                                              getProcessNames():

      self._comboBox.addItem( '' )

    processListWidgets = [ None ] * \
                         len( self._processListWidget.\
                                   getProcessListParameter().getProcessNames() )

    # adding the combo box items and corresponding process list widget(s)
    for processName in self._processListWidget.getProcessListParameter().\
                                                              getProcessNames():

      # collecing process index
      processIndex = self._processListWidget.getProcessListParameter().\
                                          getProcessNameToIndex( processName )

      # adding the process name to the right index position
      self._comboBox.setItemText( processIndex, processName )

      # creating new process list widget
      processListWidget = ProcessListBaseWidget()

      # creating a grid layout
      gridLayout = QtWidgets.QGridLayout()
      gridLayout.setVerticalSpacing( 0 )
      gridLayout.setContentsMargins( 0, 0, 0, 0 )
      processListWidget.setLayout( gridLayout )

      # collecting parameter name(s)
      parameterNames = self._processListWidget.getProcessListParameter().\
                                                 getParameterKeys( processName )

      # ordering the parameter names according to their indices
      orderedParameterNames = [ '' ] * len( parameterNames )
      for parameterName in parameterNames:

        index = self._processListWidget.getProcessListParameter().\
                                 getParameterIndex( processName, parameterName )
        orderedParameterNames[ index ] = parameterName
      while ( orderedParameterNames.count( '' ) != 0 ):

        orderedParameterNames.remove( '' )

      # adding parameter widget(s)
      parameterIndex = 0
      self._stackedWidgetSizes[ processName ] = 0
      for parameterName in orderedParameterNames:

        # creating layout for widget(s)
        layout = QtWidgets.QGridLayout()
 
        # creating frame and setting layout
        frame = Frame()
        frame.setLayout( layout )
        frame.setFrameShape( QtWidgets.QFrame.Box )
        frame.setFrameShadow( QtWidgets.QFrame.Plain )
        colorMap = frame.palette()
        colorMap.setColor( QtGui.QPalette.WindowText,
                           QtGui.QColor( 119, 120, 138 ) )
        frame.setPalette( colorMap )

        self._parameterFrames[ parameterName ] = frame

        # connecting frame signals to setCurrentIndex() method
        frame.clicked.connect( self.setCurrentIndex )

        # getting widget type
        widgetType = self._processListWidget.getProcessListParameter().\
                                     getWidgetType( processName, parameterName )

        # building widget(s) for the current parameter
        widgetInformations = self.getWidgetInformations( processName,
                                                         parameterName,
                                                         widgetType )

        # tuning widget colormap, connection and adding it to the layout
        for widgetInformation in widgetInformations:

          colorMap.setColor( QtGui.QPalette.WindowText,
                             QtGui.QColor( 0, 0, 0 ) )
          widgetInformation[ 0 ].setPalette( colorMap )
          widgetInformation[ 0 ].clicked.connect( self.setCurrentIndex )

          layout.addWidget( widgetInformation[ 0 ],   # the widget itself
                            widgetInformation[ 1 ],   # position along X
                            widgetInformation[ 2 ],   # position along Y
                            1,                        # span along X
                            widgetInformation[ 3 ] )  # span along Y

        gridLayout.addWidget( frame, parameterIndex, 0 )
        parameterIndex += 1


      # adding process list widget to the list of process list widget
      processListWidgets[ processIndex ] = processListWidget


    # adding the processListWidget(s) to the stacked widget
    for processListWidget in processListWidgets:

      self._stackedWidget.addWidget( processListWidget )
      
    # checking te default processName key; if not valid, checking 
    # ProcessListWidget in _algorithmGUI
    self._processListWidget.getProcessListParameter().getParameterKeys( \
                                                      self._currentProcessName )

    # adding label widget
    self._labelPoints = ProcessListLabel()
    text = '<body style=" font-family:\'Sans Serif\'; \
            font-size:5pt; font-weight:600; font-style:bold;">'
    for p in xrange( int( self._stackedWidgetSizes[ \
                                            self._currentProcessName ] / 60 ) ):

      text += '.<br>.<br>.<br>'
    text += '.  .  .  .'
    self._labelPoints.setText( text )
    self._labelPoints.clicked.connect( self.setCurrentIndex ) 
    self._gridLayout.addWidget( self._labelPoints, 1, 1 )

    # adding hide/show push button 
    self._pushButtonHideShow = QtWidgets.QToolButton()
    self._pushButtonHideShow.setAutoRaise( True )
    self._pushButtonHideShow.setIcon( QtGui.QIcon(
                       ResourceManager().getIconFileName( 'icon-minus.png' ) ) )
    self._pushButtonHideShow.setMaximumWidth( 15 )
    self._pushButtonHideShow.setMaximumHeight( 15 )
    font = self._pushButtonHideShow.font()
    font.setBold( True )
    font.setPointSize( 10 )
    self._pushButtonHideShow.setFont( font )
    self._gridLayout.addWidget( self._pushButtonHideShow, 1, 0 )
    self._pushButtonHideShow.clicked.connect( self.setCurrentIndex ) 
    self._pushButtonHideShow.clicked.connect( self.setStackedWidgetAsDisplayed )

    # setting maximum height of the frame
    self._frame.setMaximumHeight(  \
                     self._stackedWidgetSizes[ self._currentProcessName ] + 60 )

    # creating parameters
    self._processListWidget.getProcessListParameter().addParameters( \
                                                       self._currentProcessName,
                                                       self._index )

    # connecting widgets to parameters
    self.connectWidgetsToParameters( self._currentProcessName )
    self._comboBox.currentIndexChanged.connect( 
                                               self.setStackedWidgetParameters )

    self._comboBox.setCurrentIndex( \
                            self._processListWidget.getProcessListParameter(). \
                             getProcessNameToIndex( self._currentProcessName ) )

    if ( not self._processListWidget.getProcessListParameter().hasParameter( \
                                                   self._currentProcessName ) ):

      self._pushButtonHideShow.hide()
      self._labelPoints.hide()
      self._stackedWidget.hide()


  def getWidgetInformations( self, processName, parameterName, widgetType ):

    labelWidget = ProcessListLabel( \
                          self._processListWidget.getProcessListParameter(). \
                                             getParameterName( processName,
                                                               parameterName ) )

    widgetInformations = [ [ labelWidget, 0, 0 ] ]

    # lineEditAndBrowser type ##################################################
    if ( widgetType == 'lineEditAndBrowser' ):

      # creating line edit widget
      lineEditWidget = ProcessListLineEdit()
      lineEditWidget.setObjectName ( 'lineEdit_' + \
                                          str( parameterName ) + \
                                          str( self._widgetIndex ) )
      lineEditWidget.setText( \
                          self._processListWidget.getProcessListParameter(). \
                                     getParameterDefaultValue( processName,
                                                               parameterName ) )
      lineEditWidget.clicked.connect( self.setCurrentIndex )

      # creating browser widget
      browserWidget = QtWidgets.QPushButton( 'Browse' )
      browserWidget.setObjectName ( 'pushButton_' + \
                                    str( parameterName ) + \
                                    str( self._widgetIndex ) )

      browserWidget.clicked.connect( self.setCurrentIndex )

      # adding widget(s), their position and span
      widgetInformations[ 0 ].append( 2 )
      widgetInformations.append( [ lineEditWidget, 1, 0, 1 ] )
      widgetInformations.append( [ browserWidget, 1, 1, 1 ] )

      self._stackedWidgetSizes[ processName ] = \
                               self._stackedWidgetSizes[ processName ] + ( 60 )

    # lineEdit type ############################################################
    elif ( widgetType == 'lineEdit' ):

      # creating line edit widget
      lineEditWidget = ProcessListLineEdit()
      lineEditWidget.setObjectName ( 'lineEdit_' + \
                                          str( parameterName ) + \
                                          str( self._widgetIndex ) )
      lineEditWidget.setText( \
                          self._processListWidget.getProcessListParameter().\
                                     getParameterDefaultValue( processName,
                                                               parameterName ) )
      lineEditWidget.clicked.connect( self.setCurrentIndex )

      # adding widget(s), their position and span
      widgetInformations[ 0 ].append( 1 )
      widgetInformations.append( [ lineEditWidget, 1, 0, 1 ] )

      self._stackedWidgetSizes[ processName ] = \
                               self._stackedWidgetSizes[ processName ] + ( 60 )

    # spinBox type #############################################################
    elif ( widgetType == 'spinBox' ):

      # creating spin box widget
      spinBoxWidget = ProcessListSpinBox( \
                    self._processListWidget.getAlgorithmGUI().getPanelWidget() )
      spinBoxWidget.setObjectName( 'spinBox_' + \
                                   str( parameterName ) + \
                                   str( self._widgetIndex ) )
      spinBoxWidget.setMaximumWidth( 100 )
      spinBoxWidget.setMinimum( \
              self._processListWidget.getProcessListParameter().getLowerValue( \
                                                               processName,
                                                               parameterName ) )
      spinBoxWidget.setMaximum( \
              self._processListWidget.getProcessListParameter().getUpperValue( \
                                                               processName,
                                                               parameterName ) )
      spinBoxWidget.setValue( \
              self._processListWidget.getProcessListParameter().\
                                     getParameterDefaultValue( processName,
                                                               parameterName ) )
      spinBoxWidget.setSingleStep( \
              self._processListWidget.getProcessListParameter().\
                                            getIncrementValue( processName,
                                                               parameterName ) )
      spinBoxWidget.clicked.connect( self.setCurrentIndex )

      # adding widget(s), their position and span
      widgetInformations[ 0 ].append( 1 )
      widgetInformations.append( [ spinBoxWidget, 0, 1, 1 ] )

      self._stackedWidgetSizes[ processName ] = \
                               self._stackedWidgetSizes[ processName ] + ( 40 )

    # doubleSpinBox type ###################"###################################
    elif ( widgetType == 'doubleSpinBox' ):

      # creating double spin box widget
      doubleSpinBoxWidget = ProcessListDoubleSpinBox( \
                    self._processListWidget.getAlgorithmGUI().getPanelWidget() )
      doubleSpinBoxWidget.setObjectName( 'doubleSpinBox_' + \
                                         str( parameterName ) + \
                                         str( self._widgetIndex ) )
      doubleSpinBoxWidget.setMaximumWidth( 100 )
      doubleSpinBoxWidget.setMinimum( \
              self._processListWidget.getProcessListParameter().getLowerValue( \
                                                               processName,
                                                               parameterName ) )
      doubleSpinBoxWidget.setMaximum( \
              self._processListWidget.getProcessListParameter().getUpperValue( \
                                                               processName,
                                                               parameterName ) )
      doubleSpinBoxWidget.setValue( \
              self._processListWidget.getProcessListParameter().\
                                     getParameterDefaultValue( processName,
                                                               parameterName ) )
      doubleSpinBoxWidget.setSingleStep( \
              self._processListWidget.getProcessListParameter().\
                                            getIncrementValue( processName,
                                                               parameterName ) )
      doubleSpinBoxWidget.clicked.connect( self.setCurrentIndex )

      # adding widget(s), their position and span
      widgetInformations[ 0 ].append( 1 )
      widgetInformations.append( [ doubleSpinBoxWidget, 0, 1, 1 ] )

      self._stackedWidgetSizes[ processName ] = \
                               self._stackedWidgetSizes[ processName ] + ( 40 )

    # checkBox type ############################################################
    elif ( widgetType == 'checkBox' ):

      # creating check box widget
      checkBoxWidget = ProcessListCheckBox( '' )
      checkBoxWidget.setObjectName( 'checkBox_' + \
                                    str( parameterName ) + \
                                    str( self._widgetIndex ) )
      checkBoxWidget.setCheckState( \
                             self._processListWidget.getProcessListParameter().\
                                     getParameterDefaultValue( processName,
                                                               parameterName ) )
      checkBoxWidget.stateChanged.connect( self.setParametersToShow )
      checkBoxWidget.clicked.connect( self.setCurrentIndex )

      # adding widget(s), their position and span
      widgetInformations[ 0 ].append( 1 )
      widgetInformations.append( [ checkBoxWidget, 0, 1, 1 ] )

      self._stackedWidgetSizes[ processName ] = \
                               self._stackedWidgetSizes[ processName ] + ( 40 )

    # comboBox type ############################################################
    elif ( widgetType == 'comboBox' ):

      # creating combo box widget
      comboBoxWidget = ProcessListComboBox()
      comboBoxWidget.setObjectName( 'comboBox_' + \
                                    str( parameterName ) + \
                                    str( self._widgetIndex ) )
      choices = self._processListWidget.getProcessListParameter().getChoices( \
                                                       processName,
                                                       parameterName )
      for choice in choices:

        comboBoxWidget.addItem( choice )

      comboBoxWidget.setCurrentIndex( \
                          self._processListWidget.getProcessListParameter().\
                                     getParameterDefaultValue( processName,
                                                               parameterName ) )
      comboBoxWidget.currentIndexChanged.connect( self.setParametersToShow )
      comboBoxWidget.clicked.connect( self.setCurrentIndex )

      # adding widget(s), their position and span
      widgetInformations[ 0 ].append( 1 )
      widgetInformations.append( [ comboBoxWidget, 0, 1, 1 ] )

      self._stackedWidgetSizes[ processName ] = \
                               self._stackedWidgetSizes[ processName ] + ( 40 )
                               
    else:
    
      print '----------------->bad widget type:', widgetType

    return widgetInformations


  def connectWidgetsToParameters( self, processName ):

    # collecting list of parameter name(s)
    parameterNames = self._processListWidget.getProcessListParameter().\
                                                 getParameterKeys( processName )

    # looping over parameter name(s)
    for parameterName in parameterNames:

      # getting parameter type
      parameterType = self._processListWidget.getProcessListParameter().\
                                               getParameterType( processName,
                                                                 parameterName )
      # getting widget type for parameter
      widgetType = self._processListWidget.getProcessListParameter().\
                                               getWidgetType( processName,
                                                              parameterName )
      # creating suffix
      suffix = '_' + str( parameterName ) + str( self._widgetIndex )

      # building local parameter name
      localParameterName = 'Process' + str( self._index ) + '_' + parameterName

      # connecting widget(s) to parameter
      if ( widgetType == 'spinBox' ):

        widgetName = 'spinBox' + suffix
        self._processListWidget.getAlgorithmGUI(). \
          connectIntegerParameterToSpinBox( \
                    self._processListWidget.getProcessListParameter().getName(),
                    widgetName,
                    self._index,
                    localParameterName )
        self._widgetNameToParameterNameLut[ widgetName ] = localParameterName

      elif ( widgetType == 'doubleSpinBox' ):

        widgetName = 'doubleSpinBox' + suffix
        self._processListWidget.getAlgorithmGUI(). \
          connectDoubleParameterToSpinBox( \
                    self._processListWidget.getProcessListParameter().getName(),
                    widgetName,
                    self._index,
                    localParameterName )
        self._widgetNameToParameterNameLut[ widgetName ] = localParameterName

      elif ( widgetType == 'checkBox' ):

        widgetName = 'checkBox' + suffix
        self._processListWidget.getAlgorithmGUI(). \
          connectBooleanParameterToCheckBox( \
                    self._processListWidget.getProcessListParameter().getName(),
                    widgetName,
                    self._index,
                    localParameterName )
        self._widgetNameToParameterNameLut[ widgetName ] = localParameterName

      elif ( widgetType == 'comboBox' ):

        widgetName = 'comboBox' + suffix
        self._processListWidget.getAlgorithmGUI(). \
          connectChoiceParameterToComboBox( \
                    self._processListWidget.getProcessListParameter().getName(),
                    widgetName,
                    self._index,
                    localParameterName )
        self._widgetNameToParameterNameLut[ widgetName ] = localParameterName

      elif ( widgetType == 'lineEditAndBrowser' ):

        lineEditName = 'lineEdit' + suffix
        pushButtonName = 'pushButton' + suffix
        self._processListWidget.getAlgorithmGUI(). \
          connectStringParameterToLineEdit( \
                    self._processListWidget.getProcessListParameter().getName(),
                    lineEditName,
                    self._index,
                    localParameterName )
        self._processListWidget.getAlgorithmGUI(). \
          connectFileOrDirectoryBrowserToPushButtonAndLineEdit( \
                    self._processListWidget.getProcessListParameter().getName(),
                    pushButtonName,
                    lineEditName,
                    self._index,
                    localParameterName )
        self._widgetNameToParameterNameLut[ lineEditName ] = localParameterName
        self._widgetNameToParameterNameLut[ pushButtonName ] = \
                                                              localParameterName

      elif ( widgetType == 'lineEdit' ):

        lineEditName = 'lineEdit' + suffix
        self._processListWidget.getAlgorithmGUI(). \
          connectStringParameterToLineEdit( \
                    self._processListWidget.getProcessListParameter().getName(),
                    lineEditName,
                    self._index,
                    localParameterName )
        self._widgetNameToParameterNameLut[ lineEditName ] = localParameterName


  def disconnectWidgetsFromParameters( self ):

    for widgetName, \
        parameterName in self._widgetNameToParameterNameLut.iteritems():

      parameter = self._processListWidget.getProcessListParameter().\
                                                   getParameter( self._index,
                                                                 parameterName )
      # spinBox type ###########################################################
      if ( widgetName.find( 'spinBox' ) == 0 ):

        self._processListWidget.getAlgorithmGUI().getPanelWidget().disconnect( \
                              self._processListWidget.getChild( widgetName ),
                              QtCore.SIGNAL( 'valueChanged(int)' ),
                              parameter.setValueButDoNotUpdateWidget )

      # doubleSpinBox type #####################################################
      elif ( widgetName.find( 'doubleSpinBox' ) == 0 ):

        self._processListWidget.getAlgorithmGUI().getPanelWidget().disconnect( \
                              self._processListWidget.getChild( widgetName ),
                              QtCore.SIGNAL( 'valueChanged(double)' ),
                              parameter.setValueButDoNotUpdateWidget )

      # checkBox type ##########################################################
      elif ( widgetName.find( 'checkBox' ) == 0 ):

        self._processListWidget.getAlgorithmGUI().getPanelWidget().disconnect( \
                              self._processListWidget.getChild( widgetName ),
                              QtCore.SIGNAL( 'stateChanged(int)' ),
                              parameter.setValueButDoNotUpdateWidget )

      # lineEdit type ##########################################################
      elif ( widgetName.find( 'lineEdit' ) == 0 ):

        self._processListWidget.getAlgorithmGUI().getPanelWidget().disconnect( \
                              self._processListWidget.getChild( widgetName ),
                              QtCore.SIGNAL( 'textChanged(QString)' ),
                              parameter.setValueButDoNotUpdateWidget )

      # pushButton type ########################################################
      elif ( widgetName.find( 'pushButton' ) == 0 ):

        self._processListWidget.getAlgorithmGUI().getPanelWidget().disconnect( \
                              self._processListWidget.getChild( widgetName ),
                              QtCore.SIGNAL( 'clicked()' ),
                              self._processListWidget.getAlgorithmGUI().\
                                        _fileOrDirectoryNameChooserSignalMapper,
                              QtCore.SLOT( 'map()' ) )

    self._widgetNameToParameterNameLut.clear()


  def setProcessListWidgetItemIndex( self, index ):

    self._comboBox.setCurrentIndex( index )


  def setStackedWidgetParameters( self, index ):

    self.disconnectWidgetsFromParameters()
    self._currentProcessName = str( self._comboBox.currentText() )
    self._stackedWidget.setCurrentIndex( index )

    text = '<body style=" font-family:\'Sans Serif\'; \
            font-size:5pt; font-weight:600; font-style:bold;">'
    for p in xrange( int( self._stackedWidgetSizes[ \
                                            self._currentProcessName ] / 60 ) ):

      text += '.<br>.<br>.<br>'

    text += '.  .  .  .'
    self._labelPoints.setText( text )

    self._processListWidget.getProcessListParameter().removeParameters( \
                                                                   self._index )
    self._processListWidget.getProcessListParameter().addParameters( \
                                                       self._currentProcessName,
                                                       self._index )
    self._processListWidget.getProcessListParameter().updateParameters( \
                                                                   self._index )
    self.connectWidgetsToParameters( self._currentProcessName )
    if ( self._processListWidget.getProcessListParameter().hasParameter( \
                                                   self._currentProcessName ) ):

      self._pushButtonHideShow.show()
      self._labelPoints.show()
      self._stackedWidget.show()

    else:

      self._pushButtonHideShow.hide()
      self._labelPoints.hide()
      self._stackedWidget.hide()

    self.setParametersToShow()
    self.setStackedWidgetAsDisplayed( True )


  def setParametersToShow( self, emittedValue = None ):

    self._frameHeightVariation = 0
    parametersShowingOther = self._processListWidget.getProcessListParameter().\
                                  hasParametersToShow( self._currentProcessName )
    if ( parametersShowingOther != [] ):

      for parameterShowingOther in parametersShowingOther:

        values = self._processListWidget.getProcessListParameter().getValues( \
                                                       self._currentProcessName,
                                                       parameterShowingOther )
        parametersToShow = self._processListWidget.getProcessListParameter().\
                                  getParametersToShow( self._currentProcessName,
                                                       parameterShowingOther )

        if ( emittedValue == None ):

          emittedValue = self._processListWidget.getProcessListParameter().\
                          getParameter( self._index, \
                                        'Process' + str( self._index ) + \
                                        '_' + parameterShowingOther ).getValue()

        parametersShown = []
        for i in xrange( len( values ) ):

          if ( emittedValue == values[ i ] ):

            for parameterToShow in parametersToShow[ i ]:

              parametersShown.append( parameterToShow )
              if ( self._parameterFrames.has_key( parameterToShow ) ):

                self._parameterFrames[ parameterToShow ].show()
                self._frameHeightVariation += 40

          else:

            for parameterToShow in parametersToShow[ i ]:

              if ( parametersShown.count( parameterToShow ) == 0 ):

                if ( self._parameterFrames.has_key( parameterToShow ) ):

                  self._parameterFrames[ parameterToShow ].hide()

      parameterSet = set()
      for parameterToShow in parametersToShow:

        for parameter in parameterToShow:

          parameterSet.add( parameter )

      self._frameHeightVariation -= ( 40 * len( parameterSet ) )

      self._stackedWidget.setMaximumHeight( \
                        self._stackedWidgetSizes[ self._currentProcessName ] + \
                          self._frameHeightVariation )
      self._frame.setMaximumHeight( \
                        self._stackedWidgetSizes[ self._currentProcessName ] + \
                        self._frameHeightVariation + 60  )


  def setStackedWidgetAsDisplayed( self, forceDisplay = None ):

    if ( forceDisplay == None ):

      if ( self._isDisplayed ):

        forceDisplay = False

      else:

        forceDisplay = True

    if ( self._processListWidget.getProcessListParameter().hasParameter( \
                                                   self._currentProcessName ) ):

      if ( forceDisplay ):

        self._isDisplayed = True
        self._pushButtonHideShow.setIcon( \
          QtGui.QIcon( ResourceManager().getIconFileName( 'icon-minus.png' ) ) )
        self._stackedWidget.setMaximumHeight( \
                        self._stackedWidgetSizes[ self._currentProcessName ] + \
                        self._frameHeightVariation  )
        self._frame.setMaximumHeight( \
                        self._stackedWidgetSizes[ self._currentProcessName ] + \
                        self._frameHeightVariation + 60 )
        self._labelPoints.setMaximumWidth( 25 )

      else :

        self._pushButtonHideShow.setIcon( \
          QtGui.QIcon( ResourceManager().getIconFileName( 'icon-plus.png' ) ) )
        self._stackedWidget.setMaximumHeight( 0 )
        self._isDisplayed = False
        self._frame.setMaximumHeight( 75 )
        self._labelPoints.setMaximumWidth( 0 )

    else:

        self._frame.setMaximumHeight( 65 )


  def setStackedWidgetAsHidden( self ):

    if ( self._processListWidget.getProcessListParameter().hasParameter( \
                                                   self._currentProcessName ) ):

      self._pushButtonHideShow.setIcon( \
        QtGui.QIcon( ResourceManager().getIconFileName( 'icon-plus.png' ) ) )
      self._stackedWidget.setMaximumHeight( 0 )
      self._isDisplayed = False
      self._frame.setMaximumHeight( 75 )
      self._labelPoints.setMaximumWidth( 0 )

    else:

        self._frame.setMaximumHeight( 65 )


  def setCurrentIndex( self ):

    self._processListWidget.setCurrentIndex( self._index )


  def updateIndex( self, index ):

    self._index = index
    self._labelIndex.setText( str( index + 1 ) )


  def isDisplayed( self ):

    return self._isDisplayed


  def getFrame( self ):

    return self._frame


  def getCurrentIndex( self ):

    return self._comboBox.currentIndex()


  def getParameterValues( self ):

    parameterValues = {}
    for parameterName in self._widgetNameToParameterNameLut.values():

      if not( parameterValues.has_key( parameterName ) ):

        parameterValues[ parameterName ] = \
                  self._processListWidget.getProcessListParameter().\
                                             getParameterValue( self._index,
                                                                parameterName )
    return parameterValues


  def setParameterValues( self, parameterValues, oldIndex ):

    for parameterName, value in parameterValues.iteritems():

      parameterName = parameterName.replace( \
                                          'Process' + str( oldIndex ) + '_',
                                          'Process' + str( self._index ) + '_' )
      self._processListWidget.getProcessListParameter().updateParameterValue( \
                                                                  self._index,
                                                                  parameterName,
                                                                  value )
