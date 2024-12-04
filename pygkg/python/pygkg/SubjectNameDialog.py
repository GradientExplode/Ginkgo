from PyQt5 import QtCore, QtGui, QtWidgets, uic
from ResourceManager import *


class SubjectNameDialog( QtWidgets.QDialog ):

  def __init__( self, parent = None, subjectName = 'single-subject' ):


    QtWidgets.QDialog.__init__( self, parent  )

    self._subjectName = subjectName

    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                                     'core',
                                                     'SubjectNameDialog.ui' ) )

    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )

    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )
    lineEditSubjectName = self._findChild( self._root, 'lineEdit_SubjectName' )
    lineEditSubjectName.textChanged.connect( self.setSubjectName )
    buttonBoxOkCancel = self._findChild( self._root, 'buttonBox_OkCancel' )
    buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                             ).clicked.connect( self.accept )
    buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                             ).clicked.connect( self.reject )

  def setSubjectName( self, subjectName ):

    self._subjectName = subjectName

  def getSubjectName( self ):

    return str( self._subjectName )
