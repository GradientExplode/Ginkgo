import PyQt5
from PyQt5 import QtCore, QtGui, QtWidgets, uic

import matplotlib
matplotlib.use( 'Qt5Agg' )
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure
from matplotlib.lines import Line2D
from matplotlib.widgets import Slider

from matplotlib.pylab import *
import glob
import re
import colorsys

import os, sys


class MatrixGraphCallbackManager:

  def __init__( self,
                axesImage,
                figure ):
                
    self._axesImage = axesImage

  def sliderMatrixIntensity( self, value ):
    
    if ( value != 0 ):
            
      axesImage.set_clim( 0, value )
      figure.canvas.draw()


################################################################################
# main
################################################################################




# in case of help
if ( ( len( sys.argv ) >= 2 ) and ( sys.argv[ 1 ] != '-h' ) ):

  if QtWidgets.qApp.startingUp():

    qapp = QtWidgets.QApplication( sys.argv )
    runqt = True

  else:

    runqt = False

  fileNameConnectivityMatrix = sys.argv[ 1 ]
  if ( not os.path.exists( fileNameConnectivityMatrix ) ):
  
    message = '\'' + fileNameConnectivityMatrix + '\' does not exist:\n' + \
              'Stop here because mandatory file has not been found'
    raise( RuntimeError, message )
     
  f = open( fileNameConnectivityMatrix, 'r' )
  fileData = f.read()
  f.close()
    
  fileDataSplitted = fileData.split( ' ' )
    

  # sanity check
  if ( len ( fileDataSplitted ) < 3 ):
      
    print( 'Error while reading ', fileName, 'Expecting at least 3 data in it' )
    
  # matrix syntax:
  # dim1 dim2 nbPairs labelx labely value labelx labely value...
    
  firstIndexInList = 3
  labelPairCount = int( fileDataSplitted[ 2 ] )
    
  xlabels = set()
  ylabels = set()
    
  setw = 9
  labelCountMaximumSize = firstIndexInList + 3 * labelPairCount
    
  # find label names
  for i in range( firstIndexInList, labelCountMaximumSize ):
      
      
    dataRead = fileDataSplitted[ i ]
    indexRead = i - firstIndexInList
      
    if ( dataRead == '' ):
        
      continue
      
    # Find x labels
    if ( indexRead % 3 == 1 ):
        
      xlabels.add( dataRead )
      
    # Find y labels
    elif ( indexRead % 3 == 0 ):
        
      ylabels.add( dataRead )
    
  # sort labels using value of the string    
  xlabels = sorted( xlabels, key = lambda v: float( v ) )
  ylabels = sorted( ylabels, key = lambda v: float( v ) )
    
  # create dictionary to quickly get label indexes
  dictXLabelIndices = {}
  dictYLabelIndices = {}
    
  count = 0
  for label in xlabels:
      
    dictXLabelIndices[ label ] = count
    count += 1
    
  count = 0
  for label in ylabels:
      
    dictYLabelIndices[ label ] = count
    count += 1


  # fill matrix with 0 (sparse matrix)
  matrixDims = [ len( ylabels ), len( xlabels ) ]
  dataMatrix = zeros( matrixDims )
    
  # fill matrix with connectivity values
    
  # matrix indexes
  xIndex = 0
  yIndex = 0
    
    
  # Find label names
  for i in range( firstIndexInList, labelCountMaximumSize ):
      
    # Sanity check
    if ( dataRead == '' ):
        
      continue
        
    dataRead = fileDataSplitted[ i ]
    indexRead = i - firstIndexInList
      
    # Get x label index
    if ( indexRead % 3 == 1 ):
            
      xIndex = dictXLabelIndices[ dataRead ]  
          
    # Get y label index
    elif ( indexRead % 3 == 0 ):
            
      yIndex = dictYLabelIndices[ dataRead ]
        
    # Get connectivity value
    else:
        
      dataMatrix[ yIndex, xIndex ] = float( dataRead )
  

  print    
  print( dataMatrix )
      

  mainWindow = QtWidgets.QMainWindow()   
  view = QtWidgets.QWidget( mainWindow )
  view.setMinimumSize( 800, 800 )
                                            
  figure = Figure()
  figure.set_facecolor( 'w' )
  figure.set_frameon( True )
  axes = figure.add_subplot( 111 )
  #axes.hold( False )
  defaultLabels = [ 'label0', 'label1', 'label2', 'label3' ]

  canvas = FigureCanvasQTAgg( figure )
  #canvas.setSizePolicy( QtGui.QSizePolicy.MinimumExpanding,
  #                      QtGui.QSizePolicy.MinimumExpanding)
  #canvas.setMinimumSize( 800, 800 )
  #canvas.updateGeometry()


  horizontalLayoutMatrix = QtWidgets.QHBoxLayout( view )
  horizontalLayoutMatrix.addWidget( canvas )

  slider = QtWidgets.QSlider( QtCore.Qt.Vertical, view )
  slider.setRange( 0, 100 )
  slider.setValue( 100 )
  horizontalLayoutMatrix.addWidget( slider )


  axesImage = axes.matshow( dataMatrix,
                      aspect = 'auto',
                      interpolation = 'nearest' )

  callbackManager = MatrixGraphCallbackManager( axesImage, figure )
  
  colorbar = figure.colorbar( axesImage )
  colorbar.draw_all()
  
  slider.valueChanged.connect( callbackManager.sliderMatrixIntensity )
  
  
  # sanity check
  if ( ( len( ylabels ) != 0 ) and \
       ( len( ylabels ) != len( dataMatrix ) ) ):
    
    message = 'bad ylabels size!'
    raise( RuntimeError, message )
  
  if ( ( len( xlabels ) != 0 ) and 
       ( len( xlabels ) != len( dataMatrix[ 0 ] ) ) ):
    
    message = 'bad xAxisLabels size!'
    raise( RuntimeError, message )
  
  
  # retrieving ticks for each axis
  xTicks = axes.get_xticks()
  yTicks = axes.get_yticks()
  
  # sublist of labels which will be displayed in axis (not all label may be 
  # displayed if there are too many...)
  subXTicksLabels = []
  subYTicksLabels = []
  
  if ( len( xTicks ) > 0 ): 
    
    for i in range ( 1, len( xTicks ) ):
      
      if ( ( int( xTicks[ i ] ) ) < len( xlabels ) ):
        
        subXTicksLabels.append( xlabels[ int( xTicks[ i ] ) ] )
      
  
  if ( len( yTicks ) > 0 ):
    
    for i in range ( 1, len( yTicks ) ):
      
      if ( ( int( yTicks[ i ] ) ) < len( ylabels ) ):
        
        subYTicksLabels.append( ylabels[ int( yTicks[ i ] ) ] )
  
  
  # setting new tick    
  if ( len( xlabels ) ) :
    
    
    #Info : rotation parameter has only an effect with matplotlib > v1.00
    axes.set_xticklabels( [''] + subXTicksLabels, rotation = 45 )
  
  if ( len( xlabels ) ) :
    
    axes.set_yticklabels( [''] + subYTicksLabels )
    
  # Find matrix max value
  matrixDimX = len( dataMatrix[ 0 ] )
  matrixDimY = len( dataMatrix )
  matrixMaxValue = 0
  
  for i in range( 0, matrixDimX ):
    
    for j in range( 0, matrixDimY ):
      
      if ( dataMatrix[ j ][ i ] > matrixMaxValue ):
        
         matrixMaxValue = dataMatrix[ j ][ i ]      
    
  slider.setRange( 0, int( matrixMaxValue ) )
  slider.setValue( int( matrixMaxValue ) )
    
    
  if ( len( sys.argv ) > 2 ):
  
    axes.set_xlabel( sys.argv[ 2 ] )
    axes.set_ylabel( sys.argv[ 3 ] )
  
  mainWindow.setMinimumSize( 800, 800 )
  mainWindow.show()

  if runqt:
  
    qapp.exec_()

else:
  
  print( 'GkgDisplayConnectivityMatrix <connectivity-matrix>' )
    
  
