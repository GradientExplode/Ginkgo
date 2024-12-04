import sys, os, string
import PyQt5
from PyQt5 import QtCore, QtGui


################################################################################
# Parameter
################################################################################

class Parameter:

  def __init__( self, theName, theType ):

    self._name = theName
    self._type = theType
    self._widgetType = 'unknown'
    self._widget = None

  def getName( self ):
  
    return self._name

  def setName( self, name ):
    
    self._name = name
    print 'setName', name

  def getType( self ):
  
    return self._type
    
  def display( self ):
  
    print self._name, ' -> ', self._type

  def setWidget( self, widgetType, widget ):
  
    self._widgetType = widgetType
    self._widget = widget

  def getWidgetType( self ):
  
    return self._widgetType


  def getWidget( self ):

    return self._widget

  def updateWidget( self, value ):
  
    if ( self._widget is not None ):
    
      if ( self._widgetType == 'spinBox' ):
 
        self._widget.setValue( value )
 
      elif ( self._widgetType == 'slider' ):
 
        self._widget.setValue( value )

      elif ( self._widgetType == 'doubleSpinBox' ):
 
        self._widget.setValue( value )

      elif ( self._widgetType == 'lineEdit' ):
 
        self._widget.setText( str( value ) )

      elif ( self._widgetType == 'checkBox' ):
 
        self._widget.setCheckState( QtCore.Qt.CheckState( value ) )

      elif ( self._widgetType == 'radioButton' ):
 
        self._widget.setChecked( value )

      elif ( self._widgetType == 'textEdit' ):

        self._widget.setText( str( value ) )

      elif ( self._widgetType == 'comboBox' ):

        if value.__class__ == int:

          self._widget.setCurrentIndex( value )

        elif value.__class__ == list:

          self._widget.clear()
          self._widget.addItems( value[ 0 ] )
          self._widget.setCurrentIndex( value[ 1 ] )

      elif ( self._widgetType == 'tableWidget' ):

        theLine = value[ 0 ]
        theColumn = value[ 1 ]
        theValue = value[ 2 ]
        self._widget.item( theLine, theColumn ).setText( str( theValue ) )
 
      elif ( self._widgetType == 'treeWidget' ):
 
        action = value[ 0 ]
        # add a new line
        if ( action == 1 ):
        
          self._widget.addWidgetEntry( value[ 1: ] )
       
        # remove an existing line
        elif ( action == 2 ):
    
          self._widget.removeWidgetEntry( value[ 1 ] )

      elif ( self._widgetType == 'listWidget' ):

        nbItems = self._widget.count()
        items = ''
        for n in xrange( nbItems ):

          item = self._widget.item( n )
          items = items + item.text() + ' '
        if items != value:

          self._widget.clear()
          values = value.split()
          for v in values:

            self._widget.addItem( v )
          if ( len( values ) != 0 ):

            self._widget.setCurrentRow( 0 )

      elif ( self._widgetType == 'processListWidget' ):

        self._widget.setValues( value )


      elif ( self._widgetType == 'groupBox' ):
 
        self._widget.setChecked( value )

  def tuneWidget( self, settings ):
  
    if ( self._widget is not None ):
    
      if ( self._widgetType == 'spinBox' ):
 
        self._widget.setMinimum( int( settings[ 'lowerValue' ] ) )
        self._widget.setMaximum( int( settings[ 'upperValue' ] ) )
        self._widget.setSingleStep( int( settings[ 'incrementValue' ] ) )
        self._widget.setValue( int( settings[ 'defaultValue' ] ) )
 
      elif ( self._widgetType == 'slider' ):
 
        self._widget.setMinimum( int( settings[ 'lowerValue' ] ) )
        self._widget.setMaximum( int( settings[ 'upperValue' ] ) )
        self._widget.setSingleStep( int( settings[ 'incrementValue' ] ) )
        self._widget.setValue( int( settings[ 'defaultValue' ] ) )

      elif ( self._widgetType == 'doubleSpinBox' ):
 
        self._widget.setMinimum( float( settings[ 'lowerValue' ] ) )
        self._widget.setMaximum( float( settings[ 'upperValue' ] ) )
        self._widget.setSingleStep( float( settings[ 'incrementValue' ] ) )
        self._widget.setValue( float( settings[ 'defaultValue' ] ) )

      elif ( self._widgetType == 'lineEdit' ):
 
        self._widget.setText( str( settings[ 'defaultValue' ] ) )

      elif ( self._widgetType == 'checkBox' ):
 
        self._widget.setCheckState( QtCore.Qt.CheckState( \
                                                  settings[ 'defaultValue' ] ) )

      elif ( self._widgetType == 'radioButton' ):
 
        self._widget.setChecked( settings[ 'defaultValue' ] )

      elif ( self._widgetType == 'textEdit' ):

        self._widget.setText( str( settings[ 'defaultValue' ] ) )

      elif ( self._widgetType == 'comboBox' ):

        if ( settings.has_key( 'choices' ) ):

          self._widget.clear()
          self._widget.addItems( list( settings[ 'choices' ] ) )
          
        self._widget.setCurrentIndex( int( settings[ 'defaultValue' ] ) )


      '''elif ( self._widgetType == 'tableWidget' ):

        theLine = value[ 0 ]
        theColumn = value[ 1 ]
        theValue = value[ 2 ]
        self._widget.item( theLine, theColumn ).setText( str( theValue ) )
 
      elif ( self._widgetType == 'treeWidget' ):
 
        action = value[ 0 ]
        # add a new line
        if ( action == 1 ):
        
          self._widget.addWidgetEntry( value[ 1: ] )
       
        # remove an existing line
        elif ( action == 2 ):
    
          self._widget.removeWidgetEntry( value[ 1 ] )

      elif ( self._widgetType == 'listWidget' ):

        nbItems = self._widget.count()
        items = ''
        for n in xrange( nbItems ):

          item = self._widget.item( n )
          items = items + item.text() + ' '
        if items != value:

          self._widget.clear()
          values = value.split()
          for v in values:

            self._widget.addItem( v )
          if ( len( values ) != 0 ):

            self._widget.setCurrentRow( 0 )

      elif ( self._widgetType == 'processListWidget' ):

        self._widget.setValues( value )

      elif ( self._widgetType == 'groupBox' ):
 
        self._widget.setChecked( int( settings[ 'defaultValue' ] ) )
      '''

  def tuneToSpecy( self, settings ):
  
    raise RuntimeError, 'method must be defined in the inherited class'



