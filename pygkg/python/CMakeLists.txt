################################################################################
# SOURCES
################################################################################

set( SOURCES
pygkg/AbortableThread.py
pygkg/AdvancedParameter.py
pygkg/AdvancedParameterWidget.py
pygkg/AlgorithmGUI.py
pygkg/Algorithm.py
pygkg/Application.py
pygkg/ColorMapDialog.py
pygkg/ContainerInformationHandler.py
pygkg/DirectoryOrFileListDialog.py
pygkg/Frame.py
pygkg/MainThreadActionManager.py
pygkg/MultipleTabItem.py
pygkg/MultipleTabWindow.py
pygkg/MultiSubjectSelectionDialog.py
pygkg/MultiSubjectSettingsDialog.py
pygkg/OptionParser.py
pygkg/Parameter.py
pygkg/PathHandler.py
pygkg/PdfViewerWidget.py
pygkg/ProcessListParameter.py
pygkg/ProcessListWidgetItem.py
pygkg/ProcessListWidget.py
pygkg/ProcessProgressBar.py
pygkg/RegistrationParameter.py
pygkg/ResourceManager.py
pygkg/Rotation3dFunctions.py
pygkg/Singleton.py
pygkg/SnapshotTool.py
pygkg/SpecySelectionDialog.py
pygkg/StyleSheet.py
pygkg/SubjectNameDialog.py
pygkg/Task.py
pygkg/ToolBoxPageSettingDialog.py
pygkg/ToolBox.py
pygkg/ViewerCallbackManager.py
pygkg/Viewer.py
pygkg/VolumeInformationHandler.py
pygkg/WidgetWithOverlay.py
pygkg/XMLResourceManager.py
templates/TemplateAlgorithmGUI.py
templates/TemplateAlgorithm.py
templates/TemplateTask.py
templates/TemplateViewer.py
tutorial/TutorialPages.py
tutorial/TutorialToolBox.py
tutorial/tab1/Tab1AlgorithmGUI.py
tutorial/tab1/Tab1Algorithm.py
tutorial/tab1/Tab1Task.py
tutorial/tab1/Tab1Viewer.py
)


################################################################################
# Installation
################################################################################

if( NOT NUITKA )
  get_directories( SRC_DIR_LIST SOURCES )
  foreach( d ${SRC_DIR_LIST} )
    install( DIRECTORY ${d}
             DESTINATION share/${PROJECT_NAME}/python
             COMPONENT runtime )
  endforeach()
endif( NOT NUITKA )
