from PyQt5 import QtCore, QtWidgets

class Frame( QtWidgets.QFrame ):


  clicked = QtCore.pyqtSignal( bool )


  def __init__( self, parent = None ):

    QtWidgets.QFrame.__init__( self, parent )


  def mouseReleaseEvent( self, event ):
    if ( event.button() == QtCore.Qt.LeftButton ):

      self.clicked.emit( True )
      
