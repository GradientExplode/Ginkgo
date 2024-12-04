import os
import sys
sys.path.insert( 0, os.path.join( os.sep, 'usr', 'share', 'gkg', 'python' ) )
import gkg
from core.patterns.Singleton import *



class CommandFactory( object, metaclass = Singleton ):

  def __init__( self ):
    
    self._creators = dict()


  def registerCommand( self, command ):
  
    if ( command.getName() not in self._creators ):

      self._creators[ command.getName() ] = command.createInstance
      
    else:
    
      print( command.getName() + " already registered" )


  def executeCommand( self, dictionary ):
  
    if ( "algorithm" not in dictionary ):
    
      raise RuntimeError( "'algorithm' key not found in dictionary" )
  
    if ( "parameters" not in dictionary ):
    
      raise RuntimeError( "'parameters' key not found in dictionary" )
  
    if ( "verbose" not in dictionary ):
    
      raise RuntimeError( "'verbose' key not found in dictionary" )
  
    algorithm = dictionary[ "algorithm" ]
    parameters = dictionary[ "parameters" ]
    verbose = dictionary[ "verbose" ]
    
    if ( algorithm in self._creators ):
    
      if ( verbose ):
       
        print( "algorithm : " + algorithm )
        print( "\n" )
        for key in parameters:
         
          print( "  " + key + " -> " + str( parameters[ key ] ) )
           
        print( "\n" )
        print( "running " + algorithm + " ..." )
        print( "\n" )
    
      self._creators[ algorithm ]().execute( parameters )
       
    else:
    
      gkg.executeCommand( dictionary )



import core.command.Commands
