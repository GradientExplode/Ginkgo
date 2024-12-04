import os


def getItemTypeOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info item_type ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  itemType = fd.read()
  ret = fd.close()
  return itemType[ : -1 ]


def getDiskFormatOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info disk_format ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  diskFormat = fd.read()
  ret = fd.close()
  return diskFormat[ : -1 ]


def getSizeXOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info sizeX ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringSizeX = fd.read()
  ret = fd.close()
  return int( stringSizeX[ : -1 ] )



def getSizeYOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info sizeY ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringSizeY = fd.read()
  ret = fd.close()
  return int( stringSizeY[ : -1 ] )



def getSizeZOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info sizeZ ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringSizeZ = fd.read()
  ret = fd.close()
  return int( stringSizeZ[ : -1 ] )



def getSizeTOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info sizeT ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringSizeT = fd.read()
  ret = fd.close()
  return int( stringSizeT[ : -1 ] )


def getResolutionXOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info resolutionX ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringResolutionX = fd.read()
  ret = fd.close()
  return float( stringResolutionX[ : -1 ] )


def getResolutionYOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info resolutionY ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringResolutionY = fd.read()
  ret = fd.close()
  return float( stringResolutionY[ : -1 ] )



def getResolutionZOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info resolutionZ ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringResolutionZ = fd.read()
  ret = fd.close()
  return float( stringResolutionZ[ : -1 ] )


def getResolutionTOfVolume( fileName ):

  command = 'GkgExecuteCommand VolumeInformation ' + \
            ' -i ' + fileName + \
            ' -info resolutionT ' + \
            '-verbosePluginLoading false'
            
  fd = os.popen( command )
  stringResolutionT = fd.read()
  ret = fd.close()
  return float( stringResolutionT[ : -1 ] )






