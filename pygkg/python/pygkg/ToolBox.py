from Algorithm import *
from AlgorithmGUI import *
from Viewer import *
from MainThreadActionManager import *
from Application import *
from ResourceManager import *
from MultiSubjectSettingsDialog import *
from ToolBoxPageSettingDialog import *
from SpecySelectionDialog import *
import os


class Logo( QtWidgets.QLabel ):

  def __init__( self, name, logoFontColor = None, pixmapName = None,
                overlayApplicationNameOnLogo = True ):

    QtWidgets.QLabel.__init__( self )

    self._name = name
    self._logoFontColor = None
    if ( logoFontColor == None ):

      self._logoFontColor = QtGui.QColor( 234, 130, 17 )

    else:

      self._logoFontColor = logoFontColor

    self._pixmap = None
    if ( pixmapName == None ):

      self._pixmap = QtGui.QPixmap( ResourceManager().getIconFileName( \
                                                            'logo-pygkg.png' ) )

    else:

      self._pixmap = QtGui.QPixmap( ResourceManager().getIconFileName( \
                                                                  pixmapName ) )

    self.setAlignment( QtCore.Qt.AlignHCenter | QtCore.Qt.AlignTop )
    self.setPixmap( self._pixmap )
    self._overlayApplicationNameOnLogo = overlayApplicationNameOnLogo


  def paintEvent( self, event ):

    QtWidgets.QLabel.paintEvent( self, event )
    p = QtGui.QPainter()
    p.begin( self )
    
    font = QtGui.QFont( 'Purisa', 60 )
    font.setBold( True )
    font.setStretch( 75 )
    p.setFont( font )
                
    if ( self._overlayApplicationNameOnLogo == True ):

      p.setPen( QtGui.QColor( 50, 50, 50 ) )
      left = event.rect().left() + 2
      top = event.rect().top() - 6
      width = event.rect().width()
      height = event.rect().height()
      p.drawText( QtCore.QRect( left, top, width, height ),
                  QtCore.Qt.AlignHCenter | QtCore.Qt.AlignTop,
                  self._name )

      p.setPen( self._logoFontColor )
      left = event.rect().left()
      top = event.rect().top() - 4
      width = event.rect().width()
      height = event.rect().height()
      p.drawText( QtCore.QRect( left, top, width, height ),
                  QtCore.Qt.AlignHCenter | QtCore.Qt.AlignTop,
                  self._name )
                
    p.end()


