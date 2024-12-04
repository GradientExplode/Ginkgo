from Parameter import *

class AdvancedParameter( Parameter ):

  def __init__( self,
                theName,
                valueDictionnary ):

    Parameter.__init__( self, theName, 'advanced' )

    self._defaultValues = dict()
    self._values = dict()

    self._widgets = dict()
    self._widgetTypes  = dict()
    self._dependencies = dict()
    self._dependencyValues = dict()
    self._choices = dict()

    self._spinboxRangeValues = dict()

    for parameterKey in valueDictionnary.keys():

      if( valueDictionnary[ parameterKey ].has_key( 'defaultValue' ) ):

         self._defaultValues[ parameterKey ] = \
                              valueDictionnary[ parameterKey ][ 'defaultValue' ]
         self._values[ parameterKey ] = \
                              valueDictionnary[ parameterKey ][ 'defaultValue' ]

      else:

        message = 'parameter', parameterKey, 'has no default value'
        raise RuntimeError, message

      if( valueDictionnary[ parameterKey ].has_key( 'widget' ) ):

         self._widgets[ parameterKey ] = \
                                    valueDictionnary[ parameterKey ][ 'widget' ]

      else:

        message = 'parameter', parameterKey, 'has no widget associated'
        raise RuntimeError, message

      if( valueDictionnary[ parameterKey ].has_key( 'widgetType' ) ):

        self._widgetTypes[ parameterKey ] = \
                                valueDictionnary[ parameterKey ][ 'widgetType' ]

        if( self._widgetTypes[ parameterKey ] == 'comboBox' ):

          if( valueDictionnary[ parameterKey ].has_key( 'choices' ) ):

            self._choices[ parameterKey ] = \
                                   valueDictionnary[ parameterKey ][ 'choices' ]

          else:

            message = 'parameter', parameterKey, 'has no choices provided'
            raise RuntimeError, message

        elif( self._widgetTypes[ parameterKey ] == 'spinBox' ):

          self._spinboxRangeValues[ parameterKey ] = [ 0, 1000, 1 ]
          if( valueDictionnary[ parameterKey ].has_key( 'minimumValue' ) ):

            self._spinboxRangeValues[ parameterKey ][ 0 ] = \
                              valueDictionnary[ parameterKey ][ 'minimumValue' ]

          if( valueDictionnary[ parameterKey ].has_key( 'maximumValue' ) ):

            self._spinboxRangeValues[ parameterKey ][ 1 ] = \
                              valueDictionnary[ parameterKey ][ 'maximumValue' ]

          if( valueDictionnary[ parameterKey ].has_key( 'incrementValue' ) ):

            self._spinboxRangeValues[ parameterKey ][ 2 ] = \
                            valueDictionnary[ parameterKey ][ 'incrementValue' ]


      else:

        message = 'parameter', parameterKey, 'has no widget type provided'
        raise RuntimeError, message

      if( valueDictionnary[ parameterKey ].has_key( 'dependency' ) ):

         self._dependencies[ parameterKey ] = \
                                valueDictionnary[ parameterKey ][ 'dependency' ]
         if( valueDictionnary[ parameterKey ].has_key( 'dependencyValues' ) ):

           self._dependencyValues[ parameterKey ] = \
                          valueDictionnary[ parameterKey ][ 'dependencyValues' ]

         else:

           message = 'parameter', parameterKey 
           message += 'has no dependency values to handle dependency with'
           message += self._dependencies[ parameterKey ]
           raise RuntimeError, message


  def readValue( self ):
  
    return self._values

    
  def writeValue( self, values ):
  
    self.setValue( values )

    
  def getValue( self ):
  
    return self._values

    
  def getDefaultValue( self ):
  
    return self._defaultValues


  def getSpinboxRangeValues( self ):

    return self._spinboxRangeValues


  def setValue( self, values ):
  
    self._values = values
    self.updateWidget( self._values )
    print self._name, '->', 'new value =', self._values


  def setValueButDoNotUpdateWidget( self, value ):
  
    self._values = values
    print self._name, '->', 'new value =', self._values


  def display( self ):
  
    Parameter.display( self )
    print '  current value = ', self._values
    print '  default value = ', self._defaultValues


  def getWidgets( self ):

    return self._widgets


  def getWidgetTypes( self ):

    return self._widgetTypes


  def getDependencies( self ):

    return self._dependencies, self._dependencyValues


  def getChoices( self ):

    return self._choices


  def setValueFromKey( self, key, value ):

    self._values[ key ] = value


  def getChoice( self, key ):

    return self._choices[ key ][ self._values[ key ] ]

    
  def tuneToSpecy( self, settings ):
  
    #print 'AdvancedParameter.tuneToSpecy to be implemented'
    return