################################################################################
# IntegerParameter
################################################################################

class IntegerParameter( Parameter ):

  def __init__( self,
                theName,
                defaultValue,
                lowerValue, 
                upperValue,
                incrementValue ):

    Parameter.__init__( self, theName, 'integer' )
    self._value = int( defaultValue )
    self._defaultValue = int( defaultValue )
    self._lowerValue = int( lowerValue )
    self._upperValue = int( upperValue )
    self._incrementValue = int( incrementValue )
    
    if ( ( self._defaultValue < self._lowerValue ) or
         ( self._defaultValue > self._upperValue ) ):
         
      message = 'default value outside valid range'
      raise RuntimeError, message

  def readValue( self ):
  
    return self._value
    
  def writeValue( self, value ):
  
    self.setValue( value )
    
  def getValue( self ):
  
    return self._value
    
  def getDefaultValue( self ):
  
    return self._defaultValue
    
  def getLowerValue( self ):
  
    return self._lowerValue
    
  def getUpperValue( self ):
  
    return self._upperValue

  def getIncrementValue( self ):
  
    return self._incrementValue
    
  def setValue( self, value, verbose = False ):

    if ( ( value < self._lowerValue ) or ( value > self._upperValue ) ):
    
      message = 'value outside valid range'
      raise RuntimeError, message
      
    self._value = int( value )
    self.updateWidget( self._value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value

  def setValueButDoNotUpdateWidget( self, value, verbose = False ):
  
    if ( ( value < self._lowerValue ) or ( value > self._upperValue ) ):
    
      message = 'value outside valid range'
      raise RuntimeError, message
      
    self._value = int( value )
    if ( verbose == True ):

      print self._name, '->', 'new value =', self._value

  def tuneToSpecy( self, settings ):

    if ( settings.has_key( 'defaultValue' ) ):
    
      self._value = int( settings[ 'defaultValue' ] )
      self._defaultValue = int( settings[ 'defaultValue' ] )
      
    if ( settings.has_key( 'lowerValue' ) ):

      self._lowerValue = int( settings[ 'lowerValue' ] )

    if ( settings.has_key( 'upperValue' ) ):
    
      self._upperValue = int( settings[ 'upperValue' ] )
      
    if ( settings.has_key( 'incrementValue' ) ):
    
      self._incrementValue = int( settings[ 'incrementValue' ] )

    self.tuneWidget( settings )

  def display( self ):
  
    Parameter.display( self )
    print '  current value = ', self._value
    print '  default value = ', self._defaultValue
    print '  lower value = ', self._lowerValue
    print '  upper value = ', self._upperValue
    print '  increment value = ', self._incrementValue
    

################################################################################
# DoubleParameter
################################################################################

class DoubleParameter( Parameter ):

  def __init__( self,
                theName,
                defaultValue,
                lowerValue, 
                upperValue,
                incrementValue ):

    Parameter.__init__( self, theName, 'double' )
    self._value = float( defaultValue )
    self._defaultValue = float( defaultValue )
    self._lowerValue = float( lowerValue )
    self._upperValue = float( upperValue )
    self._incrementValue = float( incrementValue )

    if ( ( self._defaultValue < self._lowerValue ) or
         ( self._defaultValue > self._upperValue ) ):
         
      message = 'default value outside valid range'
      raise RuntimeError, message

  def readValue( self ):
  
    return self._value
    
  def writeValue( self, value ):
  
    self.setValue( value )
    
  def getValue( self ):
  
    return self._value
    
  def getDefaultValue( self ):
  
    return self._defaultValue
    
  def getLowerValue( self ):
  
    return self._lowerValue
    
  def getUpperValue( self ):
  
    return self._upperValue
    
  def getIncrementValue( self ):
  
    return self._incrementValue
    
  def setValue( self, value, verbose = False ):
  
    if ( ( float( value ) < self._lowerValue ) or
         ( float( value ) > self._upperValue ) ):
    
      message = 'value outside valid range'
      raise RuntimeError, message
      
    self._value = float( value )
    self.updateWidget( self._value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value
    
  def setValueButDoNotUpdateWidget( self, value, verbose = False ):
  
    if ( ( float( value ) < self._lowerValue ) or
         ( float( value ) > self._upperValue ) ):
    
      message = 'value outside valid range'
      raise RuntimeError, message
      
    self._value = float( value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value

  def tuneToSpecy( self, settings ):
  
    if ( settings.has_key( 'defaultValue' ) ):
    
      self._value = float( settings[ 'defaultValue' ] )
      self._defaultValue = float( settings[ 'defaultValue' ] )
      
    if ( settings.has_key( 'lowerValue' ) ):

      self._lowerValue = float( settings[ 'lowerValue' ] )

    if ( settings.has_key( 'upperValue' ) ):
    
      self._upperValue = float( settings[ 'upperValue' ] )
      
    if ( settings.has_key( 'incrementValue' ) ):
    
      self._incrementValue = float( settings[ 'incrementValue' ] )

    self.tuneWidget( settings )
    
  def display( self ):
  
    Parameter.display( self )
    print '  current value = ', self._value
    print '  default value = ', self._defaultValue
    print '  lower value = ', self._lowerValue
    print '  upper value = ', self._upperValue
    print '  increment value = ', self._incrementValue

    
################################################################################
# BooleanParameter (0=false 2=true)
################################################################################

class BooleanParameter( Parameter ):

  def __init__( self,
                theName,
                defaultValue ):

    Parameter.__init__( self, theName, 'boolean' )
    self._value = int( defaultValue )
    self._defaultValue = int( defaultValue )

  def readValue( self ):
  
    return self._value
    
  def writeValue( self, value ):
  
    self.setValue( value )
    
  def getValue( self ):
  
    return self._value
    
  def getDefaultValue( self ):
  
    return self._defaultValue
    
  def setValue( self, value, verbose = False ):
  
    self._value = int( value )
    self.updateWidget( self._value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value

  def setValueButDoNotUpdateWidget( self, value, verbose = False ):
  
    self._value = int( value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value

  def tuneToSpecy( self, settings ):
  
    if ( settings.has_key( 'defaultValue' ) ):
    
      self._value = int( settings[ 'defaultValue' ] )
      self._defaultValue = int( settings[ 'defaultValue' ] )

    self.tuneWidget( settings )
    
  def display( self ):
  
    Parameter.display( self )
    print '  current value = ', self._value
    print '  default value = ', self._defaultValue


################################################################################
# StringParameter
################################################################################

class StringParameter( Parameter ):

  def __init__( self,
                theName,
                defaultValue,
                isAPath = True,
                isAnOutput = False ):

    Parameter.__init__( self, theName, 'string' )
    self._value = str( defaultValue )
    self._defaultValue = str( defaultValue )
    self._isAnOutput = isAnOutput
    self._isAPath = isAPath


  def readValue( self ):

    return self._value

  def writeValue( self, value ):

    self.setValue( value )

  def getValue( self ):

    return self._value

  def getDefaultValue( self ):
  
    return self._defaultValue
    
  def setValue( self, value, verbose = False ):
  
    self._value = str( value )
    self.updateWidget( self._value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value


  def setValueButDoNotUpdateWidget( self, value, verbose = False ):
  
    self._value = str( value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value

  def tuneToSpecy( self, settings ):
  
    if ( settings.has_key( 'defaultValue' ) ):
    
      self._value = str( settings[ 'defaultValue' ] )
      self._defaultValue = str( settings[ 'defaultValue' ] )
      
    self.tuneWidget( settings )
    
  def display( self ):
  
    Parameter.display( self )
    print '  current value = ', self._value
    print '  default value = ', self._defaultValue

  def isAnOutputParameter( self ):

    return self._isAnOutput

  def isAPathParameter( self ):

    return self._isAPath


################################################################################
# ChoiceParameter
################################################################################

class ChoiceParameter( Parameter ):

  def __init__( self,
                theName,
                defaultValue,
                choices ):

    Parameter.__init__( self, theName, 'choice' )
    
    if ( ( defaultValue < 0 ) or ( defaultValue >= len( choices ) ) ):

      message = 'default value not valid'
      raise RuntimeError, message
    
    self._value = int( defaultValue )
    self._defaultValue = int( defaultValue )
    self._choices = choices

  def readValue( self ):
  
    return self._value
    
  def writeValue( self, value ):
  
    self.setValue( value )
    
  def getValue( self ):
  
    return self._value
    
  def getDefaultValue( self ):
  
    return self._defaultValue
    
  def getChoice( self ):
  
    choice = None
    if ( len( self._choices ) != 0 ):

      choice = self._choices[ self._value ]

    return choice
    
  def getDefaultChoice( self ):

    defaultChoice = None
    if ( ( len( self._choices ) != 0 ) and \
                           ( self._choices.count( self._defaultValue ) != 0 ) ):

      defaultChoice = self._choices[ self._defaultValue ]

    return defaultChoice
    
  def getChoices( self ):
  
    return self._choices

  def setChoices( self, choices, defaultValue = -1 ):

    defaultChoice = self.getDefaultChoice()
    if ( choices.count( defaultChoice ) ):

      self._defaultValue = choices.index( defaultChoice )

    elif ( defaultValue > -1 ):

      if ( defaultValue >= len( choices ) ):

        message = 'default value not valid'
        raise RuntimeError, message

      else:

        self._defaultValue = defaultValue
  
    currentChoice = self.getChoice()
    if ( choices.count( currentChoice ) ):

      self._value = choices.index( currentChoice )

    else:

      self._value = self._defaultValue

    self._choices = choices
    self.updateWidget( [ self._choices, self._value ] )
    
  def setValue( self, value, verbose = False ):
  
    if ( ( value < 0 ) or ( value >= len( self._choices ) ) ):

      message = 'value not in valid range'
      raise RuntimeError, message

    self._value = int( value )
    self.updateWidget( self._value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value =', self._value, '(' + \
          self.getChoice() + ')'

  def setValueButDoNotUpdateWidget( self, value, verbose = False ):

    if ( value == -1 ):

      pass

    elif ( ( value < 0 ) or ( value >= len( self._choices ) ) ):

      print self._name, 'value', value, 'choice', self._choices
      message = 'value not in valid range'
      raise RuntimeError, message

    else:

      self._value = int( value )
      if ( verbose == True ):
    
        print self._name, '->', 'new value =', self._value, '(' + \
            self.getChoice() + ')'

  def tuneToSpecy( self, settings ):
  
    if ( settings.has_key( 'defaultValue' ) ):
    
      self._value = int( settings[ 'defaultValue' ] )
      self._defaultValue = int( settings[ 'defaultValue' ] )

    if ( settings.has_key( 'choices' ) ):
    
      self._choices = settings[ 'choices' ]

    self.tuneWidget( settings )

  def display( self ):
  
    Parameter.display( self )
    print '  current value = ', self._value
    print '  default value = ', self._defaultValue
    print '  current choice = ', self.getChoice()
    print '  default choice = ', self.getDefaultChoice()
    print '  choices = ', self._choices


################################################################################
# TableOfIntegerParameter
################################################################################

class TableOfIntegerParameter( Parameter ):

  def __init__( self,
                theName,
                lineCount,
                columnCount,
                defaultValues,
                lowerValues,
                upperValues,
                incrementValues ):

    Parameter.__init__( self, theName, 'table_of_integer' )
    self._lineCount = int( lineCount )
    self._columnCount = int( columnCount )
    self._values = []
    self._defaultValues = []
    self._lowerValues = []
    self._upperValues = []
    self._incrementValues = []
    for l in range( 0, self._lineCount ):
    
      lineValues = []
      lineDefaultValues = []
      lineLowerValues = []
      lineUpperValues = []
      lineIncrementValues = []
      for c in range( 0, self._columnCount ):
      
        lineDefaultValues.append( int( defaultValues[ l ][ c ] ) )
        lineLowerValues.append( int( lowerValues[ l ][ c ] ) )
        lineUpperValues.append( int( upperValues[ l ][ c ] ) )
        lineIncrementValues.append( int( incrementValues[ l ][ c ] ) )

        if ( ( lineDefaultValues[ c ] < lineLowerValues[ c ] ) or
             ( lineDefaultValues[ c ] > lineUpperValues[ c ] ) ):
 
          message = 'default value outside valid range'
          raise RuntimeError, message

      self._values.append( lineDefaultValues )
      self._defaultValues.append( lineDefaultValues )
      self._lowerValues.append( lineLowerValues )
      self._upperValues.append( lineUpperValues )
      self._incrementValues.append( lineIncrementValues )
    
  def getLineCount( self ):
  
    return self._lineCount
    
  def getColumnCount( self ):
  
    return self._columnCount
    
  def readValue( self ):
  
    return self._values
    
  def writeValue( self, values ):
  
    if ( len( values ) != self._lineCount ):
     
      message = 'values has bad line count'
      raise RuntimeError, message

    if ( len( values[ 0 ] ) != self._columnCount ):
     
      message = 'values has bad column count'
      raise RuntimeError, message

    for l in range( 0, self._lineCount ):
    
       for c in range( 0, self._columnCount ):
       
         self.setValue( l, c, values[ l ][ c ] )
    
  def getValue( self, line, column ):
  
    return self._values[ line ][ column ]
    
  def getDefaultValue( self, line, column ):
  
    return self._defaultValues[ line ][ column ]
    
  def getLowerValue( self, line, column ):
  
    return self._lowerValues[ line ][ column ]
    
  def getUpperValue( self, line, column ):
  
    return self._upperValues[ line ][ column ]
    
  def getIncrementValue( self, line, column ):
  
    return self._incrementValues[ line ][ column ]
    
  def setValue( self, line, column, value, verbose = False ):
  
    if ( ( int( value ) < self._lowerValues[ line ][ column ] ) or
         ( int( value ) > self._upperValues[ line ][ column ] ) ):
    
      message = 'value outside valid range'
      raise RuntimeError, message
      
    self._values[ line ][ column ] = int( value )
    if ( verbose == True ):
    
      print self._name, '->', 'new value at (', line, ',', column, ') =', \
            self._values[ line ][ column ]
    
  def display( self ):
  
    Parameter.display( self )
    print '  line count = ', self._lineCount
    print '  column count = ', self._columnCount
    print '  current values = ', self._values
    print '  default values = ', self._defaultValues
    print '  lower values = ', self._lowerValues
    print '  upper values = ', self._upperValues
    print '  increment values = ', self._incrementValues

  def tuneToSpecy( self, settings ):
  
    #print 'TableOfIntegerParameter.tuneToSpecy to be implemented'
    return
    


################################################################################
# TableOfDoubleParameter
################################################################################

class TableOfDoubleParameter( Parameter ):

  def __init__( self,
                theName,
                lineCount,
                columnCount,
                defaultValues,
                lowerValues,
                upperValues,
                incrementValues ):

    Parameter.__init__( self, theName, 'table_of_double' )
    self._lineCount = int( lineCount )
    self._columnCount = int( columnCount )
    self._values = []
    self._defaultValues = []
    self._lowerValues = []
    self._upperValues = []
    self._incrementValues = []
    for l in range( 0, self._lineCount ):
    
      lineValues = []
      lineDefaultValues = []
      lineLowerValues = []
      lineUpperValues = []
      lineIncrementValues = []
      for c in range( 0, self._columnCount ):
      
        lineDefaultValues.append( float( defaultValues[ l ][ c ] ) )
        lineLowerValues.append( float( lowerValues[ l ][ c ] ) )
        lineUpperValues.append( float( upperValues[ l ][ c ] ) )
        lineIncrementValues.append( float( incrementValues[ l ][ c ] ) )

        if ( ( lineDefaultValues[ c ] < lineLowerValues[ c ] ) or
             ( lineDefaultValues[ c ] > lineUpperValues[ c ] ) ):
 
          message = 'default value outside valid range'
          raise RuntimeError, message

      self._values.append( lineDefaultValues )
      self._defaultValues.append( lineDefaultValues )
      self._lowerValues.append( lineLowerValues )
      self._upperValues.append( lineUpperValues )
      self._incrementValues.append( lineIncrementValues )
    
  def getLineCount( self ):
  
    return self._lineCount
    
  def getColumnCount( self ):
  
    return self._columnCount
    
  def readValue( self ):
  
    return self._values
    
  def writeValue( self, values ):
  
    if ( len( values ) != self._lineCount ):
     
      message = 'values has bad line count'
      raise RuntimeError, message

    if ( len( values[ 0 ] ) != self._columnCount ):
     
      message = 'values has bad column count'
      raise RuntimeError, message

    for l in range( 0, self._lineCount ):
    
       for c in range( 0, self._columnCount ):
       
         self.setValue( l, c, values[ l ][ c ] )
    
  def getValue( self, line, column ):
  
    return self._values[ line ][ column ]
    
  def getDefaultValue( self, line, column ):
  
    return self._defaultValues[ line ][ column ]
    
  def getLowerValue( self, line, column ):
  
    return self._lowerValues[ line ][ column ]
    
  def getUpperValue( self, line, column ):
  
    return self._upperValues[ line ][ column ]
    
  def getIncrementValue( self, line, column ):
  
    return self._incrementValues[ line ][ column ]
    
  def setValue( self, line, column, value, verbose = False ):
  
    if ( ( float( value ) < self._lowerValues[ line ][ column ] ) or
         ( float( value ) > self._upperValues[ line ][ column ] ) ):
    
      message = 'value outside valid range'
      raise RuntimeError, message
      
    self._values[ line ][ column ] = float( value )
    if ( verbose == True ):

      print self._name, '->', 'new value at (', line, ',', column, ') =', \
            self._values[ line ][ column ]
    self.updateWidget( [ line, column, value ] )
    
  def display( self ):
  
    Parameter.display( self )
    print '  line count = ', self._lineCount
    print '  column count = ', self._columnCount
    print '  current values = ', self._values
    print '  default values = ', self._defaultValues
    print '  lower values = ', self._lowerValues
    print '  upper values = ', self._upperValues
    print '  increment values = ', self._incrementValues

  def tuneToSpecy( self, settings ):
  
    #print 'TableOfDoubleParameter.tuneToSpecy to be implemented'
    return

################################################################################
# ListOfParameterList
################################################################################

class ListOfParameterList( Parameter ):

  def __init__( self,
                theName,
                parameterCount,
                labels,
                types ):

    Parameter.__init__( self, theName, 'list_of_parameter_list' )
    
    # storing parameter count
    self._parameterCount = int( parameterCount )
    
    # sanity checks
    if ( len( types ) != parameterCount ):
    
      message = 'invalid parameter type length'
      raise RuntimeError, message

    for t in types:
    
      if ( ( t != 'integer' ) and \
           ( t != 'double' ) and \
           ( t != 'boolean' ) and \
           ( t != 'string' ) and \
           ( t != 'choice' ) ):
           
        message = 'invalid type'
        raise RuntimeError, message
        
    # storing parameter types
    self._types = types
    
    # sanity check
    if ( len( labels ) != parameterCount ):
    
      message = 'invalid label count'
      raise RuntimeError, message
      
    # storing labels
    self._labels = labels
    
    # creating parameter label<->index lookup tables
    self._labelToIndexLut = {}
    self._indexToLabelLut = {}
    index = int( 0 )
    for l in labels:
    
      self._labelToIndexLut[ l ] = index
      self._indexToLabelLut[ index ] = l
      
      index += 1

    # creating empty list
    self._values = list()
    
  def getParameterCount( self ):
  
    return self._parameterCount
    
  def getLabels( self ):
  
    return self._labels
    
  def getEntryCount( self ):
  
    return len( self._values )
    
  def getTypes( self ):
  
    return self._types
    
  def getLabelFromIndex( self, index ):
  
    if ( not self._indexToLabelLut.has_key( index ) ):
    
      message = 'bad index'
      raise RuntimeError, message
      
    return self._indexToLabelLut[ index ]
    
  def getIndexFromLabel( self, label ):
  
    if ( not self._labelToIndexLut.has_key( label ) ):
    
      message = 'bad index'
      raise RuntimeError, message
      
    return self._labelToIndexLut[ label ]
    
  def getTypeForIndex( self, index ):
  
    if ( ( index < 0 ) or ( index >= self._parameterCount ) ):
    
      message = 'invalid index'
      raise RuntimeError, message

    return self._types[ index ]
  
  def getTypeForLabel( self, label ):
  
    return self._types[ self.getIndexFromLabel( label ) ]
  
  def readValue( self ):
  
    return self._values

  def writeValue( self, values ):
  
    self.clear()
    for entry in values:
    
      self.addEntry( entry )
      
    print self._name, '->', 'new value =', self._values

  def setValue( self, index, labels, verbose = False ):

    valuesCount = len( labels )

    for l in xrange( valuesCount ) :

      self._values[ index ][ l ] = labels[ l ]
      if ( verbose == True ):
      
        print self.getLabelFromIndex( l ), '->', 'new value =', labels[ l ]

  def getValueForEntryIndexAndLabel( self, entryIndex, label ):
  
    return self.getValueForEntryIndexAndLabelIndex( \
                                               entryIndex, \
                                               self.getIndexFromLabel( label ) )

  def getValueForEntryIndexAndLabelIndex( self, entryIndex, labelIndex ):


    if ( ( entryIndex < 0 ) or ( entryIndex >= len( self._values ) ) ):
    
      message = 'invalid entry index'
      raise RuntimeError, message
      
    if ( ( labelIndex < 0 ) or ( labelIndex >= self._parameterCount ) ):
    
      message = 'invalid name index'
      raise RuntimeError, message
      
    return self._values[ entryIndex ][ labelIndex ]

  def getValuesForLabel( self, name ):
  
    return self.getValuesForLabelIndex( self.getIndexFromLabel( label ) )
  
  def getValuesForLabelIndex( self, labelIndex ):
  
    if ( ( labelIndex < 0 ) or ( labelIndex >= self._parameterCount ) ):
    
      message = 'invalid name index'
      raise RuntimeError, message
      
    values = []
    for entry in range( 0, len( self._values ) ):
    
      values.append( entry[ labelIndex ] )
      
    return values

  def getValuesForEntryIndex( self, entryIndex ):
  
    if ( ( entryIndex < 0 ) or ( entryIndex >= len( self._values ) ) ):
    
      message = 'invalid entry index'
      raise RuntimeError, message
    
    return self._values[ entryIndex ]

  def clear( self ):
  
    while ( len( self._values ) ):
    
      self.removeEntry( len( self._values ) - 1 )
    
  def addEntry( self, values ):
  
    if ( len( values ) != self._parameterCount ):
    
      message = 'invalid value count'
      raise RuntimeError, message
  
    for i in range( 0, self._parameterCount ):
    
      if ( self._types[ i ] == 'integer' ):
      
        if ( type( values[ i ] ) != type( int() ) ):
        
          message = 'inconsistent value type for parameter of index ' + str( i )
          raise RuntimeError, message
      
      elif ( self._types[ i ] == 'double' ):
      
        if ( ( type( values[ i ] ) != type( float() ) ) and
             ( type( values[ i ] ) != type( int() ) ) ) :
        
          message = 'inconsistent type for parameter of index ' + str( i )
          raise RuntimeError, message
      
      elif ( self._types[ i ] == 'boolean' ):
      
        if ( type( values[ i ] ) != type( int() ) ) :
        
          message = 'inconsistent type for parameter of index ' + str( i )
          raise RuntimeError, message

        if ( ( values[ i ] != 0 ) and ( values[ i ] != 2 ) ) :
        
          message = 'bad values for boolean (should be 0 or 2) ' + \
                    'for parameter of index ' + str( i )
          raise RuntimeError, message

      elif ( self._types[ i ] == 'string' ):
      
        if ( type( values[ i ] ) != type( str() ) ) :
        
          message = 'inconsistent type for parameter of index ' + str( i )
          raise RuntimeError, message
          
      elif ( self._types[ i ] == 'choice' ):
      
        if ( type( values[ i ] ) != type( int() ) ) :
        
          message = 'inconsistent type for parameter of index ' + str( i )
          raise RuntimeError, message

      else:
      
        message = 'not a managed value type'
        raise RuntimeError, message
        
    self._values.append( values )
    self.updateWidget( [ 1 ] + values )

  
  def removeEntry( self, entryIndex ):
  
    if ( ( entryIndex < 0 ) or ( entryIndex >= len( self._values ) ) ):
    
      message = 'invalid entry index'
      raise RuntimeError, message
    
    del self._values[ entryIndex ]  

    self.updateWidget( [ 2, entryIndex ] )

  def display( self ):
  
    Parameter.display( self )
    print '  parameter count = ', self._parameterCount
    print '  labels = ', self._parameterNames
    print '  types = ', self._parameterTypes
    print '  values = ', self._values

  def tuneToSpecy( self, settings ):
  
    #print 'ListOfParameterList.tuneToSpecy to be implemented'
    return
