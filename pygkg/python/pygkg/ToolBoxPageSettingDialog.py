from PyQt5 import QtCore, QtGui, QtWidgets, uic
from ResourceManager import *


class ToolBoxPageSettingDialog( QtWidgets.QDialog ):

  def __init__( self, applicationName, pages, selectedPages, parent = None ):

    QtWidgets.QDialog.__init__( self, parent )

    self._pages = pages
    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                             'core',
                                             'ToolBoxPageSettingDialog.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )
    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )

    settings = ResourceManager().readSettings( applicationName )
    
    self._selectedPages = selectedPages
    self._checkBoxes = list()

    layoutPages = QtWidgets.QVBoxLayout( self._findChild( self._root, 'widget_Pages' ) )

    for page in self._pages:

      checkBox = QtWidgets.QCheckBox( page[ 'pageName' ] )
      self._checkBoxes.append( checkBox )
      layoutPages.addWidget( checkBox )
      if( selectedPages.count( page[ 'pageName' ] ) > 0 ):

        checkBox.setChecked( True )

    self.setWindowTitle( 'Toolbox pages selection settings' )
    self.setWindowModality( QtCore.Qt.WindowModal )

    # ok and cancel buttons
    self._buttonBoxOkCancel = self._findChild( self._root,
                                               'buttonBox_OkCancel' )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                                   ).clicked.connect( self.accept )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                                   ).clicked.connect( self.reject )


  def getSettings( self ):

    selectedPageNames = list()

    for checkBox in self._checkBoxes:

      if ( checkBox.isChecked() ):

        selectedPageNames.append( str( checkBox.text() ) )

    settings = { 'toolbox_pages': selectedPageNames }
    return settings


  def exec_( self ):

    self.show()
    self.raise_()
    return QtWidgets.QDialog.exec_( self )
