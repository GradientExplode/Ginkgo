import PyQt5
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from Algorithm import *
from DirectoryOrFileListDialog import *
from ProcessProgressBar import *
from MultiSubjectSelectionDialog import *
from AdvancedParameterWidget import *



################################################################################
# TableOfIntegerParameterToWidgetLink, TableOfDoubleParameterToWidgetLink
################################################################################

class ParameterToWidgetLink:
  
  def __init__( self, parameter, widget ):
  
    self._parameter = parameter
    self._widget = widget
  
  def setValue( self, line, column ):

    message = 'not implemented'
    raise RuntimeError, message


class TableOfIntegerParameterToWidgetLink( ParameterToWidgetLink ):
  
  def __init__( self, parameter, widget ):

    ParameterToWidgetLink.__init__(self, parameter, widget )
    
  
  def setValue( self, line, column ):
  
    self._parameter.setValue( line, column, 
                              int( self._widget.item( line,
                                                      column ).text() ) )


class TableOfDoubleParameterToWidgetLink( ParameterToWidgetLink ):
  
  def __init__( self, parameter, widget ):

    ParameterToWidgetLink.__init__(self, parameter, widget )
    
  
  def setValue( self, line, column ):
  
    self._parameter.setValue( line, column, 
                              float( self._widget.item( line,
                                                        column ).text() ) )


################################################################################
# TreeWidgetCallbacks
################################################################################


class TreeWidgetCallbacks:

  def __init__( self,
                algorithm,
                listOfParameterListName,
                parameterNames,
                treeWidget ):
  
    self._parameters = list()
    for p in parameterNames:

      self._parameters.append( algorithm.getParameter( p ) )
    self._listOfParameterList = algorithm.getParameter( listOfParameterListName)

    self._treeWidget = treeWidget
    self._listOfParameterList.setWidget( 'treeWidget', self )
    
  def addWidgetEntry( self, values ):
      
    # creating intermediate Qt string list
    qStringList = QtCore.QStringList()
    qStringList << str( self._listOfParameterList.getEntryCount() )
    for v in range( 0, len( values ) ):
      
      qStringList << str( values[ v ] )
        
    # creating a tree widget item
    item = QtWidgets.QTreeWidgetItem( self._treeWidget, qStringList, 0 )
      
    # inserting it to the tree widget
    self._treeWidget.addTopLevelItem( item )
    self._treeWidget.setCurrentItem( item )

  def addEntry( self ):
  
    # sanity check: checking that strings are not empty
    for p in self._parameters:

      if ( type( p.getValue() ) == type( str() ) ):
      
        if ( len( p.getValue() ) == 0 ):
        
          return

    # collecting actual values
    values = list()
    
    for p in self._parameters:

      values.append( p.getValue() )
      
    # updating list of parameter list
    self._listOfParameterList.addEntry( values )
    
    # clearing parameter(s)
    for p in self._parameters:
    
      p.setValue( p.getDefaultValue() )
  
  def removeWidgetEntry( self, index ):

    entryIndex = index + 1
    item = self._treeWidget.findItems( str( entryIndex ),
                                       QtCore.Qt.MatchExactly,
                                       0 )[ 0 ]
    parent = self._treeWidget.invisibleRootItem();
    index = parent.indexOfChild( item );
    parent.takeChild( index );

    # reordering items
    for c in range( 0, parent.childCount() ):
    
      parent.child( c ).setText( 0, str( c + 1 ) )

  def removeSelectedEntries( self ):
  
    # collecting selected items
    selectedItems = self._treeWidget.selectedItems()

    # deleting selected items
    for s in selectedItems:
    
      # collecting entry index
      entry = int( s.text( 0 ) )
      
      # removing the entry from the list of parameter list
      self._listOfParameterList.removeEntry( entry - 1 )


################################################################################
# AlgorithmGUI
################################################################################

