import PyQt5
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from Algorithm import *
from ResourceManager import *

class DirectoryOrFileListDialog( QtWidgets.QDialog ):

  def __init__( self,
                parent = None,
                previousSelection = '',
                fileSelectionEnabled = True,
                directorySelectionEnabled = True,
                multiSelection = False ):


    QtWidgets.QDialog.__init__( self, parent )

    self._fileSelectionEnabled = fileSelectionEnabled
    self._directorySelectionEnabled = directorySelectionEnabled
    self._multiSelection = multiSelection
    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                                  'core',
                                                  'DirectoryOrFileDialog.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )

    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )

    self._pushButtonBrowseDirectories = self._findChild( \
                                                self._root,
                                                'pushButton_BrowseDirectories' )
    self._pushButtonBrowseFiles = self._findChild( self._root,
                                                   'pushButton_BrowseFiles' )
    self._pushButtonAdd = self._findChild( self._root, 'pushButton_Add' )
    self._lineEditDirectoryOrFileName = self._findChild( \
                                                self._root,
                                                'lineEdit_DirectoryOrFileName' )

    self._listWidgetDirectoryOrFileList = self._findChild( \
                                              self._root,
                                              'listWidget_DirectoryOrFileList' )
    self._listWidgetDirectoryOrFileList.setSelectionMode( \
                                   QtWidgets.QAbstractItemView.ContiguousSelection )

    previousSelectionList = previousSelection.split( ';' )
    for directoryOrFileName in previousSelectionList:

      if ( directoryOrFileName != ''):

        self._listWidgetDirectoryOrFileList.addItem( directoryOrFileName )

    if ( self._fileSelectionEnabled == False ):
    
      self._pushButtonBrowseFiles.hide()

    if ( self._directorySelectionEnabled == False ):
    
      self._pushButtonBrowseDirectories.hide()

    self._toolButtonGoUp = self._findChild( self._root,
                                            'toolButton_GoUp' )
    self._toolButtonGoDown = self._findChild( self._root,
                                              'toolButton_GoDown' )
    self._toolButtonTrash = self._findChild( self._root,
                                             'toolButton_Trash' )

    self._toolButtonGoUp.setIcon( QtGui.QIcon(
                  ResourceManager().getIconFileName( 'icon-go-up.png' ) ) )
    self._toolButtonGoDown.setIcon( QtGui.QIcon(
                  ResourceManager().getIconFileName( 'icon-go-down.png' ) ) )
    self._toolButtonTrash.setIcon( QtGui.QIcon(
                  ResourceManager().getIconFileName( 'icon-user-trash.png' ) ) )

    self._buttonBoxOkCancel = self._findChild( self._root,
                                               'buttonBox_OkCancel' )

    self._fileDialog = QtWidgets.QFileDialog()

    if self._multiSelection:

      self._directoryDialog = DirectoryDialog()
      
    else:

      self._directoryDialog = QtWidgets.QFileDialog()

    self._pushButtonBrowseFiles.clicked.connect(
              lambda x: self.fileNameChooser( 'lineEdit_DirectoryOrFileName' ) )

    self._pushButtonBrowseDirectories.clicked.connect(
         lambda x: self.directoryNameChooser( 'lineEdit_DirectoryOrFileName' ) )

    self._pushButtonAdd.clicked.connect( self.addItem )
    self._toolButtonGoUp.clicked.connect( self.goUp )
    self._toolButtonGoDown.clicked.connect(  self.goDown )
    self._toolButtonTrash.clicked.connect( self.trash )

    self.setLayout( self._layout )

    if ( ( self._fileSelectionEnabled == True ) and \
         ( self._directorySelectionEnabled == True ) ):

      self.setWindowTitle( 'File and/or directory selection' )

    elif ( ( self._fileSelectionEnabled == True ) and \
           ( self._directorySelectionEnabled == False ) ):

      self.setWindowTitle( 'File selection' )

    elif ( ( self._fileSelectionEnabled == False ) and \
           ( self._directorySelectionEnabled == True ) ):

      self.setWindowTitle( 'Directory selection' )

    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                                   ).clicked.connect( self.accept )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                                   ).clicked.connect( self.reject )

    self.setWindowModality( QtCore.Qt.WindowModal )

  def fileNameChooser( self, lineEditWidgetName ):

    if ( self._multiSelection == True ):

       self._fileDialog.setFileMode( self._fileDialog.ExistingFiles )

    else:

      self._fileDialog.setFileMode( self._fileDialog.ExistingFile )
    self._fileDialog.show()
    result = self._fileDialog.exec_()
    if result:

      if ( self._multiSelection == True ):

        for fileName in self._fileDialog.selectedFiles() :

          self._listWidgetDirectoryOrFileList.addItem( fileName )
          
      else:

        self._listWidgetDirectoryOrFileList.clear()
        self._listWidgetDirectoryOrFileList.addItem(  
                                         self._fileDialog.selectedFiles()[ 0 ] )

  def directoryNameChooser( self, lineEditWidgetName ):

    if ( self._multiSelection == True ):

       self._directoryDialog.setFileMode( self._directoryDialog.ExistingFiles )
       self._directoryDialog.setOption(
                                     self._directoryDialog.DontUseNativeDialog )
       self._directoryDialog.setOption( self._directoryDialog.ShowDirsOnly )
       
    else:

      self._directoryDialog.setFileMode( self._directoryDialog.DirectoryOnly )
      
    self._directoryDialog.show()
    result = self._directoryDialog.exec_()
    if self._multiSelection == False and result:

      self._listWidgetDirectoryOrFileList.clear()
      directoryName = unicode( self._directoryDialog.selectedFiles()[ 0 ] )
      self._listWidgetDirectoryOrFileList.addItem( directoryName )

    if self._multiSelection and self._directoryDialog.result :

      selectedDirectories = self._directoryDialog.selectedFiles()
      self._directoryDialog.result = False
      for i in xrange( len( selectedDirectories ) ):

        self._listWidgetDirectoryOrFileList.addItem( selectedDirectories[ i ] )

  def getSelection( self ):
  
    selection = list()
    count = self._listWidgetDirectoryOrFileList.count()
    for row in range( 0, count ):
    
      selection.append( self._listWidgetDirectoryOrFileList.item( row ).text() )

    return selection

  def addItem( self ):
  
    name = self._lineEditDirectoryOrFileName.text()
    if ( len( name ) ):

      self._listWidgetDirectoryOrFileList.addItem( name )
      self._lineEditDirectoryOrFileName.setText( '' )

  def getOrderedSelectedItems( self ):

    selectedItems = self._listWidgetDirectoryOrFileList.selectedItems()
    if ( len( selectedItems ) ):
    
    
      orderedSelectedItems = dict()
      for item in selectedItems:
      
        index = self._listWidgetDirectoryOrFileList.row( item )
        orderedSelectedItems[ index ] = item.text()
        
      return orderedSelectedItems
      
    else:
    
      return None

  def removeItems( self, selectedItems ):
  
    if ( selectedItems is not None ):
    
      keys = selectedItems.keys()
      for key in keys[ :: -1 ]:

        item = self._listWidgetDirectoryOrFileList.takeItem( key )
        del item

  def goUp( self ):
  
    orderedSelectedItems = self.getOrderedSelectedItems()
    if ( orderedSelectedItems is not None ):
    
      keys = orderedSelectedItems.keys()
      if ( keys[ 0 ] > 0 ):

        self.removeItems( orderedSelectedItems )
        for key in keys:

          self._listWidgetDirectoryOrFileList.insertItem( \
                                                   key - 1,
                                                   orderedSelectedItems[ key ] )
          self._listWidgetDirectoryOrFileList.item( \
                                                   key - 1 ).setSelected( True )

  def goDown( self ):
  
    orderedSelectedItems = self.getOrderedSelectedItems()
    if ( orderedSelectedItems is not None ):
    
      count = self._listWidgetDirectoryOrFileList.count()
      keys = orderedSelectedItems.keys()
      if ( keys[ -1 ] < count - 1 ):

        self.removeItems( orderedSelectedItems )
        for key in keys:

          self._listWidgetDirectoryOrFileList.insertItem( \
                                                   key + 1,
                                                   orderedSelectedItems[ key ] )
          self._listWidgetDirectoryOrFileList.item( \
                                                   key + 1 ).setSelected( True )

  def trash( self ):
  
    orderedSelectedItems = self.getOrderedSelectedItems()
    if ( orderedSelectedItems is not None ):

      self.removeItems( orderedSelectedItems )

  def exec_( self ):

    self.show()
    self.raise_()
    return QtWidgets.QDialog.exec_( self )


class DirectoryDialog( QtWidgets.QFileDialog ):

  def __init__( self ):

    QtWidgets.QFileDialog.__init__( self )

  def accept( self ):

    # we redefine the accept slot just to allow multiselection of directories
    self.close()
