from AlgorithmGUI import *
from ResourceManager import *


class TemplateAlgorithmGUI( AlgorithmGUI ):

  def __init__( self, algorithm ):

    AlgorithmGUI.__init__( self,
                           algorithm,
                           ResourceManager().getUIFileName(
                                                 'template',
                                                 'Template.ui' ) )

    ################################
    # CONNECT PARAMETERS TO WIDGET #
    ################################

