from Parameter import *
from ProcessListParameter import *
from AdvancedParameter import *
from RegistrationParameter import *
from ResourceManager import *
import shutil
import subprocess
import signal
import re
import json



################################################################################
# getChildrenPids()
################################################################################

def getChildrenPids( pid ):

    proc = subprocess.Popen( 'ps -o pid,ppid ax | grep "%d"' % pid,
                             shell = True, stdout = subprocess.PIPE )
    pidppid = [ x.split() for x in proc.communicate()[ 0 ].split( '\n' ) if x ]
    return list( int( p ) for p, pp in pidppid if int( pp ) == pid )




################################################################################
# Algorithm
################################################################################

class Algorithm:

  def __init__( self, name, verbose, multiSubject = False ):

    self._name = name
    self._parameters = {}
    self._application = None
    self._verbose = verbose
    self._multiSubject = multiSubject

    self._multiSubjectSelection = None

    self._isReportModeActive = False

    # adding default output directory parameter
    self.addParameter( StringParameter( 'outputWorkDirectory',
                                        '',
                                        True,
                                        True ) )

    if ( self._multiSubject == True ):

      self.addParameter( StringParameter( '_subjectName',
                                          '',
                                          False,
                                          False ) )

    # the id of the launch thread / execute thread(s) / system pid(s)
    # to be able to abort them
    self._launchThread = None
    self._executeThreads = {}
    self._systemPids = {}


  def getName( self ):
  
    return self._name
 
  def addParameter( self, parameter ):
  
    if ( self._parameters.has_key( parameter.getName() ) ):
    
      message = '\'' + parameter.getName() + '\' parameter already present'
      raise RuntimeError, message
      
    self._parameters[ parameter.getName() ] = parameter

  def getParameter( self, name ):
  
    if ( not self._parameters.has_key( name ) ):
      
      message = '\'' + name + '\' parameter not found'
      raise RuntimeError, message

    return self._parameters[ name ]
  
  def getParameterKeys( self ):
  
    return self._parameters.keys()

  def getParameters( self ):
  
    return self._parameters

  def setParameters( self, parameters ):
  
    for n in parameters.keys():

      self.getParameter( n ).writeValue( parameters[ n ].getValue() )

  def setParameterValues( self, parameterValues ):
  
    for n in parameterValues:

      # print n
      self.getParameter( n ).writeValue( parameterValues[ n ] )

  def displayParameters( self ):
  
    for p in self._parameters.values():

      p.display()

  def readParameters( self, fileName ):

    if ( fileName is None ):
    
      message = 'filename is none'
      raise RuntimeError, message
    
    if ( not os.path.exists( fileName ) ):
  
      message = '\'' + fileName + '\' file not found'
      raise RuntimeError, message

    try:

      parameterValues = dict()
      with open( fileName, 'r' ) as f:

        parameterValues = json.load( f )
  
      if ( parameterValues.has_key( '_algorithmName' ) ):
    
        del parameterValues[ '_algorithmName' ]
  
      self.setParameterValues( parameterValues )
      self._multiSubjectSelection = self.getMultiSubjectSelection()

    except:
    
      globalVariables = dict()
      localVariables = dict()
      execfile( fileName, globalVariables, localVariables )
      self.setParameterValues( localVariables[ 'parameterValues' ] )
      self._multiSubjectSelection = self.getMultiSubjectSelection()

  def writeParameters( self, fileName ):

    print 'saving parameters in ', fileName
    parameterValues = dict()
    parameterValues[ '_algorithmName' ] = self._name
    
    for key in self.getParameterKeys():
    
      parameterValues[ key ] = self.getParameter( key ).readValue()
    
    with open( fileName, 'w' ) as f:

      json.dump( parameterValues, f, 
                 sort_keys=True, indent=2, separators=( ',', ' : ' ) )

  def resetParameters( self ):

    print 'resetting parameters for algorithm', self._name
    for key in self.getParameterKeys():
    
      parameter = self.getParameter( key )
      parameter.setValue( parameter.getDefaultValue() )

  def addApplication( self, application ):
  
    if ( self._application is not None ):
    
      message = 'application already present'
      raise RuntimeError, message
      
    self._application = application
    
  def getApplication( self ):
  
    return self._application

  def getVerbose( self ):
  
    return self._verbose

  def launch( self ):
  
    message = 'not implemented'
    raise RuntimeError, message
 
  def view( self ):
  
    message = 'not implemented'
    raise RuntimeError, message
 
  def setLaunchThread( self, launchThread ):

    self._launchThread = launchThread

  def addExecuteThread( self, subjectName, executeThread ):

    self._executeThreads[ subjectName ] = executeThread

  def removeExecuteThread( self, subjectName ):

    del self._executeThreads[ subjectName ]

  def hasExecuteThread( self, subjectName ):

    return self._executeThreads.has_key( subjectName )

  def addSystemPids( self, subjectName, pid ):
  
    self._systemPids[ subjectName ] = pid

  def removeSystemPids( self, subjectName ):
  
    del self._systemPids[ subjectName ]

  def abort( self ):

    if ( self._launchThread.isAlive() ):

      # looping of execute thread(s)
      for subjectName in self._executeThreads.keys():

        # first killing PID(s) execute through system calls
        if ( self._systemPids.has_key( subjectName ) ):

          pid = self._systemPids[ subjectName ]
          childrenPids = getChildrenPids( pid )

          for p in childrenPids:

            print '-----> killing child PID', p, 'for ', subjectName
            os.kill( p, signal.SIGKILL )

          print '-----> killing PID', pid, 'for ', subjectName
          os.kill( pid, signal.SIGKILL )

          killedPid, stat = os.waitpid( pid, os.WNOHANG )
          #if ( killedPid == 0 ):
          #  print >> sys.stderr, "PROCESS not KILLED!"

          del self._systemPids[ subjectName ]

        # then, aborting the execute thread for the given subject
        self._executeThreads[ subjectName ].terminate()
        self.removeExecuteThread( subjectName )
     

      # then aborting the launch thread
      self._launchThread.terminate()

    self._application.getParentToolBox().addLog( 'work aborted' )


  def getMultiSubjectSelection( self ):

    multiSubjectSelection = None
    for n in self._parameters:

      parameter = self.getParameter( n )
      if ( parameter.getType() == 'string' and parameter.getValue() != '' ):

        value = ''
        if ( parameter.getWidgetType() == 'listWidget' ):

          value = parameter.getValue().split()[ 0 ]

        else:

          value = parameter.getValue().split( ';' )[ 0 ]

        reString = '{(.+)}'
        for x in xrange( value.count( '{' ) - 1 ):

          reString += '(.+){(.+)}'

        pattern = re.compile( reString )
        match = pattern.search( value )
        if ( match != None ):

          multiSubjectSelection = '{' + match.group( 1 ) + '}'
          return multiSubjectSelection

    return multiSubjectSelection


  def getMultiSubjectSelectionValue( self ):

    return self._multiSubjectSelection


  def setMultiSubjectSelectionValue( self, value ):

    self._multiSubjectSelection = str( value )


  def hasParameter( self, name ):

    if ( self._parameters.has_key( name ) ):

      return True

    else:

      return False


  def removeParameter( self, name ):

    if ( self.hasParameter( name ) ):

      del self._parameters[ name ]
      print 'remove', name


  def setReportMode( self, value ):

    if ( value == 0 ):

      self._isReportModeActive = False

    elif ( value == 2 ):

      self._isReportModeActive = True


  def isReportModeActive( self ):

    return self._isReportModeActive
    
    
  def tuneToSpecy( self, applicationName ):
  
    specySettings = ResourceManager().getSpecySettings( applicationName,
                                                        self._name )
                                                        
    for parameterName in specySettings.keys():
    
      if ( self._parameters.has_key( parameterName ) ):
      
        self._parameters[ parameterName ].tuneToSpecy( \
                                                specySettings[ parameterName ] )
