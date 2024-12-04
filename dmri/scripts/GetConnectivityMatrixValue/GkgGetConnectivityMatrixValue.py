import os, sys


################################################################################
# main
################################################################################


# in case of help
if ( ( len( sys.argv ) >= 2 ) and ( sys.argv[ 1 ] != '-h' ) ):

  fileNameConnectivityMatrix = sys.argv[ 1 ]
  labelFrom = sys.argv[ 2 ]
  labelTo = sys.argv[ 3 ]
  
  if ( not os.path.exists( fileNameConnectivityMatrix ) ):
  
    message = '\'' + fileNameConnectivityMatrix + '\' does not exist:\n' + \
              'Stop here because mandatory file has not been found'
    raise RuntimeError, message
    
  f = open( fileNameConnectivityMatrix, 'r' )
  fileData = f.read()
  f.close()
    
  fileDataSplitted = fileData.split( ' ' )
    

  # sanity check
  if ( len ( fileDataSplitted ) < 3 ):
      
    print 'Error while reading ', fileName, 'Expecting at least 3 data in it'
    
  # matrix syntax:
  # dim1 dim2 nbPairs labelx labely value labelx labely value...
    
  firstIndexInList = 3
  labelPairCount = int( fileDataSplitted[ 2 ] )
    
  xlabels = set()
  ylabels = set()
    
  setw = 9
  labelCountMaximumSize = firstIndexInList + 3 * labelPairCount
    
  # find label names
  for i in range( firstIndexInList, labelCountMaximumSize ):
      
      
    dataRead = fileDataSplitted[ i ]
    indexRead = i - firstIndexInList
      
    if ( dataRead == '' ):
        
      continue
      
    # Find x labels
    if ( indexRead % 3 == 1 ):
        
      xlabels.add( dataRead )
      
    # Find y labels
    elif ( indexRead % 3 == 0 ):
        
      ylabels.add( dataRead )
    
  # sort labels using value of the string    
  xlabels = sorted( xlabels, key = lambda v: float( v ) )
  ylabels = sorted( ylabels, key = lambda v: float( v ) )

  # create dictionary to quickly get label indexes
  dictXLabelIndices = {}
  dictYLabelIndices = {}
    
  count = 0
  for label in xlabels:
      
    dictXLabelIndices[ label ] = count
    count += 1
    
  count = 0
  for label in ylabels:
      
    dictYLabelIndices[ label ] = count
    count += 1


  # fill matrix with 0 (sparse matrix)
  matrixDims = [ len( ylabels ), len( xlabels ) ]
  dataMatrix = zeros( matrixDims )
    
  # fill matrix with connectivity values
    
  # matrix indexes
  xIndex = 0
  yIndex = 0
    
    
  # Find label names
  for i in range( firstIndexInList, labelCountMaximumSize ):
      
    # Sanity check
    if ( dataRead == '' ):
        
      continue
        
    dataRead = fileDataSplitted[ i ]
    indexRead = i - firstIndexInList
      
    # Get x label index
    if ( indexRead % 3 == 1 ):
            
      xIndex = dictXLabelIndices[ dataRead ]  
          
    # Get y label index
    elif ( indexRead % 3 == 0 ):
            
      yIndex = dictYLabelIndices[ dataRead ]
        
    # Get connectivity value
    else:
        
      dataMatrix[ yIndex, xIndex ] = float( dataRead )
  
  print labelFrom, '<->', labelTo, ' : ', \
        dataMatrix[ dictYLabelIndices[ labelTo ], \
                    dictXLabelIndices[ labelFrom ] ]

else:
  
  print 'GkgGetConnectivityMatrixValue <connectivity-matrix> <label1> <label2>'
    
  
