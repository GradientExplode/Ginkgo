import os
import time
import math
from Singleton import *
from VolumeInformationHandler import *

import anatomist.direct.api as ana
from anatomist.cpp.palettecontrastaction import PaletteContrastAction
from soma import aims
from soma.aims import colormaphints

import matplotlib

matplotlib.use( 'Qt5Agg' )
import matplotlib.pyplot as plt

class SnapshotTool( object, metaclass = Singleton ):
  
  def __init__( self ):

    # start the Anatomist engine, in batch mode (no main window)
    self._anatomist = ana.Anatomist( '-b' )

  def snapitSingleVolume( self,
                          subjectName,
                          fileNameVolume,
                          colorMapName,
                          lowerValue, upperValue,
                          windowSize,
                          fileNameSnapshotAxial,
                          fileNameSnapshotCoronal,
                          fileNameSnapshotSagittal ):

    anatomistWindowAxial = self._anatomist.createWindow( wintype='Axial' )
    anatomistWindowCoronal = self._anatomist.createWindow( wintype='Coronal' )
    anatomistWindowSagittal = self._anatomist.createWindow( \
                                                           wintype='Sagittal' )
                                                                                                   
    # removing cursor
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             cursor_visibility = 0 )

    # do not display toolbar
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             show_toolbars = 0 )

    # setting window size
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             view_size = windowSize )

    # setting background color
    backGroundColor = [ 0, 0, 0 ]
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             light = { 'background' : backGroundColor } )

    # creating object list
    objects = list()

    # loading volume
    volume = self._anatomist.loadObject( fileNameVolume )
    if ( colorMapName is not None ):
    
      if ( ( lowerValue is not None ) and ( upperValue is not None ) ):
      
        volume.setPalette( palette = colorMapName,  \
                           minVal = lowerValue, \
                           maxVal = upperValue, \
                           absoluteMode = True )
        
      else:

        volume.setPalette( palette = colorMapName )

    # adding volume to object list
    objects.append( volume )

    # adding objects to windows
    self._anatomist.addObjects( objects, [ anatomistWindowAxial,
                                           anatomistWindowCoronal,
                                           anatomistWindowSagittal ] )    

    if ( volume.objectType == 'VOLUME' ):
    
      cursorPosition = [ getSizeXOfVolume( fileNameVolume ) *
                         getResolutionXOfVolume( fileNameVolume ) / 2.0,
                         getSizeYOfVolume( fileNameVolume ) *
                         getResolutionYOfVolume( fileNameVolume ) / 2.0,
                         getSizeZOfVolume( fileNameVolume ) *
                         getResolutionZOfVolume( fileNameVolume ) / 2.0,
                         getSizeTOfVolume( fileNameVolume ) *
                         getResolutionTOfVolume( fileNameVolume ) / 2.0 ]

      anatomistWindowAxial.moveLinkedCursor( cursorPosition )
      anatomistWindowCoronal.moveLinkedCursor( cursorPosition )
      anatomistWindowSagittal.moveLinkedCursor( cursorPosition )

    # taking snapshot(s)
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial ],
                             snapshot = fileNameSnapshotAxial )
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowCoronal ],
                             snapshot = fileNameSnapshotCoronal )
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowSagittal ],
                             snapshot = fileNameSnapshotSagittal )


  def snapitVolumeFusion( self,
                          subjectName,
                          fileNameVolume1,
                          colorMapName1,
                          lowerValue1, upperValue1,
                          fileNameVolume2,
                          colorMapName2,
                          lowerValue2, upperValue2,
                          fileNameTransformationVolume2ToVolume1,
                          windowSize,
                          fileNameSnapshotAxial,
                          fileNameSnapshotCoronal,
                          fileNameSnapshotSagittal ):
                                
    anatomistWindowAxial = self._anatomist.createWindow( wintype='Axial' )
    anatomistWindowCoronal = self._anatomist.createWindow( wintype='Coronal' )
    anatomistWindowSagittal = self._anatomist.createWindow( \
                                                           wintype='Sagittal' )
                                                                                                   
    # removing cursor
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             cursor_visibility = 0 )

    # do not display toolbar
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             show_toolbars = 0 )

    # setting window size
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             view_size = windowSize )

    # setting background color
    backGroundColor = [ 0, 0, 0 ]
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             light = { 'background' : backGroundColor } )
           
    # creating object list
    objects = list()

    # loading volume1
    volume1 = self._anatomist.loadObject( fileNameVolume1 )
    if ( colorMapName1 is not None ):
    
      if ( ( lowerValue1 is not None ) and ( upperValue1 is not None ) ):
      
        volume1.setPalette( palette = colorMapName,  \
                           minVal = lowerValue1, \
                           maxVal = upperValue1, \
                           absoluteMode = True )
        
      else:

        volume1.setPalette( palette = colorMapName1 )

    # creating volume1 referential
    volume1Referential = self._anatomist.createReferential()
    self._anatomist.assignReferential( volume1Referential, volume1 )

    # loading volume2
    volume2 = self._anatomist.loadObject( fileNameVolume2 )
    if ( colorMapName2 is not None ):
    
      if ( ( lowerValue2 is not None ) and ( upperValue2 is not None ) ):
      
        volume2.setPalette( palette = colorMapName2,  \
                            minVal = lowerValue2, \
                            maxVal = upperValue2, \
                            absoluteMode = True )
        
      else:

        volume2.setPalette( palette = colorMapName2 )

    # creating volume2 referential
    volume2Referential = self._anatomist.createReferential()
    self._anatomist.assignReferential( volume2Referential, volume2 )

    # fusionning volume1 and volume2
    fusionObjects = [ volume1, volume2 ]
    fusion = self._anatomist.fusionObjects( fusionObjects,
                                            method = 'Fusion2DMethod' )
    self._anatomist.execute( 'Fusion2DParams', 
                             object = fusion,
                             mode = 'Fusion2DMethod',
                             rate = 0.5 )

    # loading volume2 to volume1 transformation
    self._anatomist.loadTransformation( fileNameTransformationVolume2ToVolume1,
                                        volume2Referential,
                                        volume1Referential )
    self._anatomist.assignReferential( volume1Referential,
                                       anatomistWindowAxial )
    self._anatomist.assignReferential( volume1Referential,
                                       anatomistWindowCoronal )
    self._anatomist.assignReferential( volume1Referential,
                                       anatomistWindowSagittal )

    # adding fusion of volumes to object list
    objects.append( fusion )

    # adding objects to windows
    self._anatomist.addObjects( objects, [ anatomistWindowAxial,
                                           anatomistWindowCoronal,
                                           anatomistWindowSagittal ] )    
    
    if ( volume1.objectType == 'VOLUME' ):
    
      cursorPosition = [ getSizeXOfVolume( fileNameVolume1 ) *
                         getResolutionXOfVolume( fileNameVolume1 ) / 2.0,
                         getSizeYOfVolume( fileNameVolume1 ) *
                         getResolutionYOfVolume( fileNameVolume1 ) / 2.0,
                         getSizeZOfVolume( fileNameVolume1 ) *
                         getResolutionZOfVolume( fileNameVolume1 ) / 2.0,
                         getSizeTOfVolume( fileNameVolume1 ) *
                         getResolutionTOfVolume( fileNameVolume1 ) / 2.0 ]

      anatomistWindowAxial.moveLinkedCursor( cursorPosition )
      anatomistWindowCoronal.moveLinkedCursor( cursorPosition )
      anatomistWindowSagittal.moveLinkedCursor( cursorPosition )

    # taking snapshot(s)
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial ],
                             snapshot = fileNameSnapshotAxial )
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowCoronal ],
                             snapshot = fileNameSnapshotCoronal )
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowSagittal ],
                             snapshot = fileNameSnapshotSagittal )


  def snapitVolumeFusionAndOdfField( self,
                          subjectName,
                          fileNameVolume1,
                          colorMapName1,
                          lowerValue1, upperValue1,
                          fileNameVolume2,
                          colorMapName2,
                          lowerValue2, upperValue2,
                          fileNameOdfField,
                          fileNameTransformationVolume2ToVolume1,
                          fileNameTransformationOdfFieldToVolume1,
                          windowSize,
                          zoom,
                          fileNameSnapshotAxial,
                          fileNameSnapshotCoronal,
                          fileNameSnapshotSagittal ):
                                
    anatomistWindowAxial = self._anatomist.createWindow( wintype='Axial' )
    anatomistWindowCoronal = self._anatomist.createWindow( wintype='Coronal' )
    anatomistWindowSagittal = self._anatomist.createWindow( \
                                                           wintype='Sagittal' )
                                                                                                   
    # removing cursor
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             cursor_visibility = 0 )

    # do not display toolbar
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             show_toolbars = 0 )

    # setting window size
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             view_size = windowSize )

    # setting background color
    backGroundColor = [ 0, 0, 0 ]
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial,
                                         anatomistWindowCoronal,
                                         anatomistWindowSagittal ],
                             light = { 'background' : backGroundColor } )
           
    # creating object list
    objects = list()

    # loading volume1
    volume1 = self._anatomist.loadObject( fileNameVolume1 )
    if ( colorMapName1 is not None ):
    
      if ( ( lowerValue1 is not None ) and ( upperValue1 is not None ) ):
      
        volume1.setPalette( palette = colorMapName1,  \
                            minVal = lowerValue1, \
                            maxVal = upperValue1, \
                            absoluteMode = True )
        
      else:

        volume1.setPalette( palette = colorMapName1 )

    # creating volume1 referential
    volume1Referential = self._anatomist.createReferential()
    self._anatomist.assignReferential( volume1Referential, volume1 )

    # loading volume2
    volume2 = self._anatomist.loadObject( fileNameVolume2 )
    if ( colorMapName2 is not None ):
    
      if ( ( lowerValue2 is not None ) and ( upperValue2 is not None ) ):
      
        volume2.setPalette( palette = colorMapName2,  \
                            minVal = lowerValue2, \
                            maxVal = upperValue2, \
                            absoluteMode = True )
        
      else:

        volume2.setPalette( palette = colorMapName2 )

    # creating volume2 referential
    volume2Referential = self._anatomist.createReferential()
    self._anatomist.assignReferential( volume2Referential, volume2 )

    # loading ODF field
    odfField = self._anatomist.loadObject( fileNameOdfField )

    # creating ODF field referential
    odfFieldReferential = self._anatomist.createReferential()
    self._anatomist.assignReferential( odfFieldReferential, odfField )


    # fusionning volume1 and volume2
    fusionObjects = [ volume1, volume2 ]
    fusion = self._anatomist.fusionObjects( fusionObjects,
                                            method = 'Fusion2DMethod' )
    self._anatomist.execute( 'Fusion2DParams', 
                             object = fusion,
                             mode = 'Fusion2DMethod',
                             rate = 0.5 )

    # loading volume2 to volume1 transformation
    if ( fileNameTransformationVolume2ToVolume1 is not None ):

      self._anatomist.loadTransformation( \
                                        fileNameTransformationVolume2ToVolume1,
                                        volume2Referential,
                                        volume1Referential )
                                        
    self._anatomist.assignReferential( volume1Referential,
                                       anatomistWindowAxial )
    self._anatomist.assignReferential( volume1Referential,
                                       anatomistWindowCoronal )
    self._anatomist.assignReferential( volume1Referential,
                                       anatomistWindowSagittal )

    # loading ODF field to volume1 transformation
    if ( fileNameTransformationOdfFieldToVolume1 is not None ):

      self._anatomist.loadTransformation( \
                                        fileNameTransformationOdfFieldToVolume1,
                                        odfFieldReferential,
                                        volume1Referential )

    # adding fusion of volumes to object list
    objects.append( fusion )

    # adding ODF field to object list
    objects.append( odfField )

    # adding objects to windows
    self._anatomist.addObjects( objects, [ anatomistWindowAxial,
                                           anatomistWindowCoronal,
                                           anatomistWindowSagittal ] )    
    
    if ( volume1.objectType == 'VOLUME' ):
    
      cursorPosition = [ getSizeXOfVolume( fileNameVolume1 ) *
                         getResolutionXOfVolume( fileNameVolume1 ) / 2.0,
                         getSizeYOfVolume( fileNameVolume1 ) *
                         getResolutionYOfVolume( fileNameVolume1 ) / 2.0,
                         getSizeZOfVolume( fileNameVolume1 ) *
                         getResolutionZOfVolume( fileNameVolume1 ) / 2.0,
                         getSizeTOfVolume( fileNameVolume1 ) *
                         getResolutionTOfVolume( fileNameVolume1 ) / 2.0 ]

      anatomistWindowAxial.moveLinkedCursor( cursorPosition )
      anatomistWindowCoronal.moveLinkedCursor( cursorPosition )
      anatomistWindowSagittal.moveLinkedCursor( cursorPosition )

    # setting zoom
    anatomistWindowAxial.camera( zoom = zoom )
    anatomistWindowCoronal.camera( zoom = zoom )
    anatomistWindowSagittal.camera( zoom = zoom )

    # taking snapshot(s)
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowAxial ],
                             snapshot = fileNameSnapshotAxial )
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowCoronal ],
                             snapshot = fileNameSnapshotCoronal )
    self._anatomist.execute( 'WindowConfig',
                             windows = [ anatomistWindowSagittal ],
                             snapshot = fileNameSnapshotSagittal )

  def snapitHistogram( self,
                       subjectName,
                       xs,
                       hs,
                       xLabel,
                       yLabel,
                       fileNameSnapshotHistogram ):
                               
    fig = plt.figure()

    plt.plot( xs, hs, lw=1 )
    if ( xLabel is not None ):
    
      plt.xlabel( xLabel )
    if ( yLabel is not None ):
    
      plt.ylabel( yLabel )
    plt.grid( True )
    plt.savefig( fileNameSnapshotHistogram )


  def snapitMotionProfile( self,
                           subjectName,
                           translationX,
                           translationY,
                           translationZ,
                           rotationX,
                           rotationY,
                           rotationZ,
                           scalingX,
                           scalingY,
                           scalingZ,
                           shearingXY,
                           shearingXZ,
                           shearingYZ,
                           fileNameSnapshotMotionProfile ):

    xs = range( 0, len( translationX ) )
    for x in xs:

      rotationX[ x ] *= 180.0 / math.pi
      rotationY[ x ] *= 180.0 / math.pi
      rotationZ[ x ] *= 180.0 / math.pi
                              
    fig = plt.figure()

    fig.set_size_inches( 18.5, 10.5 )

    plt.subplot( 4, 1, 1 )
    plt.plot( xs, translationX, lw=1, label='X' )
    plt.plot( xs, translationY, lw=1, label='Y' )
    plt.plot( xs, translationZ, lw=1, label='Z' )
    plt.grid( True )
    plt.legend()
    plt.ylabel( 'translation(mm)' )

    plt.subplot( 4, 1, 2 )
    plt.plot( xs, rotationX, lw=1, label='X' )
    plt.plot( xs, rotationY, lw=1, label='Y' )
    plt.plot( xs, rotationZ, lw=1, label='Z' )
    plt.grid( True )
    plt.legend()
    plt.ylabel( 'rotation(deg)' )

    plt.subplot( 4, 1, 3 )
    plt.plot( xs, scalingX, lw=1, label='X' )
    plt.plot( xs, scalingY, lw=1, label='Y' )
    plt.plot( xs, scalingZ, lw=1, label='Z' )
    plt.grid( True )
    plt.legend()
    plt.ylabel( 'scaling' )

    plt.subplot( 4, 1, 4 )
    plt.plot( xs, shearingXY, lw=1, label='XY' )
    plt.plot( xs, shearingXZ, lw=1, label='XZ' )
    plt.plot( xs, shearingYZ, lw=1, label='YZ' )
    plt.grid( True )
    plt.legend()
    plt.ylabel( 'shearing' )

    plt.xlabel( 'diffusion orientation index' )

    plt.savefig( fileNameSnapshotMotionProfile, dpi=300 )


  def snapitOutliers( self,
                      subjectName,
                      orientations,
                      slices,
                      qSpacePointCount,
                      sliceCount,
                      fileNameSnapshotOutliers ):

    fig = plt.figure()

    fig.set_size_inches( 18.5, 10.5 )

    plt.plot( orientations, slices, 'o', label='Outliers'  )
    plt.grid( True )
    plt.xlabel( 'q-space sample index' )
    plt.ylabel( 'slice index' )
    axes = plt.gca()
    axes.set_xlim( [ 0, qSpacePointCount ] )
    axes.set_ylim( [ 0, sliceCount ] )
    axes.set_autoscale_on( False )

    plt.savefig( fileNameSnapshotOutliers, dpi=300 )


