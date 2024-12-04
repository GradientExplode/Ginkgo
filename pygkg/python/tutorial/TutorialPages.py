from Tab1Algorithm import *
from Tab1AlgorithmGUI import *
from Tab1Viewer import *


def getPages():

    return \
      [ { 'My class of algorithm' :
          [ { 'algorithm': Tab1Algorithm,
              'algorithmGUI': Tab1AlgorithmGUI,
              'createViewer': Tab1Viewer,
              'tabName' : 'My Tab1',
              'pageName' : 'My-Tab1' } ] }
      ]
