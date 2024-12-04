from ResourceManager import *
import sys
import os
import xml.dom.minidom

class XMLResourceManager:


  def __init__( self, applicationName, xmlFileName, verbose = False ):

    self._xmlFileName = ResourceManager().getXMLFileName( applicationName,
                                                          xmlFileName )

    self._document = xml.dom.minidom.Document()
    rootNode = self._document.createElementNS( '', 'file list' )
    self._document.appendChild( rootNode )

    self._fileNames = {}
    self._verbose = verbose
                                                          

  def getRootNode( self ):
  
    return self._document.documentElement


  def add( self, label, fileName ):
   
    if ( ( label is not None ) and ( fileName is not None ) ):

      if ( self._fileNames.has_key( label ) ):
      
        message = 'label already present'
        raise RuntimeError, message
      
      node = xml.dom.minidom.Element( 'file' )
      node.setAttribute( 'label', label )
      node.setAttribute( 'path', fileName )
      self.getRootNode().appendChild( node )
      self._fileNames[ label ] = fileName


  def getLabels( self ):
  
    return self._fileNames.keys()
    

  def getFileName( self, label ):
  
    if ( not self._fileNames.has_key( label ) ):
      
      message = 'label not present'
      raise RuntimeError, message
      
    return self._fileNames[ label ]


  def read( self ):
  
    self._document = xml.dom.minidom.parse( self._xmlFileName )
    for f in self.getRootNode().getElementsByTagName( "file" ):

      ok = False
      label = None
      fileName = None
      try:
      
        label = str( f.getAttributeNode( "label" ).nodeValue )
        fileName = str( f.getAttributeNode( "path" ).nodeValue )
        ok = True
        
      except:
      
        print 

      if ( ok ):
      
        self._fileNames[ label ] = fileName

  def write( self ):
  
    f = open( self._xmlFileName, 'w' )
    f.write( self._document.toxml() )
    f.close()

  def display( self ):
  
    for f in self.getRootNode().getElementsByTagName( "file" ):

      print str( f.getAttributeNode( "label" ).nodeValue ), ' -> ', \
            str( f.getAttributeNode( "path" ).nodeValue )


