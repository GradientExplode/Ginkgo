from ToolBox import *

from Tab1Algorithm import *
from Tab1AlgorithmGUI import *
from Tab1Viewer import *


class TutorialToolBox( ToolBox ):

  def __init__( self,
                pages,
                batchMode,
                batchOptions,
                verbose = False,
                parent = None ):
  
    ToolBox.__init__( \
                 self,
                 'tutorial',
                 pages,
                 '<b>tutorial</b> toolbox was developed by me and my monkey',
                 batchMode,
                 batchOptions,
                 verbose,
                 parent )

