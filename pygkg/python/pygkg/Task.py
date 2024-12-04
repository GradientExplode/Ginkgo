from Application import *
from ResourceManager import *
from SnapshotTool import *
from AbortableThread import *
import re
import time
import signal
import subprocess
import shutil


def executeCommand( task, subjectName, command, viewOnly ):

  if ( viewOnly == False ):
  
    if ( ( command[ 0 : 3 ] == 'Vip' ) or \
         ( command[ 0 : 4 ] == 'Aims' ) ):
         
      command = 'unset LD_LIBRARY_PATH; ' + \
                'export LC_NUMERIC=en_US.UTF-8; ' + \
                command

    p = subprocess.Popen( command,
                          shell = True,
                          bufsize = -1,
                          stdin = subprocess.PIPE,
                          stderr = subprocess.PIPE,
                          close_fds = True )

    print '------------->PID:', p.pid
    task._application.getAlgorithmGUI().getAlgorithm().addSystemPids( \
                                                            subjectName,
                                                            p.pid )
    (stdoutl, stderrl) = p.communicate()
    
    result = p.wait()
    
    if ( result != 0 ):

      raise RuntimeError, 'Command : ' + command + ' failed.'

    task._application.getAlgorithmGUI().getAlgorithm().removeSystemPids( \
                                                                   subjectName )

