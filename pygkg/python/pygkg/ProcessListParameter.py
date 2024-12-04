import sys, os, string
import PyQt5 
from PyQt5 import QtCore, QtGui
from Parameter import*


class ProcessListParameter( Parameter ):

  def __init__( self,
                theName,
                processDictionary ):

    Parameter.__init__( self, theName, 'processList' )

    self._processDictionary = processDictionary
    self._defaultWidgetTypes = { 'integer' : 'spinBox',
                                 'double'  : 'doubleSpinBox',
                                 'string'  : 'lineEdit',
                                 'choice'  : 'comboBox' }
    self._parameters = list()
                                
                                
  def duplicate( self ):

    processListParameter = ProcessListParameter( self._name,
                                                 self._processDictionary )
                                                  
    processListParameter._defaultWidgetTypes = self._defaultWidgetTypes
    processListParameter._parameters = self._parameters
    
    return processListParameter
                                
                                
  def readValue( self ):

    values = []
    index = 0
    for parameter in self._parameters:

      values.append( dict() )
      values[ index ][ 'processName'] = parameter[ 'processName' ]
      for p in parameter.keys():

        if not( p == 'processName' ):

          values[ index ][ p ] = parameter[ p ].getValue()

      index += 1

    return values


  def writeValue( self, values ):

    self.setValue( values )


  def getValue( self ):

    return self._parameters


  def getDefaultValue( self ):

    return list()


  def getParameterFieldValue( self, processName, parameterName, fieldName ):

    if not ( self._processDictionary.has_key( processName ) ):

      message = 'no \'' + processName + '\' process present in the process list'
      raise RuntimeError, message

    elif not ( \
              self._processDictionary[ processName ].has_key( parameterName ) ):

      message = 'process list has no parameter \'' + parameterName + '\''
      raise RuntimeError, message

    elif not ( \
      self._processDictionary[ processName ][ parameterName ].has_key( \
                                                            fieldName ) ):

      message = parameterName + ' from ' + processName + \
                ' has no key ' + fieldName
      raise RuntimeError, message

    else:

      return \
            self._processDictionary[ processName ][ parameterName ][ fieldName ]


  def getParameterDefaultValue( self, processName, parameterName ):

    return self.getParameterFieldValue( processName, 
                                        parameterName,
                                        'defaultValue' )


  def getLowerValue( self, processName, parameterName ):

    return self.getParameterFieldValue( processName, 
                                        parameterName,
                                        'minimumValue' )


  def getUpperValue( self, processName, parameterName ):

    return self.getParameterFieldValue( processName, 
                                        parameterName,
                                        'maximumValue' )


  def getIncrementValue( self, processName, parameterName ):

    return self.getParameterFieldValue( processName, 
                                        parameterName,
                                        'incrementStep' )


  def setValue( self, value ):

    self.updateWidget( value )


  def getParameter( self, index, parameterName ):

    if not ( self._parameters[ index ].has_key( parameterName ) ):

      message = 'parameters has no key \'' + parameterName + '\''
      raise RuntimeError, message

    else:

      return self._parameters[ index ][ parameterName ]


  def getProcessNames( self ):

    return self._processDictionary.keys()


  def getParameterKeys( self, processName ):

    if not ( self._processDictionary.has_key( processName ) ):

      message = 'process dictionary has no process \'' + processName + '\''
      raise RuntimeError, message

    else:

      parameterKeys = dict( self._processDictionary[ processName ] )
      k = parameterKeys.pop( 'index' )
      return parameterKeys.keys()


  def getParameterForProcess( self, processName, parameterName ):

    if not ( \
      self._processDictionary.has_key( processName ) ):

      message = 'process dictionary has no key \'' + processName + '\''
      raise RuntimeError, message

    elif not ( \
      self._processDictionary[ processName ].has_key( parameterName ) ):

      message = 'process \'' + processName + '\' has no parameter \'' + \
                parameterName + '\''
      raise RuntimeError, message

    return self._processDictionary[ processName ] [ parameterName ]


  def getParameterField( self, processName, parameterName, fieldName ):

    parameter = self.getParameterForProcess( processName, parameterName );

    if not ( parameter.has_key( fieldName ) ):

      message = '\'' + parameterName + '\' from \'' + processName + \
                '\' has no key \'' + fieldName + '\''
      raise RuntimeError, message

    else:

      return parameter[ fieldName ]


  def getParameterIndex( self, processName, parameterName ):

    return self.getParameterField( processName, parameterName, 'index' )


  def getParameterType( self, processName, parameterName ):

    return self.getParameterField( processName, parameterName, 'type' )


  def getParameterName( self, processName, parameterName ):

    return self.getParameterField( processName, parameterName, 'name' )


  def getWidgetType( self, processName, parameterName ):

    widgetType = None

    try:

      widgetType = self.getParameterField( processName, parameterName,
                                           'widgetType' )

    except:

      parameterType = self.getParameterType( processName, parameterName )
      if not ( self._defaultWidgetTypes.has_key( parameterType ) ):

        message = '\'' + parameterName + '\' from \'' + processName + \
                  '\' has no key \'widgetType\''
        raise RuntimeError, message

      else:

        widgetType = self._defaultWidgetTypes[ parameterType ]

    return widgetType


  def getChoices( self, processName, parameterName ):

    return self.getParameterField( processName, parameterName, 'choices' )


  def getParametersToShow( self, processName, parameterName ):

    return self.getParameterField( processName, parameterName,
                                   'parametersToShow' )


  def getValues( self, processName, parameterName ):

    return self.getParameterField( processName, parameterName, 'values' )


  def hasParametersToShow( self, processName ):

    parametersToShow = []
    parameters = self.getParameterKeys( processName )
    for parameter in parameters:

      if ( self._processDictionary[ processName ][ parameter ] .has_key( \
                                                                   'values' ) ):

        parametersToShow.append( parameter )


    return parametersToShow


  def updateParameterValue( self, index, parameterName, value ):

    parameter = self.getParameter( index, parameterName )
    parameter.updateWidget( value )


  def updateParameters( self, index ):

    for parameterName in self._parameters[ index ]:

      if ( parameterName != 'processName' ):

        parameter = self.getParameter( index, parameterName )

        parameter.updateWidget( parameter.getValue() )


  def removeParameters( self, index ):

    for parameterName in self._parameters[ index ]:

      if ( parameterName != 'processName' ):

        if ( self._parameters[ index ].has_key( parameterName ) ):

          parameter = self.getParameter( index, parameterName )
          del parameter

    self._parameters.remove( self._parameters[ index ] )


  def getParameterValue( self, index, parameterName ):

    return self.getParameter( index, parameterName ).getValue()


  def addParameters( self, processName, index ):

    self._parameters.insert( index, {} )
    self._parameters[ index ][ 'processName' ] = processName
    parameterNames = self.getParameterKeys( processName )
    for parameterName in parameterNames:

      parameterType = self.getParameterType( processName, parameterName )
      parameterWidgetType = self.getWidgetType( processName, parameterName )
      localParameterName = 'Process' + str( index ) + \
                           '_' + parameterName
      #adding parameter
      if ( parameterType == 'integer' ):

        self._parameters[ index ][ localParameterName ] = \
            IntegerParameter( localParameterName,
                              self.getParameterDefaultValue( processName,
                                                             parameterName ),
                              self.getLowerValue( processName,
                                                  parameterName ),
                              self.getUpperValue( processName,
                                                  parameterName ),
                              self.getIncrementValue( processName,
                                                      parameterName ) )

      elif ( parameterType == 'double' ):

        self._parameters[ index ][ localParameterName ] = \
             DoubleParameter( localParameterName,
                              self.getParameterDefaultValue( processName,
                                                             parameterName ),
                              self.getLowerValue( processName,
                                                  parameterName ),
                              self.getUpperValue( processName,
                                                  parameterName ),
                              self.getIncrementValue( processName,
                                                      parameterName ) )

      elif ( parameterType == 'string' ):

        self._parameters[ index ][ localParameterName ] = \
             StringParameter( localParameterName,
                              self.getParameterDefaultValue( processName,
                                                             parameterName ) )

      elif ( parameterType == 'boolean' ):

        self._parameters[ index ][ localParameterName ] = \
            BooleanParameter( localParameterName,
                              self.getParameterDefaultValue( processName,
                                                             parameterName ) )

      elif ( parameterType == 'choices' ):

        self._parameters[ index ][ localParameterName ] = \
             ChoiceParameter( localParameterName,
                              self.getParameterDefaultValue( processName,
                                                             parameterName ),
                              self.getChoices( processName,
                                               parameterName ) )


  def hasParameter( self, processName ):

    if not ( self._processDictionary.has_key( processName ) ):

      message = 'process dictionary has no key \'' + processName + '\''
      raise RuntimeError, message

    else:

      if ( self._processDictionary[ processName ].keys() != [ 'index' ] ):

        return True

      else:

        return False


  def getOutputFileCount( self, processName ):

    if ( not self._processDictionary.has_key( processName ) ):

      message = 'process dictionary has no key \'' + processName + '\''
      raise RuntimeError, message

    elif not ( \
      self._processDictionary[ processName ].has_key( 'outputFileCount' ) ):

      message = 'processList has no key \'outputFileCount\''
      raise RuntimeError, message

    else:

      return self._processDictionary[ processName ][ 'outputFileCount' ]


  def getProcessNameToIndex( self, processName ):

    if ( not self._processDictionary.has_key( processName ) ):

      message = 'process dictionary has no key \'' + processName + '\''
      raise RuntimeError, message

    else:

      return self._processDictionary[ processName ][ 'index' ]


  def tuneToSpecy( self, settings ):
  
    #print 'ProcessListParameter.tuneToSpecy to be implemented'
    return


