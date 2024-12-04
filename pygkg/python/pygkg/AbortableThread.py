import threading
import ctypes


def _async_raise( threadId, exceptionObject ):

  res = ctypes.pythonapi.PyThreadState_SetAsyncExc( \
                                           ctypes.c_long( threadId ),
                                           ctypes.py_object( exceptionObject ) )
  if res == 0:
  
    raise ValueError( "nonexistent thread id" )

  elif res > 1:

    # if it returns a number greater than one, you're in trouble, 
    # and you should call it again with exc=NULL to revert the effect
    ctypes.pythonapi.PyThreadState_SetAsyncExc( ctypes.c_long( threadId ), 0 )
    raise SystemError( "PyThreadState_SetAsyncExc failed" )
 
class AbortableThread( threading.Thread ):

  def raiseException( self, exceptionObject ):

    assert self.isAlive(), "thread must be started"
    for threadId, threadObject in threading._active.items():

      if threadObject is self:
      
        _async_raise( threadId, exceptionObject )
        return
      
    # the thread was alive when we entered the loop, but was not found 
    # in the dict, hence it must have been already terminated. should we raise
    # an exception here? silently ignore?
  
  def terminate( self ):
  
    # must raise the SystemExit type, instead of a SystemExit() instance
    # due to a bug in PyThreadState_SetAsyncExc
    self.raiseException( SystemExit )
    self._Thread__stop()

