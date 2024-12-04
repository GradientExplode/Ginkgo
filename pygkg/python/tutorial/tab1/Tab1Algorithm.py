from Algorithm import *
from Tab1Task import *


class Tab1Algorithm( Algorithm ):

  def __init__( self, verbose ):

    Algorithm.__init__( self, 'My-Tab1', verbose, True )

    # input data
    self.addParameter( StringParameter( 'fileNameInput', '' ) )
    self.addParameter( IntegerParameter( 'threshold', 1, 0, 1000, 1 ) )

  def launch( self ):

    if ( self._verbose ):

      print \
        'running Tab1'

    task = Tab1Task( self._application )
    task.launch( False )

  def view( self ):

    if ( self._verbose ):

      print \
        'viewing Tab1'

    task = Tab1Task( self._application )
    task.launch( True )

