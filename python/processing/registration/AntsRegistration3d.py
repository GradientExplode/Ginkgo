from core.command.Command import *
from core.command.CommandFactory import *
import glob
from array import array


################################################################################
#
# inputs:
#   - fileNameReferenceVolume
#   - fileNameFloatingVolume
#   - transformationType among rigid / affine / affine_and_diffeomorphic /
#                              rigid_and_diffeomorphic / diffeomorphic
#   - intermediate
#   - furtherANTsOptions
#   - verbose
#
#
# outputs:
#   - fileNameDirectTransform3d
#   - fileNameInverseTransform3d
#   - fileNameTransformedReferenceVolume
#   - fileNameTransformedFloatingVolume
#
################################################################################


class AntsRegistration3dCommand( Command ):

  def __init__( self ):
  
    Command.__init__( self )


    
  def execute( self, parameters ):
  
    ############################################################################
    # collecting the parameter(s)
    ############################################################################
    
    fileNameReferenceVolume = self.getParameter( "fileNameReferenceVolume",
                                                 parameters )
    fileNameReferenceMaskVolume = self.getParameter( \
                                                  "fileNameReferenceMaskVolume",
                                                  parameters,
                                                  True,
                                                  "" )
    fileNameFloatingVolume = self.getParameter( "fileNameFloatingVolume",
                                                parameters )
    fileNameFloatingMaskVolume = self.getParameter( \
                                                  "fileNameFloatingMaskVolume",
                                                  parameters,
                                                  True,
                                                  "" )
    fileNameInitialFloatingToReferenceTransform3d = self.getParameter( \
                                "fileNameInitialFloatingToReferenceTransform3d",
                                parameters,
                                True,
                                "" )
    transformationType = self.getParameter( "transformationType",
                                            parameters )
    intermediate = self.getParameter( "intermediate",
                                       parameters,
                                       True,
                                       False )
    fileNameDirectTransform3d = self.getParameter( "fileNameDirectTransform3d",
                                                   parameters )
    fileNameInverseTransform3d = self.getParameter( 
                                                   "fileNameInverseTransform3d",
                                                   parameters )
    fileNameTransformedReferenceVolume = self.getParameter( 
                                           "fileNameTransformedReferenceVolume",
                                           parameters,
                                           True )
    fileNameTransformedFloatingVolume = self.getParameter( 
                                            "fileNameTransformedFloatingVolume",
                                            parameters,
                                            True )
    samplingRatio = self.getParameter( "samplingRatio",
                                       parameters,
                                       True,
                                       1.0 )
    diffeoGradientStep = self.getParameter( "diffeoGradientStep",
                                            parameters,
                                            True,
                                            0.1 )
    diffeoUpdateFieldVariance = self.getParameter( "diffeoUpdateFieldVariance",
                                                    parameters,
                                                    True,
                                                    3.0 )
    diffeoTotalFieldVariance = self.getParameter( "diffeoTotalFieldVariance",
                                                  parameters,
                                                  True,
                                                  0.0 )

    useHistogramMatching = self.getParameter( "useHistogramMatching",
                                              parameters,
                                              True,
                                              0 )
    useHistogramMatching = int( useHistogramMatching )
    
    advanced = self.getParameter( "advanced",
                                  parameters,
                                  True,
                                  True )     # modif from Raissa
    verbose = self.getParameter( "verbose",
                                  parameters,
                                  True,
                                  False )
                                            
    if ( verbose == True ):
    
      print( "reference volume : " + fileNameReferenceVolume )
      if ( fileNameReferenceMaskVolume ):
      
        print( "reference mask volume :" + fileNameReferenceMaskVolume )
      print( "floating volume : " + fileNameFloatingVolume )
      if ( fileNameFloatingMaskVolume ):
      
        print( "floating mask volume :" + fileNameFloatingMaskVolume )

      if ( fileNameInitialFloatingToReferenceTransform3d ):
      
        print( "initial floating to reference linear transform :" + \
               fileNameInitialFloatingToReferenceTransform3d )

      print( "3D transform type : " + transformationType )
      print( "intermediate : " + str( intermediate ) )
      print( "advanced : " + str( advanced ) )
      print( "verbose : " + str( verbose ) )
      print( "direct 3D transform :" + fileNameDirectTransform3d )
      print( "inverse 3D transform :" + fileNameInverseTransform3d )
      if ( fileNameTransformedReferenceVolume is not None ):
      
        print( "transformed reference volume (in floating frame) :" + 
               fileNameTransformedReferenceVolume )

      if ( fileNameTransformedFloatingVolume is not None ):
      
        print( "transformed floating volume (in reference frame) :" + 
               fileNameTransformedFloatingVolume )
               
    ############################################################################
    # creating an intermediate directory
    ############################################################################

    intermediateDirectory = os.path.join(
                              os.path.dirname( 
                                 os.path.abspath( fileNameDirectTransform3d ) ),
                              "intermediate" )

    self.createDirectory( intermediateDirectory )
 
      
    ############################################################################
    # converting reference and floating to GIS
    ############################################################################

    fileNameReferenceImaVolume = os.path.join( intermediateDirectory,
                                               "reference.ima" )
    self.convertToGis( fileNameReferenceVolume,
                       fileNameReferenceImaVolume,
                       verbose )

    fileNameFloatingImaVolume = os.path.join( intermediateDirectory,
                                              "floating.ima" )
    self.convertToGis( fileNameFloatingVolume,
                       fileNameFloatingImaVolume,
                       verbose )
                       

    ############################################################################
    # converting reference and floating to NIFTI
    ############################################################################

    fileNameReferenceNiiVolume = os.path.join( intermediateDirectory,
                                               "reference.nii" )
    self.convertToNifti( fileNameReferenceImaVolume,
                         fileNameReferenceNiiVolume,
                         verbose )

    fileNameFloatingNiiVolume = os.path.join( intermediateDirectory,
                                              "floating.nii" )
    self.convertToNifti( fileNameFloatingImaVolume,
                         fileNameFloatingNiiVolume,
                         verbose )


    ############################################################################
    # converting reference mask and floating mask to NIFTI
    ############################################################################

    fileNameReferenceMaskNiiVolume = ""
    if ( fileNameReferenceMaskVolume ):
    
      fileNameReferenceMaskNiiVolume = os.path.join( intermediateDirectory,
                                                     "reference_mask.nii" )
      self.convertToNifti( fileNameReferenceMaskVolume,
                           fileNameReferenceMaskNiiVolume,
                           verbose )

    fileNameFloatingMaskNiiVolume = ""
    if ( fileNameFloatingMaskVolume ):
    
      fileNameFloatingMaskNiiVolume = os.path.join( intermediateDirectory,
                                                    "floating_mask.nii" )
      self.convertToNifti( fileNameFloatingMaskVolume,
                           fileNameFloatingMaskNiiVolume,
                           verbose )


    ############################################################################
    # converting registration type to ANTs format
    ############################################################################

    registrationType = ''
    if ( transformationType == 'rigid' ):

      registrationType = 'r'

    elif ( transformationType == 'affine' ):

      registrationType = 'a'
  
    elif ( transformationType == 'affine_and_diffeomorphic' ):

      registrationType = 's'
  
    elif ( transformationType == 'rigid_and_diffeomorphic' ):

      registrationType = 'sr'
  
    elif ( transformationType == 'diffeomorphic' ):

      registrationType = 'so'
  
    else:

      raise RuntimeError( "bad transformation type; must be one of " +
                          "rigid, affine, affine_and_diffeomorphic, "
                          "rigid_and_diffeomorphic, diffeomorphic" )
                       

    ############################################################################
    # converting floating to reference 3D transformation to ANTs format
    ############################################################################


    fileNameInitialFloatingToReferenceTransform3dAnts = ""
    fileNameInitialReferenceToFloatingTransform3dAnts = ""
    if ( fileNameInitialFloatingToReferenceTransform3d ):

      fileNameInitialFloatingToReferenceTransform3dAnts = os.path.join( \
                            intermediateDirectory,
                            "initial_floating_to_reference_transformation.mat" )
      self.convertTrmToAnts( fileNameInitialFloatingToReferenceTransform3d,
                             fileNameReferenceImaVolume,
                             fileNameInitialFloatingToReferenceTransform3dAnts,
                             intermediateDirectory,
                             verbose )

    
      fileNameInitialReferenceToFloatingTransform3d = os.path.join( \
                            intermediateDirectory,
                            "initial_reference_to_floating_transformation.trm" )

      CommandFactory().executeCommand(
        { "algorithm" : "Transform3dInverter",
          "parameters" : \
            { "fileNameIn" : fileNameInitialFloatingToReferenceTransform3d,
              "fileNameOut" : fileNameInitialReferenceToFloatingTransform3d,
              "verbose" : verbose
            },
          "verbose" : verbose
        } )


      fileNameInitialReferenceToFloatingTransform3dAnts = os.path.join( \
                            intermediateDirectory,
                            "initial_reference_to_floating_transformation.mat" )
      self.convertTrmToAnts( fileNameInitialReferenceToFloatingTransform3d,
                             fileNameFloatingImaVolume,
                             fileNameInitialReferenceToFloatingTransform3dAnts,
                             intermediateDirectory,
                             verbose )


    ############################################################################
    # performing ANTs direct registration
    ############################################################################

    fileNameAntsDirectTransform3d = os.path.join( intermediateDirectory,
                                                  'ants_direct_transform3d_' )
    fileNameAntsDirectTransform3dTxt = os.path.join(
                                                 intermediateDirectory,
                                                 'ants_direct_transform3d.txt' )
    if ( advanced == True ):
    
      self.antsRegistration( fileNameReferenceNiiVolume,
                             fileNameReferenceMaskNiiVolume,
                             fileNameFloatingNiiVolume,
                             fileNameFloatingMaskNiiVolume,
                             fileNameInitialFloatingToReferenceTransform3dAnts,
                             registrationType,
                             fileNameAntsDirectTransform3d,
                             fileNameAntsDirectTransform3dTxt,
                             samplingRatio,
                             diffeoGradientStep,
                             diffeoUpdateFieldVariance,
                             diffeoTotalFieldVariance,
                             useHistogramMatching,
                             verbose )
                                   
    else:

      fileNameFloatingMaskedNiiVolume = os.path.join( intermediateDirectory,
                                                       "floating_masked.nii" )

      CommandFactory().executeCommand(
        { 'algorithm' : 'Combiner',
          'parameters' : \
            { 'fileNameIns' : str( fileNameFloatingNiiVolume ),
              'fileNameOut' : str( fileNameFloatingMaskedNiiVolume ),
              'functor1s' : 'id',
              'functor2s' : 'id',
              'numerator1s' : ( 1.0, 1.0 ),
              'denominator1s' : ( 1.0, 1.0 ),
              'numerator2s' : 1.0,
              'denominator2s' : 1.0,
              'operators' : '*',
              'fileNameMask' : str( fileNameFloatingMaskNiiVolume ),
              'mode' : 'gt',
              'threshold1' : 0.0,
              'threshold2' : 0.0,
              'background' : 0.0,
              'outputType' : 'float',
              'ascii' : False,
              'format' : 'nifti',
              'verbose' : verbose
            },
          'verbose' : verbose
        } )

      self.antsRegistrationSynQuick( fileNameReferenceNiiVolume,
                                     fileNameFloatingMaskedNiiVolume,
                                     registrationType,
                                     fileNameReferenceMaskNiiVolume,
                                     fileNameAntsDirectTransform3d,
                                     fileNameAntsDirectTransform3dTxt,
                                     useHistogramMatching,
                                     verbose )
 
    ############################################################################
    # converting linear direct 3D transformation to .trm
    ############################################################################

    if ( transformationType != 'diffeomorphic' ):

      fileNameDirectTransform3dTrm = fileNameDirectTransform3d + ".trm"
      self.convertLinearTransform3d( fileNameAntsDirectTransform3dTxt,
                                     fileNameDirectTransform3dTrm,
                                     verbose )


    ############################################################################
    # converting ANTs direct diffeomorphic 3D transformation to .ima
    ############################################################################

    fileNameInverseDiffeomorphicTransform3dVolume = None
    if ( ( transformationType == 'affine_and_diffeomorphic' ) or
         ( transformationType == 'rigid_and_diffeomorphic' ) or
         ( transformationType == 'diffeomorphic' ) ):

      fileNameInverseDiffeomorphismNiiVolume = fileNameAntsDirectTransform3d + \
                                              '1Warp.nii.gz'
      fileNameInverseDiffeomorphismImaVolume = os.path.join(
                                                   intermediateDirectory,
                                                   'diffeomorphism_inverse.ima')
      self.convertToGis( fileNameInverseDiffeomorphismNiiVolume,
                         fileNameInverseDiffeomorphismImaVolume,
                         verbose )
      fileNameInverseDiffeomorphismXImaVolume = os.path.join(
                                                 intermediateDirectory,
                                                 'diffeomorphism_inverse_x.ima')
      self.subVolume( fileNameInverseDiffeomorphismImaVolume,
                      fileNameInverseDiffeomorphismXImaVolume,
                      0,
                      +1.0,
                      verbose )
      fileNameInverseDiffeomorphismYImaVolume = os.path.join(
                                                 intermediateDirectory,
                                                 'diffeomorphism_inverse_y.ima')
      self.subVolume( fileNameInverseDiffeomorphismImaVolume,
                      fileNameInverseDiffeomorphismYImaVolume,
                      1,
                      +1.0,
                      verbose )
      fileNameInverseDiffeomorphismZImaVolume = os.path.join(
                                                 intermediateDirectory,
                                                 'diffeomorphism_inverse_z.ima')
      self.subVolume( fileNameInverseDiffeomorphismImaVolume,
                      fileNameInverseDiffeomorphismZImaVolume,
                      2,
                      -1.0,
                      verbose )
                      
      fileNameInverseDiffeomorphicTransform3dVolume = \
        fileNameInverseTransform3d + ".ima"
      self.concat( fileNameInverseDiffeomorphismXImaVolume,
                   fileNameInverseDiffeomorphismYImaVolume,
                   fileNameInverseDiffeomorphismZImaVolume,
                   fileNameInverseDiffeomorphicTransform3dVolume,
                   verbose )


    ############################################################################
    # performing ANTs inverse registration
    ############################################################################

    fileNameAntsInverseTransform3d = os.path.join( intermediateDirectory,
                                                  'ants_inverse_transform3d_' )
    fileNameAntsInverseTransform3dTxt = os.path.join(
                                                intermediateDirectory,
                                                'ants_inverse_transform3d.txt' )

    if ( advanced == True ):

      self.antsRegistration( fileNameFloatingNiiVolume,
                             fileNameFloatingMaskNiiVolume,
                             fileNameReferenceNiiVolume,
                             fileNameReferenceMaskNiiVolume,
                             fileNameInitialReferenceToFloatingTransform3dAnts,
                             registrationType,
                             fileNameAntsInverseTransform3d,
                             fileNameAntsInverseTransform3dTxt,
                             samplingRatio,
                             diffeoGradientStep,
                             diffeoUpdateFieldVariance,
                             diffeoTotalFieldVariance,
                             useHistogramMatching,
                             verbose )

    else:

      fileNameReferenceMaskedNiiVolume = os.path.join( intermediateDirectory,
                                                       "reference_masked.nii" )

      CommandFactory().executeCommand(
        { 'algorithm' : 'Combiner',
          'parameters' : \
            { 'fileNameIns' : str( fileNameReferenceNiiVolume ),
              'fileNameOut' : str( fileNameReferenceMaskedNiiVolume ),
              'functor1s' : 'id',
              'functor2s' : 'id',
              'numerator1s' : ( 1.0, 1.0 ),
              'denominator1s' : ( 1.0, 1.0 ),
              'numerator2s' : 1.0,
              'denominator2s' : 1.0,
              'operators' : '*',
              'fileNameMask' : str( fileNameReferenceMaskNiiVolume ),
              'mode' : 'gt',
              'threshold1' : 0.0,
              'threshold2' : 0.0,
              'background' : 0.0,
              'outputType' : 'float',
              'ascii' : False,
              'format' : 'nifti',
              'verbose' : verbose
            },
          'verbose' : verbose
        } )

      self.antsRegistrationSynQuick( fileNameFloatingNiiVolume,
                                     fileNameReferenceMaskedNiiVolume,
                                     registrationType,
                                     fileNameFloatingMaskNiiVolume,
                                     fileNameAntsInverseTransform3d,
                                     fileNameAntsInverseTransform3dTxt,
                                     useHistogramMatching,
                                     verbose )


    ############################################################################
    # converting linear inverse 3D transformation to .trm
    ############################################################################

    if ( transformationType != 'diffeomorphic' ):

      fileNameInverseTransform3dTrm = fileNameInverseTransform3d + ".trm"
      self.convertLinearTransform3d( fileNameAntsInverseTransform3dTxt,
                                     fileNameInverseTransform3dTrm,
                                     verbose )


    ############################################################################
    # converting ANTs diffeomorphic inverse 3D transformation to .ima
    ############################################################################

    fileNameDirectDiffeomorphicTransform3dVolume = None
    if ( ( transformationType == 'affine_and_diffeomorphic' ) or
         ( transformationType == 'rigid_and_diffeomorphic' ) or
         ( transformationType == 'diffeomorphic' ) ):

      fileNameDirectDiffeomorphismNiiVolume = fileNameAntsInverseTransform3d + \
                                              '1Warp.nii.gz'
      fileNameDirectDiffeomorphismImaVolume = os.path.join(
                                                    intermediateDirectory,
                                                    'diffeomorphism_direct.ima')
      self.convertToGis( fileNameDirectDiffeomorphismNiiVolume,
                         fileNameDirectDiffeomorphismImaVolume,
                         verbose )
      fileNameDirectDiffeomorphismXImaVolume = os.path.join(
                                                  intermediateDirectory,
                                                  'diffeomorphism_direct_x.ima')
      self.subVolume( fileNameDirectDiffeomorphismImaVolume,
                      fileNameDirectDiffeomorphismXImaVolume,
                      0,
                      +1.0,
                      verbose )
      fileNameDirectDiffeomorphismYImaVolume = os.path.join(
                                                  intermediateDirectory,
                                                  'diffeomorphism_direct_y.ima')
      self.subVolume( fileNameDirectDiffeomorphismImaVolume,
                      fileNameDirectDiffeomorphismYImaVolume,
                      1,
                      +1.0,
                      verbose )
      fileNameDirectDiffeomorphismZImaVolume = os.path.join(
                                                  intermediateDirectory,
                                                  'diffeomorphism_direct_z.ima')
      self.subVolume( fileNameDirectDiffeomorphismImaVolume,
                      fileNameDirectDiffeomorphismZImaVolume,
                      2,
                      -1.0,
                      verbose )
                      
      fileNameDirectDiffeomorphicTransform3dVolume = \
        fileNameDirectTransform3d + ".ima"
      self.concat( fileNameDirectDiffeomorphismXImaVolume,
                   fileNameDirectDiffeomorphismYImaVolume,
                   fileNameDirectDiffeomorphismZImaVolume,
                   fileNameDirectDiffeomorphicTransform3dVolume,
                   verbose )

    ############################################################################
    # transforming floating to reference frame
    ############################################################################
                                   
    if ( fileNameTransformedFloatingVolume is not None ):
  
      self.resample( fileNameFloatingImaVolume,
                     fileNameReferenceImaVolume,
                     fileNameDirectTransform3dTrm,
                     fileNameDirectDiffeomorphicTransform3dVolume,
                     fileNameInverseDiffeomorphicTransform3dVolume,
                     fileNameTransformedFloatingVolume,
                     3,
                     verbose )
   
    ############################################################################
    # transforming reference to floating frame
    ############################################################################
                                   
    if ( fileNameTransformedReferenceVolume is not None ):
  
      self.resample( fileNameReferenceImaVolume,
                     fileNameFloatingImaVolume,
                     fileNameInverseTransform3dTrm,
                     fileNameInverseDiffeomorphicTransform3dVolume,
                     fileNameDirectDiffeomorphicTransform3dVolume,
                     fileNameTransformedReferenceVolume,
                     3,
                     verbose )


    ############################################################################
    # reomving intermediate result(s)
    ############################################################################
                                   
    if ( intermediate == False ):

      self.rm( intermediateDirectory )


  def createDirectory( self, directory ):

    if not os.path.isdir( directory ):
   
      os.makedirs( directory )
      if ( not os.path.exists( directory ) ):

        raise Exception( "cannot create directory " + directory )


  def rm( self, *args ):

    sources = []
    for pattern in args:
 
      sources += glob.glob( str( pattern ) )
 
    sys.stdout.flush()
    for path in sources:
 
      if not os.path.islink( path ):

        # this test avoids an error on dead symlinks
        os.chmod( path, 0o777 )

      if os.path.isdir( path ) and not os.path.islink( path ):

        self.rm( os.path.join( path, '*' ) )
        os.rmdir( path )

      else:
        os.remove( path )


  def removeMinf( self, fileNameIma ):

    fileNameMinf = fileNameIma + '.minf'
    if ( os.path.exists( fileNameMinf ) ):
  
      os.remove( fileNameMinf )


  def convertToGis( self, fileNameInputVolume, fileNameOutputVolume, verbose ):
  
    CommandFactory().executeCommand( 
      { "algorithm" : "DiskFormatConverter",
                      "parameters" : \
                        { "fileNameIn" : fileNameInputVolume,
                          "fileNameOut" : fileNameOutputVolume,
                          "inputFormat" : "",
                          "outputFormat" : "gis",
                          "ascii" : False,
                          "verbose" : verbose
                        },
                      "verbose" : verbose
      } )
    self.removeMinf( fileNameOutputVolume )
    

  def convertToNifti( self, fileNameInputVolume, fileNameOutputVolume, verbose ):
  
    CommandFactory().executeCommand( 
      { "algorithm" : "Gis2NiftiConverter",
        "parameters" : \
          { "fileNameIn" : fileNameInputVolume,
            "fileNameOut" : fileNameOutputVolume,
            "verbose" : verbose
          },
        "verbose" : verbose
      } )


  def antsRegistrationSynQuick( self,
                                fileNameReferenceNiiVolume,
                                fileNameFloatingMaskedNiiVolume,
                                registrationType,
                                fileNameReferenceMaskNiiVolume,
                                fileNameAntsDirectTransform3d,
                                fileNameAntsDirectTransform3dTxt,
                                useHistogramMatching,
                                verbose ):
                                
    if ( useHistogramMatching != 0 and \
         useHistogramMatching != 1 ):

      raise Exception( 'useHistogramMatching must be either 0 or 1' )

    command = 'antsRegistrationSyNQuick.sh ' + \
              ' -d 3 ' + \
              ' -f ' + fileNameReferenceNiiVolume + \
              ' -m ' + fileNameFloatingMaskedNiiVolume + \
              ' -t ' + registrationType + \
              ' -j ' + str( useHistogramMatching ) + \
              ' -o ' + fileNameAntsDirectTransform3d

    if ( fileNameReferenceMaskNiiVolume ):
    
      command += ' -x ' + fileNameReferenceMaskNiiVolume

    if ( verbose ):

      print( command )
  
    os.system( command )

    command = 'ConvertTransformFile ' + \
              ' 3 ' + \
              fileNameAntsDirectTransform3d + '0GenericAffine.mat ' + \
              fileNameAntsDirectTransform3dTxt

    if ( verbose ):

      print( command )
  
    os.system( command )


  def antsRegistration( self,
                        fileNameReferenceNiiVolume,
                        fileNameReferenceMaskNiiVolume,
                        fileNameFloatingNiiVolume,
                        fileNameFloatingMaskNiiVolume,
                        fileNameInitialFloatingToReferenceTransform3d,
                        registrationType,
                        fileNameAntsDirectTransform3d,
                        fileNameAntsDirectTransform3dTxt,
                        samplingRatio,
                        diffeoGradientStep,
                        diffeoUpdateFieldVariance,
                        diffeoTotalFieldVariance,
                        useHistogramMatching,
                        verbose ):
                                
    if ( useHistogramMatching != 1 and \
         useHistogramMatching != 0 ):
    
      raise RuntimeError( 'useHistogramMatching must be either 0 or 1' )

    command = 'antsRegistration ' + \
              ' --dimensionality 3 --float 0 ' + \
              ' --output [' + fileNameAntsDirectTransform3d + ',' + \
                             fileNameAntsDirectTransform3d + 'Warped.nii.gz]' \
              ' --interpolation Linear ' + \
              ' --winsorize-image-intensities [0.005,0.995]' + \
              ' --use-histogram-matching ' + str( useHistogramMatching ) + ' '
              
    if ( fileNameInitialFloatingToReferenceTransform3d ):

      command += ' --initial-moving-transform [' + \
                     fileNameInitialFloatingToReferenceTransform3d + ' ]'
                                          
    else:
    
      command += ' --initial-moving-transform [' + \
                                            fileNameReferenceNiiVolume + ',' + \
                                            fileNameFloatingNiiVolume + \
                                            ',1]'
      
                                          
                                          
    command += ' --transform Translation[0.1] ' + \
               ' --metric MI[' + fileNameReferenceNiiVolume + \
                           ',' + fileNameFloatingNiiVolume + \
                           ',1,64,Regular,' + str( samplingRatio) + ']' + \
               ' --convergence [1000x500x250x100,1e-6,10]' + \
               ' --shrink-factors 8x4x2x1' + \
               ' --smoothing-sigmas 3x2x1x0vox' + \
               ' --transform Rigid[0.1] ' + \
               ' --metric MI[' + fileNameReferenceNiiVolume + \
                           ',' + fileNameFloatingNiiVolume + \
                           ',1,64,Regular,' + str( samplingRatio) + ']' + \
               ' --convergence [1000x500x250x100,1e-6,10]' + \
               ' --shrink-factors 8x4x2x1' + \
               ' --smoothing-sigmas 3x2x1x0vox' + \
               ' --transform Affine[0.1]' + \
               ' --metric MI[' + fileNameReferenceNiiVolume + \
                           ',' + fileNameFloatingNiiVolume + \
                           ',1,64,Regular,' + str( samplingRatio) + ']' + \
               ' --convergence [1000x500x250x100,1e-6,10]' + \
               ' --shrink-factors 8x4x2x1' + \
               ' --smoothing-sigmas 3x2x1x0vox' + \
               ' --transform SyN[' + str( diffeoGradientStep ) + ',' + \
                                     str( diffeoUpdateFieldVariance ) + ',' + \
                                     str( diffeoTotalFieldVariance ) + ']' + \
               ' --metric MI[' + fileNameReferenceNiiVolume + \
                           ',' + fileNameFloatingNiiVolume + \
                           ',1,64,Regular,' + str( samplingRatio) + ']' + \
               ' --convergence [100x70x50x20,1e-6,10]' + \
               ' --shrink-factors 8x4x2x1' + \
               ' --smoothing-sigmas 2x1x1x0vox' + \
               ' -v'

    if ( fileNameReferenceMaskNiiVolume ):
    
      command += ' -x [' + fileNameReferenceMaskNiiVolume + \
                      ',' + fileNameFloatingMaskNiiVolume + ']'

    
    if ( verbose ):

      print( command )
  
    os.system( command )

    command = 'ConvertTransformFile ' + \
              ' 3 ' + \
              fileNameAntsDirectTransform3d + '0GenericAffine.mat ' + \
              fileNameAntsDirectTransform3dTxt

    if ( verbose ):

      print( command )
  
    os.system( command )


  def convertTrmToAnts( self,
                        fileNameTrmTransform3d,
                        fileNameReferenceVolume,
                        fileNameAntsTransform3d,
                        intermediateDirectory,
                        verbose ):
                   
    fileNameTrmInverseTransform3d = os.path.join( \
                    intermediateDirectory,
                    "inverse_initial_floating_to_reference_transformation.trm" )
                        
    CommandFactory().executeCommand(
      { "algorithm" : "Transform3dInverter",
        "parameters" : \
          { "fileNameIn" : fileNameTrmTransform3d,
            "fileNameOut" : fileNameTrmInverseTransform3d,
            "verbose" : verbose
          },
        "verbose" : verbose
      } )


    # reading the *.trm 3D transform since the Ants *.mat file corresponds 
    # reference and floating to intermediate directory

    f = open( fileNameTrmInverseTransform3d, "r" )
    lines = f.readlines()
    f.close()

    line1 = lines[ 0 ].split()
    line2 = lines[ 1 ].split()
    line3 = lines[ 2 ].split()
    line4 = lines[ 3 ].split()

    Tprime = [ float( line1[ 0 ] ),
               float( line1[ 1 ] ),
               float( line1[ 2 ] ) ]
 
    Rprime = [ [ float( line2[ 0 ] ),
                 float( line2[ 1 ] ), 
                 float( line2[ 2 ] ) ],
               [ float( line3[ 0 ] ),
                 float( line3[ 1 ] ),
                 float( line3[ 2 ] ) ],
               [ float( line4[ 0 ] ),
                 float( line4[ 1 ] ),
                 float( line4[ 2 ] ) ] ]

    # computing the reference center

    # extracting dimensions and resolution from the reference volume
    f = open( fileNameReferenceVolume[ : -3 ] + "dim", "r" )
    lines = f.readlines()
    line1 = lines[ 0 ].split()
    line3 = lines[ 2 ].split()

    sizeX = int( line1[ 0 ] )
    sizeY = int( line1[ 1 ] )
    sizeZ = int( line1[ 2 ] )

    resolutionX = float( line3[ 1 ] )
    resolutionY = float( line3[ 3 ] )
    resolutionZ = float( line3[ 5 ] )

    C = [ sizeX * resolutionX / 2.0,
          sizeY * resolutionY / 2.0,
          sizeZ * resolutionZ / 2.0 ]

    # computing the Ants rotation and translation

    R = Rprime
    R[ 2 ][ 0 ] = -R[ 2 ][ 0 ]
    R[ 2 ][ 1 ] = -R[ 2 ][ 1 ]
    R[ 0 ][ 2 ] = -R[ 0 ][ 2 ]
    R[ 1 ][ 2 ] = -R[ 1 ][ 2 ]

    T = [ 0.0, 0.0, 0.0 ]
    T[ 0 ] = -C[ 0 ] + ( R[ 0 ][ 0 ] * C[ 0 ] +
                         R[ 0 ][ 1 ] * C[ 1 ] +
                         R[ 0 ][ 2 ] * C[ 2 ] ) + Tprime[ 0 ]
    T[ 1 ] = -C[ 1 ] + ( R[ 1 ][ 0 ] * C[ 0 ] +
                         R[ 1 ][ 1 ] * C[ 1 ] +
                         R[ 1 ][ 2 ] * C[ 2 ] ) + Tprime[ 1 ]
    T[ 2 ] = -C[ 2 ] + ( R[ 2 ][ 0 ] * C[ 0 ] +
                         R[ 2 ][ 1 ] * C[ 1 ] +
                         R[ 2 ][ 2 ] * C[ 2 ] ) - Tprime[ 2 ]

    # saving Ants transform to .mat file

    file = open( fileNameAntsTransform3d, "wb" )

    integer_array = array( 'i', [ 0, 12, 1, 0, 27 ] )
    integer_array.tofile( file )

    file.write( b'AffineTransform_double_3_3\0' )

    double_array = array( 'd', [ R[ 0 ][ 0 ], R[ 0 ][ 1 ], R[ 0 ][ 2 ],
                                 R[ 1 ][ 0 ], R[ 1 ][ 1 ], R[ 1 ][ 2 ],
                                 R[ 2 ][ 0 ], R[ 2 ][ 1 ], R[ 2 ][ 2 ],
                                 T[ 0 ], T[ 1 ], T[ 2 ] ] )
    double_array.tofile( file )

    integer_array = array( 'i', [ 0, 3, 1, 0, 6 ] )
    integer_array.tofile( file )

    file.write( b'fixed\0' )

    double_array = array( 'd', [ C[ 0 ], C[ 1 ], C[ 2 ] ] )
    double_array.tofile( file )

    file.close()


  def convertLinearTransform3d( self,
                                fileNameAntsTransform3dTxt,
                                fileNameTransform3dTrm,
                                verbose ):

    # extracting information from Ants txt file
    f = open( fileNameAntsTransform3dTxt, 'r' )
    lines = f.readlines()
    f.close()

    line3 = lines[ 3 ].split()
    line4 =  lines[ 4 ].split()

    T = [ float( line3[ 10 ] ),
          float( line3[ 11 ] ),
          float( line3[ 12 ] ) ]
 
    C = [ float( line4[ 1 ] ),
          float( line4[ 2 ] ),
          float( line4[ 3 ] ) ]
 

    R = [ [ float( line3[ 1 ] ), float( line3[ 2 ] ), float( line3[ 3 ] ) ],
          [ float( line3[ 4 ] ), float( line3[ 5 ] ), float( line3[ 6 ] ) ],
          [ float( line3[ 7 ] ), float( line3[ 8 ] ), float( line3[ 9 ] ) ] ]

    Tprime = [ 0.0, 0.0, 0.0 ]
    for i in range( 0, 3 ):

       Tprime[ i ] = T[ i ] + C[ i ]
       for j in range( 0, 3 ):
   
         Tprime[ i ] -= R[ i ][ j ] * C[ j ]

    Tprime[ 2 ] *= -1;

    # saving rigid/affine inverse transformation to Anatomist format
    # ( note that the Ants output is the affine transformation from the 
    #  reference to the floating space )
    f = open( fileNameTransform3dTrm, 'w' )


    inverseTransformation = \
       [ [ R[ 0 ][ 0 ], R[ 0 ][ 1 ], -R[ 0 ][ 2 ], Tprime[ 0 ] ],
         [ R[ 1 ][ 0 ], R[ 1 ][ 1 ], -R[ 1 ][ 2 ], Tprime[ 1 ] ],
         [ -R[ 2 ][ 0 ], -R[ 2 ][ 1 ], R[ 2 ][ 2 ], Tprime[ 2 ] ],
         [ 0.0, 0.0, 0.0, 1.0 ] ]

    directTransformation = self.invertTransformation3d( inverseTransformation )


    f.write( '%g ' % directTransformation[ 0 ][ 3 ] )
    f.write( '%g ' % directTransformation[ 1 ][ 3 ] )
    f.write( '%g\n' % directTransformation[ 2 ][ 3 ] )

    f.write( '%g ' % directTransformation[ 0 ][ 0 ] )
    f.write( '%g ' % directTransformation[ 0 ][ 1 ] )
    f.write( '%g\n' % directTransformation[ 0 ][ 2 ] )

    f.write( '%g ' % directTransformation[ 1 ][ 0 ] )
    f.write( '%g ' % directTransformation[ 1 ][ 1 ] )
    f.write( '%g\n' % directTransformation[ 1 ][ 2 ] )

    f.write( '%g ' % directTransformation[ 2 ][ 0 ] )
    f.write( '%g ' % directTransformation[ 2 ][ 1 ] )
    f.write( '%g\n' % directTransformation[ 2 ][ 2 ] )

    f.close()


  def invertTransformation3d( self, d ):
  
    i = [ [ 0.0, 0.0, 0.0, 0.0 ],
          [ 0.0, 0.0, 0.0, 0.0 ],
          [ 0.0, 0.0, 0.0, 0.0 ],
          [ 0.0, 0.0, 0.0, 0.0 ] ]
            
  
    determinant = d[ 0 ][ 0 ] * \
                  ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ] ) - \
                  d[ 1 ][ 0 ] * \
                  ( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ] ) + \
                  d[ 2 ][ 0 ] * \
                  ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ] )

    i[ 0 ][ 0 ] = +( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ]  ) /\
                  determinant
    i[ 1 ][ 0 ] = -( d[ 1 ][ 0 ] * d[ 2 ][ 2 ] - d[ 2 ][ 0 ] * d[ 1 ][ 2 ]  ) /\
                  determinant
    i[ 2 ][ 0 ] = +( d[ 1 ][ 0 ] * d[ 2 ][ 1 ] - d[ 2 ][ 0 ] * d[ 1 ][ 1 ]  ) /\
                  determinant
    i[ 3 ][ 0 ] = 0.0

    i[ 0 ][ 1 ] = -( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ]  ) /\
                  determinant
    i[ 1 ][ 1 ] = +( d[ 0 ][ 0 ] * d[ 2 ][ 2 ] - d[ 2 ][ 0 ] * d[ 0 ][ 2 ]  ) /\
                  determinant
    i[ 2 ][ 1 ] = -( d[ 0 ][ 0 ] * d[ 2 ][ 1 ] - d[ 2 ][ 0 ] * d[ 0 ][ 1 ]  ) /\
                  determinant
    i[ 3 ][ 1 ] = 0.0

    i[ 0 ][ 2 ] = +( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ]  ) /\
                  determinant
    i[ 1 ][ 2 ] = -( d[ 0 ][ 0 ] * d[ 1 ][ 2 ] - d[ 1 ][ 0 ] * d[ 0 ][ 2 ]  ) /\
                  determinant
    i[ 2 ][ 2 ] = +( d[ 0 ][ 0 ] * d[ 1 ][ 1 ] - d[ 1 ][ 0 ] * d[ 0 ][ 1 ]  ) /\
                  determinant
    i[ 3 ][ 2 ] = 0.0

    i[ 0 ][ 3 ] = -( +d[ 0 ][ 1 ] * \
                     ( d[ 1 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 1 ][ 3 ] ) \
                     -d[ 1 ][ 1 ] * \
                     ( d[ 0 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 0 ][ 3 ] ) \
                     +d[ 2 ][ 1 ] * \
                     ( d[ 0 ][ 2 ] * d[ 1 ][ 3 ] - d[ 1 ][ 2 ] * d[ 0 ][ 3 ] ) \
                   ) / \
                  determinant
    i[ 1 ][ 3 ] = +( +d[ 0 ][ 0 ] * \
                     ( d[ 1 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 1 ][ 3 ] ) \
                     -d[ 1 ][ 0 ] * \
                     ( d[ 0 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 0 ][ 3 ] ) \
                     +d[ 2 ][ 0 ] * \
                     ( d[ 0 ][ 2 ] * d[ 1 ][ 3 ] - d[ 1 ][ 2 ] * d[ 0 ][ 3 ] ) \
                   ) / \
                  determinant
    i[ 2 ][ 3 ] = -( +d[ 0 ][ 0 ] * \
                     ( d[ 1 ][ 1 ] * d[ 2 ][ 3 ] - d[ 2 ][ 1 ] * d[ 1 ][ 3 ] ) \
                     -d[ 1 ][ 0 ] * \
                     ( d[ 0 ][ 1 ] * d[ 2 ][ 3 ] - d[ 2 ][ 1 ] * d[ 0 ][ 3 ] ) \
                     +d[ 2 ][ 0 ] * \
                     ( d[ 0 ][ 1 ] * d[ 1 ][ 3 ] - d[ 1 ][ 1 ] * d[ 0 ][ 3 ] ) \
                   ) / \
                  determinant
    i[ 3 ][ 3 ] = +( +d[ 0 ][ 0 ] * \
                     ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ] ) \
                     -d[ 1 ][ 0 ] * \
                     ( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ] ) \
                     +d[ 2 ][ 0 ] * \
                     ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ] ) \
                   ) / \
                  determinant

    for jj in range( 0, 4 ):
    
      for ii in range( 0, 4 ):
      
        i[ ii ][ jj ] /= i[ 3 ][ 3 ];

    return i


  def subVolume( self,
                 fileNameDiffeomorphismImaVolume,
                 fileNameDiffeomorphismImaSubVolume,
                 t,
                 scale,
                 verbose ):
                 
    # extracting dimensions of diffeomorphism
    f = open( fileNameDiffeomorphismImaVolume[ : -3 ] + 'dim', 'r' )
    lines = f.readlines()
    f.close()
    line1 = lines[ 0 ].split()
    sizeX = int( line1[ 0 ] )
    sizeY = int( line1[ 1 ] )
    sizeZ = int( line1[ 2 ] )

    CommandFactory().executeCommand( 
      { "algorithm" : "SubVolume",
        "parameters" : \
          { "fileNameIn" : fileNameDiffeomorphismImaVolume,
            "fileNameOut" : fileNameDiffeomorphismImaSubVolume,
            "sx" : 0,
            "sy" : 0,
            "sz" : 0,
            "st" : t,
            "ex" : sizeX - 1,
            "ey" : sizeY - 1,
            "ez" : sizeZ - 1,
            "et" : t,
            "xIndices" : (),
            "yIndices" : (),
            "zIndices" : (),
            "tIndices" : (),
            "ascii" : False,
            "format" : 'gis',
            "verbose" : verbose
          },
        'verbose' : verbose
      } )
    self.removeMinf( fileNameDiffeomorphismImaSubVolume )

    CommandFactory().executeCommand( 
      { "algorithm" : "Combiner",
        "parameters" : \
          { "fileNameIns" : ( fileNameDiffeomorphismImaSubVolume ),
            "fileNameOut" : fileNameDiffeomorphismImaSubVolume,
            "functor1s" : (),
            "functor2s" : (),
            "numerator1s" : ( float( scale ), 0.0 ),
            "denominator1s" : ( 1.0, 1.0 ),
            "numerator2s" : ( 1.0 ),
            "denominator2s" : ( 1.0 ),
            "operators" : ( '+' ),
            "fileNameMask" : '',
            "mode" : "gt",
            "threshold1" : 0.0,
            "threshold2" : 0.0,
            "background" : 0.0,
            "outputType" : 'float',
            "ascii" : False,
            "format" : 'gis',
            "verbose" : verbose
          },
        'verbose' : verbose
      } )
    self.removeMinf( fileNameDiffeomorphismImaSubVolume )


  def concat( self,
              fileNameDiffeomorphismXImaVolume,
              fileNameDiffeomorphismYImaVolume,
              fileNameDiffeomorphismZImaVolume,
              fileNameDiffeomorphicTransform3dVolume,
              verbose ):
              
    CommandFactory().executeCommand( 
      { "algorithm" : "Cat",
        "parameters" : \
          { "inputNames" : ( fileNameDiffeomorphismXImaVolume,
                             fileNameDiffeomorphismYImaVolume,
                             fileNameDiffeomorphismZImaVolume ),
            "outputName" : fileNameDiffeomorphicTransform3dVolume,
            "type" : 't',
            "ascii" : False,
            "format" : 'gis',
            "verbose" : verbose
          },
        "verbose" : verbose
      } )
    self.removeMinf( fileNameDiffeomorphicTransform3dVolume )


  def resample( self,
                fileNameReferenceImaVolume,
                fileNameTemplateImaVolume,
                fileNameDirectTransform3dTrm,
                fileNameDirectDiffeomorphicTransform3dVolume,
                fileNameInverseDiffeomorphicTransform3dVolume,
                fileNameTransformedReferenceVolume,
                order,
                verbose ):

    if ( ( fileNameDirectDiffeomorphicTransform3dVolume is None ) and \
         ( fileNameInverseDiffeomorphicTransform3dVolume is None ) ):
         
      CommandFactory().executeCommand( 
        { "algorithm" : "Resampling3d",
          "parameters" : \
            { "fileNameReference" : fileNameReferenceImaVolume,
              "fileNameTemplate" : fileNameTemplateImaVolume,
              "fileNameTransforms" : \
                              ( fileNameDirectTransform3dTrm ),
              "fileNameOut" : fileNameTransformedReferenceVolume,
              "order" : int( order ),
              "outBackground" : 0.0,
              "ascii" : False,
              "format" : "gis",
              "applyInverseLinearTransformFirst" : False,
              "verbose" : verbose
            },
          "verbose" : verbose
        } )
        
    else:
   
      CommandFactory().executeCommand( 
        { "algorithm" : "Resampling3d",
          "parameters" : \
            { "fileNameReference" : fileNameReferenceImaVolume,
              "fileNameTemplate" : fileNameTemplateImaVolume,
              "fileNameTransforms" : \
                              ( fileNameDirectTransform3dTrm,
                                fileNameDirectDiffeomorphicTransform3dVolume,
                                fileNameInverseDiffeomorphicTransform3dVolume ),
              "fileNameOut" : fileNameTransformedReferenceVolume,
              "order" : int( order ),
              "outBackground" : 0.0,
              "ascii" : False,
              "format" : "gis",
              "applyInverseLinearTransformFirst" : False,
              "verbose" : verbose
            },
          "verbose" : verbose
        } )


  @staticmethod
  def getName():
  
    return "AntsRegistration3d"
    
  @staticmethod
  def createInstance():
  
    return AntsRegistration3dCommand()


CommandFactory().registerCommand( AntsRegistration3dCommand )
