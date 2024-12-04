from core.command.Command import *
from core.command.CommandFactory import *
import glob


################################################################################
#
# inputs:
#   - fileNameInputVolume
#   - fileNameMaskVolume
#   - verbose
#
#
# output:
#   - fileNameOutputVolume
#
################################################################################


class AntsBiasFieldCorrectionCommand( Command ):

  def __init__( self ):
  
    Command.__init__( self )


  def execute( self, parameters ):

    ############################################################################
    # collecting the parameter(s)
    ############################################################################
    
    fileNameInputVolume = self.getParameter( "fileNameInputVolume",
                                             parameters )
    fileNameOutputVolume = self.getParameter( "fileNameOutputVolume", 
                                              parameters )
    fileNameMaskVolume = self.getParameter( "fileNameMaskVolume", 
                                            parameters,
                                            True,
                                            None )
    verbose = self.getParameter( "verbose",
                                  parameters,
                                  True,
                                  False )

    if ( verbose == True ):
    
      print( "input volume : " + fileNameInputVolume )
      print( "output volume : " + fileNameOutputVolume )

      if ( fileNameMaskVolume ):
        print( "mask volume : " + fileNameMaskVolume )

      print( "verbose : " + str( verbose ) )
               
    ############################################################################
    # creating an intermediate directory
    ############################################################################

    intermediateDirectory = os.path.join(
                               os.path.dirname( 
                                  os.path.abspath( fileNameOutputVolume ) ),
                               "intermediate" )

    self.createDirectory( intermediateDirectory )
                       
    ############################################################################
    # converting input volume to NIFTI
    ############################################################################

    fileNameInputNiiVolume = os.path.join( intermediateDirectory,
                                           "input.nii" )
    self.convertToNifti( fileNameInputVolume,
                         fileNameInputNiiVolume,
                         verbose )
                       
    ############################################################################
    # converting mask volume to NIFTI
    ############################################################################

    fileNameMaskNiiVolume = None

    if ( fileNameMaskVolume ):

      fileNameMaskNiiVolume = os.path.join( intermediateDirectory,
                                            "mask.nii" )
      self.convertToNifti( fileNameMaskVolume,
                           fileNameMaskNiiVolume,
                           verbose )

    ############################################################################
    # performing ANTs N4 bias field correction
    ############################################################################

    fileNameOutputNiiVolume = os.path.join( intermediateDirectory,
                                            "output.nii" )

    self.antsBiasFieldCorrection( fileNameInputNiiVolume,
                                  fileNameOutputNiiVolume,
                                  fileNameMaskNiiVolume,
                                  verbose )

    ############################################################################
    # converting ANTs bias field corrected image to .ima
    ############################################################################

    self.convertToGis( fileNameOutputNiiVolume,
                       fileNameOutputVolume,
                       verbose )

    ############################################################################
    # removing intermediate result(s)
    ############################################################################
                                   
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


  def convertToNifti( self, fileNameInputVolume, 
                      fileNameOutputVolume, verbose ):
  
    CommandFactory().executeCommand( 
      { "algorithm" : "Gis2NiftiConverter",
        "parameters" : \
          { "fileNameIn" : fileNameInputVolume,
            "fileNameOut" : fileNameOutputVolume,
            "verbose" : verbose
          },
        "verbose" : verbose
      } )


  def antsBiasFieldCorrection( self,
                               fileNameInputNiiVolume,
                               fileNameOutputNiiVolume,
                               fileNameMaskNiiVolume,
                               verbose ):

    command = 'N4BiasFieldCorrection ' + \
              ' -d 3 ' + \
              ' -i ' + fileNameInputNiiVolume + \
              ' -o ' + fileNameOutputNiiVolume + \
              ' -v '
    
    if fileNameMaskNiiVolume:

      command += ' -x ' + fileNameMaskNiiVolume

    if ( verbose ):

      print( command )
  
    os.system( command )


  @staticmethod
  def getName():
  
    return "AntsBiasFieldCorrection"
    
  @staticmethod
  def createInstance():
  
    return AntsBiasFieldCorrectionCommand()


CommandFactory().registerCommand( AntsBiasFieldCorrectionCommand )
