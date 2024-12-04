import PyQt5                                               
from PyQt5 import QtCore, QtWidgets, uic
from ResourceManager import *

class ColorMapDialog( QtWidgets.QDialog ):

  def __init__( self,
                viewer,
                objectNames,
                colorMapNames, 
                parent = None ):


    QtWidgets.QDialog.__init__( self, parent )

    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                                         'core',
                                                         'ColorMapDialog.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )

    self._colorMapIndices = {}
    index = int( 0 )
    for c in colorMapNames:
    
      self._colorMapIndices[ c ] = index
      index = index + 1

    self._viewer = viewer
    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )

    ########################### object list ####################################
    self._listWidgetObjects = self._findChild( self._root,
                                               'listWidget_Objects' )
    for objectName in objectNames:
    
      self._listWidgetObjects.addItem( objectName )
                                               
    self.connect( self._listWidgetObjects,
                  QtCore.SIGNAL(
                      'currentItemChanged(QListWidgetItem*,QListWidgetItem*)' ),
                  self.selectObjectSlot )

                                               
    ########################### colormap list ##################################
    self._listWidgetColorMaps = self._findChild( self._root,
                                                 'listWidget_ColorMaps' )
    for colorMapName in colorMapNames:
    
      self._listWidgetColorMaps.addItem( colorMapName )

    self.connect( self._listWidgetColorMaps,
                  QtCore.SIGNAL(
                      'currentItemChanged(QListWidgetItem*,QListWidgetItem*)' ),
                  self.selectColorMapSlot )

    ########################### texture minimum ################################
    self._doubleSpinBoxLowerBound = self._findChild(
                                                    self._root,
                                                    'doubleSpinBox_LowerBound' )
    self._doubleSpinBoxLowerBound.setMinimum( -1e38 )
    self._doubleSpinBoxLowerBound.setMaximum( +1e38 )

    self.connect( self._doubleSpinBoxLowerBound,
                  QtCore.SIGNAL( 'valueChanged(float)' ),
                  self.setLowerBoundSlot )

    ########################### texture maximum ################################
    self._doubleSpinBoxUpperBound = self._findChild(
                                                    self._root,
                                                    'doubleSpinBox_UpperBound' )
    self._doubleSpinBoxUpperBound.setMinimum( -1e38 )
    self._doubleSpinBoxUpperBound.setMaximum( +1e38 )

    self.connect( self._doubleSpinBoxUpperBound,
                  QtCore.SIGNAL( 'valueChanged(float)' ),
                  self.setUpperBoundSlot )

    ########################### relative minimum ###############################
    self._horizontalSliderMinimum = self._findChild(
                                                    self._root,
                                                    'horizontalSlider_Minimum' )

    self.connect( self._horizontalSliderMinimum,
                  QtCore.SIGNAL( 'valueChanged(int)' ),
                  self.setPaletteMinimumSlot )

    ########################### relative maximum ###############################
    self._horizontalSliderMaximum = self._findChild(
                                                    self._root,
                                                    'horizontalSlider_Maximum' )

    self.connect( self._horizontalSliderMaximum,
                  QtCore.SIGNAL( 'valueChanged(int)' ),
                  self.setPaletteMaximumSlot )

    ########################### button Ok/Cancel ###############################
    self._buttonBoxOkCancel = self._findChild( self._root,
                                               'buttonBox_OkCancel' )
    self.connect( self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok ),
                  QtCore.SIGNAL( 'clicked()' ),
                  self, 
                  QtCore.SLOT( 'accept()' ) )
    self.connect( self._buttonBoxOkCancel.button( \
                                                QtWidgets.QDialogButtonBox.Cancel ),
                  QtCore.SIGNAL( 'clicked()' ),
                  self,
                  QtCore.SLOT( 'reject()' ) )

    ########################### setting the layout #############################
    self.setLayout( self._layout )
    self.setWindowTitle( 'Color map selection' )

    self._listWidgetObjects.setCurrentRow( 0 )

    objectName = str( self._listWidgetObjects.currentItem().text() )
    nextObject = self._viewer.getObject( objectName )
    nextObjectInfos = nextObject.getInfos()
    
    if ( nextObjectInfos.has_key( 'palette' ) ) :
    
      colorMapName = nextObjectInfos[ 'palette' ][ 'palette' ]
      self._listWidgetColorMaps.setCurrentRow( \
                                         self._colorMapIndices[ colorMapName ] )

      self.setWindowModality( QtCore.Qt.WindowModal )

  def getObjectInformations( self, objectName ):
  
    theObject = self._viewer.getObject( objectName )
    objectInfos = theObject.getInfos()
    lowerBound = objectInfos[ 'texture' ][ 'textureMin' ]
    upperBound = objectInfos[ 'texture' ][ 'textureMax' ]
    minimum = objectInfos[ 'palette' ][ 'min' ]
    maximum = objectInfos[ 'palette' ][ 'max' ]
    colorMapName = objectInfos[ 'palette' ][ 'palette' ]

    delta = upperBound - lowerBound 
    absoluteMinimum = lowerBound + minimum * delta
    absoluteMaximum = lowerBound + maximum * delta
    
    print objectName, ':'
    print '  -> lower bound : ', lowerBound
    print '  -> upper bound : ', upperBound
    print '  -> absolute minimum : ', minimum
    print '  -> absolute maximum : ', maximum
    print '  -> colormap : ', colorMapName

    return lowerBound, upperBound, \
           minimum, maximum, \
           absoluteMinimum, absoluteMaximum, \
           colorMapName

  def selectObjectSlot( self, nextListWidgetItem, previousListWidgetItem ):
  
    if ( previousListWidgetItem is not None ):

      print 'previous object name : ', previousListWidgetItem.text()

    else:
    
      print 'previous object name : none'

    print 'next object name : ', nextListWidgetItem.text()

    objectName = str( nextListWidgetItem.text() )
    lowerBound, upperBound, \
    minimum, maximum, \
    absoluteMinimum, absoluteMaximum, \
    colorMapName = self.getObjectInformations( objectName )

    self._doubleSpinBoxLowerBound.setValue( lowerBound )
    self._doubleSpinBoxUpperBound.setValue( upperBound )

    self._horizontalSliderMinimum.setMinimum( 0 )
    self._horizontalSliderMaximum.setMinimum( 0 )

    self._horizontalSliderMinimum.setMaximum( 1000000 )
    self._horizontalSliderMaximum.setMaximum( 1000000 )

    self._horizontalSliderMinimum.setValue( minimum * 1000000 )
    self._horizontalSliderMaximum.setValue( maximum * 1000000 )

    self._listWidgetColorMaps.setCurrentRow( \
                                         self._colorMapIndices[ colorMapName ] )


  def selectColorMapSlot( self, nextListWidgetItem, previousListWidgetItem ):
  
    objectName = str( self._listWidgetObjects.currentItem().text() )
    colorMapName = str( nextListWidgetItem.text() )
    self._viewer.setColormap( objectName, colorMapName )


  def setPaletteMinimumSlot( self, value ):


    objectName = str( self._listWidgetObjects.currentItem().text() )
    lowerBound, upperBound, \
    minimum, maximum, \
    absoluteMinimum, absoluteMaximum, \
    colorMapName = self.getObjectInformations( objectName )

    newAbsoluteMinimum = lowerBound + \
              ( float( self._horizontalSliderMinimum.value() ) / 1000000.0 ) * \
              ( upperBound - lowerBound )

    self._viewer.setColormap( objectName, colorMapName,
                              newAbsoluteMinimum, absoluteMaximum )


  def setPaletteMaximumSlot( self, value ):

    objectName = str( self._listWidgetObjects.currentItem().text() )
    lowerBound, upperBound, \
    minimum, maximum, \
    absoluteMinimum, absoluteMaximum, \
    colorMapName = self.getObjectInformations( objectName )

    newAbsoluteMaximum = lowerBound + \
              ( float( self._horizontalSliderMaximum.value() ) / 1000000.0 ) * \
              ( upperBound - lowerBound )

    self._viewer.setColormap( objectName, colorMapName,
                              absoluteMinimum, newAbsoluteMaximum )


  def setLowerBoundSlot( self, value ):

    print 'setLowerBoundSlot : ', value


  def setUpperBoundSlot( self, value ):

    print 'setUpperBoundSlot : ', value
