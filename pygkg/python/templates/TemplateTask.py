from Task import *
import os

class TemplateTask( Task ):

  def __init__( self, application ):

    Task.__init__( self, application )

  ##############################################################################
  # execute()
  ##############################################################################

  def execute( self, functors, parameters, subjectName, verbose, viewOnly ):

    try:

      ####################
      # ADD PROCESS HERE #
      ####################
      pass

    except:

      self._functors[ 'task-set-status-as-failed' ]( subjectName,
                                                     sys.exc_info() )

  ##############################################################################
  # display()
  ##############################################################################



  def display( self, functors, parameters, subjectName, verbose,
               isReportModeActive ):


    ####################
    # ADD DISPLAY HERE #
    ####################
    pass

