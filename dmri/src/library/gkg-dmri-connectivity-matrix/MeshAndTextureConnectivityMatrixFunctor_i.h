#ifndef _gkg_dmri_connectivity_matrix_MeshAndTextureConnectivityMatrixFunctor_i_h_
#define _gkg_dmri_connectivity_matrix_MeshAndTextureConnectivityMatrixFunctor_i_h_

#include <gkg-dmri-connectivity-matrix/MeshAndTextureConnectivityMatrixFunctor.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <vector>



#define LABEL_FOR_MESH        -1
#define LABEL_FOR_BACKGROUND   0



template < class L, class Compare >
inline
gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::
                                       MeshAndTextureConnectivityMatrixFunctor(
                      const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& /* stringParameters */ )
                                : gkg::ConnectivityMatrixFunctor< L, Compare >()
{

  try
  {

    if ( scalarParameters.empty() )
    {

      _inputFiberResamplingStep = 0.1;
      _maximumDistanceToMesh = 1.0;
      _outputFiberResamplingStep = 5;
      _maximumFiberTangentToMeshNormalAngleInDegrees = 0.0;
      _smoothConnectivityMap = false;
      _smoothingStandardDeviation = 1.0;
      _cacheSize.x = 100;
      _cacheSize.y = 100;
      _cacheSize.z = 100;

    }
    else
    {

      if ( scalarParameters.size() != 9U )
      {

        throw std::runtime_error( "9 scalar parameters required" );

      }
      _inputFiberResamplingStep = ( float )scalarParameters[ 0 ];
      _maximumDistanceToMesh = ( float )scalarParameters[ 1 ];
      _outputFiberResamplingStep = ( float )scalarParameters[ 2 ];
      _maximumFiberTangentToMeshNormalAngleInDegrees =
                                                 ( float )scalarParameters[ 4 ];
      _smoothConnectivityMap = ( int32_t )( scalarParameters[ 4 ] + 0.5 )
                                     ? true : false;
      _smoothingStandardDeviation = ( float )scalarParameters[ 5 ];
      _cacheSize.x = ( int32_t )( scalarParameters[ 6 ] + 0.5 );
      _cacheSize.y = ( int32_t )( scalarParameters[ 7 ] + 0.5 );
      _cacheSize.z = ( int32_t )( scalarParameters[ 8 ] + 0.5 );

      // sanity checks
      if ( _inputFiberResamplingStep <= 0 )
      {

        throw std::runtime_error(
                      "input fiber resampling step must be strictly positive" );

      }

      if ( _maximumDistanceToMesh <= 0 )
      {

        throw std::runtime_error(
                         "maximum distance to mesh must be strictly positive" );

      }

      if ( _outputFiberResamplingStep <= 0 )
      {

        throw std::runtime_error(
                     "output fiber resampling step must be strictly positive" );

      }

      if ( _maximumFiberTangentToMeshNormalAngleInDegrees < 0 )
      {

        throw std::runtime_error(  "maximum fiber tangent to mesh normal "
                                   "angle must be strictly positive" );

      }

      if ( _smoothingStandardDeviation <= 0 )
      {

        throw std::runtime_error(
                     "smoothing standard deviation must be strictly positive" );

      }

      if ( _cacheSize.x <= 0 )
      {

        throw std::runtime_error( "cache X size must be strictly positive" );

      }

      if ( _cacheSize.y <= 0 )
      {

        throw std::runtime_error( "cache Y size must be strictly positive" );

      }

      if ( _cacheSize.z <= 0 )
      {

        throw std::runtime_error( "cache Z size must be strictly positive" );

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::"
             "MeshAndTextureConnectivityMatrixFunctor( "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters )" );

}


template < class L, class Compare >
inline
gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::
                                      ~MeshAndTextureConnectivityMatrixFunctor()
{
}


template < class L, class Compare >
inline
void gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::apply(
                const std::list< std::string >& fileNameBundleMaps,
                const std::list< std::string >& fileNameListOfROIs1,
                const std::string& fileNameROIs1ToBundleMapTransform3d,
                const std::list< std::string >& fileNameListOfROIs2,
                const std::string& /* fileNameROIs2ToBundleMapTransform3d */,
                const std::list< std::string >& functorNames,
                const std::list< std::string >& outputFileNames,
                const std::string& theOutputBundleMapFormat,
                const std::string& /* theOutputTextureMapFormat */,
                bool ascii,
                bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( fileNameListOfROIs1.size() != 2U )
    {

      throw std::runtime_error( 
                        "one input mesh and input texture filenames required" );

    }
    if ( fileNameListOfROIs2.size() != 0U )
    {

      throw std::runtime_error( "no ROI2 required" );

    }
    if ( functorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error(
                  "there should be as many output filenames as functor names" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // building functor name map
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string, std::string > outputFileNameMap;
    std::list< std::string >::const_iterator fn = functorNames.begin(),
                                             fne = functorNames.end();
    std::list< std::string >::const_iterator ofn = outputFileNames.begin();
    while ( fn != fne )
    {


      outputFileNameMap[ *fn ] = *ofn;
      ++ fn;
      ++ ofn;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating a set of functor for quick search
    ////////////////////////////////////////////////////////////////////////////

    std::set< std::string > setOfFunctorNames( functorNames.begin(),
                                               functorNames.end() );


    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh for ROIs1
    ////////////////////////////////////////////////////////////////////////////

    std::string meshFileNameROIs1 = fileNameListOfROIs1.front();
    if ( verbose )
    {

      std::cout << "reading ROIs1 mesh '" << meshFileNameROIs1 << "'"
                << std::flush;
    }

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > meshRois1(
                                       new gkg::MeshMap< int32_t, float, 3U > );
    gkg::Reader::getInstance().read( meshFileNameROIs1, *meshRois1 );
    if ( meshRois1->vertices.getRankCount() != 1 )
    {

      throw std::runtime_error( "mesh ROI1 must have a unique rank" );

    }
    int32_t vertexCount = meshRois1->vertices.getSiteCount( 0 );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 + ( int32_t )meshFileNameROIs1.size() )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading input texture for ROIs1 : a corriger en fonction de TextureMap
    ////////////////////////////////////////////////////////////////////////////

    std::string textureFileNameROIs1 = fileNameListOfROIs1.back();
    if ( verbose )
    {

      std::cout << "reading ROIs1 texture '" << textureFileNameROIs1 << "'"
                << std::flush;

    }

    gkg::TextureMap< int16_t >  textureRois1;
    gkg::Reader::getInstance().read( textureFileNameROIs1, textureRois1 );

    if ( textureRois1.getCount() != vertexCount )
    {

      throw std::runtime_error( "texture and mesh in ROI1 must have"
                                " the same number of elements" );

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 + ( int32_t )textureFileNameROIs1.size() )
                << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading 3D transform for ROIs1
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromROIs1RealToBundleReal;
    if ( fileNameROIs1ToBundleMapTransform3d.empty() )
    {

      if ( verbose )
      {

       std::cout << "initializing 3D ROIs1->Bundles transform to identity"
                 << std::flush;

      }
      // by default, initializaing to inverse scaling
      transform3dFromROIs1RealToBundleReal.reset(
                                        new gkg::IdentityTransform3d< float > );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 52 )
                  << std::flush;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading rigid or affine 3D ROIs1->Bundles transform"
                 << std::flush;

      }

      // Read transformation file, first try rigid, then affine
      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > > rigidTransform3d;
        rigidTransform3d.reset( new gkg::RigidTransform3d< float > );

        std::ifstream is( fileNameROIs1ToBundleMapTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromROIs1RealToBundleReal = rigidTransform3d;

      }
      catch ( std::exception& )
      {

        gkg::RCPointer< gkg::AffineTransform3d< float > > affineTransform3d;
        affineTransform3d.reset( new gkg::AffineTransform3d< float > );

        std::ifstream is( fileNameROIs1ToBundleMapTransform3d.c_str() );
        affineTransform3d->readTrm( is );
        is.close();
        transform3dFromROIs1RealToBundleReal = affineTransform3d;

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 41 )
                  << std::flush;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the number of ROIs1 labels in textureRois1
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "computing labels of ROIs1" << std::flush;

    }
    std::set< int16_t > rois1Labels;
    
    gkg::TextureMap< int16_t >::const_iterator
      v = textureRois1.begin(),
      ve = textureRois1.end();
    int16_t value;
    while ( v != ve )
    {
      value = (*v).second;
      if ( value > 0 )
      {

        rois1Labels.insert( value );

      }
      ++ v;

    }
    int32_t rois1LabelCount = ( int32_t )rois1Labels.size();
    int32_t maximumLabel = ( int32_t )*rois1Labels.rbegin();
    if ( verbose )
    {

      std::cout << gkg::Eraser( 25 )
                << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building the mesh cache for meshROIs1
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "computing mesh cache for ROIs1" << std::flush;

    }

    gkg::Vector3d< double > maskResolution;
    maskResolution.x = 1.0;
    maskResolution.y = 1.0;
    maskResolution.z = .0;
 
    gkg::BoundingBox< float >
        boundingBox( -0.5f * ( float )maskResolution.x,
                     ( ( float )_cacheSize.x - 0.5f ) *
                       ( float )maskResolution.x,
                     -0.5f * ( float )maskResolution.y,
                     ( ( float )_cacheSize.y - 0.5f ) *
                       ( float )maskResolution.y,
                     -0.5f * ( float )maskResolution.z,
                     ( ( float )_cacheSize.z - 0.5f ) *
                       ( float )maskResolution.z );

    //gkg::BoundingBox< float >
    //  meshBoundingBox( meshRois1->vertices.getBoundingBox( 0 ) );
      
      
    //boundingBox.setLowerX( boundingBox.getLowerX() - _maximumDistanceToMesh );
    //boundingBox.setLowerY( boundingBox.getLowerY() - _maximumDistanceToMesh );
    //boundingBox.setLowerZ( boundingBox.getLowerZ() - _maximumDistanceToMesh );

    //boundingBox.setLowerX( 0.0 );
    //boundingBox.setLowerY( 0.0 );
    //boundingBox.setLowerZ( 0.0 );


    gkg::RCPointer< gkg::SceneModeler > sceneModeler( 
                new gkg::SceneModeler( boundingBox, 
                                      //meshRois1->vertices.getBoundingBox( 0 ),
                                       1.0,
                                       1,
                                       1,
                                       _cacheSize,
                                       verbose ) );
    gkg::RCPointer< gkg::VertexEvolutionFunction >
      staticVertexEvolutionFunction( new gkg::StaticVertexEvolutionFunction() );
    gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh(
                           new gkg::EvolvingMesh( sceneModeler,
                                                  meshRois1,
                                                  staticVertexEvolutionFunction,
                                                  _maximumDistanceToMesh,
                                                  false ) );
    sceneModeler->addEvolvingMesh( "ROIs1", evolvingMesh );
    gkg::Volume< uint8_t > rois1Mask;
    sceneModeler->getEvolvingMeshCache().getBinaryMask( rois1Mask );
    gkg::BoundingBox< float > rois1BoundingBox = sceneModeler->getBoundingBox();

    gkg::Writer::getInstance().write( "rois1mask.ima", rois1Mask );
    
    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating a sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::SparseMatrix connectivityMatrix( maximumLabel + 1, vertexCount );

    ////////////////////////////////////////////////////////////////////////////
    // allocating average length sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SparseMatrix > averageLengthMatrix;
    if ( setOfFunctorNames.find( "average-length-matrix" ) !=
         setOfFunctorNames.end() )
    {

      averageLengthMatrix.reset( new gkg::SparseMatrix( maximumLabel + 1,
                                                        vertexCount ) );

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating the output bundle map
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::BundleMap< L, Compare > > roiToMeshBundleMap;
    if ( setOfFunctorNames.find( "roi-to-roi-bundle-map" ) !=
         setOfFunctorNames.end() )
    {

      roiToMeshBundleMap.reset( new gkg::BundleMap< L, Compare > );

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing 3D transforms:
    // - from bundle real frame to ROIs1 real frame
    ////////////////////////////////////////////////////////////////////////////

    gkg::Transform3d< float >* 
      transform3dFromBundleRealFrameToROIs1RealFrame = 
        new gkg::InverseTransform3d< float >( 
                                         transform3dFromROIs1RealToBundleReal );

    ////////////////////////////////////////////////////////////////////////////
    // computing the connectivity matrix
    ////////////////////////////////////////////////////////////////////////////

    std::list< std::string >::const_iterator
      b = fileNameBundleMaps.begin(),
      be = fileNameBundleMaps.end();
    while ( b != be )
    {

      if ( verbose )
      {

       std::cout << "reading '" << *b << "' bundle map"
                 << std::flush;

      }
      gkg::BundleMap< L, std::less< L > > inputBundleMap;
      gkg::Reader::getInstance().read( *b, inputBundleMap );

      typename gkg::BundleMap< L, Compare >::const_iterator
        l = inputBundleMap.begin(),
        le = inputBundleMap.end();
      while ( l != le )
      {

        const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

        typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
          f = bundle.begin(),
          fe = bundle.end();
        while ( f != fe )
        {

           const typename gkg::BundleMap< L, Compare >::Fiber& fiber = *f;
           addContribution( fiber,
                            textureRois1,
                            rois1BoundingBox,
                            transform3dFromBundleRealFrameToROIs1RealFrame,
                            sceneModeler,
                            rois1Mask,
                            evolvingMesh,
                            connectivityMatrix,
                            averageLengthMatrix,
                            roiToMeshBundleMap );

           ++ f;

        }

        ++ l;

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 + ( int32_t )b->size() )
                  << std::flush;

      }

      ++ b;

    }

    ////////////////////////////////////////////////////////////////////////////
    // normalizing the average length matrix : je comprends pas ...
    ////////////////////////////////////////////////////////////////////////////

    if ( !averageLengthMatrix.isNull() )
    {

      const std::map< size_t, double >&
        connectivityMatrixData = connectivityMatrix.getData();
      std::map< size_t, double >&
        averageLengthMatrixData = averageLengthMatrix->getData();

      std::map< size_t, double >::const_iterator
        cm = connectivityMatrixData.begin(),
        cme = connectivityMatrixData.end();
      std::map< size_t, double >::iterator
        am = averageLengthMatrixData.begin();
      while ( cm != cme )
      {

       am->second /= cm->second;
       ++ cm;
       ++ am;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // filling the 4D texture
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::TextureMap< float > > roiToRoiConnectivityTextureMap;
    if ( setOfFunctorNames.find( "roi-to-mesh-connectivity-texture-map" ) !=
         setOfFunctorNames.end() )
    {

      roiToRoiConnectivityTextureMap.reset( new gkg::TextureMap< float > );
      std::set< int16_t >::const_iterator r1 = rois1Labels.begin(),
                                          r1e = rois1Labels.end();

      int32_t s2 = 0;
      std::vector< float > connectivities( vertexCount );
      for ( ; r1 != r1e; r1++ )
      {

        connectivities.assign( vertexCount, 0.0 );
        for ( s2 = 0; s2 < vertexCount; s2++ )
        {

          if ( connectivityMatrix.hasElement( *r1, s2 ) )
          {
 
            connectivities[ s2 ] = connectivityMatrix( *r1, s2 );

          }

        }
        roiToRoiConnectivityTextureMap->addTextures( connectivities );

      }

      // adding rank information to the header
      std::vector< int32_t > ranks( rois1LabelCount );
      r1 = rois1Labels.begin();
      int32_t rank = 0;
      for ( rank = 0; rank < rois1LabelCount; rank++ )
      {

        ranks[ rank ] = rank; 

      }

      std::vector< int32_t > vertexCounts( rois1LabelCount, vertexCount );      
      roiToRoiConnectivityTextureMap->getHeader().addAttribute(
                                                              "aims_rank_count",
                                                              rois1LabelCount );
      roiToRoiConnectivityTextureMap->getHeader().addAttribute( "aims_rank",
                                                                ranks );
      roiToRoiConnectivityTextureMap->getHeader().addAttribute( 
                                                              "aims_item_count",
                                                              vertexCounts );

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving connectivity matrix
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "saving connectivity matrix" << std::flush;

    }

    gkg::ItemWriter< gkg::SparseMatrix >& sparseMatrixW =
      gkg::SparseMatrixItemIOFactory::getInstance().getWriter( true, false );
    std::ofstream os( outputFileNameMap[ "roi-to-mesh-connectivity-matrix" ].c_str() );
    sparseMatrixW.write( os, connectivityMatrix );
    os.close();

    if ( verbose )
    {

      std::cout << gkg::Eraser( 26 ) << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving average length matrix
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "average-length-matrix" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << "saving average length matrix" << std::flush;

      }

      os.open( outputFileNameMap[ "average-length-matrix" ].c_str() );
      sparseMatrixW.write( os, *averageLengthMatrix );
      os.close();

      if ( verbose )
      {

        std::cout << gkg::Eraser( 28 ) << std::flush;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving ROIs1 to mesh bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "roi-to-roi-bundle-map" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << "saving ROIs1 to ROIs1 bundle map" << std::flush;

      }


      gkg::Writer::getInstance().write(
                                  outputFileNameMap[ "roi-to-roi-bundle-map" ],
                                  *roiToMeshBundleMap,
                                  ascii,
                                  theOutputBundleMapFormat );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 31 ) << std::flush;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving ROIs1 labeledRegions to meshVertices connectivity texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "roi-to-mesh-connectivity-texture-map" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << 
          "saving ROIs1 labeledRegions to meshVertices connectivity texture map"
                 << std::flush;

      }


      gkg::Writer::getInstance().write(
                    outputFileNameMap[ "roi-to-mesh-connectivity-texture-map" ],
                    *roiToRoiConnectivityTextureMap,
                    ascii,
                    theOutputBundleMapFormat );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 46 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::"
             "apply( "
             "const std::list< std::string >& fileNameBundleMaps, "
             "const std::list< std::string >& fileNameListOfROIs1, "
             "const std::string& fileNameROIs1ToBundleMapTransform3d, "
             "const std::list< std::string >& fileNameListOfROIs2, "
             "const std::string& fileNameROIs2ToBundleMapTransform3d, "
             "const std::list< std::string >& functorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::string& theOutputBundleMapFormat, "
             "const std::string& theOutputTextureMapFormat, "
             "bool ascii, "
             "bool verbose ) const" );

}


template < class L, class Compare >
inline
std::string 
gkg::MeshAndTextureConnectivityMatrixFunctor< L,
                                            Compare >::getScalarParameterHelp()
{

  try
  {

    std::string help;
    help += ".  <P1>: input fiber resampling step in mm (default=0.1)\n";
    help += ".  <P2>: maximum distance to mesh in mm (default=1.0)\n";
    help += ".  <P3>: output fiber resampling step in mm (default=4.0)\n";
    help += ".  <P4>: maximum angle between fiber tangent and mesh normal\n "
            ".        in degrees (default=0->tangent not taken into account)\n";
    help += ".  <P5>: smooth connectivity map (default=0)\n";
    help += ".  <P6>: standard deviation of smoothing (default=1.0)\n";
    help += ".  <P7>: cache X size (default=100)\n";
    help += ".  <P8>: cache Y size (default=100)\n";
    help += ".  <P9>: cache Z size (default=100)\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::MeshAndTextureConnectivityMatrixFunctor< L, "
             "Compare >::getScalarParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::MeshAndTextureConnectivityMatrixFunctor< L,
                                             Compare >::getStringParameterHelp()
{

  try
  {

    std::string help;
    help += ".  N/A\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::MeshAndTextureConnectivityMatrixFunctor< L, "
             "Compare >::getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::MeshAndTextureConnectivityMatrixFunctor< L,
                                              Compare >::getFunctorHelp()
{

  try
  {

    std::string help;
    help += ".  connectivity-matrix\n";
    help += ".  average-length-matrix\n";
    help += ".  roi-to-roi-bundle-map\n";
    help += ".  roi-to-mesh-connectivity-texture-map\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::MeshAndTextureConnectivityMatrixFunctor< L, "
             "Compare >::getFunctorHelp()" );

}


template < class L, class Compare >
inline
void gkg::MeshAndTextureConnectivityMatrixFunctor< L, 
                                                   Compare >::addContribution(
     const typename gkg::BundleMap< L, Compare >::Fiber& fiber,
     const gkg::TextureMap< int16_t>& textureRois1,
     const gkg::BoundingBox< float >& rois1BoundingBox,
     gkg::Transform3d< float >* transform3dFromBundleRealFrameToROIs1RealFrame,
     const gkg::RCPointer< gkg::SceneModeler >& sceneModeler,
     const gkg::Volume< uint8_t >& rois1Mask,
     const gkg::RCPointer< gkg::EvolvingMesh >& evolvingMesh,
     gkg::SparseMatrix& connectivityMatrix,
     gkg::RCPointer< gkg::SparseMatrix >& averageLengthMatrix,
     gkg::RCPointer< gkg::BundleMap< L, Compare > >& roiToMeshBundleMap ) const
{

  try
  {

    float maximumFiberTangentToMeshNormalAngleInRadians =
      ( _maximumFiberTangentToMeshNormalAngleInDegrees * M_PI ) / 180.0;

    ////////////////////////////////////////////////////////////////////////////
    // resampling the fiber at the input resolution
    ////////////////////////////////////////////////////////////////////////////

    float fiberLength = fiber.getLength();

    int32_t resampledPointCount = std::max( 2, ( int32_t )( fiberLength /
                                            _inputFiberResamplingStep + 0.5 ) );

    gkg::LightCurve3d< float > 
      resampledFiber = fiber.getEquidistantCurve( resampledPointCount );

    ////////////////////////////////////////////////////////////////////////////
    // allocating the label vector
    ////////////////////////////////////////////////////////////////////////////

    std::vector< int16_t > labels( resampledPointCount, LABEL_FOR_BACKGROUND );

    ////////////////////////////////////////////////////////////////////////////
    // allocating the polygon vector
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Polygon< 3U > > polygons( resampledPointCount );

    ////////////////////////////////////////////////////////////////////////////
    // allocating the distance vector
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > distances( resampledPointCount, 1e38 );

    ////////////////////////////////////////////////////////////////////////////
    // allocating the projection point vector
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Vector3d< float > > 
      projectionPoints( resampledPointCount );

    ////////////////////////////////////////////////////////////////////////////
    // looping over points of the fiber and determining their label
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > voxelInRois1VoxelFrame;
    gkg::Vector3d< float > pointInRois1RealFrame;
    gkg::Vector3d< float > nextPointInRois1RealFrame;
    gkg::Vector3d< float > tangent;
    gkg::Vector3d< float > normal;

    gkg::LightCurve3d< float >::const_iterator p = resampledFiber.begin(),
                                               pe = resampledFiber.end();
    gkg::LightCurve3d< float >::const_iterator nextP;
    std::vector< int16_t >::iterator l = labels.begin();
    std::vector< gkg::Polygon< 3U > >::iterator po = polygons.begin();
    std::vector< float >::iterator d = distances.begin();
    std::vector< gkg::Vector3d< float > >::iterator 
                                                  pr = projectionPoints.begin();

    while ( p != pe )
    {

      // processing the real coordinates in the ROIs1 real frame
      transform3dFromBundleRealFrameToROIs1RealFrame->getDirect(
                                                    *p, pointInRois1RealFrame );

      // processing the voxel coordinates in the ROIs1 voxel frame
      sceneModeler->getCacheVoxel( pointInRois1RealFrame,  
                                   voxelInRois1VoxelFrame );

      // if we are inside the bounding box of the ROIs1 mesh
      if ( rois1BoundingBox.contains( pointInRois1RealFrame ) )
      {

        // if we are close to the ROIs1 mesh
        if ( rois1Mask( voxelInRois1VoxelFrame ) )
        {

          if ( _maximumFiberTangentToMeshNormalAngleInDegrees > 0.0 )


          {

            evolvingMesh->
                 getFastClosestPolygonAndDistanceAndProjectionPointAndNormal(
                                                       pointInRois1RealFrame,
                                                       voxelInRois1VoxelFrame,
                                                       *po,
                                                       *d,
                                                       *pr,
                                                       normal );
            // when the polygon cache is created, the distance threshold
            // consists of the minimum distance to the closest vertex of the
            // triangle, and not to the distance to the triangle globally
            // so, it may produce distances bigger than the one asked; as a
            // a consequence, we need to perform a further test on the distance
            // here; in addition, a condition about the angle between the
            // fiber tangent and the normal to the surface is added

            if ( *d <= _maximumDistanceToMesh )
            {

              nextP = p + 1;
              if ( nextP != pe )
              {
 
                transform3dFromBundleRealFrameToROIs1RealFrame->getDirect(
                                          *nextP, nextPointInRois1RealFrame );
 
              }
              else
              {
 
                nextP = p - 1;
                transform3dFromBundleRealFrameToROIs1RealFrame->getDirect(
                                          *nextP, nextPointInRois1RealFrame );

              }
              tangent = nextPointInRois1RealFrame - pointInRois1RealFrame;
 
              if ( gkg::getLineAngles( tangent, normal ) <=
                              maximumFiberTangentToMeshNormalAngleInRadians )
              {

                *l = LABEL_FOR_MESH;

              }

            }

          }
          else
          {

            evolvingMesh->getFastClosestPolygonAndDistanceAndProjectionPoint(
                                                       pointInRois1RealFrame,
                                                       voxelInRois1VoxelFrame,
                                                       *po,
                                                       *d,
                                                       *pr );
            // when the polygon cache is created, the distance threshold
            // consists of the minimum distance to the closest vertex of the
            // triangle, and not to the distance to the triangle globally
            // so, it may produce distances bigger than the one asked; as a
            // a consequence, we need to perform a further test on the distance
            // here

            if ( *d <= _maximumDistanceToMesh )
            {

              *l = LABEL_FOR_MESH;

            }

          }

        }

      }

      ++ p;
      ++ l;
      ++ po;
      ++ d;
      ++ pr;

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating vector of ranges from the vector of labels
    ////////////////////////////////////////////////////////////////////////////

    int32_t i = 0;
    std::pair< int32_t, int32_t > range;
    std::vector< std::pair< int32_t, int32_t > > ranges;
    int16_t label = LABEL_FOR_BACKGROUND;
    float distance = 0.0;

    for ( i = 0; i < resampledPointCount; i++ )
    {

      label = getRange( labels, i, range );

      // if a range is found
      if ( label )
      {

        ranges.push_back( range );
        i = range.second + 1;

      }
      // else if no range is found
      else
      {

        break;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // splitting the current fiber into smallest fibers and storing them into
    // the labelled fiber map;
    // plus filling the connectivity matrix
    ////////////////////////////////////////////////////////////////////////////

    std::map< L, std::list< gkg::LightCurve3d< float > > > labelledFibers;
    std::map< int16_t, std::list< gkg::Polygon< 3U > > > polygonMap;
    std::map< int16_t, std::list< gkg::Vector3d< float > > > projectionPointMap;

    int32_t rangeCount = ( int32_t )ranges.size();
    int32_t r1 = -1;
    int32_t r2 = 0;
    int32_t index = 0;
    int32_t index1 = 0;
    int32_t index2 = 0;
    int32_t indexOfMinimumDistance1 = 0;
    float minimumDistance1;
    int32_t indexOfMinimumDistance2 = 0;
    float minimumDistance2;
    int16_t label1 = LABEL_FOR_BACKGROUND;
    int16_t label2 = LABEL_FOR_BACKGROUND;
    float newFiberLength = 0.0;
    int32_t newPointCount = 0;



    for ( ; r1 < rangeCount; r1++, r2++ )
    {

      if ( r1 == -1 )
      {

        index1 = 0;
        label1 = LABEL_FOR_BACKGROUND;

      }
      else
      {

        index1 = ranges[ r1 ].first;
        label1 = labels[ ranges[ r1 ].first ];

      }
      if ( r2 == rangeCount )
      {

        index2 = resampledPointCount - 1;
        label2 = LABEL_FOR_BACKGROUND;

      }
      else
      {

        index2 = ranges[ r2 ].second;
        label2 = labels[ ranges[ r2 ].second ];

      }

      if ( ( label1 == LABEL_FOR_MESH ) &&
           ( label2 == LABEL_FOR_MESH ) )
      {

        ///////////////////////////////////////////////////////////////////////
        // looking for the index of minimum distance in the first range
        ///////////////////////////////////////////////////////////////////////

        indexOfMinimumDistance1 = index1;
        minimumDistance1 = distances[ index1 ];
        index = index1;
        do
        {

          distance = distances[ index ];
          if ( distance < minimumDistance1 )
          {

            indexOfMinimumDistance1 = index;
            minimumDistance1 = distance;

          }
          ++ index;

        }
        while ( labels[ index ] == LABEL_FOR_MESH );

        ///////////////////////////////////////////////////////////////////////
        // looking for the index of minimum distance in the second range
        ///////////////////////////////////////////////////////////////////////

        indexOfMinimumDistance2 = index2;
        minimumDistance2 = distances[ index2 ];
        index = index2;
        do
        {

          distance = distances[ index ];
          if ( distance < minimumDistance2 )
          {

            indexOfMinimumDistance2 = index;
            minimumDistance2 = distance;

          }
          -- index;

        }
        while ( labels[ index ] == LABEL_FOR_MESH );


        std::list< gkg::Vector3d< float > > points;
        for ( i = indexOfMinimumDistance2; i >= indexOfMinimumDistance1; i-- )
        {

          points.push_back( resampledFiber.getPoint( i ) );

        }

        gkg::LightCurve3d< float > newFiber( points );
        newFiberLength = newFiber.getLength();

        newPointCount = std::max( 2, ( int32_t )( newFiberLength /
                                     _outputFiberResamplingStep + 0.5 ) );

        gkg::LightCurve3d< float > 
          outputFiber = newFiber.getEquidistantCurve( newPointCount );

        ///////////////////////////////////////////////////////////////////////
        // Finding the index of meshRois1's closest point to 
        // indexOfMinimumDistance1 projectionPoint and the associated label
        // in textureRois1 
        ///////////////////////////////////////////////////////////////////////
        gkg::Polygon< 3U > po1 = polygons[ indexOfMinimumDistance1 ];
        gkg::Vector3d< float > pp1 = projectionPoints[
                                           indexOfMinimumDistance1 ];
        std::vector< float > weigths1;

        float weight1 = 0.0;
        float weight2 = 0.0;
        float weight3 = 0.0;

        int32_t indexOfMeshRois1ClosestPointToIndexOfMinimumDistance1 = 
                                                              po1.indices[ 0 ];
        float weightBetweenMeshRois1ClosestPointToIndexOfMinimumDistance1 = 
                                                              weight1;
        float weight = 0;
        int16_t textureLabel1 = 0;

        evolvingMesh->getPartialTriangleAreaWeighting( po1,
                                                       pp1,
                                                       weight1,
                                                       weight2,
                                                       weight3 );
        weigths1.push_back( weight1 );
        weigths1.push_back( weight2 );
        weigths1.push_back( weight3 );

        for ( int16_t pcount = 1; pcount < 3; ++pcount )
        {

          weight = weigths1[ pcount ];
          if ( weight > 
               weightBetweenMeshRois1ClosestPointToIndexOfMinimumDistance1 )
          {

            indexOfMeshRois1ClosestPointToIndexOfMinimumDistance1 = 
                                                         po1.indices[ pcount ];
            weightBetweenMeshRois1ClosestPointToIndexOfMinimumDistance1 = 
                                                         weight;

          }

        }

        textureLabel1 = textureRois1.getTexture(
                       indexOfMeshRois1ClosestPointToIndexOfMinimumDistance1 );

        ///////////////////////////////////////////////////////////////////////
        // Finding the index of meshRois1's closest point to 
        // indexOfMinimumDistance2 projectionPoint and the associated label
        // in textureRois1 
        ///////////////////////////////////////////////////////////////////////
        gkg::Polygon< 3U > po2 = polygons[ indexOfMinimumDistance2 ];
        gkg::Vector3d< float > pp2 = projectionPoints[
                                           indexOfMinimumDistance2 ];
        std::vector< float > weigths2;
        weight1 = 0.0;
        weight2 = 0.0;
        weight3 = 0.0;

        int32_t indexOfMeshRois1ClosestPointToIndexOfMinimumDistance2 = 
                                                              po2.indices[ 0 ];
        float weightBetweenMeshRois1ClosestPointToIndexOfMinimumDistance2 = 
                                                              weight1;

        weight = 0;
        int16_t textureLabel2 = 0;

        evolvingMesh->getPartialTriangleAreaWeighting( po2,
                                                       pp2,
                                                       weight1,
                                                       weight2,
                                                       weight3 );
        
        weigths2.push_back( weight1 );
        weigths2.push_back( weight2 );
        weigths2.push_back( weight3 );

        for ( int16_t pcount = 1; pcount < 3; ++pcount )
        {

          weight = weigths2[ pcount ];
          if ( weight > 
               weightBetweenMeshRois1ClosestPointToIndexOfMinimumDistance2 )
          {

            indexOfMeshRois1ClosestPointToIndexOfMinimumDistance2 = 
                                                         po2.indices[ pcount ];
            weightBetweenMeshRois1ClosestPointToIndexOfMinimumDistance2 = 
                                                         weight;

          }

        }

        textureLabel2 = textureRois1.getTexture(
                       indexOfMeshRois1ClosestPointToIndexOfMinimumDistance2 );

        
        L outputFiberLabel = getLabel( textureLabel1, textureLabel2 );
        labelledFibers[ outputFiberLabel ].push_back( outputFiber );

        ////////////////////////////////////////////////////////////////////////
        // filling :
        // - the connectivity matrix
        // - the average length matrix
        // - the ROI to mesh bundle map
        ////////////////////////////////////////////////////////////////////////

        int32_t indexOfPo1Vertex;
        int32_t indexOfPo2Vertex;
        int16_t labelOfPo1Vertex;
        int16_t labelOfPo2Vertex;
        for ( int16_t p1count = 0; p1count < 3; ++p1count )
        {

          indexOfPo1Vertex = po1.indices[ p1count ];
          labelOfPo1Vertex = textureRois1.getTexture( indexOfPo1Vertex );
          for (  int16_t p2count = 0; p2count < 3; ++p2count )
          {

            indexOfPo2Vertex = po2.indices[ p2count ];
            labelOfPo2Vertex = textureRois1.getTexture( indexOfPo2Vertex );
            weight = 0.5 * ( weigths1[ p1count ] + weigths2[ p2count ] );
            if ( labelOfPo1Vertex > LABEL_FOR_BACKGROUND )
            {

              if ( !connectivityMatrix.hasElement( labelOfPo1Vertex, 
                                                  indexOfPo2Vertex ) )
              {

                connectivityMatrix( labelOfPo1Vertex, indexOfPo2Vertex ) = 
                                                            ( double )weight;

              }
              else
              {

                connectivityMatrix( labelOfPo1Vertex, indexOfPo2Vertex  ) += 
                                                            ( double )weight;

              }

              if ( labelOfPo2Vertex > LABEL_FOR_BACKGROUND )
              {

                if ( !connectivityMatrix.hasElement( labelOfPo2Vertex, 
                                                    indexOfPo1Vertex ) )
                {

                  connectivityMatrix( labelOfPo2Vertex, indexOfPo1Vertex ) = 
                                                              ( double )weight;

                }
                else
                {

                  connectivityMatrix( labelOfPo2Vertex, indexOfPo1Vertex  ) += 
                                                              ( double )weight;

                }

              }

            }
            else if ( labelOfPo2Vertex > LABEL_FOR_BACKGROUND )
            {

              if ( !connectivityMatrix.hasElement( labelOfPo2Vertex, 
                                                  indexOfPo1Vertex ) )
              {

                connectivityMatrix( labelOfPo2Vertex, indexOfPo1Vertex ) = 
                                                            ( double )weight;

              }
              else
              {

                connectivityMatrix( labelOfPo2Vertex, indexOfPo1Vertex  ) += 
                                                            ( double )weight;

              }

            }

            if ( !averageLengthMatrix.isNull() )
            {

              if ( labelOfPo1Vertex > LABEL_FOR_BACKGROUND )
              {

                if ( !averageLengthMatrix->hasElement( labelOfPo1Vertex, 
                                                    indexOfPo2Vertex ) )
                {

                 ( *averageLengthMatrix )( labelOfPo1Vertex, 
                                           indexOfPo2Vertex ) = 
                                                            ( double )weight;

                }
                else
                {

                  ( *averageLengthMatrix )( labelOfPo1Vertex, 
                                            indexOfPo2Vertex ) += 
                                                            ( double )weight;

                }
                if ( labelOfPo2Vertex > LABEL_FOR_BACKGROUND )
                {

                  if ( !averageLengthMatrix->hasElement( labelOfPo2Vertex, 
                                                      indexOfPo1Vertex ) )
                  {

                    ( *averageLengthMatrix )( labelOfPo2Vertex, 
                                              indexOfPo1Vertex ) = 
                                                            ( double )weight;

                  }
                  else
                  {

                    ( *averageLengthMatrix )( labelOfPo2Vertex, 
                                              indexOfPo1Vertex ) += 
                                                            ( double )weight;

                  }

                }

              }
              else if ( labelOfPo2Vertex > LABEL_FOR_BACKGROUND )
              {

                if ( !averageLengthMatrix->hasElement( labelOfPo2Vertex, 
                                                    indexOfPo1Vertex ) )
                {

                  ( *averageLengthMatrix )( labelOfPo2Vertex, 
                                            indexOfPo1Vertex ) = 
                                                            ( double )weight;

                }
                else
                {

                  connectivityMatrix( labelOfPo2Vertex, indexOfPo1Vertex  ) += 
                                                              ( double )weight;

                }

              }

            }

          }

        }

      }

    }
    if ( !roiToMeshBundleMap.isNull() )
    {

      typename std::map< L, 
                    std::list< gkg::LightCurve3d< float > > >::const_iterator
        b = labelledFibers.begin(),
        be = labelledFibers.end();
      while ( b != be )
      {

        roiToMeshBundleMap->addCurve3ds( b->first, b->second );
        ++b;

      }

    }

  }

  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::"
             "addContribution( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& fiber, "
             "const gkg::TextureMap< int16_t>& textureRois1, "
             "const gkg::BoundingBox< int32_t >& rois1BoundingBox, "
             "gkg::Transform3d< float >* "
             "transform3dFromBundleRealFrameToROIs1RealFrame, "
             "const gkg::RCPointer< gkg::SceneModeler >& sceneModeler, "
             "const gkg::Volume< uint8_t >& rois1Mask, "
             "const gkg::RCPointer< gkg::EvolvingMesh >& evolvingMesh, "
             "gkg::SparseMatrix& connectivityMatrix, "
             "gkg::RCPointer< gkg::SparseMatrix >& averageLengthMatrix, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > >& "
             "roiToMeshBundleMap ) const" );


}


template < class L, class Compare >
inline
int16_t gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::getRange(
                                   const std::vector< int16_t >& labels,
                                   int32_t index,
                                   std::pair< int32_t, int32_t >& range ) const
{

  try
  {

    int32_t startIndex = index;
    int32_t pointCount = ( int32_t )labels.size();

    if ( ( index < 0 ) || ( index >= pointCount ) )
    {

      throw std::runtime_error( "bad index" );

    }

    // looking for the first point of the range
    while ( startIndex < pointCount )
    {

      if ( labels[ startIndex ] != 0 )
      {

        break;

      }

      ++ startIndex;

    }



    // if found
    if ( startIndex < pointCount )
    {

      // collecting the corresponding label
      int16_t label = labels[ startIndex ];
      int32_t endIndex = startIndex;

      // looking for the end point of the range
      while ( endIndex < pointCount )
      {

        if ( labels[ endIndex ] != label )
        {

          break;

        }

        ++ endIndex;

      }

      // filling the range information
      range.first = startIndex;
      range.second = endIndex - 1;

      return label;

    }
    return 0;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int16_t "
             "gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::"
             "getRange( "
             "const std::vector< int16_t >& labels, "
             "int32_t index, "
             "std::pair< int32_t, int32_t >& range ) const" );

}

template < class L, class Compare >
inline
L gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::getLabel(
                                                         int16_t label1,
                                                         int16_t label2 ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "L gkg::MeshAndTextureConnectivityMatrixFunctor< L, Compare >::"
             "getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


namespace gkg
{

template <>
inline
std::string 
MeshAndTextureConnectivityMatrixFunctor< std::string,
                                       std::less< std::string > >::getLabel(
                                                         int16_t label1, 
                                                         int16_t label2 ) const
{

  try
  {

    std::string stringLabel1, stringLabel2, stringLabel;
    stringLabel1 = gkg::StringConverter::toString( label1 );
    stringLabel2 = gkg::StringConverter::toString( label2 );
    if ( label1 < label2 )
    {

      stringLabel = stringLabel1 + "_" + stringLabel2;

    }
    else if (  label1 == label2 )
    {

      stringLabel = stringLabel1;

    }
    else 
    {

      stringLabel = stringLabel2 + "_" + stringLabel1;

    }

    return stringLabel;

  }
  GKG_CATCH( "inline "
             "std::string  "
             "MeshAndTextureConnectivityMatrixFunctor< std::string, "
             "std::less< std::string > >::getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


template <>
inline
int16_t 
MeshAndTextureConnectivityMatrixFunctor< int16_t,
                                       std::less< int16_t > >::getLabel(
                                                         int16_t label1,
                                                         int16_t label2 ) const
{

  try
  {

    int16_t label;
    if ( label1 < label2 )
    {

      label = label1 *10 + label2;

    }
    else if (  label1 == label2 )
    {

      label = label1;

    }
    else 
    {

      label = label2 *10 + label1;

    }

    return label;

  }
  GKG_CATCH( "inline "
             "int16_t  "
             "MeshAndTextureConnectivityMatrixFunctor< int16_t, "
             "std::less< int16_t > >::getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


}


#undef LABEL_FOR_MESH
#undef LABEL_FOR_BACKGROUND



#endif
