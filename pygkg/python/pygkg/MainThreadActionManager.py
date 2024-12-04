import sys
import threading
import PyQt5                                                     
from PyQt5 import QtCore, QtGui, uic


class MainThreadActionManager( QtCore.QObject ):

  def __init__( self, mainThread, parent = None ):
  
    QtCore.QObject.__init__( self, parent )
    
    self._lock = threading.RLock()
    self._condition = threading.Condition()
    self._actions = []
    self._timer = QtCore.QTimer( self )
    self._timer.timeout.connect( self._launchActions )
    self._timer.start( 100 )

    self._mainThread = mainThread

    self._exceptions = dict()
    self._results = dict()
    self._functionIndex = 0

  
  def push( self, function, *args, **kwargs ):
  
    if  threading.currentThread() == self._mainThread:
    
      if ( kwargs is None ) or ( len( kwargs ) == 0 ):
      
        apply( function, args )
        
      else:
      
        apply( function, args, kwargs )
        
    else:
    
      self._lock.acquire()
      try:
      
        self._actions.append( ( function, args, kwargs ) )
        
      finally:
      
        self._lock.release()
        
        
  def call( self, function, *args, **kwargs ):

    if  threading.currentThread() == self._mainThread:

      if ( kwargs is None ) or ( len( kwargs ) == 0 ):
      
        return apply( function, args )
        
      else:

        return apply( function, args, kwargs )
        
    else:
    
      self._condition.acquire()
      try:
      
        self._lock.acquire()
        try:

          self._functionIndex += 1
          index = self._functionIndex
          self._actions.append( ( self._callAndWakeUp,
                                  ( function, args, kwargs, index ),
                                  {} ) )
                                  
        finally:
        
          self._lock.release()
          
        self._condition.wait()
        
      finally:
      
        self._condition.release()

      if self._exceptions[ index ] is not None:

        e, v, t = self._exceptions.pop( index )
        result = self._results.pop( index )
        raise e, v, t
        
      else:

        exception = self._exceptions.pop( index )
        return self._results.pop( index )
        
        
  def _callAndWakeUp( self, function, args, kwargs, index ):
  
    self._exceptions[ index ] = None
    self._results[ index ] = None
    try:
    
      if ( kwargs is None ) or ( len( kwargs ) == 0 ):

        self._results[ index ] = apply( function, args )
        
      else:

        self._results[ index ] = apply( function, args, kwargs )

    except:

      self._exceptions[ index ] = sys.exc_info()

    self._condition.acquire()
    self._condition.notify()
    self._condition.release()


  def _launchActions( self ):

    self._functionIndex = 0
    self._lock.acquire()
    try:
    
      actions = self._actions
      self._actions = []
      
    finally:
    
      self._lock.release()
      
    for ( function, args, kwargs ) in actions:
    
      try:
      
        if ( kwargs is None ) or ( len( kwargs ) == 0 ):
      
          apply( function, args )
        
        else:
      
          apply( function, args, kwargs )

      except:
      
        showException()

