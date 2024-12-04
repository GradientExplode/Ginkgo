from PyQt5 import QtCore, QtGui, QtWidgets, uic
from ResourceManager import *


class SpecySelectionDialog( QtWidgets.QDialog ):

  def __init__( self, applicationName, parent = None ):

    QtWidgets.QDialog.__init__( self, parent )

    # collecting default values
    settings = ResourceManager().readSettings( applicationName )
    self._specy = settings[ 'specy' ][ 0 ]
    self._acquisitionType = settings[ 'specy' ][ 1 ]
    
    print '----------------------->', self._acquisitionType
    
    initialSpecy = self._specy
    initialAcquisitionType = self._acquisitionType

    # reading species from disk ressources
    self._species = ResourceManager().getSpecies( applicationName )
    
    self._specyToIndexLut = {}
    index = int( 0 )
    for s in self._species:
    
      self._specyToIndexLut[ s ] = index
      index += 1
    
    # preparing icons for the various species
    self._pixmaps = {}
    for s in self._species:
    
      iconFileName = ResourceManager().getSpecyIconFileName( applicationName, \
                                                             s )
      self._pixmaps[ s ] = QtGui.QPixmap( iconFileName )
    
    #  reading acquisition types for all species from disk
    self._acquisitionTypes = {}
    self._acquisitionTypeToIndexLut = {}
    for s in self._species:

      acquisitionTypes = ResourceManager().getAcquisitionTypesOfSpecy( \
                                                               applicationName,
                                                               s )
      self._acquisitionTypes[ s ] = acquisitionTypes

      self._acquisitionTypeToIndexLut[ s ] = {}
      index = int( 0 )
      for t in acquisitionTypes:
      
        self._acquisitionTypeToIndexLut[ s ][ t ] = index
        index += 1


    # getting access to the root dialog widget
    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                                  'core',
                                                  'SpecySelectionDialog.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )
    # putting th widget into a layout
    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )

    # setting window title
    self.setWindowTitle( 'Selection of specy' )
    self.setWindowModality( QtCore.Qt.WindowModal )

    # getting access to the specy combo box
    self._comboBoxSpecy = self._findChild( self._root, 'comboBox_Specy' )
    for s in self._species:

      self._comboBoxSpecy.addItem( s )

    # getting access to the acquisition type combo box
    self._comboBoxAcquisitionType = self._findChild( \
                                                   self._root, \
                                                   'comboBox_AcquisitionType' )

    # initializing combo box from current specy
    for t in self._acquisitionTypes[ self._specy ]:
    
      self._comboBoxAcquisitionType.addItem( t )
      
    # preparing the icon of the specy and filling it with the current specy
    self._labelSpecyIcon = self._findChild( self._root, 'label_SpecyIcon' )
    self._labelSpecyIcon.setAlignment( QtCore.Qt.AlignHCenter | \
                                       QtCore.Qt.AlignTop )
    self._labelSpecyIcon.setPixmap( self._pixmaps[ self._specy ] )
    

    # connecting combo boxes to slot(s)
    self._comboBoxSpecy.currentIndexChanged.connect( self.setSpecy )
    self._comboBoxAcquisitionType.currentIndexChanged.connect(
                                                       self.setAcquisitionType )

    # connecting ok and cancel buttons
    self._buttonBoxOkCancel = self._findChild( self._root,
                                               'buttonBox_OkCancel' )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                                   ).clicked.connect( self.accept )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                                   ).clicked.connect( self.reject )

    self._comboBoxSpecy.setCurrentIndex( self._specyToIndexLut[ initialSpecy ] )
    self._comboBoxAcquisitionType.setCurrentIndex( \
                     self._acquisitionTypeToIndexLut[ initialSpecy ]\
                                                    [ initialAcquisitionType ] )


  def setSpecy( self, value ):

    self._specy = self._species[ value ]

    # updating specy icon accordingly    
    self._labelSpecyIcon.setPixmap( self._pixmaps[ self._specy ] )

    # removing old items of the  acquisition type combo box
    while ( self._comboBoxAcquisitionType.count() ):
    
      self._comboBoxAcquisitionType.removeItem( 0 )
    
    # updating acquisition type combo box accordingly
    for t in self._acquisitionTypes[ self._specy ]:
    
      self._comboBoxAcquisitionType.addItem( t )


  def setAcquisitionType( self, value ):

    self._acquisitionType = str( value )

      
  def getSpecy( self ):
  
    return self._specy
    
    
  def getAcquisitionType( self ):
  
    return self._acquisitionType
      
 
  def exec_( self ):

    self.show()
    self.raise_()
    return QtWidgets.QDialog.exec_( self )
                                                                
                                                                

    
