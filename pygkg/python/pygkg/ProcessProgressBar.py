from PyQt5 import QtCore, QtGui, QtWidgets
from ResourceManager import *
from Frame import *


class ProcessProgressBar:


  selectionChanged = QtCore.pyqtSignal( 'QString' )

  def __init__( self, task, subjectName ):

    self._task = task
    self._subjectName = subjectName
    self._algorithmGUI = self._task._application.getAlgorithmGUI()
    self._hasFailed = False
    self._isDisplayable = True
    self._isDisplayed = False

    self._awin = self._algorithmGUI.getPanelWidget()
    self._findChild = lambda x, y: QtCore.QObject.findChild( x,
                                                             QtCore.QObject,
                                                             y )

    self._textEditLogError = self._findChild( self._awin, 'textEdit_LogError' )
    self._scrollAreaProgressBars = self._findChild( self._awin,
                                                    'scrollArea_ProgressBars' )

    widgetList = self._scrollAreaProgressBars.findChildren( \
                                                      QtWidgets.QWidget,
                                                      'mainProgressBarsWidget' )
    layoutList = self._scrollAreaProgressBars.findChildren( \
                                                      QtWidgets.QGridLayout,
                                                      'mainProgressBarsLayout' )

    # creating or collecting main widget
    if ( len( widgetList ) ):
    
      mainWidget = widgetList[ 0 ]

    else:
    
      mainProgressBarsWidget = QtWidgets.QWidget()
      mainProgressBarsWidget.setObjectName( 'mainProgressBarsWidget' )
      self._scrollAreaProgressBars.setWidget( mainProgressBarsWidget )

    # creating or collecting main layout
    if ( len( layoutList ) ):

      mainProgressBarsLayout = layoutList[ 0 ]
      
    else:

      mainProgressBarsLayout = QtWidgets.QGridLayout()
      mainProgressBarsLayout.setObjectName( 'mainProgressBarsLayout' )
      mainProgressBarsWidget.setLayout( mainProgressBarsLayout )

    # collecting layout row count
    mainProgressBarsLayoutRowCount = mainProgressBarsLayout.rowCount()
    if mainProgressBarsLayout.count() == 0:

      mainProgressBarsLayoutRowCount = 0

    # adding a button-like frame
    self._frame = Frame( )
    self._frame.setFrameShape( QtWidgets.QFrame.Panel )
    self._frame.setMaximumHeight( 65 )

    self._frame.setLineWidth( 3 )
    colorMap = self._frame.palette()
    colorMap.setColor( QtGui.QPalette.Dark, QtGui.QColor( 107, 120, 142 ) )
    self._frame.setPalette( colorMap )
    mainProgressBarsLayout.addWidget( self._frame, 
                                      mainProgressBarsLayoutRowCount, 0 )
    mainProgressBarsLayout.setVerticalSpacing( 3 )
    self._frame.clicked.connect( self.displayCallback )

    # creating a vertical layout
    vBoxLayout = QtWidgets.QVBoxLayout()
    self._frame.setLayout( vBoxLayout )

    # adding progress bar layout
    progressBarLayout = QtWidgets.QGridLayout()

    # adding subject name label
    self._subjectLabel = QtWidgets.QLabel( self._subjectName )

    vBoxLayout.addWidget( self._subjectLabel )

    # adding progress bar
    self._progressBar = QtWidgets.QProgressBar()
    self._progressBar.setRange( 0, 100 )
    self._progressBar.setValue( 0 )

    progressBarLayout.addWidget( self._progressBar, 0, 1  )

    # adding status label
    self._statusLabel = QtWidgets.QLabel( '' )
    self._statusLabel.setMinimumWidth( 72 )

    progressBarLayout.addWidget( self._statusLabel, 0, 2  )

    # adding display button/icon
    self._labelDisplay =  QtWidgets.QLabel( '', None  )
    self._labelDisplay.setMinimumSize( QtCore.QSize( 25, 20 ) )
    self._labelDisplay.setMaximumSize( QtCore.QSize( 25, 20 ) )
    self._labelDisplay.setScaledContents( True )
    progressBarLayout.addWidget( self._labelDisplay, 0, 3  )

    vBoxLayout.addLayout( progressBarLayout )

    self.displayCallback( False )
    self.setStatusAsWaiting()


  def setStatusAsWaiting( self ):

    if not( self._hasFailed ):

      self._statusLabel.setText( 'Waiting...' )


  def setStatusAsProcessing( self ):

    if not( self._hasFailed ):

      self._statusLabel.setText( 'Processing...' )

      processingMsg = self._subjectName + ' launched.'
      message = self._textEditLogError.toHtml() + str( processingMsg ) + \
                ' (' + time.ctime() + ')' 
      self._textEditLogError.setText( message )

      self._algorithmGUI.getAlgorithm().getApplication().addLog( processingMsg )


  def setStatusAsDone( self, warningMessage = None ):

    if not( self._hasFailed ):

      self._statusLabel.setText( 'Done.' )

      doneMsg = self._subjectName + ' finished.'
      message = self._textEditLogError.toHtml() + str( doneMsg ) + \
                ' (' + time.ctime() + ')<br>'

      if ( warningMessage != None ):

        message += 'Warning message: ' + warningMessage
             
      self._textEditLogError.setText( message )

      self._algorithmGUI.getAlgorithm().getApplication().addLog( doneMsg )


  def setStatusAsDoneWithErrorMessage( self,
                                       warningMessage = None ):

    if not( self._hasFailed ):

      self._statusLabel.setText( 'Done but with error message ( see log ).' )

      doneMsg = self._subjectName + ' finished with error message.'
      message = self._textEditLogError.toHtml() + str( doneMsg ) + \
                ' (' + time.ctime() + ')<br>'

      if ( warningMessage != None ):

        message += 'Warning message: ' + warningMessage
             
      self._textEditLogError.setText( message )

      self._algorithmGUI.getAlgorithm().getApplication().addLog( doneMsg )


  def setStatusAsFailed( self, executionInfo ):

    self._statusLabel.setText( "<font color=\"red\">Failed</font>" )
    self._hasFailed = True

    logError = ''

    colorMap = self._progressBar.palette()
    colorMap.setColor( QtGui.QPalette.Highlight, QtGui.QColor( "red" ) )
    self._progressBar.setPalette( colorMap )

    errorMsg = self._subjectName + ' ( ' + str( time.ctime() ) + ' ) : ' + \
               '<font color=\"purple\">' + str( executionInfo[ 1 ] ) + \
               '</font><br>'
    logError = self._subjectName + ' : ' + str( executionInfo[ 1 ] )
    nextTraceBack = executionInfo[ 2 ]
    while nextTraceBack is not None:

      errorMsg += str( nextTraceBack.tb_frame.f_code.co_filename ) + \
                 ' in <font color=\"purple\">' + \
                 str( nextTraceBack.tb_frame.f_code.co_name ) + '</font>' + \
                 ' at line: <font color=\"purple\">' + \
                 str( nextTraceBack.tb_lineno ) + '</font><br>'
      logError += str( nextTraceBack.tb_frame.f_code.co_filename ) + \
                 ' in ' + \
                 str( nextTraceBack.tb_frame.f_code.co_name ) + \
                 ' at line ' + \
                 str( nextTraceBack.tb_lineno )
      nextTraceBack = nextTraceBack.tb_next
        
    message = self._textEditLogError.toHtml() + \
              '<font color=\"red\">'+ str( errorMsg ) + '</font>'
    self._textEditLogError.setText( message )

    self._algorithmGUI.getAlgorithm().getApplication().addLog( logError )


  def displayCallback( self, isDisplayed ):

    # BUG_PYANATOMIST
    print 'ProcessProgressBar:displayCallback: ', isDisplayed
    #return
    
    if ( self._isDisplayable == True ):

      if isDisplayed:

        if ( isDisplayed != self._isDisplayed ):

          self._frame.setFrameShadow( QtWidgets.QFrame.Sunken )
          self._labelDisplay.setPixmap( QtGui.QPixmap( \
                    ResourceManager().getIconFileName( 'icon-open-eye.png' ) ) )
          self._task._application.getAlgorithmGUI().\
                          updateProgressBarsDisplayStatus( self._subjectName )
          
          self._isDisplayed = isDisplayed
          # BUG_PYANATOMIST
          #self._task.activateDisplay( self._subjectName )

        else:

          self._frame.setFrameShadow( QtWidgets.QFrame.Raised )
          self._labelDisplay.setPixmap( QtGui.QPixmap( \
                  ResourceManager().getIconFileName( 'icon-closed-eye.png' ) ) )
          self._task._application.getAlgorithmGUI().\
                                           updateProgressBarsDisplayStatus( '' )
          self._isDisplayed = False

      else:

        self._frame.setFrameShadow( QtWidgets.QFrame.Raised )
        self._labelDisplay.setPixmap( QtGui.QPixmap( \
                  ResourceManager().getIconFileName( 'icon-closed-eye.png' ) ) )
        self._isDisplayed = isDisplayed
        # BUG_PYANATOMIST
        #self._task.abortDisplay()


  def setValue( self, value ):

    self._progressBar.setValue( value )


  def hasFailed( self ):

    return self._hasFailed


  def isDisplayed( self ):

    return self._isDisplayed