class ToolBox( QtWidgets.QMainWindow ):


  def __init__( self,
                name,
                pages,
                about,
                batchMode,
                distributionSettings = {},
                verbose = False,
                parent = None,
                logoFontColor = QtGui.QColor( 234, 130, 17 ),
                overlayApplicationNameOnLogo = True ):


    self._name = name
    self._logoFontColor = logoFontColor

    # displaying splash screen
    if ( batchMode == False ):
    
      self.displaySplashScreen()

    # distribution args
    self._distributionStrategy = 'single-threading'
    self._thread_count = 1
    self._oldHorizontalTabWidgetIndex = 0
    self._oldVerticalTabWidgetIndex = 0

    if( batchMode == False ):

      # change the app's look and feel
      # one of "windows", "motif", "cde",
      # "plastique", "windowsxp", or "macintosh"
      QtWidgets.QApplication.setStyle( QtGui.QStyleFactory.create( "plastique" ) )

      # and the corresponding palette
      QtWidgets.QApplication.setPalette( 
                                  QtGui.QApplication.style().standardPalette() )

      # initializing widget
      QtWidgets.QMainWindow.__init__( self, parent )

    else:

      if ( distributionSettings.has_key( 'thread_count' ) ):

        self._thread_count = distributionSettings[ 'thread_count' ]

      if ( distributionSettings.has_key( 'distributionStrategy' ) ):

        self._distributionStrategy = \
                                  distributionSettings[ 'distributionStrategy' ]


    self._pages = list()
    self._pagesTabIndices = dict()
    tabNames = list()
    index = 0
    for tab in pages:
 
      for tabName, tabValues in zip( tab, tab.values() ):

        tabNames.append( tabName )
        for page in tabValues:

          self._pagesTabIndices[ page[ 'pageName' ] ] = index 
          self._pages.append( page )

      index += 1

    self._about = about

    self._verbose = verbose

    self._overlayApplicationNameOnLogo = overlayApplicationNameOnLogo
    
    self._algorithmGUIs = {}
    self._viewerCreators = {}
    self._applications = {}
    self._tabLabels = {}
    self._pageIndices = {}
    self._pageNames = list()
    self._selectedPageNames = list()
    self._availablePageNames = list()

    for page in self._pages:

      self._availablePageNames.append( page[ 'pageName' ] )

    self._preferedViewerSize = 0

    if( batchMode == False ):

      # menu bar
      self._loadParametersAction = QtWidgets.QAction( '&Load parameters', self )
      self._loadParametersAction.setStatusTip( \
                                              'Loading parameters from a file' )
      self._loadParametersAction.triggered.connect( self.slotLoadParameters )

      self._saveParametersAction = QtWidgets.QAction( '&Save parameters', self )
      self._saveParametersAction.setStatusTip( 'Saving parameters to a file' )
      self._saveParametersAction.triggered.connect( self.slotSaveParameters )

      self._resetParametersAction = QtWidgets.QAction( '&Reset parameters', self )
      self._resetParametersAction.setStatusTip( 'Resetting parameters' )
      self._resetParametersAction.triggered.connect( self.slotResetParameters )

      self._multiSubjectSettingsAction = QtWidgets.QAction( '&Multi-subject', self )
      self._multiSubjectSettingsAction.setStatusTip( \
                                         'Multi-subject distribution settings' )
      self._multiSubjectSettingsAction.triggered.connect( \
                                                 self.slotMultiSubjectSettings )

      self._toolboxPageSelectionSettingsAction = QtWidgets.QAction( \
                                               '&Toolbox page selection', self )
      self._toolboxPageSelectionSettingsAction.setStatusTip( \
                                             'Toolbox page selection settings' )
      self._toolboxPageSelectionSettingsAction.triggered.connect( \
                                         self.slotToolboxPageSelectionSettings )
                    
      self._specyPageSelectionSettingsAction = QtWidgets.QAction( '&Specy', self )
      self._specyPageSelectionSettingsAction.setStatusTip( \
                                               'Specy page selection settings' )
      self._specyPageSelectionSettingsAction.triggered.connect( \
                                           self.slotSpecyPageSelectionSettings )

      self._quitAction = QtWidgets.QAction( '&Quit', self )
      self._quitAction.setStatusTip( 'Quit the  application' )
      self._quitAction.triggered.connect( self.slotQuit )
    
      self._aboutAction = QtWidgets.QAction( '&About', self )
      self._aboutAction.setStatusTip( 'Show the application\'s About box' )
      self._aboutAction.triggered.connect( self.slotAbout )
    
      self._helpMenu = self.menuBar().addMenu( '&Actions' );
      self._helpMenu.addAction( self._loadParametersAction );
      self._helpMenu.addAction( self._saveParametersAction );
      self._helpMenu.addAction( self._resetParametersAction );
      self._helpMenu.addAction( self._quitAction );

      self._helpMenu = self.menuBar().addMenu( '&Settings' );
      self._helpMenu.addAction( self._multiSubjectSettingsAction );
      self._helpMenu.addAction( self._toolboxPageSelectionSettingsAction );
      self._helpMenu.addAction( self._specyPageSelectionSettingsAction );

      self._helpMenu = self.menuBar().addMenu( '&Help' );
      self._helpMenu.addAction( self._aboutAction );

      # central widget
      self._centralWidget = QtWidgets.QWidget()
      self.setCentralWidget( self._centralWidget )

      # mother horizontal layout of the central widget
      self._layout = QtWidgets.QVBoxLayout( self._centralWidget )
      self._centralWidget.setLayout( self._layout )

      # adding icon
      if ( os.path.exists( ResourceManager().getIconFileName(
                                            'icon-' + self._name + '.png' ) ) ):

        iconPixmap = QtGui.QPixmap( ResourceManager().getIconFileName(
                                               'icon-' + self._name + '.png' ) )
                                               
      else:
    
        iconPixmap = QtGui.QPixmap( ResourceManager().getIconFileName(
                                                            'icon-pygkg.png' ) )

      # adding application pixmap
      name = self._name[ 0 : 1 ].swapcase() + self._name[ 1 : ]
      if ( os.path.exists( ResourceManager().getIconFileName(
                                            'logo-' + self._name + '.png' ) ) ):

        logo = Logo( name, self._logoFontColor, 'logo-' + self._name + '.png',
                     self._overlayApplicationNameOnLogo )
   
        self._layout.addWidget( logo )

      else:
    
        logo = Logo( name )    
        self._layout.addWidget( logo )


      # adding the tab widgets
      self._horizontalTabWidget = QtWidgets.QTabWidget()
      self._horizontalTabWidget.setTabPosition( QtGui.QTabWidget.North )
      self._layout.addWidget( self._horizontalTabWidget )

      self._horizontalTabWidget.currentChanged.connect( 
                                                     self.slotTabWidgetChanged )

      self._verticalTabWidgets = list()
      index = 0
      for tabName in tabNames:

        tabWidget = QtWidgets.QTabWidget() 
        self._horizontalTabWidget.insertTab( index, tabWidget, tabName )
        tabWidget.setTabPosition( QtWidgets.QTabWidget.West )
        self._verticalTabWidgets.append(tabWidget )
        tabWidget.currentChanged.connect( self.slotTabWidgetChanged )
        
        index += 1

    # adding logs
    ResourceManager().clearOldLog( self._name )
    self._logFileName = ResourceManager().getLogFileName( self._name )


  def getName( self ):
  
    return self._name
    

  def resetPages( self, selectedPageNames = None ):

    oldSelectedPagesNames = self._selectedPageNames
    self._selectedPageNames = selectedPageNames

    if ( self._selectedPageNames == None ):

      self._selectedPageNames = self._availablePageNames

    index = 0
    for page in self._pages:

      if( self._selectedPageNames.count( page[ 'pageName' ] ) > 0 and \
                        oldSelectedPagesNames.count( page[ 'pageName' ] ) == 0 ):

        algorithm = page[ 'algorithm' ]( self._verbose )
        self.addPage( page[ 'algorithmGUI' ]( algorithm ),
                      page[ 'createViewer' ],
                      page[ 'tabName' ],
                      index )
        index += 1

      elif( self._selectedPageNames.count( page[ 'pageName' ] ) == 0 and \
                        oldSelectedPagesNames.count( page[ 'pageName' ] ) > 0 ):

        self.removePage( index )

      elif( self._selectedPageNames.count( page[ 'pageName' ] ) > 0 ):

        self._pageIndices.pop( page[ 'pageName' ] )
        self._pageIndices[ page[ 'pageName' ] ] = index
        index += 1



  def removePage( self, index ):

    previousTabCount = 0
    for previousTabIndex in xrange( \
                            self._pagesTabIndices[ self._pageNames[ index ] ] ):

      previousTabCount += self._verticalTabWidgets[ previousTabIndex ].count()

    tabIndex = index - previousTabCount
    self._verticalTabWidgets[ \
          self._pagesTabIndices[ self._pageNames[ index ] ] ].removeTab( tabIndex )
    self._algorithmGUIs.pop( self._pageNames[ index ] )
    self._viewerCreators.pop( self._pageNames[ index ] )
    self._pageIndices.pop( self._pageNames[ index ] )
    self._applications.pop( self._pageNames[ index ] )
    self._pageNames.remove( self._pageNames[ index ] )

  def getSelectedPageNames( self ):

    settings = ResourceManager().readSettings( self.getName() )
    selectedPages = None
    if ( settings.has_key( 'toolbox_pages' ) ):

      selectedPages = settings[ 'toolbox_pages' ]

    return selectedPages

    
  def addPage( self, algorithmGUI, viewerCreator, tabLabel, index ):

    pageName = algorithmGUI.getAlgorithm().getName()
   
   
    settings = ResourceManager().readSettings( self._name )
     
    #print 'tunning parameters of', pageName, 'to specy', settings[ 'specy' ]
    
    algorithmGUI.getAlgorithm().tuneToSpecy( self._name )
    
    if ( self._algorithmGUIs.has_key( pageName ) ):
    
      message = '\'' + pageName + '\' page already present'
      raise RuntimeError, message

    self._algorithmGUIs[ pageName ] = algorithmGUI
    self._viewerCreators[ pageName ] = viewerCreator
    self._tabLabels[ pageName ] = tabLabel
    self._pageIndices[ pageName ] = index
    self._pageNames.insert( index, pageName )
    application = Application( algorithmGUI,
                                viewerCreator,
                                [ self._preferedViewerSize,
                                  self._preferedViewerSize ],
                                self )
    application.setBatchMode( False )
    self._applications[ pageName ] = application
    previousTabCount = 0
    for previousTabIndex in xrange( \
                            self._pagesTabIndices[ self._pageNames[ index ] ] ):

      previousTabCount += self._verticalTabWidgets[ previousTabIndex ].count()

    tabIndex = index - previousTabCount


    self._verticalTabWidgets[ self._pagesTabIndices[ pageName ] ].insertTab( \
                                                 tabIndex, 
                                                 self._applications[ pageName ],
                                                 self._tabLabels[ pageName ] )
    

  def getIndexFromPageName( self, pageName ):

    if ( not self._pageIndices.has_key( pageName ) ):
    
      message = '\'' + pageName + '\' page not found'
      raise RuntimeError, message
      
    return self._pageIndices[ pageName ]
  

  def getPageNameFromIndex( self, pageIndex ):
  
    if ( len( self._pageNames ) <= pageIndex ):
    
      message = '\'' + str( pageIndex ) + '\' page index not found'
      raise RuntimeError, message
      
    return self._pageNames[ pageIndex ]
  

  def getAlgorithmGUI( self, pageName ):

    if ( not self._algorithmGUIs.has_key( pageName ) ):
    
      message = '\'' + pageName + '\' page not found'
      raise RuntimeError, message
      
    return self._algorithmGUIs[ pageName ]


  def getParametersOfPage( self, pageName ):
  
    algorithmGUI = self.getAlgorithmGUI( pageName )
    return algorithmGUI.getAlgorithm().getParameters()

  
  def slotQuit( self ):
  
    if QtGui.QMessageBox.warning( self,
                                  self._name,
                                  'Do you really want to quit ' +
                                  self._name + '?',
                                  QtGui.QMessageBox.No |
                                  QtGui.QMessageBox.Yes ) == \
       QtGui.QMessageBox.Yes:

      QtGui.QApplication.exit( 0 )


  def slotAbout( self ):
  
    QtWidgets.QMessageBox.about( self,
                            'About ' + self._name + '...',
                            self._about )  


  def slotLoadParameters( self ):

    index = 0
    for tabIndex in xrange( self._horizontalTabWidget.currentIndex() ):

      index +=  self._verticalTabWidgets[ tabIndex ].count()
   
    index += self._verticalTabWidgets[ \
                       self._horizontalTabWidget.currentIndex() ].currentIndex()
    algorithmGUI = self.getAlgorithmGUI( self.getPageNameFromIndex( index ) )
    fileName = algorithmGUI.chooseFileName( False )
    if ( fileName is not None ):

      algorithmName = ''
      
      try:
      
        parameterValues = dict()
        with open( fileName, 'r' ) as f:

          parameterValues = json.load( f )

        if ( parameterValues.has_key( '_algorithmName' ) ):
    
          algorithmName = parameterValues[ '_algorithmName' ]
  
      except:
      
        globalVariables = dict()
        localVariables = dict()
        execfile( fileName, globalVariables, localVariables )

        if ( localVariables.has_key( 'algorithmName' ) ):
    
          algorithmName = localVariables[ 'algorithmName' ]

      if ( len( algorithmName ) ):
    
        newIndex = self.getIndexFromPageName( algorithmName )
        if ( newIndex != index ):
 
          horizontalTabIndex = \
                  self._pagesTabIndices[ self.getPageNameFromIndex( newIndex ) ]
          self._horizontalTabWidget.setCurrentIndex( horizontalTabIndex )
          index = 0
          for tabIndex in xrange( horizontalTabIndex ):

            index +=  self._verticalTabWidgets[ tabIndex ].count()

          tabIndex = newIndex - index
          self._verticalTabWidgets[ horizontalTabIndex ].setCurrentIndex( \
                                                                      tabIndex )
          index = newIndex        

          algorithmGUI = self.getAlgorithmGUI( algorithmName )

      algorithmGUI.getAlgorithm().readParameters( fileName )
    
      QtWidgets.QMessageBox.about( self,
                              'Load parameters',
                              'Loading parameters from ' + fileName + ' for ' +
                              self.getPageNameFromIndex( index ) + ' done !' )  


  def slotSaveParameters( self ):

    index = 0
    for tabIndex in xrange( self._horizontalTabWidget.currentIndex() ):

      index +=  self._verticalTabWidgets[ tabIndex ].count()
   
    index += self._verticalTabWidgets[ \
                       self._horizontalTabWidget.currentIndex() ].currentIndex()
    algorithmGUI = self.getAlgorithmGUI( self.getPageNameFromIndex( index ) )
    fileName = algorithmGUI.chooseFileName( True )
    if ( fileName is not None ):
    
      algorithmGUI.getAlgorithm().writeParameters( fileName )
    
      QtWidgets.QMessageBox.about( self,
                               'Save parameters',
                               'Saving parameters to ' + fileName + ' for ' +
                               self.getPageNameFromIndex( index ) + ' done !' )


  def slotResetParameters( self ):

    index = 0
    for tabIndex in xrange( self._horizontalTabWidget.currentIndex() ):

      index +=  self._verticalTabWidgets[ tabIndex ].count()

    index += self._verticalTabWidgets[ \
                       self._horizontalTabWidget.currentIndex() ].currentIndex()
    algorithmGUI = self.getAlgorithmGUI( self.getPageNameFromIndex( index ) )
    algorithm = algorithmGUI.getAlgorithm()
    algorithmName = algorithm.getName()

    message = 'Do you really want to reset parameters of algorithm ' + \
              algorithmName + ' ?'

    answer = QtWidgets.QMessageBox.question( None,
                                         'Resetting algorithm parameters',
                                         message,
                                         QtWidgets.QMessageBox.Yes | \
                                         QtWidgets.QMessageBox.No )
    if answer == QtWidgets.QMessageBox.Yes:

      algorithm.resetParameters()
    
      QtWidgets.QMessageBox.about( self,
                              'Resetting algorithm parameters',
                              'Parameters reset for algorithm \'' + \
                              algorithmName +'\' !' )


  def slotMultiSubjectSettings( self ):

    multiSubjectSettingsDialog = MultiSubjectSettingsDialog( self._name )
    result = multiSubjectSettingsDialog.exec_()

    if result:

      settings = multiSubjectSettingsDialog.getSettings()
      ResourceManager().writeSettings( self.getName(), settings )
      print settings


  def slotToolboxPageSelectionSettings( self ):

    toolBoxPageSettingDialog = ToolBoxPageSettingDialog( \
                                                       self._name,
                                                       self._pages,
                                                       self._selectedPageNames )
    result = toolBoxPageSettingDialog.exec_()

    if result:

      settings = toolBoxPageSettingDialog.getSettings()
      ResourceManager().writeSettings( self.getName(), settings )
      print settings
      self.resetPages( settings[ 'toolbox_pages' ] )


  def slotTabWidgetChanged( self, index ):
  
  
    #print 'old ( horizontal, vertical ) tab indices: (', \
    #      self._oldHorizontalTabWidgetIndex, ',', \
    #      self._oldVerticalTabWidgetIndex, ')'
  
    currentHorizontalTabWidgetIndex = self._horizontalTabWidget.currentIndex()
    
    if ( len( self._verticalTabWidgets ) > currentHorizontalTabWidgetIndex ):
    
      currentVerticalTabWidgetIndex = self._verticalTabWidgets[ \
                                currentHorizontalTabWidgetIndex ].currentIndex()
                                
    else:
    
      currentVerticalTabWidgetIndex = 0
    #print 'current ( horizontal, vertical ) tab indices: (', \
    #      currentHorizontalTabWidgetIndex, ',', \
    #      currentVerticalTabWidgetIndex, ')'
          
    self._oldHorizontalTabWidgetIndex = currentHorizontalTabWidgetIndex
    self._oldVerticalTabWidgetIndex = currentVerticalTabWidgetIndex


  def slotSpecyPageSelectionSettings( self ):

    specySelectionDialog = SpecySelectionDialog( self._name )
    result = specySelectionDialog.exec_()

    if result:

      specy = specySelectionDialog.getSpecy()
      acquisitionType = specySelectionDialog.getAcquisitionType()
      
      print 'specy : ', specy, '/ acquisition type: ', acquisitionType

      settings = {}
      settings[ 'specy' ] = [ specy, acquisitionType ]
      ResourceManager().writeSettings( self._name, settings )
      
      if ( len( self._algorithmGUIs ) ):
      
        for pageName in self._algorithmGUIs.keys():
          
          self._algorithmGUIs[ pageName ].getAlgorithm().tuneToSpecy( \
                                                                    self._name )


  def displaySplashScreen( self ):

    name = self._name[ 0 : 1 ].swapcase() + self._name[ 1 : ]
    if ( os.path.exists( ResourceManager().getIconFileName(
                                    'splashscreen-' + self._name + '.png' ) ) ):

      splashScreenName = ResourceManager().getIconFileName(
                                         'splashscreen-' + self._name + '.png' )

    else:
    
      splashScreenName = ResourceManager().getIconFileName(
                                                      'splashscreen-pygkg.png' )

    self._splashScreen = QtGui.QSplashScreen( QtGui.QPixmap( \
                                                            splashScreenName ) )

    self._splashScreen.show()
    self._splashScreen.showMessage( 'Launching ' + name,
                                    QtCore.Qt.AlignHCenter |
                                    QtCore.Qt.AlignBottom,
                                    self._logoFontColor )
    self._splashScreen.update()
    QtGui.QApplication.processEvents()


  def hideSplashScreen( self ):
  
    self._splashScreen.finish( self )
    del self._splashScreen


  def getPageName( self, parameterFileName ):
  
    if ( parameterFileName is None ):
    
      message = 'parameter filename is none'
      raise RuntimeError, message
    
    if ( not os.path.exists( parameterFileName ) ):
  
      message = 'parameter file \'' + parameterFileName + '\' not found'
      raise RuntimeError, message

    parameterValues = dict()
    with open( parameterFileName, 'r' ) as f:

      parameterValues = json.load( f )

    return parameterValues[ '_algorithmName' ]


  def launch( self, parameterFileName = None, batchMode = False ):

    # if not in batch mode, build the user interface
    if ( batchMode == False ):

      # processing the adequate viewer size    
      availableGeometry = QtGui.QDesktopWidget().availableGeometry()
      print 'available geometry on the desktop : ( ', \
            availableGeometry.width(), ', ', \
            availableGeometry.height(), ' )'                                   

      self._preferedViewerSize = ( \
                                ( availableGeometry.height() - 200 ) / 50 ) * 50
      print 'prefered viewer size : ( ', self._preferedViewerSize, ', ', \
                                         self._preferedViewerSize, ' )'

      # adding pages to the tab widget
      self.resetPages( self.getSelectedPageNames() )


      # reading page name if parameter filename exists
      pageName = None
      if ( ( parameterFileName is not None ) and
           ( parameterFileName != '' ) ):
           
        pageName = self.getPageName( parameterFileName )
    

      # switching to the good page
      if ( pageName is not None ):

        horizontalTabIndex = self._pagesTabIndices[ pageName ]
        self._horizontalTabWidget.setCurrentIndex( horizontalTabIndex )
        pageIndex = self.getIndexFromPageName( pageName )
        previousTabCount = 0
        for previousTabIndex in xrange( \
                            self._pagesTabIndices[ pageName ] ):

          previousTabCount += \
                            self._verticalTabWidgets[ previousTabIndex ].count()

        tabIndex = self._pageIndices[ pageName ] - previousTabCount
        self._verticalTabWidgets[ horizontalTabIndex ].setCurrentIndex( \
                                                                      tabIndex )

      # reading the algorithm parameters
      if ( ( parameterFileName is not None ) and
           ( parameterFileName != '' ) ):
    
        self.getAlgorithmGUI( pageName ).getAlgorithm().readParameters(
                                                             parameterFileName )

      self.show()

    # if in batch mode do not build the user interface
    else:

      pageName = self.getPageName( parameterFileName )
    
      index = 0
      for page in self._pages:

        if( pageName == page[ 'pageName' ] ):

          break

        else:

          index += 1

      algorithm = self._pages[ index ][ 'algorithm' ]( self._verbose )
      algorithmGUI = AlgorithmGUI( algorithm, None )

      # reading the algorithm parameters
      algorithm.readParameters( parameterFileName )

      application = Application( algorithmGUI, None, None, self )
      algorithm.launch()
      self._applications[ pageName ] = application


  def resetViewers( self ):
  
    for application in self._applications.values():
    
      if ( application.getBatchMode() == False ):
      
        application.getViewer().reset()


  def closeViewersExceptForApplication( self, exceptApplication ):
  
    for application in self._applications.values():

      if ( application != exceptApplication ):

        if ( application.getBatchMode() == False ):
      
          application.closeViewer()


  def addLog( self, logText ):
  
    ResourceManager().addLog( self._logFileName, logText )


  def getDistributionStrategy( self ):

    return self._distributionStrategy


  def getThreadCount( self ):

    return self._thread_count


