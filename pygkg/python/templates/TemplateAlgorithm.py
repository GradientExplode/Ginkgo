from Algorithm import *
from TemplateTask import *


class TemplateAlgorithm( Algorithm ):

  def __init__( self, verbose ):

    Algorithm.__init__( self, 'Template', verbose, True )

    #######################
    # ADD PARAMETERS HERE #
    #######################
    pass

  def launch( self ):

    if ( self._verbose ):

      print 'running Template'

    task = TemplateTask( self._application )
    task.launch( False )

  def view( self ):

    if ( self._verbose ):

      print 'viewing Template'

    task = TemplateTask( self._application )
    task.launch( True )

