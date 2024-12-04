import os
import time
from Singleton import *
import json


class ResourceManager( object, metaclass = Singleton ):
  

  def getSharePath( self ):
  
    sharePath = os.path.join( os.sep, 'usr', 'share', 'gkg' )
    envGkgSharePath = os.getenv( 'GKG_SHARE_PATH' )
    if ( envGkgSharePath is not None ) :
    
      if ( os.path.exists( envGkgSharePath ) ):
    
        sharePath = os.path.join( envGkgSharePath, 'gkg' )
    return sharePath


  def getUIFileName( self, applicationName, uiName ):

    return os.path.join( self.getSharePath(),
                         'ui',
                         applicationName,
                         uiName )


  def getIconFileName( self, iconName ):
      
    return os.path.join( self.getSharePath(),
                         'icons',
                         iconName )


  def getXMLFileName( self, applicationName, xmlFileName ):

    return os.path.join( self.getSharePath(),
                         'xml',
                         applicationName,
                         xmlFileName )


  def getDataDirectory( self ):
  
    return os.path.join( self.getSharePath(),
                         'data' )


  def getTemplatesPath( self, specy, fileName ):
  
    return os.path.join( self.getDataDirectory(),
                         specy,
                         'templates',
                         fileName )


  def getSpeciesDirectory( self, applicationName ):
  
    return os.path.join( self.getDataDirectory(), applicationName, 'species' )
    

  def getSpecies( self, applicationName ):
  
    return os.listdir( self.getSpeciesDirectory( applicationName ) )
  

  def getSpecyAcquisitionTypesDirectory( self, applicationName, specy ):
  
    return os.path.join( self.getSpeciesDirectory( applicationName ), specy )
    

  def getAcquisitionTypesOfSpecy( self, applicationName, specy ):
  
    items = os.listdir( self.getSpecyAcquisitionTypesDirectory( \
                                                              applicationName, \
                                                              specy ) )
    acquisitionTypes = []
    for i in items:
    
      if ( len( i ) >= 4 ):
      
        if ( i[ -4: ] != '.png' ):
        
          acquisitionTypes.append( i )
          
      else:
      
        acquisitionTypes.append( i )

    return acquisitionTypes


  def getSpecyIconFileName( self, applicationName, specy ):
      
    return os.path.join( self.getSpecyAcquisitionTypesDirectory( \
                             applicationName, specy ),
                         specy + '.png' )


  def getCurrentSpecy(  self, applicationName ):

    settings = ResourceManager().readSettings( applicationName )
    specy = settings[ 'specy' ][ 0 ]
  
    return specy
    

  def getSpecySettings( self, applicationName, algorithmName ):
  
    settings = ResourceManager().readSettings( applicationName )
    specy = settings[ 'specy' ][ 0 ]
    acquisitionType = settings[ 'specy' ][ 1 ]

    fileName = os.path.join( self.getSpecyAcquisitionTypesDirectory(
                                              applicationName, specy ),
                             acquisitionType,
                             algorithmName + '-Settings.py' )

    specySettings = {}
    if ( os.path.exists( fileName ) ):

      globalVariables = dict()
      localVariables = dict()
      execfile( fileName, globalVariables, localVariables )
      specySettings = localVariables[ 'specySettings' ]

    return specySettings


  def getApplicationDirectory( self, applicationName ):
  
    applicationDirectory = os.path.join( os.getenv( 'HOME' ),
                                                         '.' + applicationName )
    if not os.path.exists( applicationDirectory ):

      os.mkdir( applicationDirectory )
      
    return applicationDirectory
    

  def getApplicationTempDirectory( self, applicationName ):
  
    applicationTempDirectory = os.path.join( os.getenv( 'HOME' ),
                                             '.' + applicationName,
                                             'temp' )
    if not os.path.exists( applicationTempDirectory ):

      os.mkdir( applicationTempDirectory )
      
    return applicationTempDirectory
    

  def getSettingsFileName( self, applicationName ):

    applicationDirectory = self.getApplicationDirectory( applicationName )
    
    return os.path.join( applicationDirectory, applicationName + 'rc' )


  def readSettings( self, applicationName ):

    settings = {}
    settingsFileName = self.getSettingsFileName( applicationName )
    if os.path.exists( settingsFileName ):

      globalVariables = dict()
      localVariables = dict()
      execfile( settingsFileName, globalVariables, localVariables )

      if not localVariables.has_key( 'attributes' ):

         message = applicationName + 'rc has no key attributes'
         RuntimeError, message

      ##########################################################################
      # display of toolbox tabs
      ##########################################################################

      if localVariables[ 'attributes' ].has_key( 'toolbox_pages' ):

        settings[ 'toolbox_pages' ] = \
                               localVariables[ 'attributes' ][ 'toolbox_pages' ]

      ##########################################################################
      # distribution strategy
      ##########################################################################

      if not localVariables[ 'attributes' ].has_key( 'distribution_strategy' ):

         settings[ 'distribution_strategy' ] = 'single-threading'

      else:

         settings[ 'distribution_strategy' ] = \
                       localVariables[ 'attributes' ][ 'distribution_strategy' ]

      if settings[ 'distribution_strategy' ] == 'multiple-threading':

        if localVariables[ 'attributes' ].has_key( 'thread_count' ):

          settings[ 'thread_count' ] = \
                                localVariables[ 'attributes' ][ 'thread_count' ]

        else:

          settings[ 'thread_count' ] = os.sysconf( 'SC_NPROCESSORS_CONF' ) - 1
          if ( settings[ 'thread_count' ] == 0 ):
          
            settings[ 'thread_count' ] = 1


      ##########################################################################
      # specy
      ##########################################################################

      if not localVariables[ 'attributes' ].has_key( 'specy' ):

         settings[ 'specy' ] = [ 'Human', 'InVivo' ]

      else:

         settings[ 'specy' ] = \
                       localVariables[ 'attributes' ][ 'specy' ]
    else:

      settings[ 'distribution_strategy' ] = 'single-threading'
      settings[ 'thread_count' ] = 1
      settings[ 'specy' ] = [ 'Human', 'InVivo' ]

    return settings


  def writeSettings( self, applicationName, settings ):

    actualSettings = self.readSettings( applicationName )
    
    for s in settings.keys():
    
      actualSettings[ s ] = settings[ s ]

    settingsFileName = self.getSettingsFileName( applicationName )
    of = open( settingsFileName, 'w' )
    of.write( 'attributes = ' )
    of.write( json.dumps( actualSettings, sort_keys=True, indent=2,
                          separators=( ',', ' : ' ) ) )
    of.close()


  def getLogFileName( self, applicationName ):

    applicationDirectory = self.getApplicationDirectory( applicationName )
    sessionNumber = str( int( time.time() ) )
    logFileName = os.path.join( applicationDirectory, 'log' + sessionNumber )

    return logFileName


  def addLog( self, logFileName, logText ):

    of = open( logFileName, 'a' )
    of.write( logText + ' (' + str( time.ctime() ) + ')\n' )
    of.close()


  def clearOldLog( self, applicationName ):

    applicationDirectory = self.getApplicationDirectory( applicationName )

    files = os.listdir( applicationDirectory )

    logFiles = []
    for f in files:

      if f[ 0 : 3 ] == 'log':

        logFiles.append( os.path.join( applicationDirectory, f ) )

    for logFile in logFiles:

      localDay = time.time()
      logDay = os.path.getmtime( logFile )

      # remove log file if older than one month
      if ( localDay - logDay > 2600000.0 ):

        os.remove( logFile )

