#include <gkg-anatomist-plugin-objects/AInteractiveBundleMapObject.h>
#include <gkg-anatomist-plugin-rendering-options/AInteractiveBundleMapRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-core-exception/Exception.h>

#include <anatomist/surface/glcomponent.h>
#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>

#include <parallel/algorithm>
#include <map>
#include <fstream>
#include <cmath>
#include <chrono>


#define DEFAULT_MINIMUM_FIBER_DISPLAY_PERCENTAGE    1.0
#define DEFAULT_MAXIMUM_FIBER_DISPLAY_PERCENTAGE  100.0
#define DEFAULT_MAXIMUM_LEVEL_OF_DETAIL_COUNT       5
#define DEFAULT_LEVEL_OF_DETAIL_INDEX               1
#define DEFAULT_FIELD_OF_VIEW_PARTITION_COUNT       8
#define DEFAULT_FIBER_SUBSAMPLING_FACTOR            1
#define DEFAULT_MAXIMUM_COMPRESSION_STEP           10
#define DEFAULT_MAXIMUM_COMPRESSION_ERROR           0.20


int32_t gkg::AInteractiveBundleMapObject::_classType =
                              gkg::AInteractiveBundleMapObject::registerClass();
Tree* gkg::AInteractiveBundleMapObject::_optionTree = 0;



//
// struct AInteractiveBundleMapObject::FiberInformation
//

gkg::AInteractiveBundleMapObject::FiberInformation::FiberInformation(
                                   const gkg::LightCurve3d< float >* thePointer,
                                   int32_t theBundleIndex,
                                   int32_t theFiberIndex,
                                   int32_t theLevelOfDetailIndex )
                                 : pointer( thePointer ),
                                   bundleIndex( theBundleIndex ),
                                   fiberIndex( theFiberIndex ),
                                   levelOfDetailIndex( theLevelOfDetailIndex )
{
}


gkg::AInteractiveBundleMapObject::FiberInformation::FiberInformation( 
               const gkg::AInteractiveBundleMapObject::FiberInformation& other )
                                : pointer( other.pointer ),
                                  bundleIndex( other.bundleIndex ),
                                  fiberIndex( other.fiberIndex ),
                                  levelOfDetailIndex( other.levelOfDetailIndex )
{
}


gkg::AInteractiveBundleMapObject::FiberInformation& 
gkg::AInteractiveBundleMapObject::operator=(
               const gkg::AInteractiveBundleMapObject::FiberInformation& other )
{

  try
  {

    pointer = other.pointer;
    bundleIndex = other.bundleIndex;
    fiberIndex = other.fiberIndex;
    levelOfDetailIndex = other.levelOfDetailIndex;

    return *this;

  }
  GKG_CATCH( "gkg::AInteractiveBundleMapObject::FiberInformation& "
             "gkg::AInteractiveBundleMapObject::operator=( "
             "const gkg::AInteractiveBundleMapObject::FiberInformation& "
             "other )" );

}


//
// struct AInteractiveBundleMapObject::FiberSegment
//


gkg::AInteractiveBundleMapObject::FiberSegment::FiberSegment(
                                                  int32_t theTotalFiberIndex,
                                                  int32_t theStartingPointIndex,
                                                  int32_t theEndingPointIndex )
                                               : totalFiberIndex(
                                                           theTotalFiberIndex ),
                                                 startingPointIndex(
                                                        theStartingPointIndex ),
                                                 endingPointIndex(
                                                           theEndingPointIndex )
{
}
                                                 
gkg::AInteractiveBundleMapObject::FiberSegment::FiberSegment(
                   const gkg::AInteractiveBundleMapObject::FiberSegment& other )
                                               : totalFiberIndex(
                                                        other.totalFiberIndex ),
                                                 startingPointIndex(
                                                     other.startingPointIndex ),
                                                 endingPointIndex(
                                                        other.endingPointIndex )
{
}



gkg::AInteractiveBundleMapObject::FiberSegment& 
gkg::AInteractiveBundleMapObject::operator=( 
                   const gkg::AInteractiveBundleMapObject::FiberSegment& other )
{

  try
  {

    totalFiberIndex = other.totalFiberIndex;
    startingPointIndex = other.startingPointIndex;
    endingPointIndex = other.endingPointIndex;

    return *this;

  }
  GKG_CATCH( "gkg::AInteractiveBundleMapObject::FiberSegment& "
             "gkg::AInteractiveBundleMapObject::operator=( "
             "const gkg::AInteractiveBundleMapObject::FiberSegment& "
             "other )" );

}


//
// class AInteractiveBundleMapObject
//

gkg::AInteractiveBundleMapObject::AInteractiveBundleMapObject(
        const gkg::RCPointer< gkg::BundleMap< std::string > >& bundleMap,
        const gkg::RCPointer< std::vector< gkg::RGBComponent > >& bundleColors )
                                 : _bundleMap( bundleMap ),
                                   _bundleColors( bundleColors )
{

  try
  {

    _type = gkg::AInteractiveBundleMapObject::_classType;

    ////////////////////////////////////////////////////////////////////////////
    // collecting the bounding box corresponding to the bundle map
    ////////////////////////////////////////////////////////////////////////////

    _boundingBox = _bundleMap->getBoundingBox();


    ////////////////////////////////////////////////////////////////////////////
    // initializing rendering parameter(s)
    ////////////////////////////////////////////////////////////////////////////

    _fiberLineWidth = 1.0;
    _fiberColorEncoding = 0;
    _fileNameScalarFeatureVolume = "";
    _fileNameScalarFeatureVolumeToBundleTransform3d = "";
    _lowerBound = -1e38;
    _upperBound = +1e38;

    _minimumFiberDisplayPercentage = DEFAULT_MINIMUM_FIBER_DISPLAY_PERCENTAGE;
    _maximumFiberDisplayPercentage = DEFAULT_MAXIMUM_FIBER_DISPLAY_PERCENTAGE;
    _levelOfDetailCount = DEFAULT_MAXIMUM_LEVEL_OF_DETAIL_COUNT;
    _currentLevelOfDetailIndex = DEFAULT_LEVEL_OF_DETAIL_INDEX;
    _fieldOfViewPartitionCount = DEFAULT_FIELD_OF_VIEW_PARTITION_COUNT;
    _enableZoomZero = true;

    _fiberSubSamplingFactor = DEFAULT_FIBER_SUBSAMPLING_FACTOR;

    _hasFiberCompression = false;
    _maximumCompressionStep = DEFAULT_MAXIMUM_COMPRESSION_STEP;
    _maximumCompressionError = DEFAULT_MAXIMUM_COMPRESSION_ERROR;


    ////////////////////////////////////////////////////////////////////////////
    // setting default material when using static material rnedering for fibers
    ////////////////////////////////////////////////////////////////////////////

    this->GetMaterial().SetDiffuse( 0.15, 0.15, 0.15, 0.3 );
    

    ////////////////////////////////////////////////////////////////////////////
    // creating structures to store linearly fibers
    ////////////////////////////////////////////////////////////////////////////

    int64_t bundleCount = bundle.getCurve3dCount();
    int64_t totalFiberCount = 0;
    gkg::BundleMap< std::string >::const_iterator
      b = _bundleMap->begin(),
      be = _bundleMap->end();
    while ( b != be )
    {

      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;
      totalFiberCount += bundleCount;
      ++ b;

    }

    _allFibers.reset(
      new std::vector< gkg::AInteractiveBundleMapObject::FiberInformation >(
                                                            totalFiberCount ) );
    _bundleStartIndices.reset( new std::vector< int32_t >( 
                                               _bundleMap->getBundleCount() ) );


    b = _bundleMap->begin();
    std::vector< gkg::AInteractiveBundleMapObject::FiberInformation >::iterator
      af = _allFibers->begin();
    std::vector< int32_t >::iterator
      bsi = _bundleStartIndices->begin();
    int32_t bundleIndex = 0;
    int32_t fiberIndex = 0;
    int32_t totalFiberIndex = 0;
    while ( b != be )
    {

      *bsi = totalFiberIndex;
      fiberIndex = 0;
      gkg::BundleMap< std::string >::Bundle::const_iterator
        f = b->second.begin(),
        fe = b->second.end();
      while ( f != fe )
      {

        af->pointer = &( *f );
        af->bundleIndex = bundleIndex;
        af->fiberIndex = fiberIndex;
        af->levelOfDetailIndex = -1;
        ++ fiberIndex;
        ++ totalFiberIndex;
        ++ af;
        ++ f;

      }
      ++ bundleIndex;
      ++ bsi;
      ++ b;

    }


    ////////////////////////////////////////////////////////////////////////////
    // randomly ordering the fibers of each bundle
    ////////////////////////////////////////////////////////////////////////////

    int64_t bundleIndex = 0;
    for ( bundleIndex = 0; bundleIndex < bundleCount; bundleIndex++ )
    {

      std::vector<
        gkg::AInteractiveBundleMapObject::FiberInformation >::const_iterator
        f1 = _allFibers->begin(),
        f2 = _allFibers->begin(),
        fe = _allFibers->end();
      std::advance( f1, _bundleStartIndices[ bundleIndex ] );
      if ( bundleIndex == bundleCount - 1 )
      {

        f2 = fe;

      }
      else
      {

        f2 = _allFibers->begin();
        std::advance( f2, _bundleStartIndices[ bundleIndex + 1 ] );

      }
      std::random_shuffle( f1, f2 );


    }


    ////////////////////////////////////////////////////////////////////////////
    // computing for all sub-field of views their map of fiber segments
    ////////////////////////////////////////////////////////////////////////////

    fillFiberSegmentMap()



/////////////////////
    fillIndicesInCubesVector();
    fillPointsToKeepVector();

    // adding icon
    if ( QObjectTree::TypeNames.find( _classType ) == 
         QObjectTree::TypeNames.end() )
    {

      std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                        "icon-interactive-fiber-clusters.png" );
      if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
      {

        QObjectTree::TypeIcons.erase( _classType );
        std::cerr << "icon " << iconPath << " not found!" << std::endl;

      }
      QObjectTree::TypeNames[ _classType ] = "INTERACTIVE_FIBER_CLUSTERS";

    }

    // precomputing a vector of pointer to all fibers and the corresponding 
    // vector of bundle and fiber indices in the orginal bundle map


  }
  GKG_CATCH( "gkg::AInteractiveBundleMapObject::AInteractiveBundleMapObject( "
             "const gkg::RCPointer< gkg::BundleMap< std::string > >& "
             "bundleMap, "
             "const gkg::RCPointer< std::vector< gkg::RGBComponent > >& "
             "bundleColors )" );

}


