import sys
from PyQt5 import QtGui, QtWidgets, QtCore
#import poppler



class PdfViewerWidget( QtWidgets.QWidget ):

  def __init__( self, parent = None ):

    super( PdfViewerWidget, self ).__init__( parent )
    
    self._label = QtWidgets.QLabel( self )
    #self._label.setMinimumSize( 400, 800 )

   
    self._comboBox = QtWidgets.QComboBox( self )
                               
    self._vBoxLayout = QtWidgets.QVBoxLayout( self )
    self._vBoxLayout.addWidget( self._label )
    self._vBoxLayout.addWidget( self._comboBox )

    self._comboBox.currentIndexChanged.connect( self.onCurrentIndexChanged )

    self._images = []


  def load( self, fileNamePdf ):
  
    #self._document = popplerqt5.Poppler.Document.load( fileNamePdf )

    #self._document.setRenderHint( \
    #                              popplerqt5.Poppler.Document.Antialiasing and 
    #                              popplerqt5.Poppler.Document.TextAntialiasing )
    #self._comboBox.addItems( [ "Page {}".format( i + 1 ) for i in \
    #                           range( self._document.numPages() ) ] )

    self.initImages()

    #self._label.setPixmap( QtGui.QPixmap.fromImage( self._images[ 0 ] ) )

 
  def onCurrentIndexChanged( self, index ):

    if ( ( len( self._images ) > 0 ) and ( index < len( self._images ) ) ):
  
      self._label.setPixmap( QtGui.QPixmap.fromImage( self._images[ index ] ) )
 
 
  def initImages( self ):

    self._images = []
    
    #for page in range( self._document.numPages() ):
    
    #  self._images.append( self._document.page( page ).renderToImage() )
 