class Task:

  def __init__( self, application, defaultViewIndex = 1, sleepTime = 1 ):

    self._application = application
    self._functors = {}
    self._conditions = {}
    self._displayThread = None
    self._executeThread = dict()

    self.addFunctor( 'update-progress-bar',
                     self.updateProgressBar )
    self.addFunctor( 'viewer-reset',
                     self.viewerReset )
    self.addFunctor( 'viewer-abort',
                     self.abortDisplay )
    self.addFunctor( 'viewer-zoom-out-all-windows-and-graphs',
                     self.viewerZoomOutAllWindowsAndGraphs )
    self.addFunctor( 'viewer-set-view',
                     self.viewerSetView )
    self.addFunctor( 'viewer-load-object',
                     self.viewerLoadObject )
    self.addFunctor( 'viewer-delete-object',
                     self.viewerDeleteObject )
    self.addFunctor( 'viewer-reload-object',
                     self.viewerReloadObject )
    self.addFunctor( 'viewer-fusion-objects',
                     self.viewerFusionObjects )
    self.addFunctor( 'viewer-set-fusion2d-parameters',
                     self.viewerSetFusion2dParameters )
    self.addFunctor( 'viewer-set-colormap',
                     self.viewerSetColormap )
    self.addFunctor( 'viewer-set-diffuse',
                     self.viewerSetDiffuse )
    self.addFunctor( 'viewer-add-object-to-window',
                      self.viewerAddObjectToWindow )
    self.addFunctor( 'viewer-remove-object-from-window',
                      self.viewerRemoveObjectFromWindow )
    self.addFunctor( 'viewer-remove-object-from-all-windows',
                      self.viewerRemoveObjectFromAllWindows )
    self.addFunctor( 'viewer-create-referential',
                      self.viewerCreateReferential )
    self.addFunctor( 'viewer-assign-referential-to-object',
                      self.viewerAssignReferentialToObject )
    self.addFunctor( 'viewer-assign-referential-to-window',
                      self.viewerAssignReferentialToWindow )
    self.addFunctor( 'viewer-assign-object-to-graph',
                      self.viewerAssignObjectToGraph )
    self.addFunctor( 'viewer-load-transformation',
                      self.viewerLoadTransformation )
    self.addFunctor( 'viewer-add-object-to-graph',
                     self.viewerAddObjectToGraph )
    self.addFunctor( 'viewer-add-object-to-matrix-graph',
                     self.viewerAddObjectToMatrixGraph )
    self.addFunctor( 'viewer-create-roi-graph',
                     self.viewerCreateRoiGraph )
    self.addFunctor( 'viewer-create-hierarchy',
                     self.viewerCreateHierarchy )
    self.addFunctor( 'viewer-set-scene',
                     self.viewerSetScene )
    self.addFunctor( 'viewer-add-pdf-to-pdfviewer',
                     self.viewerAddPdfToPdfViewer )
    self.addFunctor( 'viewer-snapshot',
                     self.viewerSnapshot )
    self.addFunctor( 'snapshot-single-volume',
                     self.snapshotSingleVolume )
    self.addFunctor( 'snapshot-volume-fusion',
                     self.snapshotVolumeFusion )
    self.addFunctor( 'snapshot-volume-fusion-and-odf-field',
                     self.snapshotVolumeFusionAndOdfField )
    self.addFunctor( 'snapshot-histogram',
                     self.snapshotHistogram )
    self.addFunctor( 'snapshot-motion-profile',
                     self.snapshotMotionProfile )
    self.addFunctor( 'snapshot-outliers',
                     self.snapshotOutliers )
    self.addFunctor( 'viewer-adjust-camera',
                      self.viewerAdjustCamera )
    self.addFunctor( 'viewer-mute-window-to-axial',
                     self.viewerMuteWindowToAxial )
    self.addFunctor( 'viewer-mute-window-to-coronal',
                     self.viewerMuteWindowToCoronal )
    self.addFunctor( 'viewer-mute-window-to-sagittal',
                     self.viewerMuteWindowToSagittal )
    self.addFunctor( 'toolbox-get-parameters-of-page',
                     self.toolboxGetParametersOfPage )
    self.addFunctor( 'task-set-status-as-processing',
                     self.taskSetStatusAsProcessing )
    self.addFunctor( 'task-set-status-as-done',
                     self.taskSetStatusAsDone )
    self.addFunctor( 'task-set-status-as-failed',
                     self.taskSetStatusAsFailed )
    self.addFunctor( 'task-display-run-and-view-stacked-widget',
                     self.taskDisplayRunAndViewStackedWidget )
    self.addFunctor( 'condition-acquire',
                     self.conditionAcquire )
    self.addFunctor( 'condition-notify-and-release',
                     self.conditionNotifyAndRelease )
    self.addFunctor( 'condition-wait-and-release',
                     self.conditionWaitAndRelease )
    self.addFunctor( 'get-subject-list',
                     self.getSubjectList )
    self.addFunctor( 'hide-display-object2D',
                     self.viewerDisplayHideObject2D )
    self.addFunctor( 'synchronize3DWindows',
                     self.viewerSynchronize3DWindows )

    self._launchThread = None
    self._sleepTime = sleepTime
    self._failedSubjectNames = list()
    self._subjectList = None
    
    self._application.getParentToolBox().closeViewersExceptForApplication(
                                                             self._application )
    self._application.openViewer( defaultViewIndex )


  def setSleepTime( self, sleepTime ):

    self._sleepTime = sleepTime


  def launch( self, viewOnly ):

    self._application.getParentToolBox().addLog( \
                  self._application.getAlgorithmGUI().getAlgorithm().getName() )

    self.viewerZoomOutAllWindowsAndGraphs()

    multiSubjectParameters = self.getMultiSubjectParameters()
    self._subjectList = multiSubjectParameters.keys()
    self._subjectList.sort()

    self._launchThread = AbortableThread( target = self.launchMultiSubject,
                                          args = ( multiSubjectParameters,
                                                   viewOnly ) )
    self._application.getAlgorithmGUI().getAlgorithm().setLaunchThread( \
                                                            self._launchThread )
    self._launchThread.start()


  def launchMultiSubject( self, multiSubjectParameters, viewOnly ):

    self._executeThread = dict()
    distributionStrategy = None
    settings = None
    # getting distribution strategy
    if ( self._application.getBatchMode() ):

      distributionStrategy = \
                  self._application.getParentToolBox().getDistributionStrategy()

    else:

      settings = ResourceManager().readSettings( \
                                self._application.getParentToolBox().getName() )
      distributionStrategy = settings[ 'distribution_strategy' ]

      for subjectName in multiSubjectParameters:

        if not( os.path.exists( multiSubjectParameters[ subjectName ]\
                                       [ 'outputWorkDirectory' ].getValue() ) ):

          os.mkdir( os.path.exists( multiSubjectParameters[ subjectName ]\
                                        [ 'outputWorkDirectory' ].getValue() ) )

        if not viewOnly:

          self._application.getAlgorithmGUI().getAlgorithm().writeParameters( \
                os.path.join( \
                multiSubjectParameters[ subjectName ]\
                                      [ 'outputWorkDirectory' ].getValue(),
                self._application.getAlgorithmGUI().getAlgorithm().getName() + \
                '-parameters.py' ) )

    subjectNames = multiSubjectParameters.keys()
    subjectNames.sort()

    # single thread / single machine
    if ( distributionStrategy == 'single-threading' ):

      if ( self._application.getBatchMode() == False ):

        self._application.getMainThreadActionManager().call(
                            self._application.getAlgorithmGUI().addProgressBars,
                            self, 
                            multiSubjectParameters.keys() )

      for subjectName in subjectNames:

        threadExecute = AbortableThread(
          target = self.execute,
          args = (
            self._functors,
            multiSubjectParameters[ subjectName ],
            subjectName,
            self._application.getAlgorithmGUI().getAlgorithm().getVerbose(),
            viewOnly ) )

        self._application.getAlgorithmGUI().getAlgorithm().addExecuteThread( \
                                                    subjectName, threadExecute )
        self._executeThread[ subjectName ] = threadExecute
        self.taskSetStatusAsProcessing( subjectName )
        threadExecute.start()
        threadExecute.join()
        time.sleep( self._sleepTime )
        self.taskSetStatusAsDone( subjectName )

    # multi thread / single machine
    elif distributionStrategy == 'multiple-threading':

      maximumThreadCount = 1
      if ( self._application.getBatchMode() == False ):

        self._application.getMainThreadActionManager().call(
                            self._application.getAlgorithmGUI().addProgressBars,
                            self, 
                            multiSubjectParameters.keys() )
        maximumThreadCount = settings[ 'thread_count' ]

      else:

        maximumThreadCount = \
                          self._application.getParentToolBox().getThreadCount()

      subjectCount = len( multiSubjectParameters )
      if ( maximumThreadCount > subjectCount ):

        maximumThreadCount = subjectCount

      createdThreadCount = 0
      launchNewThread = True

      while ( ( createdThreadCount < subjectCount ) and \
                                                 self._launchThread.isAlive() ):

        if ( len( self._executeThread ) < maximumThreadCount ):

          launchNewThread = True

        else:

          launchNewThread = False
          # set previous process as done before launching new ones
          for k in self._executeThread.keys():

            if not self._executeThread[ k ].isAlive():

              self._application.getAlgorithmGUI().getAlgorithm()\
                                                       .removeExecuteThread( k )
              launchNewThread = True
              self.taskSetStatusAsDone( k )

        if ( launchNewThread ):

          subjectName = subjectNames[ createdThreadCount ]
          threadExecute = AbortableThread(
            target = self.execute,
            args = (
              self._functors,
              multiSubjectParameters[ subjectName ],
              subjectName,
              self._application.getAlgorithmGUI().getAlgorithm().getVerbose(),
              viewOnly ) )

          threadExecute.setName( subjectName )
          self._executeThread[ subjectName ] = threadExecute

          self._application.getAlgorithmGUI().getAlgorithm().addExecuteThread( \
                                                    subjectName, threadExecute )

          threadExecute.start()
          self.taskSetStatusAsProcessing( subjectName )

          createdThreadCount += 1

        else:

          time.sleep( self._sleepTime )

      # wait until last threads finished
      while ( self._executeThread != {} ):

        for k in self._executeThread.keys():

          if not self._executeThread[ k ].isAlive():

            self.taskSetStatusAsDone( k )

        if ( self._executeThread != {} ):

          time.sleep( self._sleepTime )



    # displaying the run and view tab
    self.taskDisplayRunAndViewStackedWidget()


  def activateDisplay( self, subjectName ):

    self._application.getViewer().setActivated( True )
    
    multiSubjectParameters = self.getMultiSubjectParameters()

    self._displayThread = AbortableThread(
      target = self.display,
      args = ( self._functors,
               multiSubjectParameters[ subjectName ],
               subjectName,
               self._application.getAlgorithmGUI().getAlgorithm().getVerbose(),
               self._application.getAlgorithmGUI().getAlgorithm().\
                                                        isReportModeActive() ) )

    self._displayThread.start()


  def abortDisplay( self ):

    if ( ( self._displayThread is not None ) and
         self._displayThread.isAlive() ):

      self._application.getViewer().setActivated( False )
      self._displayThread.terminate()

    self._application.getViewer().reset()
    self._displayThread = None


  def addFunctor( self, functorName, functor ):

    if ( self._functors.has_key( functorName ) ):

      message = '\'' + functorName + '\' functor already existing'
      raise RuntimeError, message

    self._functors[ functorName ] = functor
      

  def updateProgressBar( self, subjectName, value ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
                           self._application.getAlgorithmGUI().getProgressBar( \
                                                         subjectName ).setValue,
                           value )

  def viewerReset( self ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().reset )
        #self._application.getParentToolBox().resetViewers )


  def viewerZoomOutAllWindowsAndGraphs( self ):

    # BUG_PYANATOMIST
    #return

    if ( self._application.getBatchMode() == False ):

      if ( self._application.getViewer() is not None ):
      
        self._application.getMainThreadActionManager().call(
          self._application.getViewer().zoomOutAllWindowsAndGraphs )


  def viewerSetView( self, viewName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().setViewFromName,
        viewName )


  def viewerLoadObject( self, fileName, objectName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().loadObject,
        fileName, objectName )


  def viewerDeleteObject( self, objectName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().deleteObject,
        objectName )


  def viewerReloadObject( self, objectName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().reloadObject,
        objectName )


  def viewerFusionObjects( self, objectNames, fusionName, method = None ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().fusionObjects,
        objectNames, fusionName, method )


  def viewerSetFusion2dParameters( self, fusionName, fusionRate,
                                   method = None ):
  
    if ( self._application.getBatchMode() == False ):
    
      self._application.getMainThreadActionManager().call(
        self._application.getViewer().setFusion2dParameters,
        fusionName, fusionRate, method )
      

  def viewerSetColormap( self, objectName, colormapName,
                         lowerValue = None, upperValue = None ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().setColormap,
        objectName, colormapName, lowerValue, upperValue )


  def viewerSetDiffuse( self, objectName, diffuse ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().setDiffuse,
        objectName, diffuse )


  def viewerAddObjectToWindow( self, objectName, viewName, windowName,
                               addChildren = False,
                               addGraphNodes = True, 
                               addGraphRelations = False,
                               addWholeGraphObject = False ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().addObjectToWindow,
        objectName, viewName, windowName,
        addChildren, addGraphNodes, addGraphRelations, addWholeGraphObject )
      self.viewerAdjustCamera( viewName, windowName )


  def viewerRemoveObjectFromWindow( self, objectName, viewName, windowName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().removeObjectFromWindow,
        objectName, viewName, windowName )


  def viewerRemoveObjectFromAllWindows( self, objectName, viewName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().removeObjectFromAllWindows,
        objectName, viewName )


  def viewerCreateReferential( self, referentialName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().createReferential,
        referentialName )


  def viewerAssignReferentialToObject( self, referentialName, objectName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().assignReferentialToObject,
        referentialName, objectName )


  def viewerAssignReferentialToWindow( self, referentialName,
                                       viewName, windowName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().assignReferentialToWindow,
        referentialName, viewName, windowName )


  def viewerAssignObjectToGraph( self, objectName, graphName ):
  
    if ( self._application.getBatchMode() == False ):
    
      self._application.getMainThreadActionManager().call(
        self._application.getViewer().assignObjectToGraph,
        objectName, graphName )


  def viewerLoadTransformation( self, transformation,
                                originReferentialName,
                                destinationReferentialName ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().loadTransformation,
        transformation,
        originReferentialName,
        destinationReferentialName )


  def viewerAddObjectToGraph( self, x, y,
                              xlabel, ylabel,
                              xMinimum, xMaximum,
                              yMinimim, yMaximum,
                              viewName, graphName ):
 
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getViewer().addObjectToGraph,
        x, y,
        xlabel, ylabel,
        xMinimum, xMaximum,
        yMinimim, yMaximum,
        viewName, graphName )


  def viewerAddObjectToMatrixGraph( self,
                                    viewName, graphName,
                                    dataMatrix,
                                    xLabel = '', 
                                    yLabel = '',
                                    xAxisLabels = '',
                                    yAxisLabels = ''):
  
    if ( self._application.getBatchMode() == False ):
    
      self._application.getMainThreadActionManager().call(
        self._application.getViewer().addObjectToMatrixGraph,
        viewName, graphName, dataMatrix,
        xLabel, yLabel,
        xAxisLabels, yAxisLabels )
      
      
  def viewerCreateRoiGraph( self, graphName ):
  
    if ( self._application.getBatchMode() == False ):
  
        self._application.getMainThreadActionManager().call(    
           self._application.getViewer().createRoiGraph, graphName )
        
        
  def viewerCreateHierarchy( self, hierarchyName, graphSyntaxName, 
                             listOfNames, listOfRGBColors, 
                             fileNameToSave = '' ):
  
    if ( self._application.getBatchMode() == False ):
  
        self._application.getMainThreadActionManager().call(    
           self._application.getViewer().createHierarchy, hierarchyName,
                graphSyntaxName, listOfNames, listOfRGBColors, fileNameToSave )
      
      
  def viewerAdjustCamera( self, viewName, windowName, 
                          cursorPosition = None,
                          viewQuaternion = None,
                          zoom = None ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(    
         self._application.getViewer().adjustCamera,
         viewName, windowName, cursorPosition, viewQuaternion, zoom )


  def viewerAddPdfToPdfViewer( self, viewName, pdfViewerName, fileNamePdf ):
  
    if ( self._application.getBatchMode() == False ):
  
        self._application.getMainThreadActionManager().call(    
           self._application.getViewer().addPdfToPdfViewer,
           viewName, pdfViewerName, fileNamePdf )
        
        
  def viewerMuteWindowToAxial( self, viewName, windowName ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(    
         self._application.getViewer().muteWindowToAxial,
         viewName, windowName )


  def viewerMuteWindowToCoronal( self, viewName, windowName ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(    
         self._application.getViewer().muteWindowToCoronal,
         viewName, windowName )


  def viewerMuteWindowToSagittal( self, viewName, windowName ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(    
         self._application.getViewer().muteWindowToSagittal,
         viewName, windowName )

  def viewerSetScene( self,
                      viewName,
                      windowName, 
                      cursor_visibility = None, 
                      show_toolbars = None, 
                      view_size = None, 
                      backGroundColor = None ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(    
         self._application.getViewer().setScene,
         viewName,
         windowName,
         cursor_visibility, 
         show_toolbars, 
         view_size, 
         backGroundColor )

  def viewerSnapshot( self, viewName, windowName, fileName ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(    
        self._application.getViewer().snapshot, viewName, windowName, fileName )


  def snapshotSingleVolume( self,
                            subjectName,
                            fileNameVolume,
                            colorMapName,
                            lowerValue, upperValue,
                            windowSize,
                            fileNameSnapshotAxial,
                            fileNameSnapshotCoronal,
                            fileNameSnapshotSagittal ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
      
        SnapshotTool().snapitSingleVolume,
        subjectName,
        fileNameVolume,
        colorMapName,
        lowerValue, upperValue,
        windowSize,
        fileNameSnapshotAxial,
        fileNameSnapshotCoronal,
        fileNameSnapshotSagittal )
        
    else:

      SnapshotTool().snapitSingleVolume( \
        subjectName,
        fileNameVolume,
        colorMapName,
        lowerValue, upperValue,
        windowSize,
        fileNameSnapshotAxial,
        fileNameSnapshotCoronal,
        fileNameSnapshotSagittal )


  def snapshotVolumeFusion( self,
                            subjectName,
                            fileNameVolume1,
                            colorMapName1,
                            lowerValue1, upperValue1,
                            fileNameVolume2,
                            colorMapName2,
                            lowerValue2, upperValue2,
                            fileNameTransformationVolume2ToVolume1,
                            windowSize,
                            fileNameSnapshotAxial,
                            fileNameSnapshotCoronal,
                            fileNameSnapshotSagittal ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        SnapshotTool().snapitVolumeFusion,
        subjectName,
        fileNameVolume1,
        colorMapName1,
        lowerValue1, upperValue1,
        fileNameVolume2,
        colorMapName2,
        lowerValue2, upperValue2,
        fileNameTransformationVolume2ToVolume1,
        windowSize,
        fileNameSnapshotAxial,
        fileNameSnapshotCoronal,
        fileNameSnapshotSagittal )
        
    else:
    
      SnapshotTool().snapitVolumeFusion( \
        subjectName,
        fileNameVolume1,
        colorMapName1,
        lowerValue1, upperValue1,
        fileNameVolume2,
        colorMapName2,
        lowerValue2, upperValue2,
        fileNameTransformationVolume2ToVolume1,
        windowSize,
        fileNameSnapshotAxial,
        fileNameSnapshotCoronal,
        fileNameSnapshotSagittal )


  def snapshotVolumeFusionAndOdfField( self,
                                       subjectName,
                                       fileNameVolume1,
                                       colorMapName1,
                                       lowerValue1, upperValue1,
                                       fileNameVolume2,
                                       colorMapName2,
                                       lowerValue2, upperValue2,
                                       fileNameOdfField,
                                       fileNameTransformationVolume2ToVolume1,
                                       fileNameTransformationOdfFieldToVolume1,
                                       windowSize,
                                       zoom,
                                       fileNameSnapshotAxial,
                                       fileNameSnapshotCoronal,
                                       fileNameSnapshotSagittal ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        SnapshotTool().snapitVolumeFusionAndOdfField,
        subjectName,
        fileNameVolume1,
        colorMapName1,
        lowerValue1, upperValue1,
        fileNameVolume2,
        colorMapName2,
        lowerValue2, upperValue2,
        fileNameOdfField,
        fileNameTransformationVolume2ToVolume1,
        fileNameTransformationOdfFieldToVolume1,
        windowSize,
        zoom,
        fileNameSnapshotAxial,
        fileNameSnapshotCoronal,
        fileNameSnapshotSagittal )
        
    else:
    
      SnapshotTool().snapitVolumeFusionAndOdfField(
        subjectName,
        fileNameVolume1,
        colorMapName1,
        lowerValue1, upperValue1,
        fileNameVolume2,
        colorMapName2,
        lowerValue2, upperValue2,
        fileNameOdfField,
        fileNameTransformationVolume2ToVolume1,
        fileNameTransformationOdfFieldToVolume1,
        windowSize,
        zoom,
        fileNameSnapshotAxial,
        fileNameSnapshotCoronal,
        fileNameSnapshotSagittal )


  def snapshotHistogram( self,
                         subjectName,
                         xs,
                         hs,
                         xLabel,
                         yLabel,
                         fileNameSnapshotHistogram ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        SnapshotTool().snapitHistogram,
        subjectName,
        xs,
        hs,
        xLabel,
        yLabel,
        fileNameSnapshotHistogram )
        
    else:
    
      SnapshotTool().snapitHistogram(
        subjectName,
        xs,
        hs,
        xLabel,
        yLabel,
        fileNameSnapshotHistogram )
    

  def snapshotMotionProfile( self,
                             subjectName,
                             translationX,
                             translationY,
                             translationZ,
                             rotationX,
                             rotationY,
                             rotationZ,
                             scalingX,
                             scalingY,
                             scalingZ,
                             shearingXY,
                             shearingXZ,
                             shearingYZ,
                             fileNameSnapshotMotionProfile ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        SnapshotTool().snapitMotionProfile,
        subjectName,
        translationX,
        translationY,
        translationZ,
        rotationX,
        rotationY,
        rotationZ,
        scalingX,
        scalingY,
        scalingZ,
        shearingXY,
        shearingXZ,
        shearingYZ,
        fileNameSnapshotMotionProfile )
        
    else:

      SnapshotTool().snapitMotionProfile(
        subjectName,
        translationX,
        translationY,
        translationZ,
        rotationX,
        rotationY,
        rotationZ,
        scalingX,
        scalingY,
        scalingZ,
        shearingXY,
        shearingXZ,
        shearingYZ,
        fileNameSnapshotMotionProfile )

  def snapshotOutliers( self,
                        subjectName,
                        orientations,
                        slices,
                        qSpacePointCount,
                        sliceCount,
                        fileNameSnapshotOutliers ):
    
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        SnapshotTool().snapitOutliers,
        subjectName,
        orientations,
        slices,
        qSpacePointCount,
        sliceCount,
        fileNameSnapshotOutliers )
        
    else:
    
      SnapshotTool().snapitOutliers(
        subjectName,
        orientations,
        slices,
        qSpacePointCount,
        sliceCount,
        fileNameSnapshotOutliers )


  def toolboxGetParametersOfPage( self, pageName ):
  
    parametersOfPage = \
      self._application.getMainThreadActionManager().call(
      self._application.getParentToolBox().getParametersOfPage,
      pageName )

    return parametersOfPage
      

  def taskSetStatusAsProcessing( self, subjectName ):
  
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getAlgorithmGUI().setStatusAsProcessing,
        subjectName )
      

  def taskSetStatusAsDone( self,
                           subjectName,
                           warningMessage = None ):

    if ( self._failedSubjectNames.count( subjectName ) == 0 ):

      if( self._executeThread.has_key( subjectName ) ):

        self._executeThread.pop( subjectName )

      if ( self._application.getBatchMode() == False ):

        self._application.getMainThreadActionManager().call(
          self._application.getAlgorithmGUI().setStatusAsDone,
          subjectName, warningMessage )

      else:

        print subjectName, 'done successfully.'


  def taskSetStatusAsDoneWithErrorMessage( self,
                                           subjectName,
                                           warningMessage = None ):

    if ( self._failedSubjectNames.count( subjectName ) == 0 ):

      if( self._executeThread.has_key( subjectName ) ):

        self._executeThread.pop( subjectName )

      if ( self._application.getBatchMode() == False ):

        self._application.getMainThreadActionManager().call(
          self._application.getAlgorithmGUI().setStatusAsDoneWithErrorMessage,
          subjectName, warningMessage )

      else:

        print subjectName, 'done but with error message', warningMessage


  def taskSetStatusAsFailed( self, subjectName, executionInfo ):

    self._failedSubjectNames.append( subjectName )
    if ( self._application.getBatchMode() == False ):

      if( self._application.getAlgorithmGUI().getProgressBar( \
                                                  subjectName ).isDisplayed() ):
        self.abortDisplay()

      if( self._executeThread.has_key( subjectName ) ):

        self._executeThread.pop( subjectName )

      self._application.getMainThreadActionManager().call(
        self._application.getAlgorithmGUI().setStatusAsFailed,
        subjectName, executionInfo )

    else:

      print subjectName, 'failed with error:'
      nextTraceBack = executionInfo[ 2 ]
      logError =  '\nError: ' + str( executionInfo[ 1 ] ) + '\n' 
      while nextTraceBack is not None:

        logError += str( nextTraceBack.tb_frame.f_code.co_filename ) + \
                   ' in ' + \
                   str( nextTraceBack.tb_frame.f_code.co_name ) + \
                   ' at line ' + \
                   str( nextTraceBack.tb_lineno ) + ' \n'
        nextTraceBack = nextTraceBack.tb_next

      print logError


  def taskDisplayRunAndViewStackedWidget( self ):
  
    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
        self._application.getAlgorithmGUI().displayRunAndViewStackedWidget )
      

  def conditionAcquire( self, subjectName, conditionName ):
  

    if ( self._application.getBatchMode() == False ):

      prefixedConditionName = subjectName + ':' + conditionName
      if ( self._conditions.has_key( prefixedConditionName ) ):

        message = '\'' + prefixedConditionName + '\' condition already existing'
        raise RuntimeError, message

      self._conditions[ prefixedConditionName ] = threading.Condition()
      self._conditions[ prefixedConditionName ].acquire()
      

  def conditionNotifyAndRelease( self, subjectName, conditionName ):
  
    if ( self._application.getBatchMode() == False ):

      prefixedConditionName = subjectName + ':' + conditionName
      if ( self._conditions.has_key( prefixedConditionName ) == False ):

        message = 'Condition ' + prefixedConditionName + \
                      ' not acquired.'
        raise RuntimeError, message

      else:

        self._conditions[ prefixedConditionName ].notifyAll()
        self._conditions[ prefixedConditionName ].release()
      

  def conditionWaitAndRelease( self, subjectName, conditionName ):

    if ( self._application.getBatchMode() == False ):

      prefixedConditionName = subjectName + ':' + conditionName
      count = 0
      while ( self._conditions.has_key( prefixedConditionName ) == False ):

        print prefixedConditionName + 'condition not acquired yet...'
        time.sleep( self._sleepTime )
        count += 1

        if( count == 20 ):

          message = 'Condition ' + prefixedConditionName + \
                    ' not acquired before limit time elapsed.'
          raise RuntimeError, message

      self._conditions[ prefixedConditionName ].acquire()
      self._conditions[ prefixedConditionName ].release()


  def getSubjectList( self ):

    return self._subjectList  


  def execute( self, functors, parameters, subjectName, verbose, viewOnly ):

    message = 'not implemented'
    raise RuntimeError, message


  def display( self, functors, parameters, subjectName, verbose,
               isReportModeActive ):

    message = 'not implemented'
    print message


  def getMultiSubjectParameters( self ):

    subjectNames = []

    parameters = \
              self._application.getAlgorithmGUI().getAlgorithm().getParameters()

    # add multi subject directory if not given
    subjectSelection = self._application.getAlgorithmGUI()\
                                      .getAlgorithm().getMultiSubjectSelection()
    if( subjectSelection != None ):

      singleSubject = subjectSelection[ 1: len( subjectSelection ) - 1 ]
      outputWorkDirectoryParameter = parameters[ 'outputWorkDirectory' ]

      if( outputWorkDirectoryParameter.getValue().find( singleSubject ) == -1 ):

        if( outputWorkDirectoryParameter.getValue().count( \
                                                      subjectSelection ) == 0 ):

          outputWorkDirectoryParameter.setValue( \
                          os.path.join( outputWorkDirectoryParameter.getValue(),
                                        subjectSelection ) )
    # getting subject names
    firstMultiSubjectParameterKey = None
    firstMultiSubjectParameter = None
    firstRegularExpressionCount = 0
    for k in parameters.keys():

      parameter = parameters[ k ]
      if ( parameter.getType() == 'chainList' ):

        for singleParameter in parameter._parameters:

          for sp in singleParameter.keys():

            if not( sp == 'processName' ):

              if ( parameter.getType() == 'string' ):

                value = ''
                if ( parameter.getWidgetType() == 'listWidget' ):

                  value = singleParameter[ sp ].getValue().split()[ 0 ]

                else:

                  value = singleParameter[ sp ].getValue()

                regularExpressionCount = value.count( '*' ) + value.count( '{' )

                if ( ( regularExpressionCount > 0 ) and \
                   ( not parameter.isAnOutputParameter() ) and \
                   parameter.isAPathParameter() ):

                  firstMultiSubjectParameter = singleParameter[ sp ]
                  firstRegularExpressionCount = regularExpressionCount

      elif ( parameter.getType() == 'string' ):

        value = ''
        if ( parameter.getWidgetType() == 'listWidget' ):

          parameterSplitted = parameter.getValue().split()
          if ( len( parameterSplitted ) > 0 ):

            value = parameterSplitted[ 0 ]

        else:

          value = parameter.getValue()

        regularExpressionCount = value.count( '*' ) + value.count( '{' )

        if ( ( regularExpressionCount > 0 ) and \
             ( not parameter.isAnOutputParameter() ) and \
             parameter.isAPathParameter() ):

          firstMultiSubjectParameter = parameter
          firstRegularExpressionCount = regularExpressionCount

    if ( firstMultiSubjectParameter is not None ):

      # get initial subjects
      regularExpressionSubject = 'noMultiSubject'
      firstMultiSubjectParameterString = firstMultiSubjectParameter.getValue()
      firstMultiSubjectParameterString = \
                              firstMultiSubjectParameterString.split( ';' )[ 0 ]

      if ( firstMultiSubjectParameterString.count( '{' ) ):

        regularExpression = '(.+){(.+)}'
        pattern = re.compile( regularExpression )
        match = pattern.search( firstMultiSubjectParameterString )
        subjectsString = match.group( 2 )
        subjectNames = subjectsString.split( ',' )
        regularExpressionSubject = '{' + subjectsString + '}'

      else:

        regularExpressionSubject = '*'
        regularExpression = '(.+)'
        rootPath = ''
        regularExpressionCount = firstMultiSubjectParameterString.count( '*' )
        for r in xrange( regularExpressionCount ):

          regularExpression = regularExpression + '\*(.+)'

        pattern = re.compile( regularExpression )
        match = pattern.search( firstMultiSubjectParameterString )
        if ( match is None ):

          pattern = re.compile( '(.+)\*' )
          match = pattern.search( firstMultiSubjectParameterString )

        if ( match is None ):

          message = firstMultiSubjectParameterKey + ' is not a path'
          raise RuntimeError, message

        subjectNames = []
        path =  os.path.split( match.group( 1 ) )
        files = os.listdir( path[ 0 ] )
        if ( path[ 1 ] == '' ):

          subjectNames = files

        else:

          pathParts = match.group( 1 ).split( os.sep )
          pathParts.reverse()
          first = pathParts[ 0 ]
          if ( len( match.groups() ) >= 2 ):

            second = match.group( 2 ).split( os.sep  )[ 0 ]

          else:

            second = ''

          for file in files:

            groups = re.search( '(' + first + ')(.+)(' + second + ')', file )
            if ( groups is not None ):

              subjectNames.append( groups.group( 2 ) )

          subjectNamesSet = set( subjectNames )
          subjectNames = []
          for i in range( 0, len( subjectNamesSet ) ):

            subjectNames.append( subjectNamesSet.pop() )

      # checking if subjects exist for every given parameter
      for k in parameters.keys():

        parameter = parameters[ k ]

        if ( parameter.getType() == 'chainList' ):

          for singleParameter in parameter._parameters:

            for sp in singleParameter.keys():

              if not( sp == 'processName' ):

                if ( parameter.getType() == 'string' ):

                  regularExpressionCount = \
                                       singleParameter[ sp ].getValue().count( \
                                                      regularExpressionSubject )

                  if ( ( regularExpressionCount > 0 ) and \
                   ( not singleParameter[ sp ].isAnOutputParameter() ) and \
                   singleParameter[ sp ].isAPathParameter() ):

                    s = 0
                    while ( s < len( subjectNames ) ):

                      path = singleParameter[ sp ].getValue().replace( \
                                                   regularExpressionSubject,
                                                   subjectNames[ s ] )
                      if ( singleParameter[ sp ].getWidgetType() \
                                                              == 'listWidget' ):

                        paths = path.split()
                        for path in paths:

                          if not os.path.exists( path ):

                            subjectNames.remove( subjectNames[ s ] )
                            message = 'error: ' + path + 'is not existing.'
                            print message
                            print s + ' not processed.'
                            #raise RuntimeError, message
                            break

                        s += 1

                      else:

                        paths = path.split( ';' )
                        for partPath in paths:

                          if not os.path.exists( partPath ):

                            subjectNames.remove( subjectNames[ s ] )

                          else:

                            s += 1


        elif ( parameter.getType() == 'string' ):

          regularExpressionCount = parameter.getValue().count( \
                                                      regularExpressionSubject )

          if ( ( regularExpressionCount > 0 ) and \
               ( not parameter.isAnOutputParameter() ) and \
               parameter.isAPathParameter() ):

            s = 0

            while ( s < len( subjectNames ) ):

              path = parameter.getValue().replace( regularExpressionSubject,
                                                   subjectNames[ s ] )

              if ( parameter.getWidgetType() == 'listWidget' ):

                paths = path.split()
                for path in paths:

                  if not os.path.exists( path ):

                    subjectNames.remove( subjectNames[ s ] )
                    message = 'error: ' + path + 'is not existing.'
                    print message
                    print s + ' not processed.'
                    #raise RuntimeError, message
                    break

                s += 1

              else:

                paths = path.split( ';' )
                for partPath in paths:

                  if not os.path.exists( partPath ):

                    subjectNames.remove( subjectNames[ s ] )

                  else:

                    s += 1

      # defining parameters for each subject
      parametersBySubject = {}
      for subjectName in subjectNames:

        parametersDictionary = {}
        parametersBySubject[ subjectName ] = parametersDictionary

      for k in parameters.keys():

        parameter = parameters[ k ]
        if ( parameter.getType() == 'chainList' ):

          for subjectName in subjectNames:

            subjectParameter = parameter
            for singleParameter in subjectParameter._parameters:

              for sp in singleParameter.keys():

                if not( sp == 'processName' ):

                  if ( singleParameter[ sp ].getType() == 'string' ):

                    regularExpressionCount = \
                                       singleParameter[ sp ].getValue().count( \
                                       regularExpressionSubject )

                    if ( regularExpressionCount > 0 ):

                      parameterValue = \
                                     singleParameter[ sp ].getValue().replace( \
                                     regularExpressionSubject, subjectName )

                      singleParameter[ sp ] = \
                                            StringParameter( \
                                   k,
                                   parameterValue,
                                   singleParameter[ sp ].isAPathParameter(),
                                   singleParameter[ sp ].isAnOutputParameter() )

            parametersBySubject[ subjectName ][ k ] = subjectParameter

        elif ( parameter.getType() == 'string' ):

          regularExpressionCount = parameter.getValue().count( \
                                                      regularExpressionSubject )

          if ( regularExpressionCount > 0 ):

            for subjectName in subjectNames:

                parameterValue = parameter.getValue().replace( \
                                         regularExpressionSubject, subjectName )

                parametersBySubject[ subjectName ][ k ] = \
                  StringParameter( k,
                                   parameterValue,
                                   parameter.isAPathParameter(),
                                   parameter.isAnOutputParameter() )

                if parameter.isAnOutputParameter():

                  if not os.path.exists( parameterValue ):

                    os.makedirs( parameterValue )

          else:

            for subjectName in subjectNames:
            
              parametersBySubject[ subjectName ][ k ] = parameter

        else:

          for subjectName in subjectNames:

            parametersBySubject[ subjectName ][ k ] = parameter


      if( parametersBySubject == {} ):

        message = firstMultiSubjectParameter.getValue() + \
                                                      ' is not an existing path'
        self._application.getAlgorithmGUI().displayRunAndViewStackedWidget()
        raise RuntimeError, message


    else:

      parametersBySubject = { 'single-subject' : parameters }

      for k in parameters.keys():

        parameter = parameters[ k ]
        if ( parameter.getType() == 'string' ):

          if parameter.isAnOutputParameter():

            parameterValue = parameter.getValue()
            if not os.path.exists( parameterValue ):

              os.makedirs( parameterValue )
    
    return parametersBySubject


  def viewerDisplayHideObject2D( self, viewName, name ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
             self._application.getViewer().displayHideObject2D, viewName, name )


  def viewerSynchronize3DWindows( self, viewName, name ):

    if ( self._application.getBatchMode() == False ):

      self._application.getMainThreadActionManager().call(
            self._application.getViewer().synchronize3DWindows, viewName, name )