class AlgorithmGUI:

  def __init__( self, algorithm, uiName, displayDefaultSubject = True,
                displayReportModeCheckBox = False ):

    self._algorithm = algorithm
    self._uifile = uiName
    if ( self._uifile != None ):

      self._displayDefaultSubject = displayDefaultSubject
      self._awin = uic.loadUi( self._uifile )
      self._findChild = lambda x, y: QtCore.QObject.findChild( x,
                                                               QtCore.QObject,
                                                               y )

      self._tableParameterToWidgetLinks = {}
      self._treeWidgetCallbacks = {}

      self._processControlUi = uic.loadUi( ResourceManager().getUIFileName( \
                                                      'core',
                                                      'ProcessControlGUI.ui' ) )

      self._awin.setMinimumHeight( self._awin.height() + \
                                   self._processControlUi.height() )

      self.getPanelWidget().layout().addWidget( self._processControlUi )
      self._processControlUi.setParent( self.getPanelWidget() )

      if not( self._algorithm._multiSubject ):

        widget = self._findChild( self._processControlUi,
                               'ProcessGroupBox_MultiSubject' )
        widget.hide()

      self._progressBars = {}
      self.resetProgressBars()
      scrollAreaProgressBars = self._findChild( self._awin,
                                                'scrollArea_ProgressBars' )

      # connecting output work directory interface
      self.connectStringParameterToLineEdit( 'outputWorkDirectory',
                                             'lineEdit_OutputWorkDirectory' )
      self.connectDirectoryBrowserToPushButtonAndLineEdit(
                                             'pushButton_OutputWorkDirectory',
                                             'lineEdit_OutputWorkDirectory' )
      self.connectPushButtonToCustomCallback( \
                                          'pushButton_OutputWorkDirectoryClear',
                                          self.clearOutputWorkDirectory )

      # connecting launch, view and abort buttons    
      self.connectLaunchToPushButton()
      self.connectPushButtonToCustomCallback( 'pushButton_Run',
                                              self.displayAbortStackedWidget )
      self.connectViewToPushButton()
      self.connectCheckBoxToCustomCallback( 'checkBox_ReportMode',
                                            self._algorithm.setReportMode )
      self.connectAbortToPushButton()
      self._isAborted = False
      # connecting multi-subject selection button
      self.connectMultiSubjectSelectionToPushButton( \
                                            'pushButton_MultiSubjectSelection' )

      # hiding report button
      if ( displayReportModeCheckBox == False ):

        self.hideReportModeCheckBox()

    # list of putative subject names to be rendered
    self._subjectNames = None


  def getAlgorithm( self ):

    return self._algorithm


  def chooseFileName( self, toBeSaved ):

    fileDialog = QtWidgets.QFileDialog()
    if ( toBeSaved is False ):

      fileDialog.setFileMode( QtWidgets.QFileDialog.ExistingFile )
      fileDialog.setAcceptMode( QtWidgets.QFileDialog.AcceptOpen )

    else:

      fileDialog.setFileMode( QtWidgets.QFileDialog.AnyFile )
      fileDialog.setAcceptMode( QtWidgets.QFileDialog.AcceptSave )

    result = fileDialog.exec_()
    fileName = None
    if result:

      fileName = unicode( fileDialog.selectedFiles()[ 0 ] )

    return fileName


  def fileNameChooser( self, lineEditWidgetName ):

    widget = self._findChild( self._awin, lineEditWidgetName )

    fileDialog = QtWidgets.QFileDialog()
    fileDialog.setFileMode( QtWidgets.QFileDialog.ExistingFile )
    result = fileDialog.exec_()
    if result:

      text = unicode( unicode( fileDialog.selectedFiles()[ 0 ] ) )
      subjectSelection = self._algorithm.getMultiSubjectSelectionValue()
      subjectSelectionValue = self._algorithm.getMultiSubjectSelectionValue()
      if ( subjectSelection != None and subjectSelectionValue != None \
                                 and text.find( subjectSelectionValue ) == -1 ):

        subjectSelection = subjectSelection.replace( '{', '' )
        subjectSelection = subjectSelection.replace( '}', '' )
        subjectSelection = subjectSelection.split( ',' )

        for subjectName in subjectSelection:

          if ( text.find( subjectName ) != -1 ):

            message = 'Do you want to replace ' \
                      +  subjectName \
                      + ' in your file path by: \n' \
                      +  self._algorithm.getMultiSubjectSelectionValue() \
                      + ' \nin order to handle multi selection?'
            answer = QtWidgets.QMessageBox.question( None,
                                      'Filename multi-selection',
                                      message,
                                      QtWidgets.QMessageBox.Yes | \
                                      QtWidgets.QMessageBox.No )

            if answer == QtWidgets.QMessageBox.Yes:

              text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break

      widget.setText( text )


  def fileNameListChooser( self, lineEditWidgetName ):

    widget = self._findChild( self._awin, lineEditWidgetName )
    previousSelection = widget.text()

    fileListDialog = DirectoryOrFileListDialog( None,
                                                previousSelection,
                                                True,
                                                False,
                                                True )
    result = fileListDialog.exec_()
    if result:

      fileListString = ''
      for f in fileListDialog.getSelection():

        fileListString += f + ';'

      fileListString = fileListString[ : -1 ]
      text = unicode( fileListString )
      subjectSelection = self._algorithm.getMultiSubjectSelectionValue()
      subjectSelectionValue = self._algorithm.getMultiSubjectSelectionValue()
      if ( subjectSelection != None and subjectSelectionValue != None \
                                 and text.find( subjectSelectionValue ) == -1 ):

        subjectSelection = subjectSelection.replace( '{', '' )
        subjectSelection = subjectSelection.replace( '}', '' )
        subjectSelection = subjectSelection.split( ',' )
        for subjectName in subjectSelection:

          if ( text.find( subjectName ) != -1 ):

            message = 'Do you want to replace ' \
                      +  subjectName \
                      + ' in your file paths by: \n' \
                      +  self._algorithm.getMultiSubjectSelectionValue() \
                      + ' \nin order to handle multi selection?'
            answer = QtWidgets.QMessageBox.question( None,
                                      'Filename multi-selection',
                                      message,
                                      QtWidgets.QMessageBox.Yes | \
                                      QtWidgets.QMessageBox.No )

            if answer == QtWidgets.QMessageBox.Yes:

              text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break

      widget.setText( text )

    del fileListDialog


  def directoryNameChooser( self, lineEditWidgetName ):

    widget = self._findChild( self._awin, lineEditWidgetName )

    directoryDialog = QtWidgets.QFileDialog()
    directoryDialog.setFileMode( QtWidgets.QFileDialog.DirectoryOnly )
    result = directoryDialog.exec_()
    if result:

      text = unicode( directoryDialog.selectedFiles()[ 0 ] )
      subjectSelection = self._algorithm.getMultiSubjectSelection()
      subjectSelectionValue = self._algorithm.getMultiSubjectSelectionValue()
      if ( subjectSelection != None and subjectSelectionValue != None \
                                 and text.find( subjectSelectionValue ) == -1 ):

        subjectSelection = subjectSelection.replace( '{', '' )
        subjectSelection = subjectSelection.replace( '}', '' )
        subjectSelection = subjectSelection.split( ',' )

        subjectNameFound = False
        for subjectName in subjectSelection:

          if ( lineEditWidgetName == 'lineEdit_OutputWorkDirectory' ):

            if ( text.find( subjectName ) != -1 ):

              subjectNameFound = True
              message = 'Do you want to replace ' \
                        +  subjectName \
                        + ' in your directory path by: \n' \
                        +  self._algorithm.getMultiSubjectSelectionValue() \
                        + ' \nin order to handle multi selection?'
              answer = QtWidgets.QMessageBox.question( None,
                                                   'Filename multi-selection',
                                                   message,
                                                   QtWidgets.QMessageBox.Yes | \
                                                   QtWidgets.QMessageBox.No )

              if ( answer == QtWidgets.QMessageBox.Yes ):

                text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break


          else:

            if ( text.find( subjectName ) != -1 ):

              message = 'Do you want to replace ' \
                        +  subjectName \
                        + ' in your directory path by: \n' \
                        +  self._algorithm.getMultiSubjectSelectionValue() \
                        + ' \nin order to handle multi selection?'
              answer = QtWidgets.QMessageBox.question( None,
                                                   'Filename multi-selection',
                                                   message,
                                                   QtWidgets.QMessageBox.Yes | \
                                                   QtWidgets.QMessageBox.No )

              if ( answer == QtWidgets.QMessageBox.Yes ):

                text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break

        if ( lineEditWidgetName == 'lineEdit_OutputWorkDirectory' ):

          if not( subjectNameFound ):

            text = os.path.join( text,
                               self._algorithm.getMultiSubjectSelectionValue() )

      widget.setText( text )


  def directoryNameListChooser( self, lineEditWidgetName ):

    widget = self._findChild( self._awin, lineEditWidgetName )
    previousSelection = widget.text()

    directoryListDialog = DirectoryOrFileListDialog( None,
                                                     previousSelection,
                                                     False,
                                                     True,
                                                     True )
    result = directoryListDialog.exec_()
    if result:

      directoryListString = ''
      for f in directoryListDialog.getSelection():

        directoryListString += f + ';'

      directoryListString = directoryListString[ : -1 ]

      text = unicode( directoryListString )
      subjectSelection = self._algorithm.getMultiSubjectSelectionValue()
      subjectSelectionValue = self._algorithm.getMultiSubjectSelectionValue()
      if ( subjectSelection != None and subjectSelectionValue != None \
                                 and text.find( subjectSelectionValue ) == -1 ):

        subjectSelection = subjectSelection.replace( '{', '' )
        subjectSelection = subjectSelection.replace( '}', '' )
        subjectSelection = subjectSelection.split( ',' )

        for subjectName in subjectSelection:

          if ( text.find( subjectName ) != -1 ):

            message = 'Do you want to replace ' \
                      +  subjectName \
                      + ' in your directory paths by: \n' \
                      +  self._algorithm.getMultiSubjectSelectionValue() \
                      + ' \nin order to handle multi selection?'
            answer = QtWidgets.QMessageBox.question( None,
                                                 'Filename multi-selection',
                                                 message,
                                                 QtWidgets.QMessageBox.Yes | \
                                                 QtWidgets.QMessageBox.No )

            if answer == QtWidgets.QMessageBox.Yes:

              text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break

      widget.setText( text )


  def fileOrDirectoryNameChooser( self, lineEditWidgetName ):

    widget = self._findChild( self._awin, lineEditWidgetName )
    previousSelection = widget.text()

    fileOrDirectoryDialog = DirectoryOrFileListDialog( None,
                                                       previousSelection,
                                                       True,
                                                       True )
    result = fileOrDirectoryDialog.exec_()
    if result:

      text = unicode( fileOrDirectoryDialog.getSelection()[ 0 ] )
      subjectSelection = self._algorithm.getMultiSubjectSelectionValue()
      subjectSelectionValue = self._algorithm.getMultiSubjectSelectionValue()
      if ( subjectSelection != None and subjectSelectionValue != None \
                                 and text.find( subjectSelectionValue ) == -1 ):

        subjectSelection = subjectSelection.replace( '{', '' )
        subjectSelection = subjectSelection.replace( '}', '' )
        subjectSelection = subjectSelection.split( ',' )

        for subjectName in subjectSelection:

          if ( text.find( subjectName ) != -1 ):

            message = 'Do you want to replace ' \
                      +  subjectName \
                      + ' in your directory or file path by: \n' \
                      +  self._algorithm.getMultiSubjectSelectionValue() \
                      + ' \nin order to handle multi selection?'
            answer = QtWidgets.QMessageBox.question( None,
                                      'Filename multi-selection',
                                      message,
                                      QtWidgets.QMessageBox.Yes | \
                                      QtWidgets.QMessageBox.No )

            if answer == QtWidgets.QMessageBox.Yes:

              text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break

      widget.setText( text )

  def fileOrDirectoryNameListChooser( self, lineEditWidgetName ):

    print 'je passe par la'

    widget = self._findChild( self._awin, lineEditWidgetName )
    previousSelection = widget.text()

    fileOrDirectoryListDialog = DirectoryOrFileListDialog( None,
                                                           previousSelection,
                                                           True,
                                                           True,
                                                           True )
    result = fileOrDirectoryListDialog.exec_()
    if result:

      fileOrDirectoryListString = ''
      for f in fileOrDirectoryListDialog.getSelection():

        fileOrDirectoryListString += f + ';'

      fileOrDirectoryListString = fileOrDirectoryListString[ : -1 ]

      text = unicode( fileOrDirectoryListString )
      subjectSelection = self._algorithm.getMultiSubjectSelectionValue()
      subjectSelectionValue = self._algorithm.getMultiSubjectSelectionValue()
      if ( subjectSelection != None and subjectSelectionValue != None \
                                 and text.find( subjectSelectionValue ) == -1 ):

        subjectSelection = subjectSelection.replace( '{', '' )
        subjectSelection = subjectSelection.replace( '}', '' )
        subjectSelection = subjectSelection.split( ',' )

        for subjectName in subjectSelection:

          if ( text.find( subjectName ) != -1 ):

            message = 'Do you want to replace ' \
                      +  subjectName \
                      + ' in your directory or file paths by: \n' \
                      +  self._algorithm.getMultiSubjectSelectionValue() \
                      + ' \nin order to handle multi selection?'
            answer = QtWidgets.QMessageBox.question( None,
                                      'Filename multi-selection',
                                      message,
                                      QtWidgets.QMessageBox.Yes | \
                                      QtWidgets.QMessageBox.No )

            if answer == QtWidgets.QMessageBox.Yes:

              text = text.replace( subjectName,
                               self._algorithm.getMultiSubjectSelectionValue() )
              break

      widget.setText( text )


  def connectIntegerParameterToSpinBox( self,
                                        parameterName,
                                        widgetName,
                                        index = None,
                                        subParameterName = None ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    if ( type( parameter.getValue() ) == type( list() ) ):

      parameter = parameter.getParameter( index, subParameterName )

    widget.setMinimum( parameter.getLowerValue() )
    widget.setMaximum( parameter.getUpperValue() )
    widget.setSingleStep( parameter.getIncrementValue() )
    widget.setValue( parameter.getValue() )
    widget.valueChanged.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'spinBox', widget )


  def connectIntegerParameterToSlider( self, parameterName, widgetName ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    widget.setMinimum( parameter.getLowerValue() )
    widget.setMaximum( parameter.getUpperValue() )
    widget.setSingleStep( parameter.getIncrementValue() )
    widget.setValue( parameter.getValue() )
    widget.valueChanged.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'slider', widget )


  def connectDoubleParameterToSpinBox( self,
                                       parameterName,
                                       widgetName,
                                       index = None,
                                       subParameterName = None ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    if ( type( parameter.getValue() ) == type( list() ) ):

      parameter = parameter.getParameter( index, subParameterName )

    widget.setMinimum( parameter.getLowerValue() )
    widget.setMaximum( parameter.getUpperValue() )
    widget.setSingleStep( parameter.getIncrementValue() )
    widget.setValue( parameter.getValue() )
    widget.valueChanged.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'doubleSpinBox', widget )


  def connectDoubleParameterToLineEdit( self, parameterName, widgetName ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    widget.setText( str( parameter.getValue() ) )
    widget.textChanged.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'lineEdit', widget )


  def connectStringParameterToLineEdit( self,
                                        parameterName,
                                        widgetName,
                                        index = None,
                                        subParameterName = None ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    if ( type( parameter.getValue() ) == type( list() ) ):

      parameter = parameter.getParameter( index, subParameterName )

    widget.setText( parameter.getValue() )
    widget.textChanged.connect(  parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'lineEdit', widget )


  def connectBooleanParameterToCheckBox( self,
                                         parameterName,
                                         widgetName,
                                         index = None,
                                         subParameterName = None ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    if ( type( parameter.getValue() ) == type( list() ) ):

      parameter = parameter.getParameter( index, subParameterName )

    widget.setCheckState( QtCore.Qt.CheckState( parameter.getValue() ) )
    widget.stateChanged.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'checkBox', widget )


  def connectCheckBoxToCustomCallback( self, checkBoxName, customCallback ):

    checkBoxWidget = self._findChild( self._awin, checkBoxName )

    customCallback( checkBoxWidget.checkState() )
    checkBoxWidget.stateChanged.connect( customCallback )


  def connectSpinBoxToCustomCallback( self, spinBoxName, customCallback ):
  
    spinBoxWidget = self._findChild( self._awin, spinBoxName )

    spinBoxWidget.valueChanged.connect( customCallback )

  def connectDoubleSpinBoxToCustomCallback( self, spinBoxName, customCallback ):
  
    spinBoxWidget = self._findChild( self._awin, spinBoxName )

    spinBoxWidget.valueChanged.connect( customCallback )
    

  def connectSliderToCustomCallback( self, sliderName, customCallback ):
  
    sliderWidget = self._findChild( self._awin, sliderName )

    sliderWidget.valueChanged.connect( customCallback )

  def connectLineEditToCustomCallback( self, lineEditName, customCallback ):

    lineEditWidget = self._findChild( self._awin, lineEditName )

    lineEditWidget.textChanged.connect( customCallback )
    

  def connectBooleanParameterToRadioButton( self, parameterName, widgetName ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    widget.setChecked( parameter.getValue() )
    widget.toggled.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'radioButton', widget )


  def connectChoiceParameterToComboBox( self,
                                        parameterName,
                                        widgetName,
                                        index = None,
                                        subParameterName = None ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    if ( type( parameter.getValue() ) == type( list() ) ):

      parameter = parameter.getParameter( index, subParameterName )

    widget.clear()
    for c in parameter.getChoices():

      widget.addItem( c )

    widget.setCurrentIndex( parameter.getValue() )
    widget.currentIndexChanged.connect( parameter.setValueButDoNotUpdateWidget )
    parameter.setWidget( 'comboBox', widget )


  def connectFileBrowserToPushButtonAndLineEdit( self,
                                                 pushButtonWidgetName,
                                                 lineEditWidgetName ):

    pushButtonWidget = self._findChild( self._awin, pushButtonWidgetName )
    pushButtonWidget.clicked.connect(
                         lambda x: self.fileNameChooser( lineEditWidgetName ) )

  def connectFileListBrowserToPushButtonAndLineEdit( self,
                                                     pushButtonWidgetName,
                                                     lineEditWidgetName ):

    pushButtonWidget = self._findChild( self._awin, pushButtonWidgetName )
    pushButtonWidget.clicked.connect(
                      lambda x: self.fileNameListChooser( lineEditWidgetName ) )


  def connectDirectoryBrowserToPushButtonAndLineEdit( self,
                                                      pushButtonWidgetName,
                                                      lineEditWidgetName ):

    pushButtonWidget = self._findChild( self._awin, pushButtonWidgetName )
    pushButtonWidget.clicked.connect(
                    lambda x: self.directoryNameChooser( lineEditWidgetName ) )


  def connectDirectoryListBrowserToPushButtonAndLineEdit( self,
                                                      pushButtonWidgetName,
                                                      lineEditWidgetName ):

    pushButtonWidget = self._findChild( self._awin, pushButtonWidgetName )
    pushButtonWidget.clicked.connect(
                 lambda x: self.directoryNameListChooser( lineEditWidgetName ) )


  def connectFileOrDirectoryBrowserToPushButtonAndLineEdit( self,
                                                      pushButtonWidgetName,
                                                      lineEditWidgetName ):

    pushButtonWidget = self._findChild( self._awin, pushButtonWidgetName )
    pushButtonWidget.clicked.connect(
           lambda x: self.fileOrDirectoryNameListChooser( lineEditWidgetName ) )


  def connectFileOrDirectoryListBrowserToPushButtonAndLineEdit( self,
                                                          pushButtonWidgetName,
                                                          lineEditWidgetName ):

    pushButtonWidget = self._findChild( self._awin, pushButtonWidgetName )
    pushButtonWidget.clicked.connect(
           lambda x: self.fileOrDirectoryNameListChooser( lineEditWidgetName ) )
                          

  def connectComboBoxToStackedWidget( self, comboBoxName, stackedWidgetName ):

    comboBoxWidget = self._findChild( self._awin, comboBoxName )
    stackedWidget = self._findChild( self._awin, stackedWidgetName )
    stackedWidget.setCurrentIndex( comboBoxWidget.currentIndex() )

    comboBoxWidget.currentIndexChanged.connect(
                                                 stackedWidget.setCurrentIndex )


  def connectComboBoxToComboBox( self, comboBoxName1, comboBoxName2 ):

    comboBoxWidget1 = self._findChild( self._awin, comboBoxName1 )
    comboBoxWidget2 = self._findChild( self._awin, comboBoxName2 )

    comboBoxWidget1.currentIndexChanged.connect(
                                               comboBoxWidget2.setCurrentIndex )


  def connectComboBoxToCustomCallback( self, comboBoxName, customCallback ):

    comboBoxWidget = self._findChild( self._awin, comboBoxName )

    comboBoxWidget.currentIndexChanged.connect( customCallback )


  def connectTableOfIntegerParameterToTableWidget( self, parameterName,
                                                   widgetName ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )
    for l in range(0,parameter.getLineCount()):

      for c in range( 0, parameter.getColumnCount() ):

        cell = QtWidgets.QTableWidgetItem( str( parameter.getValue( l, c ) ) )
        widget.setItem( l, c, cell )

    self._tableParameterToWidgetLinks[ parameterName ] ={}
    self._tableParameterToWidgetLinks[ parameterName ][ widgetName ] = \
                        TableOfIntegerParameterToWidgetLink( parameter, widget )
    widget.cellChanged.connect(
     self._tableParameterToWidgetLinks[ parameterName ][ widgetName ].setValue )
    parameter.setWidget( 'tableWidget', widget )


  def enableTableRow( self, widgetName, rowIndex, state ):

    widget = self._findChild( self._awin, widgetName )
    if ( state == 2 ):

      for c in range( 0, widget.columnCount() ):

        widget.item( rowIndex, c ).setFlags( \
              widget.item( rowIndex, c ).flags() | QtCore.Qt.ItemIsEnabled )

    else:

      for c in range( 0, widget.columnCount() ):

        widget.item( rowIndex, c ).setFlags( \
              widget.item( rowIndex, c ).flags() & ~QtCore.Qt.ItemIsEnabled )


  def enableTableColumn( self, widgetName, columnIndex, state ):

    widget = self._findChild( self._awin, widgetName )
    if ( state == 2 ):

      for r in range( 0, widget.rowCount() ):

        widget.item( r, columnIndex ).setFlags( \
              widget.item( r, columnIndex ).flags() | QtCore.Qt.ItemIsEnabled )

    else:

      for r in range( 0, widget.rowCount() ):

        widget.item( r, columnIndex ).setFlags( \
              widget.item( r, columnIndex ).flags() & ~QtCore.Qt.ItemIsEnabled )


  def connectTableOfDoubleParameterToTableWidget( self, parameterName,
                                                  widgetName ):

    widget = self._findChild( self._awin, widgetName )
    parameter = self._algorithm.getParameter( parameterName )

    self._tableParameterToWidgetLinks[ parameterName ] ={}
    self._tableParameterToWidgetLinks[ parameterName ][ widgetName ] = \
                         TableOfDoubleParameterToWidgetLink( parameter, widget )
    widget.cellChanged.connect(
     self._tableParameterToWidgetLinks[ parameterName ][ widgetName ].setValue )

    for l in range( 0, parameter.getLineCount() ):

      for c in range( 0, parameter.getColumnCount() ):

        cell = QtWidgets.QTableWidgetItem( str( parameter.getValue( l, c ) ) )
        widget.setItem( l, c, cell )
    parameter.setWidget( 'tableWidget', widget )


  def connectListOfParameterToPushButtonAndTreeWidget( self,
                                                       listOfParameterListName,
                                                       parameterNames,
                                                       pushButtonAddName,
                                                       pushButtonDeleteName,
                                                       treeWidgetName ):

    treeWidget = self._findChild( self._awin, treeWidgetName )
    labels = self._algorithm.getParameter( listOfParameterListName ).getLabels()
    treeWidget.setHeaderLabels( [ 'Rank' ] + labels )

    self._treeWidgetCallbacks[ listOfParameterListName ] = \
                                TreeWidgetCallbacks( self._algorithm,
                                                     listOfParameterListName,
                                                     parameterNames,
                                                     treeWidget );
    self.connectPushButtonToCustomCallback( \
                 pushButtonAddName,
                 self._treeWidgetCallbacks[ listOfParameterListName ].addEntry )

    self.connectPushButtonToCustomCallback( \
                 pushButtonDeleteName,
                 self._treeWidgetCallbacks[ listOfParameterListName \
                                                       ].removeSelectedEntries )


  def connectPushButtonToCustomCallback( self, pushButtonName, customCallback ):

    pushButtonWidget = self._findChild( self._awin, pushButtonName )

    pushButtonWidget.clicked.connect( customCallback )


  def connectLaunchToPushButton( self ):

    widget = self._findChild( self._awin, 'pushButton_Run' )
    widget.clicked.connect( self._algorithm.launch )


  def connectViewToPushButton( self ):

    widget = self._findChild( self._awin,  'pushButton_View'  )
    widget.clicked.connect( self._algorithm.view )


  def hideReportModeCheckBox( self ):

    widget = self._findChild( self._awin, 'checkBox_ReportMode' )
    widget.hide()


  def connectAbortToPushButton( self ):

    widget = self._findChild( self._awin, 'pushButton_Abort' )
    widget.setIcon( QtGui.QIcon( \
                       ResourceManager().getIconFileName( 'icon-abort.png' ) ) )
    widget.clicked.connect( self.abort )


  def abort( self ):

    # abort confirmation
    message = 'Do you really want to abort?'
    answer = QtWidgets.QMessageBox.question( None,
                                         'Abort',
                                         message,
                                         QtWidgets.QMessageBox.Yes | \
                                         QtWidgets.QMessageBox.Cancel )

    if answer == QtWidgets.QMessageBox.Yes:

      self.abortDisplay()
      self._algorithm.abort()
      self.resetProgressBars()
      self.displayRunAndViewStackedWidget()
    

  def clearOutputWorkDirectory( self ):

    # clear confirmation
    outputWorkDirectory = self._algorithm.getParameter( \
                                              'outputWorkDirectory' ).getValue()
    print "outputWorkDirectory", outputWorkDirectory
    if( outputWorkDirectory != "" ):

      message = 'Clearing will remove every file and directories in ' + \
                outputWorkDirectory + '.\n' + \
                'Do you really want to clear your output work directory?'
      answer = QtWidgets.QMessageBox.question( None,
                                           'Remove output directories',
                                           message,
                                           QtWidgets.QMessageBox.Yes | \
                                           QtWidgets.QMessageBox.Cancel )

      if answer == QtWidgets.QMessageBox.Yes:

        outputWorkDirectoryNames = None
        reString = '{(.+)}'
        outputWorkDirectoryNames = ''
        for x in xrange( outputWorkDirectory.count( '{' ) - 1 ):

          reString += '(.+){(.+)}'

        pattern = re.compile( reString )
        match = pattern.search( outputWorkDirectory )
        if( match != None ):

          regularExpressionSubjectName = '{' + match.group( 1 ) +'}'
          subjectNames = match.group( 1 ).split(',')


          for subjectName in subjectNames:

            outputWorkDirectoryName = outputWorkDirectory.replace( \
                                                       regularExpressionSubjectName,
                                                       subjectName )
            outputWorkDirectoryNames += outputWorkDirectoryName + '\n'

            directoriesToErase = outputWorkDirectoryNames.split( '\n' )
            for directoryToErase in directoriesToErase:

              if os.path.exists( directoryToErase ):

                shutil.rmtree( directoryToErase )

        else:

          if os.path.exists( outputWorkDirectory ):

            shutil.rmtree( outputWorkDirectory )        

    else:

      print( 'It is too much responsibility to remove your current directory.')
      print( 'Please do it by yourself. Sorry.' )

  def displayRunAndViewStackedWidget( self ):

    self._isAborted = True
    stackedWidget = self._findChild( self._awin, 'stackedWidget_RunAndAbort' )
    stackedWidget.setCurrentIndex( 0 )


  def displayAbortStackedWidget( self ):

    if not self._isAborted:

      stackedWidget = self._findChild( self._awin, 'stackedWidget_RunAndAbort' )
      stackedWidget.setCurrentIndex( 1 )

    self._isAborted = False

  def connectMultiSubjectSelectionToPushButton( self, widgetName ):

    widget = self._findChild( self._awin, widgetName )
    widget.clicked.connect( self.selectMultiSubject )


  def selectMultiSubject( self ):

    multiSubjectSelectionDialog = MultiSubjectSelectionDialog( \
                                               self._algorithm.getParameters() )

    result = multiSubjectSelectionDialog.exec_()
    if result:

      self._algorithm.setParameters( \
                            multiSubjectSelectionDialog.getUpdatedParameters() )
      self._algorithm.setMultiSubjectSelectionValue( \
                             multiSubjectSelectionDialog.getSubjectSelection() )


  def resetProgressBars( self ):

    mainProgressBarsLayout = self._findChild( self._awin,
                                              'mainProgressBarsLayout' )
    mainProgressBarsWidget = self._findChild( self._awin,
                                              'mainProgressBarsWidget' )
    if ( ( mainProgressBarsLayout is not None ) and \
         ( mainProgressBarsWidget is not None ) ):

      children = mainProgressBarsWidget.findChildren( QtWidgets.QWidget )

      for child in children:

        child.deleteLater()


  def updateProgressBarsDisplayStatus( self, subjectName ):

    for k in self._progressBars.keys():

      if ( self._progressBars[ k ].isDisplayed() and
           ( k != subjectName ) ):

        self._progressBars[ k ].displayCallback( False )


  def abortDisplay( self ):

    for k in self._progressBars.keys():

      if self._progressBars[ k ].isDisplayed():

        self._progressBars[ k ].displayCallback( False )


  def addProgressBars( self, task, subjectNames ):

    subjectNames.sort()
    print 'subjectNames =', subjectNames
    self.resetProgressBars()
    for subjectName in subjectNames:

      self._progressBars[ subjectName ] = ProcessProgressBar( \
                                                             task,
                                                             subjectName )

    if ( self._displayDefaultSubject ):

      self._progressBars[ subjectNames[ 0 ] ].displayCallback( True )


  def getProgressBar( self, subjectName ):

    if not self._progressBars.has_key( subjectName ):

        message = '\' progressBars have no key ' + subjectName
        raise RuntimeError, message

    return self._progressBars[ subjectName ]


  def setStatusAsProcessing( self, subjectName ):

    self.getProgressBar( subjectName ).setStatusAsProcessing()


  def setStatusAsDone( self,
                       subjectName,
                       warningMessage = None ):

    self.getProgressBar( subjectName ).setStatusAsDone( warningMessage )

  def setStatusAsDoneWithErrorMessage( self,
                                       subjectName,
                                       warningMessage = None ):

    self.getProgressBar( subjectName ).setStatusAsDoneWithErrorMessage( \
                                                              warningMessage )


  def setStatusAsFailed( self, subjectName, executionInfo ):

      if self.getAlgorithm().hasExecuteThread( subjectName ):

        self.getProgressBar( subjectName ).setStatusAsFailed( executionInfo )


  def getPanelWidget( self ):

    return self._awin


  def show( self ):

    self._awin.show()


  def connectPushButtonToAdvancedParameterWidget( self,
                                                  pushButtonName,
                                                  uiName,
                                                  parameterName ):

    pushButton = self._findChild( self._awin, pushButtonName )

    return AdvancedParameterWidget( self._algorithm,
                                    pushButton,
                                    parameterName,
                                    uiName )