gkg::AInteractiveBundleMapObject::~AInteractiveBundleMapObject()
{

}


bool gkg::AInteractiveBundleMapObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::Is2DObject()" );

}


bool gkg::AInteractiveBundleMapObject::Is3DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::Is3DObject()" );

}


Tree* gkg::AInteractiveBundleMapObject::optionTree() const
{

  try
  {

    if ( !_optionTree )
    {

      Tree* t;
      Tree* t2;
      _optionTree = new Tree( true, "option tree" );
      t = new Tree( true, "File" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Reload" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::fileReload );
      t->insert( t2 );
      t2 = new Tree( true, "Save" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::saveStatic );
      t->insert( t2 );
      t2 = new Tree( true, "Rename object" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::renameObject );
      t->insert( t2 );

      t = new Tree( true, "Color" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Material" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::colorMaterial );
      t->insert( t2 );

      t = new Tree( true, "Referential" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Load" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::referentialLoad );
      t->insert( t2 );

      t = new Tree( true, "Render" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Options" );
      t2->setProperty( "callback",
                    &gkg::AInteractiveBundleMapRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::AInteractiveBundleMapObject::optionTree() const" );

}


float gkg::AInteractiveBundleMapObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MinX2D() const" );

}


float gkg::AInteractiveBundleMapObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MinY2D() const" );

}


float gkg::AInteractiveBundleMapObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MinZ2D() const" );

}


float gkg::AInteractiveBundleMapObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MaxX2D() const" );

}


float gkg::AInteractiveBundleMapObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MaxY2D() const" );

}


float gkg::AInteractiveBundleMapObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MaxZ2D() const" );

}


bool gkg::AInteractiveBundleMapObject::boundingBox( std::vector< float > & bmin,
                                             std::vector< float > & bmax ) const
{

  try
  {

    bmin.resize( 3 );
    bmax.resize( 3 );

    bmin[ 0 ] = _boundingBox.getLowerX();
    bmin[ 1 ] = _boundingBox.getLowerY();
    bmin[ 2 ] = _boundingBox.getLowerZ();

    bmax[ 0 ] = _boundingBox.getUpperX();
    bmax[ 1 ] = _boundingBox.getUpperY();
    bmax[ 2 ] = _boundingBox.getUpperZ();


    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


void gkg::AInteractiveBundleMapObject::fillFiberSegmentMap()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the total number of sub-field of views
    ////////////////////////////////////////////////////////////////////////////

    int32_t fieldOfViewCount = _fieldOfViewPartitionCount *
                               _fieldOfViewPartitionCount *
                               _fieldOfViewPartitionCount;


    ////////////////////////////////////////////////////////////////////////////
    // computing the fiber display percentage according to the minimum and
    // maximum fiber display percentage, and to the level of detail count
    ////////////////////////////////////////////////////////////////////////////

    computeFiberDisplayPercentages();


    ////////////////////////////////////////////////////////////////////////////
    // erasing former per level of detail fiber segment map and fiber segment
    // map for current level of detail
    ////////////////////////////////////////////////////////////////////////////

    _fiberSegmentMap.clear();
    _allFiberSegmentsForCurrentLevelOfDetail.clear();


    ////////////////////////////////////////////////////////////////////////////
    // assigning fibers to a level of detail
    ////////////////////////////////////////////////////////////////////////////

    auto starta = std::chrono::high_resolution_clock::now();

    int32_t bundleCount = ( int32_t )_bundleStartIndices.size();
    int32_t bundleIndex = 0;

    int64_t startFiberIndexForCurrentBundle = 0;
    int64_t fiberCountForCurrentBundle = 0;
    for ( bundleIndex = 0; bundleIndex < bundleCount; bundleIndex++ )
    {

      startFiberIndexForCurrentBundle = _bundleStartIndices[ bundleIndex ];
      if ( bundleIndex == bundleCount - 1 )
      {

        fiberCountForCurrentBundle = ( int64_t )_allFibers.size() - 
                                     startFiberIndexForCurrentBundle;

      }
      else
      {

        fiberCountForCurrentBundle = _bundleStartIndices[ bundleIndex + 1 ] - 
                                     startFiberIndexForCurrentBundle;

      }


      std::vector< int64_t > 
        fiberStartIndexForCurrentBundleAndNextLevelOfDetail(
                                                      _levelOfDetailCount + 1 );

      fiberStartIndexForCurrentBundleAndNextLevelOfDetail[ 0 ] = 0;
      int32_t levelOfDetailIndex = 0;
      for ( levelOfDetailIndex = 1;
            levelOfDetailIndex <= _levelOfDetailCount;
            levelOfDetailIndex++ )
      {

         fiberStartIndexForCurrentBundleAndNextLevelOfDetail[
           levelOfDetailIndex ] = 
           ( int64_t )( _fiberDisplayPercentages[ levelOfDetailIndex ] * 
                        ( fiberCountForCurrentBundle / 100.0 + 0.5 );

      }

      int32_t levelOfDetailIndex = 0;
      for ( fiberIndex = 0;
            fiberIndex < fiberCountForCurrentBundle;
            fiberIndex )
      {

        if ( fiberIndex >=
             fiberStartIndexForCurrentBundleAndNextLevelOfDetail[
                                                          levelOfDetailIndex ] )
        {

          ++ levelOfDetailIndex;

        }
        ( *_allFibers )[ startFiberIndexForCurrentBundle + fiberIndex ].
          levelOfDetailIndex = levelOfDetailIndex;

      }

    }

    auto stopa = std::chrono::high_resolution_clock::now();
    auto durationa = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                               stopa - starta );
    std::cout << "\n" << "Time taken to assign levels of detail: "
              << durationa.count() << " milliseconds" << "\n";


    ////////////////////////////////////////////////////////////////////////////
    // preallocating memory for our vector (of vectors) of tuples vector
    ////////////////////////////////////////////////////////////////////////////

    auto startb = std::chrono::high_resolution_clock::now();
    std::vector< std::vector< int32_t > > memoryNeededInVector(
                             _maxLevelOfDetail + 1, 
                             std::vector< int32_t > ( totalNumberOfCubes, 0 ) );

    
    b = _bundleMap->begin();
    bundleIndex = 0;


    // calculating size needed for each cube per level of detail
    while ( b != be )
    {
      
      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;

      fiberCount = bundle.getCurve3dCount();

      gkg::AInteractiveBundleMapPreallocateContext
        aInteractiveBundleMapPreallocateContext( bundle,
                                                 bundleIndex,
                                                 _numberOfCubesPerAxis,
                                                 _boundingBox,
                                                 fiberToLevelOfDetail,
                                                 memoryNeededInVector );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &aInteractiveBundleMapPreallocateContext,
                      0,                                   // start index
                      fiberCount,                          // count
                      0 );                                 // parallel
      threadedLoop.launch();

      ++ b;
      ++ bundleIndex;

    }

    _indicesInCubesVector.resize( _maxLevelOfDetail + 1 );
    _indicesInCubesVector[ 0 ].resize( totalNumberOfCubes );


    // preallocating memory needed to fill our vector in the next step
    for ( int levelOfDetail = 1;
          levelOfDetail <= _maxLevelOfDetail;
          ++ levelOfDetail )
    {
      
      _indicesInCubesVector[ levelOfDetail ].resize( totalNumberOfCubes );


      for ( int indiceOfCube = 0;
            indiceOfCube < totalNumberOfCubes;
            ++ indiceOfCube )
      {
        
        _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].reserve( 
                        memoryNeededInVector[ levelOfDetail ][ indiceOfCube ] );
        // std::cout << levelOfDetail << " " << indiceOfCube << " " 
        //           << memoryNeededInVector[ levelOfDetail ][ indiceOfCube ]
        //           << "\n";

      }

    }

    auto stopb = std::chrono::high_resolution_clock::now();
    auto durationb = std::chrono::duration_cast< std::chrono::milliseconds >( 
                                                               stopb - startb );
    std::cout << "Time taken to preallocate vector: " << durationb.count()
              << " milliseconds" << "\n";

    ////////////////////////////////////////////////////////////////////////////
    // filling with the indices of the fibers passing through the cubes
    ////////////////////////////////////////////////////////////////////////////

    auto startc = std::chrono::high_resolution_clock::now();

    b = _bundleMap->begin();
    bundleIndex = 0;


    // filling our vector with the indices of the fibers
    while ( b != be )
    {
      
      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;

      fiberCount = bundle.getCurve3dCount();

      gkg::AInteractiveBundleMapFillContext
        aInteractiveBundleMapFillContext( bundle,
                                          bundleIndex,
                                          _numberOfCubesPerAxis,
                                          _boundingBox,
                                          fiberToLevelOfDetail,
                                          _indicesInCubesVector );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &aInteractiveBundleMapFillContext,
                      0,                                   // start index
                      fiberCount,                          // count
                      0 );                                 // parallel
      threadedLoop.launch();

      ++ b;
      ++ bundleIndex;

    }

    auto stopc = std::chrono::high_resolution_clock::now();
    auto durationc = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                               stopc - startc );
    std::cout << "Time taken to fill vector: " << durationc.count()
              << " milliseconds" << "\n";

    ////////////////////////////////////////////////////////////////////////////
    // sorting the vector ( sorting the tuples vector for each cube )
    ////////////////////////////////////////////////////////////////////////////

    auto startd = std::chrono::high_resolution_clock::now();
    int totalNumberOfTuples = 0, tuplesInCube = 0;


    // sort indices inside each cube per level of detail in the right order
    for ( int indiceOfCube = 0;
          indiceOfCube < totalNumberOfCubes;
          ++ indiceOfCube )
    {


      for ( int levelOfDetail = 1;
            levelOfDetail <= _maxLevelOfDetail;
            ++ levelOfDetail )
      {

        tuplesInCube = (int)
          _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].size();

        if ( tuplesInCube != 0 )
        {
          
          __gnu_parallel::sort(
            _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].begin(),
            _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].end() );
          // std::cout << levelOfDetail << " " << indiceOfCube << " " 
          //           << _indicesInCubesVector[ levelOfDetail ]
          //              [ indiceOfCube ].size() << "\n";
          totalNumberOfTuples += tuplesInCube;

        }

      }

    }


    /* for ( indiceOfCube = 0; indiceOfCube < totalNumberOfCubes; ++ indiceOfCube )
    {

      for ( levelOfDetail = 0; levelOfDetail <= 5; ++ levelOfDetail )
      {

        std::cout << indiceOfCube << " " << levelOfDetail << " "
        << memoryNeededInVector[ levelOfDetail ][ indiceOfCube ] << "\n";

      }

    } */

    auto stopd = std::chrono::high_resolution_clock::now();
    auto durationd = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                               stopd - startd );
    std::cout << "Time taken to sort vector: " << durationd.count()
              << " milliseconds" << "\n";
    // std::cout << "total number of tuples: " << totalNumberOfTuples
    //           << "\n";

    ////////////////////////////////////////////////////////////////////////////
    // preparing a vector to render minimum zoom faster
    ////////////////////////////////////////////////////////////////////////////

    setIndicesZoomZero( _currentLevelOfDetail );


  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::"
             "fillIndicesInCubesVector()" );

}


