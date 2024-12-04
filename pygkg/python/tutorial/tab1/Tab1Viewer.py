from Viewer import *


class Tab1Viewer( Viewer ):

  def __init__( self, minimumSize, parent ):

    Viewer.__init__( self, minimumSize, parent )

    ########################## First View ######################################

    self.createView( 'first_view' )

    # input file
    self.addAxialWindow( 'first_view', 0, 0, 1, 1,
                         'input file', 'input file',
                         [ 0.0, 0.0, 0.0, 1.0 ], True )

    # output file
    self.addAxialWindow( 'first_view', 0, 1, 1, 1,
                         'output file', 'output file',
                         [ 0.0, 0.0, 0.0, 1.0 ], True )

def createTab1Viewer( minimumSize, parent ):

  return Tab1Viewer( minimumSize, parent )

