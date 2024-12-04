from PathHandler import *


def getContainerType( fileName ):

  extension = getExtension( fileName )
  
  if ( ( extension == '.ima' ) or
       ( extension == '.dim' ) or
       ( extension == '.ima.minf' ) or
       ( extension == '.nii' ) or
       ( extension == '.nii.minf' ) or
       ( extension == '.nii.gz' ) or
       ( extension == '.nii.gz.minf' ) ):
       
    return 'Volume'
    
  elif ( ( extension == '.bundles' ) or
         ( extension == '.bundlesdata' ) or
         ( extension == '.bundlemap' ) or
         ( extension == '.bundlemap.minf' ) ):

       
    return 'BundleMap'
    
  elif ( ( extension == '.tex' ) or
         ( extension == '.tex.minf' ) or
         ( extension == '.texturemap' ) or
         ( extension == '.texturemap.minf' ) ):

       
    return 'TextureMap'

  elif ( ( extension == '.sitemap' ) or
         ( extension == '.sitemap.minf' ) ):

       
    return 'SiteMap'

  elif ( ( extension == '.polygonmap' ) or
         ( extension == '.polygonmap.minf' ) ):

       
    return 'PolygonMap'

  elif ( ( extension == '.meshmap' ) or
         ( extension == '.meshmap.minf' ) or
         ( extension == '.mesh' ) or
         ( extension == '.mesh.minf' ) or
         ( extension == '.tri' ) or
         ( extension == '.tri.minf' ) ):

       
    return 'MeshMap'

  elif ( extension == '.hie' ):

       
    return 'Hierarchy'

  else:
  
    return None