void gkg::AInteractiveBundleMapObject::computeFiberDisplayPercentages()
{

  try
  {

    _fiberDisplayPercentages.resize( _levelOfDetailCount + 1 );
    _fiberDisplayPercentages[ 0 ] = 0.0;

    if ( _levelOfDetailCount > 1 )
    {

      int32_t levelOfDetailIndex = 0;
      for ( levelOfDetailIndex = 1;
            levelOfDetailIndex <= _levelOfDetailCount;
            levelOfDetailIndex++ )
      {

        _fiberDisplayPercentages[ levelOfDetailIndex ] =
                              _minimumFiberDisplayPercentage *
                              std::pow( _maximumFiberDisplayPercentage /
                                        _minimumFiberDisplayPercentage,
                                        ( ( double )levelOfDetailIndex - 1 ) /
                                        ( ( double )_levelOfDetailCount - 1 ) );

      }

    }
    else if ( _levelOfDetailCount == 1 )
    {

      _fiberDisplayPercentages[ 1 ] = _maximumFiberDisplayPercentage;

    }

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::"
             "computeFiberDisplayPercentages() " );

}




float gkg::AInteractiveBundleMapObject::getFiberLineWidth() const
{

  try
  {

    return _fiberLineWidth;

  }
  GKG_CATCH( "float "
             "gkg::AInteractiveBundleMapObject::getFiberLineWidth() const" );

}


