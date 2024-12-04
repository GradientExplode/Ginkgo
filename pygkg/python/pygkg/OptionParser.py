import os
from optparse import OptionParser


def getArgs( pages ):

  pageNames = list()
  for tab in pages:

    for tabValues in tab.values():

      for page in tabValues:

        pageNames.append( page[ 'pageName' ] )

  parser = OptionParser( '\n\ndmri [options]' )
  batchOptions = dict()
  pages = ''
  for pageName in pageNames:

    pages += '-  ' + pageName + \
             '                                           \n'

  parserBatchMode = parser.add_option(
                '-b',
                '--batch',
                dest = 'batchMode',
                action = 'store_true',
                help = 'run in batch mode',
                default = False )

  parserParameterFileName = parser.add_option(
                '-p',
                '--parameter-filename',
                dest = 'parameterFileName',
                help = 'parameter file name containing the algorithm to be ' + \
                'launched and the associated dictionary of parameters ' + \
                'with the following syntax:\n\n' + \
                '+-----------------------------------+\n' + \
                '                                           ' + \
                '|algorithmName = <algorithm>        |\n' + \
                '                                           ' + \
                '|parameterValues = {                |\n' + \
                '                                           ' + \
                '|\'parameter_name1\' : <value1>,      |\n' + \
                '                                           ' + \
                '|\'parameter_name2\' : <value2>,      |\n' + \
                '                                           ' + \
                '|...                                |\n' + \
                '                                           ' + \
                '|...                                |\n' + \
                '                                           ' + \
                '|...                                |\n' + \
                '                                           ' + \
                '|\'parameter_nameN-1\' : <valueN-1>,  |\n' + \
                '                                           ' + \
                '|\'parameter_nameN\' : <valueN>}      |\n' + \
                '                                           ' + \
                '+-----------------------------------+\n' + \
                '                                           ' + \
                '<algorithm> being one of:' + \
                '                                           ' + \
                '                                           ' + \
                pages,
                type = 'string',
                default = '' )

  parserDistributionStrategy = parser.add_option(
                '-d',
                '--distribution-strategy',
                dest = 'distributionStrategy',
                help = 'distribution strategy when processing ' + \
                'multiple subjects:' + \
                '                                           ' + \
                '-  single-threading  --> single thread' + \
                '                                           ' + \
                '-  multiple-threading  --> multi thread',
                type = 'string',
                default = 'single-threading' )

  parserDistributionOptions = parser.add_option(
                '-o',
                '--distribution-options',
                dest = 'distributionOptions',
                help = 'distribution options:' + \
                '                                           ' + \
                '  - in case of single thread\n' + \
                '                                           ' + \
                '.    N/A' + \
                '                                           ' + \
                '  - in case of multi thread' + \
                '                                           ' + \
                '.    <P1>: maximum number of used CPUs',
                type = 'string',
                default = '' )



  (options, args) = parser.parse_args()

  batchMode = options.batchMode
  parameterFileName = options.parameterFileName
  distributionSettings = dict()

  # sanity check(s)

  if ( ( batchMode == True ) and ( parameterFileName == '' ) ):

    parser.error( 'parameter file name mandatory in batch mode' )


  if ( batchMode == True ):
  
    if ( ( options.distributionStrategy != 'single-threading' ) and \
          ( options.distributionStrategy != 'multiple-threading' ) ):
          
      parser.error( 'distribution strategy must be \'single-threading\' or' +
                    '\'multiple-threading\'' )
                    
    distributionSettings[ 'distributionStrategy' ] = \
                                                   options.distributionStrategy
    if ( ( options.distributionStrategy == 'multiple-threading' ) and 
         ( options.distributionOptions == '' ) ):

      distributionSettings[ 'thread_count' ] = \
                                         os.sysconf( 'SC_NPROCESSORS_CONF' ) - 1

    else:

      distributionSettings[ 'thread_count' ] = int( 1 )

  return batchMode, parameterFileName, distributionSettings
