import os

from OptionParser import *
from StyleSheet import *
from TutorialToolBox import *
from TutorialPages import *

################################################################################
# main
################################################################################

pages = getPages()
pageName, parameterFileName, batchOptions = getArgs( pages )

# in case of batch mode
if ( pageName != None ):

  tutorialToolBox = TutorialToolBox( pages,
                                     True,
                                     batchOptions,
                                     verbose = True )
  tutorialToolBox.launch( pageName, parameterFileName, True )

# in case of graphical user interface mode
else:

  runqt = True
  if QtGui.qApp.startingUp():

   qapp = QtGui.QApplication( sys.argv )

   #set custom style
   qapp.setStyleSheet( getPyGkgStyleSheet() )

  else:

    runqt = False

  tutorialToolBox = TutorialToolBox( pages,
                                     False,
                                     batchOptions,
                                     verbose = True )
  tutorialToolBox.launch( None, None, False )
  tutorialToolBox.hideSplashScreen()
  
  if runqt:
  
    qapp.exec_()



