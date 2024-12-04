import os
import re


def isNiftiFile( fileName ):

  extensions = [ '.nii', \
                 '.nii.minf', \
                 '.nii.gz' ]
  fileNameLength = len( fileName )
  for e in extensions:
  
    extensionLength = len( e )
    if ( fileNameLength >= extensionLength ):

      if ( fileName[ fileNameLength - extensionLength: ] == e ):
  
        return True
        
  return False

def isGisFile( fileName ):

  extensions = [ '.ima', \
                 '.ima.minf' ]
  fileNameLength = len( fileName )
  for e in extensions:
  
    extensionLength = len( e )
    if ( fileNameLength >= extensionLength ):

      if ( fileName[ fileNameLength - extensionLength: ] == e ):
  
        return True
        
  return False


def getNiftiPath( fileName ):

  return getPathWithoutExtension( fileName ) + '.nii'


def getGisPath( fileName ):

  return getPathWithoutExtension( fileName ) + '.ima'


def getPathWithoutExtension( fileName, \
                             furtherExtensions = None ):

  extensions = [ '.ima', \
                 '.dim', \
                 '.ima.minf', \
                 '.nii', \
                 '.nii.minf', \
                 '.nii.gz', \
                 '.nii.gz.minf', \
                 '.bundles', \
                 '.bundlesdata' ]
  if ( furtherExtensions is not None ):
  
    extensions = extensions + furtherExtensions
    
  fileNameLength = len( fileName )
  for e in extensions:
  
    extensionLength = len( e )
    if ( fileNameLength >= extensionLength ):

      if ( fileName[ fileNameLength - extensionLength: ] == e ):
        
        fileNameWithoutExtension = \
                                fileName[ 0 : fileNameLength - extensionLength ]
        return fileNameWithoutExtension
        
  return fileName


def getExtension( fileName, furtherExtensions = None ):

  extensions = [ '.ima', \
                 '.dim', \
                 '.ima.minf', \
                 '.nii', \
                 '.nii.minf', \
                 '.nii.gz', \
                 '.nii.gz.minf', \
                 '.bundles', \
                 '.bundlesdata', \
                 '.bundlemap', \
                 '.bundlemap.minf', \
                 '.tex', \
                 '.tex.minf', \
                 '.texturemap', \
                 '.texturemap.minf', \
                 '.sitemap', \
                 '.sitemap.minf', \
                 '.polygonmap', \
                 '.polygonmap.minf', \
                 '.meshmap', \
                 '.meshmap.minf', \
                 '.mesh', \
                 '.mesh.minf', \
                 '.tri', \
                 '.tri.minf', \
                 '.hie' \
               ]
  if ( furtherExtensions is not None ):
  
    extensions = extensions + furtherExtensions
    
  fileNameLength = len( fileName )
  for e in extensions:
  
    extensionLength = len( e )
    if ( fileNameLength >= extensionLength ):

      if ( fileName[ fileNameLength - extensionLength: ] == e ):
          
        return e
        
  return None


def getBaseNameWithoutExtension( fileName, \
                                 furtherExtensions = None ):

  return os.path.basename( getPathWithoutExtension( fileName,
                                                    furtherExtensions ) )


def getSingleSubjectPath( path ):

    subjectNames = []
    # getting subject names
    firstMultiSubjectParameterKey = None
    firstMultiSubjectParameter = None
    firstRegularExpressionCount = 0
    singleSubjectPath = path

    regularExpressionCount = singleSubjectPath.count( '*' ) + \
             ( singleSubjectPath.count( '{' ) * singleSubjectPath.count( '}' ) )

    if ( regularExpressionCount > 0 ):

      # get initial subjects
      regularExpressionSubject = 'noMultiSubject'
      if ( singleSubjectPath.count( '{' ) and singleSubjectPath.count( '}' )):

        regularExpression = '(.+){(.+)}'
        pattern = re.compile( regularExpression )
        match = pattern.search( singleSubjectPath )
        subjectsString = match.group( 2 )
        subjectNames = subjectsString.split( ',' )
        regularExpressionSubject = '{' + subjectsString + '}'
        singleSubjectPath = singleSubjectPath.replace( regularExpressionSubject,
                                                       subjectNames[ 0 ] )

      else:

        regularExpressionSubject = '*'
        regularExpression = '(.+)'
        rootPath = ''
        regularExpressionCount = singleSubjectPath.count( '*' )
        for r in xrange( regularExpressionCount ):

          regularExpression = regularExpression + '\*(.+)'

        pattern = re.compile( regularExpression )
        match = pattern.search( singleSubjectPath )
        if ( match is None ):

          pattern = re.compile( '(.+)\*' )
          match = pattern.search( singleSubjectPath )

        if ( match is None ):

          message = singleSubjectPath + ' is not an existing path'
          raise RuntimeError, message

        subjectNames = []
        rootPath =  os.path.split( str( match.group( 1 ) ) )
        files = os.listdir( rootPath[ 0 ] )
        if ( path[ 1 ] == '' ):

          subjectNames = files

        else:

          pathParts = str( match.group( 1 ) ).split( os.sep )
          print pathParts
          pathParts.reverse()
          first = pathParts[ 0 ]
          if ( len( match.groups() ) >= 2 ):

            second = match.group( 2 ).split( os.sep  )[ 0 ]

          else:

            second = ''

          for file in files:

            groups = re.search( '(' + first + ')(.+)(' + second + ')', file )
            if ( groups is not None ):

              subjectNames.append( groups.group( 2 ) )

          subjectNamesSet = set( subjectNames )
          subjectNames = []
          for i in range( 0, len( subjectNamesSet ) ):

            subjectNames.append( subjectNamesSet.pop() )

        singleSubjectPath = singleSubjectPath.replace( \
                                   regularExpressionSubject, subjectNames[ 0 ] )
    
    return singleSubjectPath

  

