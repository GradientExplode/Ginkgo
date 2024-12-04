from AlgorithmGUI import *
from ResourceManager import *


class Tab1AlgorithmGUI( AlgorithmGUI ):

  def __init__( self, algorithm ):

    AlgorithmGUI.__init__( self,
                           algorithm,
                           ResourceManager().getUIFileName(
                                                 'tutorial',
                                                 'Tab1.ui' ) )


    ###########################################################################
    # connecting input data
    ###########################################################################

    self.connectStringParameterToLineEdit( 'fileNameInput',
                                           'lineEdit_FileNameInput' )
    self.connectFileOrDirectoryBrowserToPushButtonAndLineEdit(
                                                'fileNameInput',
                                                'pushButton_FileNameInput',
                                                'lineEdit_FileNameInput' )

    self.connectIntegerParameterToSpinBox( 'threshold', 
                                           'spinBox_Threshold' )