void 
gkg::AInteractiveBundleMapObject::setFiberLineWidth( float value )
{

  try
  {

    if ( value < 1.0 )
    {

      throw std::runtime_error(
                             "fiber line width must be greater or equal to 1" );

    }
    _fiberLineWidth = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setFiberLineWidth( "
             "float value )" );

}


int32_t gkg::AInteractiveBundleMapObject::getFiberColorEncoding() const
{

  try
  {

    return _fiberColorEncoding;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getFiberColorEncoding() "
             "const" );

}


const std::string& gkg::AInteractiveBundleMapObject::
                                          getFileNameScalarFeatureVolume() const
{

  try
  {

    return _fileNameScalarFeatureVolume;

  }
  GKG_CATCH( "const std::string& "
             "gkg::AInteractiveBundleMapObject::getFileNameScalarFeatureVolume("
             ") const" );

}


const std::string& 
gkg::AInteractiveBundleMapObject::
                       getFileNameScalarFeatureVolumeToBundleTransform3d() const
{

  try
  {

    return _fileNameScalarFeatureVolumeToBundleTransform3d;

  }
  GKG_CATCH( "const std::string& "
             "gkg::AInteractiveBundleMapObject::"
             "getFileNameScalarFeatureVolumeToBundleTransform3d() const" );

}


double gkg::AInteractiveBundleMapObject::getLowerBound() const
{

  try
  {

    return _lowerBound;

  }
  GKG_CATCH( "double gkg::AInteractiveBundleMapObject::getLowerBound() const" );

}


double gkg::AInteractiveBundleMapObject::getUpperBound() const
{

  try
  {

    return _upperBound;

  }
  GKG_CATCH( "double gkg::AInteractiveBundleMapObject::getUpperBound() const" );

}


void gkg::AInteractiveBundleMapObject::setFiberColorEncoding(
              int32_t colorEncodingType,
              const std::string& fileNameScalarFeatureVolume,
              const std::string& fileNameScalarFeatureVolumeToBundleTransform3d,
              double lowerBound,
              double upperBound )
{

  try
  {

    if ( ( colorEncodingType < 0 ) || ( colorEncodingType > 3 ) )
    {

      throw std::runtime_error( "fiber color encoding must be 0, 1, 2 or 3" );

    }
    _fiberColorEncoding = colorEncodingType;

    if ( _fiberColorEncoding == 3 )
    {

      bool hasChanged = false;
      if ( ( fileNameScalarFeatureVolume != 
             _fileNameScalarFeatureVolume ) ||
           ( fileNameScalarFeatureVolumeToBundleTransform3d != 
             _fileNameScalarFeatureVolumeToBundleTransform3d ) ||
           ( lowerBound != _lowerBound ) ||
           ( upperBound != _upperBound ) )
      {

        hasChanged = true;

      }
      _fileNameScalarFeatureVolume = fileNameScalarFeatureVolume;
      _fileNameScalarFeatureVolumeToBundleTransform3d = 
                                 fileNameScalarFeatureVolumeToBundleTransform3d;
      _lowerBound = lowerBound;
      _upperBound = upperBound;

      if ( hasChanged )
      {

        this->computeScalarFeatureVolumeBasedColors();

      }

    }

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setFiberColorEncoding( "
             "int32_t value )" );

}


double gkg::AInteractiveBundleMapObject::getMinRatioPercent() const
{

  try
  {

    return _minRatioPercent;

  }
  GKG_CATCH( "double " 
             "gkg::AInteractiveBundleMapObject::getMinRatioPercent() const" );

}


void gkg::AInteractiveBundleMapObject::setMinRatioPercent( double value )
{

  try
  {

    if ( value < 0.1 )
    {

      value = 0.1;

    }
    else if ( value > _maxRatioPercent )
    {

      value = _maxRatioPercent;

    }
    _minRatioPercent = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setMinRatioPercent( "
             "double value )" );

}


double gkg::AInteractiveBundleMapObject::getMaxRatioPercent() const
{

  try
  {

    return _maxRatioPercent;

  }
  GKG_CATCH( "double "
             "gkg::AInteractiveBundleMapObject::getMaxRatioPercent() const" );

}


void gkg::AInteractiveBundleMapObject::setMaxRatioPercent( double value )
{

  try
  {

    if ( value < 0.1 )
    {

      value = 0.1;

    }
    else if ( value > 100 )
    {

      value = 100;

    }
    else if ( value < _minRatioPercent )
    {

      value = _minRatioPercent;

    }
    _maxRatioPercent = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setMaxRatioPercent( "
             "double value )" );

}


int32_t gkg::AInteractiveBundleMapObject::getMaxLevelOfDetail() const
{

  try
  {

    return _maxLevelOfDetail;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getMaxLevelOfDetail() const" );

}


void gkg::AInteractiveBundleMapObject::setMaxLevelOfDetail( int32_t value )
{

  try
  {

    if ( value < 0 )
    {

      throw std::runtime_error(
                "rendering max level of detail must be greater or equal to 0" );

    }
    else if ( _currentLevelOfDetail > value )
    {

      _currentLevelOfDetail = value;

    }
    _maxLevelOfDetail = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setMaxLevelOfDetail( "
             "int32_t value )" );

}


int32_t gkg::AInteractiveBundleMapObject::getCurrentLevelOfDetail() const
{

  try
  {

    return _currentLevelOfDetail;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getCurrentLevelOfDetail() "
             "const" );

}


void gkg::AInteractiveBundleMapObject::setCurrentLevelOfDetail( int32_t value )
{

  try
  {

    if ( value < 0 )
    {

      value = 0;

    }
    else if ( value > _maxLevelOfDetail )
    {

      value = _maxLevelOfDetail;

    }
    _currentLevelOfDetail = value;
    setIndicesZoomZero( _currentLevelOfDetail );

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setCurrentLevelOfDetail( "
             "int32_t value )" );

}


int32_t gkg::AInteractiveBundleMapObject::getNumberOfCubesPerAxis() const
{

  try
  {

    return _numberOfCubesPerAxis;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getNumberOfCubesPerAxis() "
             "const" );

}


void gkg::AInteractiveBundleMapObject::setNumberOfCubesPerAxis( int32_t value )
{

  try
  {

    if ( value < 0 )
    {

      value = 0;

    }
    _numberOfCubesPerAxis = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setNumberOfCubesPerAxis( "
             "int32_t value )" );

}


int32_t gkg::AInteractiveBundleMapObject::getFiberSubSamplingFactor() const
{

  try
  {

    return _fiberSubSamplingFactor;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getFiberSubSamplingFactor() "
             "const" );

}


void 
gkg::AInteractiveBundleMapObject::setFiberSubSamplingFactor( int32_t value )
{

  try
  {

    if ( value < 1 )
    {

      value = 1;

    }
    _fiberSubSamplingFactor = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setFiberSubSamplingFactor( "
             "int32_t value )" );

}


bool gkg::AInteractiveBundleMapObject::getEnableResampling() const
{

  try
  {

    return _enableResampling;

  }
  GKG_CATCH( "bool "
             "gkg::AInteractiveBundleMapObject::getEnableResampling() "
             "const" );

}


void gkg::AInteractiveBundleMapObject::setEnableResampling( bool value )
{

  try
  {

    _enableResampling = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setEnableResampling( "
             "bool value )" );

}


double gkg::AInteractiveBundleMapObject::getResamplingMaximumStep() const
{

  try
  {

    return _resamplingMaximumStep;

  }
  GKG_CATCH( "double "
             "gkg::AInteractiveBundleMapObject::getResamplingMaximumStep() "
             "const" );

}


void 
gkg::AInteractiveBundleMapObject::setResamplingMaximumStep( double value )
{

  try
  {

    if ( value < 0 )
    {

      value = 0;

    }
    _resamplingMaximumStep = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setResamplingMaximumStep( "
             "double value )" );

}


double gkg::AInteractiveBundleMapObject::getResamplingErrorThreshold() const
{

  try
  {

    return _resamplingErrorThreshold;

  }
  GKG_CATCH( "double "
             "gkg::AInteractiveBundleMapObject::getResamplingErrorThreshold() "
             "const" );

}


void 
gkg::AInteractiveBundleMapObject::setResamplingErrorThreshold( double value )
{

  try
  {

    if ( value < 0 )
    {

      value = 0;

    }
    _resamplingErrorThreshold = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setResamplingErrorThreshold( "
             "double value )" );

}


bool gkg::AInteractiveBundleMapObject::render( 
                                         anatomist::PrimList& primitiveList,
                                         const anatomist::ViewState& viewState )
{

  try
  {

    anatomist::GLComponent* glComponent = anatomist::GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::AFiberObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::AInteractiveBundleMapObject::glMakeBodyGLL(
                                      const anatomist::ViewState& /*viewState*/,
                                      const anatomist::GLList& glList ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating a new list
    ////////////////////////////////////////////////////////////////////////////

    glNewList( glList.item(), GL_COMPILE );

    glDisable( GL_LIGHTING );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );


    ////////////////////////////////////////////////////////////////////////////
    // if zoom is low enough, taking up the minimum zoom indices
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
      indicesToDraw;
    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
      compressedIndicesToDraw;
    int compressedIndicesToDrawSize;

    if ( _enableZoomZero )
    {

      compressedIndicesToDraw = _indicesZoomZero;
      compressedIndicesToDrawSize = _indicesZoomZero.size();

    }
    

    ////////////////////////////////////////////////////////////////////////////
    // if zoom is high enough, preparing the tuples of fiber coordinates to draw
    ////////////////////////////////////////////////////////////////////////////

    else
    {
    
      indicesToDraw
        = getIndicesFromSelectedCubes( _currentLevelOfDetail );
      compressedIndicesToDraw
        = getCompressedIndices( indicesToDraw );
      compressedIndicesToDrawSize = compressedIndicesToDraw.size();
    
    }

    
    ////////////////////////////////////////////////////////////////////////////
    // collecting transparency
    ////////////////////////////////////////////////////////////////////////////

    const anatomist::Material& material = this->material();
    // by default, fibers are colored using the default material
    float red = ( float )material.Diffuse( 0 );
    float green = ( float )material.Diffuse( 1 );
    float blue = ( float )material.Diffuse( 2 );
    float opacity = ( float )material.Diffuse( 3 );

    
    ////////////////////////////////////////////////////////////////////////////
    // drawing fibers
    ////////////////////////////////////////////////////////////////////////////

    auto start = std::chrono::high_resolution_clock::now();

    glLineWidth( _fiberLineWidth );


    glBegin( GL_LINES );


    std::tuple< int32_t, int32_t, int32_t, int32_t > indicesInCube;
    std::tuple< int32_t, int32_t, int32_t, int32_t > indicesInCubePrev;

    int32_t bundleIndex, fiberIndex, pointIndexMin, pointIndexMax;
    int32_t bundleIndexPrev, fiberIndexPrev, pointIndexMaxPrev;

    int32_t pointIndex1, pointIndex2;
    int32_t pointIndexPrev1, pointIndexPrev2;
    int32_t iMin, iMax, i1, i2;
    
    gkg::Vector3d< float > color;
    gkg::BundleMap< std::string >::const_iterator
      b = _bundleMap->begin();
    std::vector< gkg::RGBComponent >::const_iterator
      c = _bundleColors->begin();
    std::vector< float >::const_iterator cPrime1, cPrime2;

    if ( _fiberColorEncoding == 3 )
    {

      cPrime1 = _scalarFeatureVolumeBasedColors->begin();
      cPrime2 = _scalarFeatureVolumeBasedColors->begin();
      std::advance( cPrime2, _fiberSubSamplingFactor );

    }

    std::vector< gkg::LightCurve3d< float > >::const_iterator f, fe;
    gkg::LightCurve3d< float >::const_iterator p1, p2, pe;

    b = _bundleMap->begin();
    c = _bundleColors->begin();
    f = ( b->second ).begin();

    p1 = f->begin();
    p2 = p1;
    pe = p1;


    for ( int i = 0; i < compressedIndicesToDrawSize; ++ i )
    {

      indicesInCube = compressedIndicesToDraw[ i ];
      
      bundleIndex = std::get< 0 > ( indicesInCube );
      fiberIndex = std::get< 1 > ( indicesInCube );
      pointIndexMin = std::get< 2 > ( indicesInCube );
      pointIndexMax = std::get< 3 > ( indicesInCube );

      // loading previous fiber to compare with current fiber
      if ( i > 0 )
      { 

        indicesInCubePrev = compressedIndicesToDraw[ i - 1 ];
        bundleIndexPrev = std::get< 0 > ( indicesInCubePrev );
        fiberIndexPrev = std::get< 1 > ( indicesInCubePrev );
        pointIndexMaxPrev = std::get< 3 > ( indicesInCubePrev );

      }
      else
      {

        bundleIndexPrev = 0;
        fiberIndexPrev = 0;
        pointIndexMaxPrev = 0;

      }

      // if previous bundle is different, advance to next bundle
      if ( bundleIndex - bundleIndexPrev != 0 )
      {

        std::advance( b, bundleIndex - bundleIndexPrev );
        std::advance( c, bundleIndex - bundleIndexPrev );
        f = ( b->second ).begin();

        fiberIndexPrev = 0;
        pointIndexMaxPrev = 0;

        p1 = f->begin();
        p2 = p1;
        pe = p1;

      }

      // if previous fiber is different, advance to next fiber
      if ( fiberIndex - fiberIndexPrev != 0 )
      {

        std::advance( f, fiberIndex - fiberIndexPrev );

        pointIndexMaxPrev = 0;

        p1 = f->begin();
        p2 = p1;
        pe = p1;

      }
      // if previous fiber is the same, advance to next segment
      else
      {

        std::advance( p1, pointIndexMin - pointIndexMaxPrev );
        std::advance( p2, pointIndexMin - pointIndexMaxPrev );
        std::advance( pe, pointIndexMax - pointIndexMaxPrev );

      }

      if ( _fiberColorEncoding == 0 )
      {

        const gkg::RGBComponent& rgbComponent = *c;
        red = ( float )rgbComponent.r / 255.0f;
        green = ( float )rgbComponent.g / 255.0f;
        blue = ( float )rgbComponent.b / 255.0f;

      }
      
      // applying on the fly resampling
      const std::vector< int32_t >&
        pointsToKeep = _pointsToKeepVector[ bundleIndex ][ fiberIndex ];

      pointIndexPrev1 = pointIndexMin;
      pointIndexPrev2 = pointIndexMin;

      if ( _enableResampling )
      {

        iMin = std::lower_bound( pointsToKeep.begin(),
                                 pointsToKeep.end(),
                                 pointIndexMin )
              - pointsToKeep.begin();
        iMax = std::lower_bound( pointsToKeep.begin(),
                                 pointsToKeep.end(),
                                 pointIndexMax )
              - pointsToKeep.begin();
        i1 = iMin;
        i2 = iMin + _fiberSubSamplingFactor;

        pointIndex1 = pointsToKeep[ i1 ];
        pointIndex2 = pointsToKeep[ i2 ];

      }
      else
      {

        pointIndex1 = pointIndexMin;
        pointIndex2 = pointIndexMin + _fiberSubSamplingFactor;
        i1 = 0, i2 = 0, iMax = 0;

      }
      
      // drawing a segment of points from tuple coordinates
      while ( pointIndex2 <= pointIndexMax && i2 <= iMax )
      {

        std::advance( p1, pointIndex1 - pointIndexPrev1 );
        std::advance( p2, pointIndex2 - pointIndexPrev2 );

        if ( _fiberColorEncoding == 1 )
        {

          color = ( *p2 - *p1 ).absoluteCoordinates();
          color.normalize();
          red = color.x;
          green = color.y;
          blue = color.z;

        }
        else if ( _fiberColorEncoding == 3 )
        {

          red = *cPrime1;
          green = 0.0;
          blue = 1 - *cPrime1;

          std::advance( cPrime1, _fiberSubSamplingFactor );
          std::advance( cPrime2, _fiberSubSamplingFactor );

        }
        glColor4f( red, green, blue, opacity );
        glVertex3f( p1->x, p1->y, p1->z );

        if ( _fiberColorEncoding == 3 )
        {

          red = *cPrime2;
          green = 0.0;
          blue = 1 - *cPrime2;

        }
        glColor4f( red, green, blue, opacity );
        glVertex3f( p2->x, p2->y, p2->z );

        pointIndexPrev1 = pointIndex1;
        pointIndexPrev2 = pointIndex2;
        
        if ( _enableResampling )
        {

          i1 += _fiberSubSamplingFactor;
          i2 += _fiberSubSamplingFactor;
          pointIndex1 = pointsToKeep[ i1 ];
          pointIndex2 = pointsToKeep[ i2 ];

        }
        else
        {

          pointIndex1 += _fiberSubSamplingFactor;
          pointIndex2 += _fiberSubSamplingFactor;

        }

      }

    }

    glEnd();
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                                 stop - start );
    std::cout << "\n" << "Time taken to draw: " << duration.count()
              << " milliseconds" << "\n";


    ////////////////////////////////////////////////////////////////////////////
    // ending the list
    ////////////////////////////////////////////////////////////////////////////
    glEndList();

    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::AInteractiveBundleMapObject::load(
                                    const std::string& fileName,
                                    anatomist::ObjectReader::PostRegisterList &,
                                    carto::Object /* options */ )
{

  try
  {

    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 10U );
    std::string fileNameBundleMap = fileNameRoot + ".bundles";
    std::string fileNameHierarchy = fileNameRoot + ".hie";

    // reading bundle map
    gkg::RCPointer< gkg::BundleMap< std::string > >
      bundleMap( new gkg::BundleMap< std::string > );
    gkg::Reader::getInstance().read( fileNameBundleMap, *bundleMap );

    // exploring hierarchy and collecting bundle colors
    std::map< std::string, gkg::RGBComponent > colors;

    gkg::File hierarchyFile( fileNameHierarchy );

    if ( hierarchyFile.isExisting() )
    {

      std::ifstream is( fileNameHierarchy.c_str() );

      std::string syntaxId;
      std::string label;
      gkg::RGBComponent rgbComponent;
      int32_t red = 0;
      int32_t green = 0;
      int32_t blue = 0;
      while ( !is.eof() )
      {

        is >> syntaxId;

        if ( syntaxId == "name" )
        {

          is >> label;
          is >> syntaxId;
          if ( syntaxId == "color" )
          {

            is >> red;
            is >> green;
            is >> blue;
            colors[ label ] = gkg::RGBComponent( ( uint8_t )red,
                                                 ( uint8_t )green,
                                                 ( uint8_t )blue ) ;

          }

        }

      }

      is.close();

    }

    // in case no hierarchy file found, randomly generating colors
    gkg::RandomColorGenerator randomColorGenerator( 0.85, 1.00 );

    // building bundle color map
    int32_t colorCount = bundleMap->getBundleCount();
    gkg::RCPointer< std::vector< RGBComponent > > 
      bundleColors( new std::vector< RGBComponent >( colorCount ) );
    gkg::BundleMap< std::string >::const_iterator
      b = bundleMap->begin(),
      be = bundleMap->end();
    std::vector< gkg::RGBComponent >::iterator c = bundleColors->begin();
    std::map< std::string, gkg::RGBComponent >::const_iterator color;
    while ( b != be )
    {

      if ( hierarchyFile.isExisting() )
      {

        color = colors.find( b->first );
        if ( color != colors.end() )
        {

          *c = color->second;

        }
        else
        {

        *c = randomColorGenerator.getRandomColor();

        }

      }
      else
      {

        *c = randomColorGenerator.getRandomColor();

      }
      ++ b;
      ++ c;

    }

    // creating AInteractiveBundleMapObject object
    gkg::AInteractiveBundleMapObject* aInteractiveBundleMapObject
                         = new gkg::AInteractiveBundleMapObject( bundleMap,
                                                                 bundleColors );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aInteractiveBundleMapObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::AInteractiveBundleMapObject::load( "
             "const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::AInteractiveBundleMapObject::registerClass()
{

  try
  {

    // associating a new loader to the .clusters extension
    anatomist::ObjectReader::registerLoader(
                                       "iclusters",
                                       gkg::AInteractiveBundleMapObject::load );

    // creating a new class type
    _classType = registerObjectType( "INTERACTIVE_FIBER_CLUSTERS" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "INTERACTIVE_FIBER_CLUSTERS loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AInteractiveBundleMapObject::registerClass()" );

}


int32_t gkg::AInteractiveBundleMapObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AInteractiveBundleMapObject::getClassType()" );

}


////////////////////////////////////////////////////////////////////////////////
// level of detail methods
////////////////////////////////////////////////////////////////////////////////

void gkg::AInteractiveBundleMapObject::getStaticCubeCoordinates(
                                      const gkg::Vector3d< float >& point,
                                      int32_t& indiceOfCube,
                                      const int32_t& numberOfCubesPerAxis,
                                      const BoundingBox< float >& boundingBox )
{

  try
  {

    int32_t iCube = (int32_t)( (float)numberOfCubesPerAxis 
                               * ( point.x - boundingBox.getLowerX() )
                               / ( boundingBox.getSpanX() + 0.1f ) );
    int32_t jCube = (int32_t)( (float)numberOfCubesPerAxis 
                               * ( point.y - boundingBox.getLowerY() )
                               / ( boundingBox.getSpanY() + 0.1f ) );
    int32_t kCube = (int32_t)( (float)numberOfCubesPerAxis 
                               * ( point.z - boundingBox.getLowerZ() )
                               / ( boundingBox.getSpanZ() + 0.1f ) );
    indiceOfCube = iCube + jCube * numberOfCubesPerAxis
                         + kCube * numberOfCubesPerAxis * numberOfCubesPerAxis;

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::getCubeCoordinates( "
             "const gkg::Vector3d< float >& point, "
             "int32_t& indiceOfCube, "
             "const int32_t& numberOfCubesPerAxis, "
             "const BoundingBox< float >& boundingBox )" );

}


void gkg::AInteractiveBundleMapObject::getCubeCoordinates(
                                            const gkg::Vector3d< float >& point,
                                            int32_t& indiceOfCube ) const
{

  try
  {

    int32_t iCube = (int32_t)( (float)_numberOfCubesPerAxis 
                               * ( point.x - MinX2D() )
                               / ( _boundingBox.getSpanX() + 0.1f ) );
    int32_t jCube = (int32_t)( (float)_numberOfCubesPerAxis 
                               * ( point.y - MinY2D() )
                               / ( _boundingBox.getSpanY() + 0.1f ) );
    int32_t kCube = (int32_t)( (float)_numberOfCubesPerAxis 
                               * ( point.z - MinZ2D() )
                               / ( _boundingBox.getSpanZ() + 0.1f ) );
    indiceOfCube = iCube + jCube*_numberOfCubesPerAxis
                         + kCube*_numberOfCubesPerAxis*_numberOfCubesPerAxis;

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::getCubeCoordinates( "
             "const gkg::Vector3d< float >& point, "
             "int32_t& indiceOfCube )" );

}


std::vector< std::pair< int32_t, int32_t > >
  gkg::AInteractiveBundleMapObject::getSelectedCubes(
                                            int32_t currentLevelOfDetail ) const
{

  try
  {

    std::vector< std::pair< int32_t, int32_t > > selectedCubes;
    std::pair< int32_t, int32_t > pairCubeLevelOfDetail;

    int totalNumberOfCubes = pow( _numberOfCubesPerAxis, 3 );
    int selectedCubesSize = totalNumberOfCubes;
    selectedCubes.reserve( selectedCubesSize );


    for ( int indiceOfCube = 0;
          indiceOfCube < totalNumberOfCubes;
          ++ indiceOfCube )
    {

      pairCubeLevelOfDetail.first = indiceOfCube;
      pairCubeLevelOfDetail.second = currentLevelOfDetail;
      selectedCubes.push_back( pairCubeLevelOfDetail );

    }

    return selectedCubes;

  }
  GKG_CATCH( "std::vector< std::pair< int32_t, int32_t > > "
             "gkg::AInteractiveBundleMapObject::getSelectedCubes( "
             "int32_t currentLevelOfDetail )" );

}


std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
  gkg::AInteractiveBundleMapObject::getIndicesFromSelectedCubes(
    int32_t currentLevelOfDetail ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // concatenating tuples of fiber coordinates from selected cubes
    ////////////////////////////////////////////////////////////////////////////

    auto starta = std::chrono::high_resolution_clock::now();

    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
        indices;
    int indicesSize = 0;

    int totalNumberOfCubes = pow( _numberOfCubesPerAxis, 3 );


    // allocating memory for the fiber indices vector to come
    for ( int levelOfDetail = 1;
          levelOfDetail <= currentLevelOfDetail;
          ++ levelOfDetail )
    {

    
      for ( int indiceOfCube = 0;
            indiceOfCube < totalNumberOfCubes;
            ++ indiceOfCube )
      {

        int indicesInCubesSize =
          _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].size();
        indicesSize += indicesInCubesSize;
      
      }

    }

    indices.reserve( indicesSize );


    // filling ( and sorting ) the tuples taken from those storaged in cubes
    for ( int levelOfDetail = 1;
          levelOfDetail <= currentLevelOfDetail;
          ++ levelOfDetail )
    {
    

      for ( int indiceOfCube = 0;
            indiceOfCube < totalNumberOfCubes;
            ++ indiceOfCube )
      {

        int indicesInCubesSize =
          _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].size();

        if ( indicesInCubesSize != 0 )
        {
          
          indices.insert( indices.end(),
            _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].begin(),
            _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].end() );

        }

      }

    }

    __gnu_parallel::sort( indices.begin(),
                          indices.end() );

    auto stopa = std::chrono::high_resolution_clock::now();
    auto durationa = std::chrono::duration_cast< std::chrono::milliseconds >( 
                                                               stopa - starta );
    // std::cout << "indicesSize: " << indicesSize << "\n";
    std::cout << "\n" << "Time taken to sort indices: " << durationa.count()
              << " milliseconds" << "\n";

    return indices;

  }
  GKG_CATCH( "std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > > "
             "gkg::AInteractiveBundleMapObject::"
             "getIndicesFromSelectedCubes( int32_t currentLevelOfDetail ) "
             "const" );

}


std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
  gkg::AInteractiveBundleMapObject::getCompressedIndices(
    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
      indices ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // merging the consecutive tuples in a new vector
    ////////////////////////////////////////////////////////////////////////////

    auto startb = std::chrono::high_resolution_clock::now();

    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
        compressedIndices;
    int indicesSize = indices.size();
    int compressedIndicesSize = indicesSize;

    std::tuple< int32_t, int32_t, int32_t, int32_t > indicesInCubes;
    std::tuple< int32_t, int32_t, int32_t, int32_t > indicesInCubePrev;

    int32_t bundleIndex, fiberIndex, pointIndexMin, pointIndexMax;
    int32_t bundleIndexPrev, fiberIndexPrev, pointIndexMaxPrev;


    // allocating memory for the new compressed vector
    for ( int i = 1; i < indicesSize; ++ i )
    {

      indicesInCubes = indices[ i ];
      bundleIndex = std::get< 0 > ( indicesInCubes );
      fiberIndex = std::get< 1 > ( indicesInCubes );
      pointIndexMin = std::get< 2 > ( indicesInCubes );
      pointIndexMax = std::get< 3 > ( indicesInCubes );

      indicesInCubePrev = indices[ i - 1 ];
      bundleIndexPrev = std::get< 0 > ( indicesInCubePrev );
      fiberIndexPrev = std::get< 1 > ( indicesInCubePrev );
      pointIndexMaxPrev = std::get< 3 > ( indicesInCubePrev );

      if ( bundleIndexPrev == bundleIndex
           && fiberIndexPrev == fiberIndex
           && pointIndexMaxPrev == pointIndexMin )
      {

        compressedIndicesSize -= 1;

      }

    }

    compressedIndices.resize( compressedIndicesSize );
    if ( indicesSize > 0 ) { compressedIndices[ 0 ] = indices[ 0 ]; }
    int tupleIndex = 0;


    // filling the compressed vector with less tuples
    for ( int i = 1; i < indicesSize; ++ i )
    {

      indicesInCubes = indices[ i ];
      bundleIndex = std::get< 0 > ( indicesInCubes );
      fiberIndex = std::get< 1 > ( indicesInCubes );
      pointIndexMin = std::get< 2 > ( indicesInCubes );
      pointIndexMax = std::get< 3 > ( indicesInCubes );

      indicesInCubePrev = indices[ i - 1 ];
      bundleIndexPrev = std::get< 0 > ( indicesInCubePrev );
      fiberIndexPrev = std::get< 1 > ( indicesInCubePrev );
      pointIndexMaxPrev = std::get< 3 > ( indicesInCubePrev );


      if ( bundleIndexPrev == bundleIndex
           && fiberIndexPrev == fiberIndex
           && pointIndexMaxPrev == pointIndexMin )
      {

        std::get< 3 > ( compressedIndices[ tupleIndex ] ) = pointIndexMax;

      }
      else
      {

        compressedIndices[ ++ tupleIndex ] = indicesInCubes;

      }

    }

    auto stopb = std::chrono::high_resolution_clock::now();
    auto durationb = std::chrono::duration_cast< std::chrono::milliseconds >( 
                                                               stopb - startb );
    // std::cout << "compressedIndicesSize: "
    //           << compressedIndicesSize << "\n";
    std::cout << "Time taken to merge indices: " << durationb.count()
              << " milliseconds" << "\n";

    return compressedIndices;

  }
  GKG_CATCH( "std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > > "
             "gkg::AInteractiveBundleMapObject::getCompressedIndices( "
             "std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > > "
             "indices ) const" );

}


void gkg::AInteractiveBundleMapObject::setIndicesZoomZero(
                                                         int32_t levelOfDetail )
{

  try
  {

    _indicesZoomZero
      = getCompressedIndices(
          getIndicesFromSelectedCubes( levelOfDetail ) );

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::setIndicesZoomZero( "
             "int32_t levelOfDetail ) " );

}


void gkg::AInteractiveBundleMapObject::computeScalarFeatureVolumeBasedColors()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading the scalar feature volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > scalarFeatureVolume;
    gkg::TypedVolumeReaderProcess< float >
      typedVolumeReaderProcess( scalarFeatureVolume );
    typedVolumeReaderProcess.execute( _fileNameScalarFeatureVolume );

    gkg::Vector3d< double > resolution;
    scalarFeatureVolume.getResolution( resolution );

    ////////////////////////////////////////////////////////////////////////////
    // reading the 3D transformation
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > > transform3d;
    if ( !_fileNameScalarFeatureVolumeToBundleTransform3d.empty() )
    {

      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream 
          is( _fileNameScalarFeatureVolumeToBundleTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3d.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        gkg::AffineTransform3d< float >*
          affineTransform3d = new gkg::AffineTransform3d< float >;
        std::ifstream 
          is( _fileNameScalarFeatureVolumeToBundleTransform3d.c_str() );
        affineTransform3d->readTrm( is );
        is.close();
        transform3d.reset( affineTransform3d );

      }

    }
    else
    {

      transform3d.reset( new gkg::IdentityTransform3d< float > );

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating the color vector
    ////////////////////////////////////////////////////////////////////////////

    _scalarFeatureVolumeBasedColors.reset( new std::vector< float >(
                                           _bundleMap->getTotalPointCount() ) );


    ////////////////////////////////////////////////////////////////////////////
    // computing the color barycenters (between blue and red colors)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > pInScalarFeatureVolumeFrame;
    float value = 0.0f;
    gkg::BundleMap< std::string >::const_iterator
      b = _bundleMap->begin(),
      be = _bundleMap->end();
    std::vector< float >::iterator
      c = _scalarFeatureVolumeBasedColors->begin();
    while ( b != be )
    {

      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;

      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;
        gkg::LightCurve3d< float >::const_iterator
          p = fiber.begin(),
          pe = fiber.end();
        while ( p != pe )
        {

          transform3d->getInverse( *p, pInScalarFeatureVolumeFrame );
          gkg::PartialVolumingQuickResampler< float >::getInstance().resample(
            scalarFeatureVolume,
            0.0f,
            pInScalarFeatureVolumeFrame,
            value,
            &resolution );

          if ( value < _lowerBound )
          {

            *c = 0.0f;

          }
          else if ( value > _upperBound )
          {

            *c = 1.0f;

          }
          else
          {

            *c = ( value - _lowerBound ) / ( _upperBound - _lowerBound );

          }

          ++ p;
          ++ c;

        }

        ++ f;

      }

      ++ b;

    }

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::"
             "computeScalarFeatureVolumeBasedColors()" );


}


////////////////////////////////////////////////////////////////////////////////
// on the fly resampling methods
////////////////////////////////////////////////////////////////////////////////

void gkg::AInteractiveBundleMapObject::fillPointsToKeepVector()
{

  try
  {

    auto start = std::chrono::high_resolution_clock::now();

    // resizing our vector to bundles' size
    int32_t bundleIndex = 0;
    gkg::BundleMap< std::string >::const_iterator
      b = _bundleMap->begin(),
      be = _bundleMap->end();
    
    int32_t fiberCount, bundleCount;
    bundleCount = _bundleMap->getBundleCount();
    
    _pointsToKeepVector.resize( bundleCount );


    for ( int bundleIndex = 0; bundleIndex < bundleCount; ++ bundleIndex )
    {

      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;
      fiberCount = bundle.getCurve3dCount();

      _pointsToKeepVector[ bundleIndex ].resize( fiberCount );

      ++ b;

    }
    
    // looping over input bundles
    bundleIndex = 0;
    b = _bundleMap->begin(),
    be = _bundleMap->end();


    while ( b != be )
    {

      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;
      
      gkg::AInteractiveBundleMapResamplerContext
        aInteractiveBundleMapResamplerContext(
                                  bundle,
                                  bundleIndex,
                                  _resamplingMaximumStep,
                                  _resamplingErrorThreshold,
                                  _pointsToKeepVector );


      gkg::ThreadedLoop< int32_t >
        threadedLoop( &aInteractiveBundleMapResamplerContext,
                      0,                                      // start index
                      bundle.getCurve3dCount(),               // count
                      0 );                                    // parallel
      threadedLoop.launch();

      ++ b;
      ++ bundleIndex;

    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                                 stop - start );
    std::cout << "\n" << "Time taken to fill points to keep: " 
              << duration.count() << " milliseconds" << "\n";
  
  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::fillPointsToKeepVector()" );

}


////////////////////////////////////////////////////////////////////////////////
// multi-threading methods
////////////////////////////////////////////////////////////////////////////////

//
// class AInteractiveBundleMapPreallocateContext
//

gkg::AInteractiveBundleMapPreallocateContext::
                                        AInteractiveBundleMapPreallocateContext(
        const gkg::BundleMap< std::string >::Bundle& bundle,
        const int32_t& bundleIndex,
        const int32_t& numberOfCubesPerAxis,
        const BoundingBox< float >& boundingBox,
        const std::vector< std::vector< int32_t > >& fiberToLevelOfDetail,
        std::vector< std::vector< int32_t > >& memoryNeededInVector )
                   : gkg::LoopContext< int32_t >(),
                     _bundle( bundle ),
                     _bundleIndex( bundleIndex ),
                     _numberOfCubesPerAxis( numberOfCubesPerAxis ),
                     _boundingBox( boundingBox ),
                     _fiberToLevelOfDetail( fiberToLevelOfDetail ),
                     _memoryNeededInVector( memoryNeededInVector )
{
}


gkg::AInteractiveBundleMapPreallocateContext::
                                      ~AInteractiveBundleMapPreallocateContext()
{
}


void gkg::AInteractiveBundleMapPreallocateContext::doIt( int32_t startIndex,
                                                         int32_t count )
{

  try
  {

    int32_t maxLevelOfDetail = _memoryNeededInVector.size() - 1;
    int32_t totalNumberOfCubes = _memoryNeededInVector[ 0 ].size();
    
    std::vector< std::vector< int32_t > > localMemoryNeeded(
                             maxLevelOfDetail + 1, 
                             std::vector< int32_t > ( totalNumberOfCubes, 0 ) );
    
    std::vector< gkg::LightCurve3d< float > >::const_iterator f, fe;
    gkg::LightCurve3d< float >::const_iterator p, pe;
    int32_t pointCount;
    int32_t fiberIndex, pointIndex;
    bool fiberFirstPoint, fiberLastPoint;

    int32_t levelOfDetail;
    int32_t indiceOfCube, indiceOfNextCube;
    std::tuple< int32_t, int32_t, int32_t, int32_t > indicesInCubes;
    
    f = _bundle.begin();
    std::advance( f, startIndex );
    fe = f;
    std::advance( fe, count );

    fiberIndex = startIndex;
    indiceOfCube = 0;


    while ( f != fe )
    {
      
      const gkg::LightCurve3d< float >& fiber = *f;
      p = fiber.begin(),
      pe = fiber.end();
      pointIndex = 0;
      pointCount = fiber.getPointCount();
      fiberFirstPoint = true;
      fiberLastPoint = false;
      levelOfDetail = _fiberToLevelOfDetail[ _bundleIndex ][ fiberIndex ];
      // getting hold of the first fiber point cube indice
      gkg::AInteractiveBundleMapObject::getStaticCubeCoordinates(
                                                          *p,
                                                          indiceOfNextCube,
                                                          _numberOfCubesPerAxis,
                                                          _boundingBox );
      // quadruplets stands for fiber segment coordinates passing through a cube
      // indicesInCube( bundleIndex, fiberIndex, pSegmentMin, pSegmentMax )
      std::get< 0 > ( indicesInCubes ) = _bundleIndex;
      std::get< 1 > ( indicesInCubes ) = fiberIndex;
      std::get< 2 > ( indicesInCubes ) = 0;
      std::get< 3 > ( indicesInCubes ) = 0;

      if ( levelOfDetail != 0 )
      {


        while ( pointIndex < pointCount )
        {

          if ( pointIndex + 1 >= pointCount )
          { 

            fiberLastPoint = true; 

          }
          
          if ( !fiberLastPoint ) { 

            // next cube now becomes current cube
            indiceOfCube = indiceOfNextCube;
            // getting hold of the upcoming cube for the next fiber point
            gkg::AInteractiveBundleMapObject::getStaticCubeCoordinates(
                                                          *( p + 1 ),
                                                          indiceOfNextCube,
                                                          _numberOfCubesPerAxis,
                                                          _boundingBox );

            // if cube has changed, adding a quadruplet in our vector
            if ( indiceOfCube != indiceOfNextCube )
            {
              
              // adding one to the quadruplets count i.e. the segment count
              localMemoryNeeded[ levelOfDetail ][ indiceOfCube ] += 1;

            }

            ++ pointIndex;
            ++ p;

          }
          else
          {
            
            // adding one to the quadruplets count for the last segment
            localMemoryNeeded[ levelOfDetail ][ indiceOfCube ] += 1;
            ++ pointIndex;

          }

          if ( fiberFirstPoint ) { fiberFirstPoint = false; }
          if ( fiberLastPoint ) { fiberLastPoint = false; }
          
        }

      }

      ++ f;
      ++ fiberIndex;
    
    }

    lock();


    for ( int levelOfDetail = 1;
          levelOfDetail <= maxLevelOfDetail;
          ++ levelOfDetail )
    {
      

      for ( int indiceOfCube = 0;
            indiceOfCube < totalNumberOfCubes;
            ++ indiceOfCube )
      {
        
        int32_t m = localMemoryNeeded[ levelOfDetail ][ indiceOfCube ];

        if ( m > 0 )
        {
          
          _memoryNeededInVector[ levelOfDetail ][ indiceOfCube ] += m;

        }

      }

    }

    unlock();

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapPreallocateContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
// class AInteractiveBundleMapFillContext
//

gkg::AInteractiveBundleMapFillContext::AInteractiveBundleMapFillContext(
       const gkg::BundleMap< std::string >::Bundle& bundle,
       const int32_t& bundleIndex,
       const int32_t& numberOfCubesPerAxis,
       const BoundingBox< float >& boundingBox,
       const std::vector< std::vector< int32_t > >& fiberToLevelOfDetail,
       std::vector< std::vector< std::vector< std::tuple<
         int32_t, int32_t, int32_t, int32_t > > > >& indicesInCubesVector )
                    : gkg::LoopContext< int32_t >(),
                      _bundle( bundle ),
                      _bundleIndex( bundleIndex ),
                      _numberOfCubesPerAxis( numberOfCubesPerAxis ),
                      _boundingBox( boundingBox ),
                      _fiberToLevelOfDetail( fiberToLevelOfDetail ),
                      _indicesInCubesVector( indicesInCubesVector )
{
}


gkg::AInteractiveBundleMapFillContext::~AInteractiveBundleMapFillContext()
{
}


void gkg::AInteractiveBundleMapFillContext::doIt( int32_t startIndex,
                                                  int32_t count )
{

  try
  {

    int32_t maxLevelOfDetail = _indicesInCubesVector.size() - 1;
    int32_t totalNumberOfCubes = _indicesInCubesVector[ 0 ].size();

    std::vector< std::vector< std::vector<
      std::tuple< int32_t, int32_t, int32_t, int32_t > > > > localIndices;

    localIndices.resize( maxLevelOfDetail + 1 );
    localIndices[ 0 ].resize( totalNumberOfCubes );
    

    for ( int levelOfDetail = 1;
          levelOfDetail <= maxLevelOfDetail;
          ++ levelOfDetail )
    {
      
      localIndices[ levelOfDetail ].resize( totalNumberOfCubes );
    
    }

      /* for ( int indiceOfCube = 0;
            indiceOfCube < totalNumberOfCubes;
            ++ indiceOfCube )
      {
        
        localIndices[ levelOfDetail ][ indiceOfCube ].reserve( 
                        memoryNeededInVector[ levelOfDetail ][ indiceOfCube ] );

      } */
    
    std::vector< gkg::LightCurve3d< float > >::const_iterator f, fe;
    gkg::LightCurve3d< float >::const_iterator p, pe;
    int32_t pointCount;
    int32_t fiberIndex, pointIndex;
    bool fiberFirstPoint, fiberLastPoint;

    int32_t levelOfDetail;
    int32_t indiceOfCube, indiceOfNextCube;
    std::tuple< int32_t, int32_t, int32_t, int32_t > indicesInCubes;
    
    f = _bundle.begin();
    std::advance( f, startIndex );
    fe = f;
    std::advance( fe, count );

    fiberIndex = startIndex;
    indiceOfCube = 0;


    while ( f != fe )
    {
      
      const gkg::LightCurve3d< float >& fiber = *f;
      p = fiber.begin(),
      pe = fiber.end();
      pointIndex = 0;
      pointCount = fiber.getPointCount();
      fiberFirstPoint = true;
      fiberLastPoint = false;
      levelOfDetail = _fiberToLevelOfDetail[ _bundleIndex ][ fiberIndex ];
      // getting hold of the first fiber point cube indice
      gkg::AInteractiveBundleMapObject::getStaticCubeCoordinates(
                                                          *p,
                                                          indiceOfNextCube,
                                                          _numberOfCubesPerAxis,
                                                          _boundingBox );
      // quadruplets stands for fiber segment coordinates passing through a cube
      // indicesInCube( bundleIndex, fiberIndex, pSegmentMin, pSegmentMax )
      std::get< 0 > ( indicesInCubes ) = _bundleIndex;
      std::get< 1 > ( indicesInCubes ) = fiberIndex;
      std::get< 2 > ( indicesInCubes ) = 0;
      std::get< 3 > ( indicesInCubes ) = 0;

      if ( levelOfDetail != 0 )
      {
      

        while ( pointIndex < pointCount )
        {

          if ( pointIndex + 1 >= pointCount )
          { 

            fiberLastPoint = true; 

          }
          
          if ( !fiberLastPoint ) { 
            
            // next cube now becomes current cube
            indiceOfCube = indiceOfNextCube;
            // getting hold of the upcoming cube for the next fiber point
            gkg::AInteractiveBundleMapObject::getStaticCubeCoordinates(
                                                          *( p + 1 ),
                                                          indiceOfNextCube,
                                                          _numberOfCubesPerAxis,
                                                          _boundingBox );

            // if cube has changed, adding a quadruplet in our vector
            if ( indiceOfCube != indiceOfNextCube )
            {
              
              // setting the current segment ending point
              std::get< 3 > ( indicesInCubes ) = pointIndex + 1;
              localIndices[ levelOfDetail ][ indiceOfCube ]
                                                   .push_back( indicesInCubes );
              // setting the next segment beginning point
              std::get< 2 > ( indicesInCubes )
                = std::get< 3 > ( indicesInCubes );

            }

            ++ pointIndex;
            ++ p;

          }
          else
          {
            
            // setting the last segment ending point
            std::get< 3 > ( indicesInCubes ) = pointIndex;
            localIndices[ levelOfDetail ][ indiceOfCube ]
                                                   .push_back( indicesInCubes );
            ++ pointIndex;

          }

          if ( fiberFirstPoint ) { fiberFirstPoint = false; }
          if ( fiberLastPoint ) { fiberLastPoint = false; }
          
        }

      }

      ++ f;
      ++ fiberIndex;

    }

    lock();


    for ( int levelOfDetail = 1;
          levelOfDetail <= maxLevelOfDetail;
          ++ levelOfDetail )
    {
      

      for ( int indiceOfCube = 0;
            indiceOfCube < totalNumberOfCubes;
            ++ indiceOfCube )
      {
        
        if ( localIndices[ levelOfDetail ][ indiceOfCube ].size() > 0 )
        {
          
          _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].insert(
                   _indicesInCubesVector[ levelOfDetail ][ indiceOfCube ].end(),
                   localIndices[ levelOfDetail ][ indiceOfCube ].begin(),
                   localIndices[ levelOfDetail ][ indiceOfCube ].end() );

        }

      }

    }

    unlock();

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapFillContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
// class AInteractiveBundleMapResamplerContext
//

gkg::AInteractiveBundleMapResamplerContext::
                                          AInteractiveBundleMapResamplerContext(
        const gkg::BundleMap< std::string >::Bundle& bundle,
        const int32_t& bundleIndex,
        const double& resamplingMaximumStep,
        const double& resamplingErrorThreshold,
        std::vector< std::vector< std::vector< int32_t > > >&
          pointsToKeepVector )
                    : gkg::LoopContext< int32_t >(),
                      _bundle( bundle ),
                      _bundleIndex( bundleIndex ),
                      _resamplingMaximumStep( resamplingMaximumStep ),
                      _resamplingErrorThreshold( resamplingErrorThreshold ),
                      _pointsToKeepVector( pointsToKeepVector )
{
}


gkg::AInteractiveBundleMapResamplerContext::
                                        ~AInteractiveBundleMapResamplerContext()
{
}


void gkg::AInteractiveBundleMapResamplerContext::doIt( int32_t startIndex,
                                                       int32_t count )
{

  try
  {

    // looping over input bundles
    int32_t fiberIndex = startIndex;

    std::vector< std::vector< int32_t > > pointsToKeepTemp;
    pointsToKeepTemp.resize( count );

    // collecting the selected fibers
    gkg::Vector3d< float > tp;
    std::vector< gkg::LightCurve3d< float > >::const_iterator
      f = _bundle.begin(),
      fe = _bundle.begin();
    std::advance( f, fiberIndex );
    std::advance( fe, fiberIndex + count );
    while ( f != fe )
    {

      const std::vector< int32_t >& pointsToKeep
          = f->getSmartCurveIndices( _resamplingMaximumStep,
                                     _resamplingErrorThreshold );
      
      int size = (int)pointsToKeep.size();
      _pointsToKeepVector[ _bundleIndex ][ fiberIndex ].reserve( size );
      _pointsToKeepVector[ _bundleIndex ][ fiberIndex ] = pointsToKeep;

      // pointsToKeepTemp[ fiberIndex - startIndex ].reserve( size );
      // pointsToKeepTemp[ fiberIndex - startIndex ] = pointsToKeep;
      
      ++ fiberIndex;
      ++ f;

    }

    // lock();
    

    // for ( int32_t fiberIndex = startIndex;
    //       fiberIndex < startIndex + count;
    //       ++ fiberIndex )
    // {

    //   int size = (int)pointsToKeepTemp[ fiberIndex - startIndex ].size();
    //   _pointsToKeepVector[ _bundleIndex ][ fiberIndex ].reserve( size );
    //   _pointsToKeepVector[ _bundleIndex ][ fiberIndex ] =
    //                                pointsToKeepTemp[ fiberIndex - startIndex ];

    // }

    // unlock();

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapResamplerContext< L, Compare >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#undef DEFAULT_MINIMUM_FIBER_DISPLAY_PERCENTAGE
#undef DEFAULT_MAXIMUM_FIBER_DISPLAY_PERCENTAGE
#undef DEFAULT_MAXIMUM_LEVEL_OF_DETAIL_COUNT
#undef DEFAULT_LEVEL_OF_DETAIL
#undef DEFAULT_FIELD_OF_VIEW_PARTITION_COUNT


