from Task import *
import os

class Tab1Task( Task ):

  def __init__( self, application ):

    Task.__init__( self, application )

  ##############################################################################
  # execute()
  ##############################################################################

  def execute( self, functors, parameters, subjectName, verbose, viewOnly ):

    try:

      functors[ 'condition-acquire' ]( subjectName, 'threshold-processed' )

      functors[ 'update-progress-bar' ]( subjectName, 0 )

      fileNameInput = parameters[ 'fileNameInput' ].getValue()
      outputWorkDirectory = parameters[ 'outputWorkDirectory' ].getValue()

      threshold = parameters[ 'threshold' ].getValue()

      command = 'GkgBinarizer' + \
                  ' -i ' + fileNameInput + \
                  ' -o ' + os.path.join( outputWorkDirectory, 'output.ima' ) + \
                  ' -m gt ' + \
                  ' -t1 ' + str( threshold )
      executeCommand( self, subjectName, command, viewOnly )

      functors[ 'condition-notify-and-release' ]( subjectName,
                                                 'threshold-processed' )
      functors[ 'update-progress-bar' ]( subjectName, 100 )


    except:

      self._functors[ 'task-set-status-as-failed' ]( subjectName,
                                                     sys.exc_info() )

  ##############################################################################
  # display()
  ##############################################################################


  def display( self, functors, parameters, subjectName, verbose,
               isReportModeActive ):

    outputWorkDirectory = parameters[ 'outputWorkDirectory' ].getValue()
    fileNameInput = parameters[ 'fileNameInput' ].getValue()

    view = 'first_view'
    functors[ 'viewer-load-object' ]( fileNameInput,
                                      'fileNameInput' )
    functors[ 'viewer-add-object-to-window' ]( 'fileNameInput',
                                               view,
                                               'input file' )

    functors[ 'condition-wait-and-release' ]( subjectName,
                                              'threshold-processed' )

    fileNameOutput = os.path.join( outputWorkDirectory, 'output.ima' )
    functors[ 'viewer-load-object' ]( fileNameOutput,
                                      'fileNameOutput' )
    functors[ 'viewer-add-object-to-window' ]( 'fileNameOutput',
                                               view,
                                               'output file' )

