import os
import sys
import optparse

scriptDirectory = os.path.dirname( os.path.abspath( __file__) )
gkgSharePath = os.path.dirname( scriptDirectory )
sys.path.insert( 0, os.path.join( gkgSharePath, 'python' ) )

from core.command.CommandFactory import *



################################################################################
################################################################################
# main function
################################################################################
################################################################################


parser = optparse.OptionParser()
parser.add_option( '-r', '--reference',
                   dest = 'fileNameReferenceVolume',
                   help = 'reference volume filename',
                   metavar = 'FILE' )
parser.add_option( '-m', '--referenceMask',
                   dest = 'fileNameReferenceMaskVolume',
                   help = 'reference maskvolume filename',
                   metavar = 'FILE' )
parser.add_option( '-f', '--floating',
                   dest = 'fileNameFloatingVolume',
                   help = 'floating volume filename',
                   metavar = 'FILE' )
parser.add_option( '-M', '--floatingMask',
                   dest = 'fileNameFloatingMaskVolume',
                   help = 'floating mask volume filename',
                   metavar = 'FILE' )
parser.add_option( '-d', '--outputDirectTransform3d',
                   dest = 'fileNameDirectTransform3d',
                   default = 'direct_transform3d',
                   help = 'Output direct 3D transform (default: %default)',
                   metavar = 'FILE' )
parser.add_option( '-i', '--outputInverseTransform3d',
                   dest = 'fileNameInverseTransform3d',
                   default = 'inverse_transform3d',
                   help = 'Output inverse 3D transform (default: %default)',
                   metavar = 'FILE' )
parser.add_option( '-T', '--initialFloatingToReferenceTransform3d',
                   dest = 'fileNameInitialFloatingToReferenceTransform3d',
                   default = '',
                   help = 'Initial floating ot reference 3D *.trm transform ' +\
                   '(default: %default)',
                   metavar = 'FILE' )
parser.add_option( '-t', '--type',
                   dest = 'transformationType',
                   default = 'affine_and_diffeomorphic',
                   help = 'Transformation type: rigid, affine, '
                          'affine_and_diffeomorphic, rigid_and_diffeomorphic, '
                          'diffeomorphic (default: %default)' )
parser.add_option( '-R', '--transformedReference',
                   dest = 'fileNameTransformedReferenceVolume',
                   help = 'Transformed (in floating frame) reference volume '
                          'filename',
                   metavar = 'FILE' )
parser.add_option( '-F', '--transformedFloating',
                   dest = 'fileNameTransformedFloatingVolume',
                   help = 'Transformed (in reference frame) floating volume '
                          'filename',
                   metavar = 'FILE' )
parser.add_option( '-I', '--intermediate',
                   dest = 'intermediate',
                   action = 'store_true',
                   default = False,
                   help = 'Store intermediate result(s) (default: %default)' )
parser.add_option( '-s', '--samplingRatio',
                   dest = 'samplingRatio',
                   default = 1.0,
                   help = 'Sampling ratio used by similarity criteria ' + \
                          '(default: %default)' )
parser.add_option( '-g', '--diffeoGradientStep',
                   dest = 'diffeoGradientStep',
                   default = 0.1,
                   help = 'Diffeomorphic gradient step ' + \
                          '(default: %default)' )
parser.add_option( '-u', '--diffeoUpdateFieldVariance',
                   dest = 'diffeoUpdateFieldVariance',
                   default = 3.0,
                   help = 'Diffeomorphic update regularization field ' + \
                          'variance (default: %default)' )
parser.add_option( '-V', '--diffeoTotalFieldVariance',
                   dest = 'diffeoTotalFieldVariance',
                   default = 0.0,
                   help = 'Diffeomorphic total regularization field ' + \
                          'variance (default: %default)' )
parser.add_option( '-j', '--useHistogramMatching',
                   dest = 'useHistogramMatching',
                   default = 0,
                   help = 'Use histogram matching (default: %default)' )
parser.add_option( '-a', '--advanced',
                   dest = 'advanced',
                   action = 'store_true',
                   default = False,
                   help = 'Used advanced ANTs registration instead of ' +
                          'SynQuick (default: %default)' )
parser.add_option( '-v', '--verbose',
                   dest = 'verbose',
                   action = 'store_true',
                   default = False,
                   help = 'Show as much information as possible '
                          '(default: %default)' )


( options, args ) = parser.parse_args()


################################################################################
# checking that input files exist
################################################################################

if ( options.fileNameReferenceVolume is None ):

  raise Exception( 'a reference volume is required' )

else:

  if ( not os.path.exists( options.fileNameReferenceVolume ) ):

    raise Exception( 'cannot read reference volume' )


if ( options.fileNameFloatingVolume is None ):

  raise Exception( 'a floating volume is required' )

else:

  if ( not os.path.exists( options.fileNameReferenceVolume ) ):

    raise Exception( 'cannot read floating volume' )


if ( options.fileNameReferenceMaskVolume is not None ):

  if ( not os.path.exists( options.fileNameReferenceMaskVolume ) ):

    raise Exception( 'cannot read reference mask volume' )


if ( options.fileNameFloatingMaskVolume is not None ):

  if ( not os.path.exists( options.fileNameFloatingMaskVolume ) ):

    raise Exception( 'cannot read floating mask volume' )



################################################################################
# launching the ANTs registration command
################################################################################

CommandFactory().executeCommand( 
  { "algorithm" : "AntsRegistration3d",
    "parameters" : \
       { "fileNameReferenceVolume" : options.fileNameReferenceVolume,
         "fileNameReferenceMaskVolume" : options.fileNameReferenceMaskVolume,
         "fileNameFloatingVolume" : options.fileNameFloatingVolume,
         "fileNameFloatingMaskVolume" : options.fileNameFloatingMaskVolume,
         "fileNameInitialFloatingToReferenceTransform3d" : \
                          options.fileNameInitialFloatingToReferenceTransform3d,
         "transformationType" : options.transformationType,
         "intermediate" : options.intermediate,
         "fileNameDirectTransform3d" : options.fileNameDirectTransform3d,
         "fileNameInverseTransform3d" : options.fileNameInverseTransform3d,
         "fileNameTransformedReferenceVolume" : \
                                     options.fileNameTransformedReferenceVolume,
         "fileNameTransformedFloatingVolume" : \
                                      options.fileNameTransformedFloatingVolume,
         "samplingRatio" : options.samplingRatio,
         "diffeoGradientStep" : options.diffeoGradientStep,
         "diffeoUpdateFieldVariance" : options.diffeoUpdateFieldVariance,
         "diffeoTotalFieldVariance" : options.diffeoTotalFieldVariance,
         "useHistogramMatching" : options.useHistogramMatching,
         "advanced" : options.advanced,
         "verbose" : options.verbose
       },
    "verbose" : options.verbose
  } )
