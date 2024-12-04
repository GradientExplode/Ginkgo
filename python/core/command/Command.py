import os
import sys
sys.path.insert( 0, os.path.join( os.sep, 'usr', 'share', 'gkg', 'python' ) )
import gkg


class Command:


  def __init__( self ):
  
    pass


  def execute( self, parameters ):
  
    raise RuntimeError( "should not enter this method" )
    

  def getParameter( self, name, parameters,
                    optional = False,
                    defaultValue = None ):
  
    if ( name in parameters ):

      return parameters[ name ]
      
    else:
    
      if ( optional == False ):
      
        raise RuntimeError( "parameter '" + name +
                             "' is mandatory and missing" )
                             
      return defaultValue

