from Viewer import *


class TemplateViewer( Viewer ):

  def __init__( self, minimumSize, parent ):

    Viewer.__init__( self, minimumSize, parent )

    ###############################
    # ADD WINDOWS AND GRAPHS HERE #
    ###############################

def createTemplateViewer( minimumSize, parent ):

  return TemplateViewer( minimumSize, parent )

