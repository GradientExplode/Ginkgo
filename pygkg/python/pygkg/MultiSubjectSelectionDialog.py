import re
import os
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from ResourceManager import *

class MultiSubjectSelectionDialog( QtWidgets.QDialog ):

  def __init__( self, parameters, parent = None ):


    QtWidgets.QDialog.__init__( self, parent )

    self._parameters = parameters
    self._updatedParameters = {}
    self._selectableSubjects = []

    self.findRegularExpression()

    self._subjectSelection = None

    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                           'core',
                                           'MultiSubjectSelectionDialog.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )

    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )

    self._lineEditSingleSubjectName = self._findChild( self._root,
                                                  'lineEdit_SingleSubjectName' )
    self._lineEditSingleSubjectName.setText( self._singleSubjectName )
    self._listWidgetSubjectSelection = self._findChild( self._root,
                                               'listWidget_SubjectSelection' )
    self._listWidgetParameters = self._findChild( self._root,
                                               'listWidget_Parameters' )
    self._listWidgetSubjectSelection.setSelectionMode( \
                                   QtWidgets.QAbstractItemView.ExtendedSelection )
    buttonBoxOkCancel = self._findChild( self._root,
                                               'buttonBox_OkCancel' )
    self._lineEditSingleSubjectName.textChanged.connect(
                        self.singleSubjectNameCallback )

    self._listWidgetSubjectSelection.itemSelectionChanged.connect(
                        self.checkSingleSubjectParameters )

    self.setLayout( self._layout )

    self.setWindowTitle( 'Multi-subject selection' )

    buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                             ).clicked.connect( self.accept )
    buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                             ).clicked.connect( self.reject )

    self.setWindowModality( QtCore.Qt.WindowModal )

    if ( len( self._singleSubjectName ) ):

      self.updateAvailableSubjects()


  def findRegularExpression( self ):

    key = None
    regularExpressionType = 0
    for k in self._parameters.keys():

      if ( self._parameters[ k ].getType() == 'string' ):

        if ( ( self._parameters[ k ].getValue().count( '{' ) > 0 ) and \
             ( self._parameters[ k ].getValue().count( '}' ) > 0 ) ):

          key = k
          regularExpressionType = 1
          break
          
        elif ( self._parameters[ k ].getValue().count( '*' ) > 0 ):
        
          key = k
          regularExpressionType = 2
          break

    if key is not None :

      if ( regularExpressionType == 1 ):
      
        regularExpressionString = '{(.+)}'
        iterationCount = self._parameters[ key ].getValue().count( '{' ) - 1
        for x in xrange( iterationCount ):

          regularExpressionString += '(.+){(.+)}'

        pattern = re.compile( regularExpressionString )
        match = pattern.search( self._parameters[ key ].getValue() )
        self._singleSubjectName = '{' + match.group( 1 ) + '}'
        
      else:
      
        self._singleSubjectName = '*'

    else:

      self._singleSubjectName = ''


  def singleSubjectNameCallback( self, singleSubjectName ):
  
    self._singleSubjectName = singleSubjectName
    self.updateAvailableSubjects()


  def updateAvailableSubjects( self ):

    # automatically detecting all subjects
    subjects = []

    if ( len( self._singleSubjectName ) ):

      subjectParameters = []

      # getting subject name(s)
      firstMultiSubjectParameter = None

      for k in self._parameters.keys():

        if ( self._parameters[ k ].getType() == 'string' ):
        
          value = ''
          if self._parameters[ k ].getWidgetType() == 'listWidget':

            value = self._parameters[ k ].getValue().split()[ 0 ]

          else:

            value = self._parameters[ k ].getValue().split( ';' )[ 0 ]

          regularExpressionCount = value.count( self._singleSubjectName )
          if ( ( regularExpressionCount > 0 ) and \
               ( not self._parameters[ k ].isAnOutputParameter() ) and \
               ( self._parameters[ k ].isAPathParameter() ) ):

            firstMultiSubjectParameter = self._parameters[ k ]
            break
            
      if ( firstMultiSubjectParameter is not None 
                                              and regularExpressionCount < 15 ):

        # get initial subjects
        regularExpression = ''
        rootPath = ''
        for r in xrange( regularExpressionCount ):

          regularExpression += '(.+)'
          if ( self._singleSubjectName == '*' ):
          
            regularExpression = regularExpression + '\*'

          else:
          
            if( str( self._singleSubjectName )[ 0 ] == '{' ):

              regularExpression = regularExpression + '\\' + \
                                  str( self._singleSubjectName )

            else:

              regularExpression = regularExpression + \
                                  str( self._singleSubjectName )

        s = firstMultiSubjectParameter.getValue().split( ';' )[ 0 ]  
        if firstMultiSubjectParameter.getWidgetType() == 'listWidget':

          s = firstMultiSubjectParameter.getValue().split()[ 0 ]

        pattern = re.compile( regularExpression + '(.+)' )
        match = pattern.search( s )
        if ( match == None ):

          pattern = re.compile( regularExpression )
          match = pattern.search( s )


        if ( match is not None ):

          path =  os.path.split( match.group( 1 ) )
          if ( os.path.exists( path[ 0 ] ) ):

            files = os.listdir( path[ 0 ] )
            if ( path[ 1 ] == '' ): #and match.group( 2 ).find( '/' ) == 0 ):

              for file in files:

                if ( os.path.isdir( os.path.join( path[ 0 ] , file ) ) ):

                  subjects.append( file )

            else:

              #if( match.group( 2 ) == '' ):
              #
              #  regularExpression = match.group( 1 ) + '(.+)'
              #
              #else:
              #
              #  regularExpression = match.group( 1 ) + '(.+)' + match.group( 2 )

              if( ( len( match.groups() ) > 1 ) and ( match.group( 2 ) == '' ) ):

                regularExpression = match.group( 1 ) + '(.+)'

              elif ( len( match.groups() ) > 1 ):

                regularExpression = match.group( 1 ) + '(.+)' + match.group( 2 )

              pattern = re.compile( regularExpression  )

              for file in files:

                match = pattern.search( os.path.join( path[ 0 ], file ) )

                if ( match != None ):

                  subjects.append( match.group( 1 ) )

    subjectSet = set( subjects )
    
    subjects = []
    for x in xrange( len( subjectSet ) ):

      subjects.append( subjectSet.pop())

    # updating the list widget

    self._listWidgetSubjectSelection.clear()
    self._listWidgetParameters.clear()
    self._selectableSubjects = []
    for s in subjects:

      item = QtWidgets.QListWidgetItem( s )
      self._listWidgetSubjectSelection.insertItem( 0, item )
      if not self.hasAllParametersAvailable( s ) :

        item.setBackground( QtGui.QBrush( QtGui.QColor( 255, 215, 215 ) ) )

      else:

        item.setBackground( QtGui.QBrush( QtGui.QColor( 220, 255, 220 ) ) )
        self._selectableSubjects.append( s )

    self._listWidgetSubjectSelection.sortItems()
    self._listWidgetSubjectSelection.setCurrentRow( 0 )


  def hasAllParametersAvailable( self, subject ):

    allParametersFound = True
    for k in self._parameters.keys():

      if ( self._parameters[ k ].getType() == 'string' ):

        regularExpressionCount = self._parameters[ k ].getValue().count( \
                                                       self._singleSubjectName )

        if ( ( regularExpressionCount > 0 ) and \
             ( not self._parameters[ k ].isAnOutputParameter() ) and \
             self._parameters[ k ].isAPathParameter() ):

          path = self._parameters[ k ].getValue().replace( \
                                                        self._singleSubjectName,
                                                        subject )
          if ( self._parameters[ k ].getWidgetType() == 'listWidget' ):

            paths = path.split()
            for path in paths:

              if not os.path.exists( path ):

                allParametersFound = False
                break

          else:

            paths = path.split( ';' )
            for path in paths:

              if not os.path.exists( path ):

                allParametersFound = False
                break

    return allParametersFound


  def checkSingleSubjectParameters( self ):

    subject = self._listWidgetSubjectSelection.currentItem().text()
    self._listWidgetParameters.clear()
    row = 0
    for k in self._parameters.keys():

      parameter = self._parameters[ k ]
      if ( parameter.getType() == 'string' ):

        regularExpressionCount = parameter.getValue().count( \
                                                       self._singleSubjectName )

        if ( ( regularExpressionCount > 0 ) and \
             ( not parameter.isAnOutputParameter() ) and \
             parameter.isAPathParameter() ):

          itemLabel = QtWidgets.QListWidgetItem( k + ':' )
          itemLabel.setFont( QtGui.QFont( 'Sans Serif',
                                          7,
                                          QtGui.QFont.Bold,
                                          False ) )
          self._listWidgetParameters.insertItem( row, itemLabel )
          row += 1
          path = parameter.getValue().replace( self._singleSubjectName, 
                                               subject )
          if ( parameter.getWidgetType() == 'listWidget' ):

            paths = path.split()
            for path in paths:

              item = QtWidgets.QListWidgetItem( '   ' + path )
              self._listWidgetParameters.insertItem( row, item )
              row += 1
              if os.path.exists( path ):

                item.setBackground( QtGui.QBrush( QtGui.QColor( 220,
                                                                255,
                                                                220 ) ) )
                item.setText( item.text() + '  ... ok' )
                itemLabel.setBackground( QtGui.QBrush( QtGui.QColor( 220,
                                                                     255,
                                                                     220 ) ) )

              else:

                item.setBackground( QtGui.QBrush( QtGui.QColor( 255,
                                                                215,
                                                                215 ) ) )
                item.setText( item.text() + '  ... not found' )
                itemLabel.setBackground( QtGui.QBrush( QtGui.QColor( 255,
                                                                     215,
                                                                     215 ) ) )
          else:

            item = QtWidgets.QListWidgetItem( '   ' + path )
            self._listWidgetParameters.insertItem( row, item )
            row += 1
            pathFound = True
            paths = path.split( ';' )
            for path in paths:

              if not os.path.exists( path ):

                pathFound = False

            if pathFound:

              item.setBackground( QtGui.QBrush( QtGui.QColor( 220,
                                                              255,
                                                              220 ) ) )
              item.setText( item.text() + '  ... ok' )
              itemLabel.setBackground( QtGui.QBrush( QtGui.QColor( 220,
                                                                   255,
                                                                   220 ) ) )

            else:

              item.setBackground( QtGui.QBrush( QtGui.QColor( 255,
                                                              215,
                                                              215 ) ) )
              item.setText( item.text() + '  ... not found' )
              itemLabel.setBackground( QtGui.QBrush( QtGui.QColor( 255,
                                                                   215,
                                                                   215 ) ) )


  def exec_( self ):

    self.show()
    self.raise_()
    return QtWidgets.QDialog.exec_( self )


  def getUpdatedParameters( self ):

    # building the selection of subject(s)
    selectedSubjects = list()
    selectedItems = self._listWidgetSubjectSelection.selectedItems()
    invalidSubjects = []
    for selectedItem in selectedItems:

      subject = selectedItem.text()
      if ( self._selectableSubjects.count( subject ) > 0 ):

        selectedSubjects.append( subject )

      else:

        invalidSubjects.append( subject )

    invalidSubjectCount = len( invalidSubjects )
    if invalidSubjectCount:

      if ( invalidSubjectCount == 1 ):

        message = 'Warning! Subject ' + invalidSubjects[ 0 ] + ' invalid. \n'
        message = message + 'It will be ignored.'

      else:

        message = 'Subjects: \n'
        subjectNames = ''
        for invalidSubject in invalidSubjects:

          subjectNames = subjectNames + '  - ' + invalidSubject + '\n'

        message = message + subjectNames + ' are invalid. \n'
        message = message + 'They will be ignored.'

      msgBox = QtWidgets.QMessageBox.warning( self,
                                         QtCore.QString( 'Invalid Subject(s)'),
                                         QtCore.QString( message ) )

    # building the new parameter dictionary
    newParameters = {}
    if ( len( selectedSubjects ) ):

      subjectList = '{'
      for subject in selectedSubjects:

        subjectList += subject
        subjectList += ','
      subjectList = subjectList[ 0: len( subjectList ) - 1 ] + '}'


      self._subjectSelection = subjectList
      for k in self._parameters.keys():
    
        parameter = self._parameters[ k ]

        if ( parameter.getType() == 'chainList' ):

          for singleParameter in parameter._parameters:

            for sp in singleParameter.keys():

              if not( sp == 'processName' ):

                if ( singleParameter[ sp ].getType() == 'string' ):

                  singleSubjectNameCount = \
                                       singleParameter[ sp ].getValue().count( \
                                       str( self._singleSubjectName ) )

                  if ( singleSubjectNameCount > 0 ):

                    newValue = singleParameter[ sp ].getValue().replace( \
                                                     self._singleSubjectName,
                                                     subjectList )
                    singleParameter[ sp ].setValue( newValue )

          newParameters[ k ] = parameter

        elif ( parameter.getType() == 'string' ):

          singleSubjectNameCount = parameter.getValue().count( \
                                                str( self._singleSubjectName ) )
          if ( singleSubjectNameCount > 0 ):

            newValue = parameter.getValue().replace( self._singleSubjectName,
                                                     subjectList )
            newParameters[ k ] = parameter
            newParameters[ k ].setValue( newValue )

          else:

            if parameter.isAnOutputParameter():

              newParameters[ k ] = parameter
              newParameters[ k ].setValue( os.path.join( \
                                                    str( parameter.getValue() ),
                                                    str( subjectList ) ) )
    
    return newParameters


  def getSubjectSelection( self ):

    return self._subjectSelection


  def getSingleSubjectName( self ):

    return self._singleSubjectName
