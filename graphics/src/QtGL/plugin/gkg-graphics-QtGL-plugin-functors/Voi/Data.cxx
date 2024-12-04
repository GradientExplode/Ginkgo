#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelAxialStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ReadProcess.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-io/GisDiskFormat.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>

#include <cmath>
#include <map>


gkg::VoiData::VoiData() 
             : gkg::VoiObservable(), 
               dataOk( false ),
               labelOk( false ),
               _isContour( false ),
               minimum( 0.0f ),
               maximum( 0.0f ),
               fusFactLabel( 25 ), 
               fusFactBaseVolume( 25 ), 
               _morphoRadius( 2.0 ),
               _morphoOperation( 4 ),
               _cylinderDiameter( 70 ),
               _cylinderOrientation( 1 )
{

  strgy = new gkg::VoiLabelAxialStrategy();
  _brushOffsets.push_back( gkg::Vector2d< int32_t >( 0, 0 ) );

}


gkg::VoiData::~VoiData()
{
}


gkg::VoiVolumeBaseFusion& gkg::VoiData::getBaseFusion()
{ 

  return _baseFusion;
  
}


const gkg::VoiVolumeBaseFusion& gkg::VoiData::getBaseFusion() const
{ 

  return _baseFusion;
  
}


void gkg::VoiData::initialize( const std::string& t1Name,
                               const std::string& volBName,
                               const std::string& transfoName,
                               const std::string& labelName )
{

  if ( _baseFusion.load( t1Name, volBName, transfoName ) )
  {

    gkg::Volume<float>& t1 = _baseFusion.getT1();

    minimum = _baseFusion.getMinT1();
    maximum = _baseFusion.getMaxT1();
    pctPlage = ( maximum - minimum ) / 200.0;

    int32_t dx = t1.getSizeX();
    int32_t dy = t1.getSizeY();
    int32_t dz = t1.getSizeZ();
    gkg::Vector3d< double > res( 1.0, 1.0, 1.0 );
    gkg::Vector3d< double > lres( 1.0, 1.0, 1.0 );
  
    t1.getResolution( res );
    _labels.getResolution( lres );
 
    bool sameDim = ( _labels.getSizeX() == dx ) && 
                   ( _labels.getSizeY() == dy ) &&
                   ( _labels.getSizeZ() == dz );
    bool sameSize = ( lres.x == res.x ) && 
                    ( lres.y == res.y ) && 
                    ( lres.z == res.z );

    if ( !sameDim || !sameSize )
    {
  
      _labels.reallocate( dx, dy, dz );
      _labels.setResolution( res );
      _labels.fill( 0 );
      _slicer.initialize( *this );
      labelOk = true;

    }

    dataOk = true;
  
    notifyDataLoaded();

  }

  if ( !labelName.empty() )
  {

    gkg::ReadProcess process( gkg::TypeOf< int16_t >::getName(), &_labels );
  
    process.execute( labelName );
    labInfo.readHeader( _labels );
    
    if ( !labelOk )
    {

      _slicer.initialize( *this );
      labelOk = true;

    }
    notifyData();
    notifyLabelLoaded();

  }

  if ( dataOk || labelOk )
  {

    notifyScrollBar();

  }

}


void gkg::VoiData::saveLabel( const std::string& fname, 
                              const QString& filt )
{

  // sanity checks to check if we can write
  gkg::File file( fname );

  gkg::File baseDirectory( file.getDirectoryName() +
                           gkg::getDirectorySeparator() + "." );

  if ( !baseDirectory.isWritable() )
  {

    std::cout << "cannot write in '"
              << file.getDirectoryName()
              << "' directory"
              << std::endl;
    return;

  }

  labInfo.writeHeader( _labels );

  if ( filt.contains( "8 bits" ) )
  {

    // check that file with same name does not exist and is unwritable
    std::string baseName = 
      gkg::GisDiskFormat< uint8_t >::getInstance().getNameWithoutExtension(
                                                                        fname );

    gkg::File fileIma( baseName + ".ima" );
    gkg::File fileDim( baseName + ".dim" );
    gkg::File fileMinf( baseName + "." +
            gkg::GisDiskFormat< uint8_t >::getInstance().getHeaderExtension() );

    if ( ( fileIma.isExisting() && !fileIma.isWritable() ) ||
         ( fileDim.isExisting() && !fileDim.isWritable() ) ||
         ( fileMinf.isExisting() && !fileMinf.isWritable() ) )
    {

      std::cout << "cannot write file due to bad file permissions"
                << std::endl;
      return;

    }

    // writing file
    gkg::Volume< uint8_t > vol_out;
    gkg::Converter< gkg::Volume< int16_t >, gkg::Volume< uint8_t > > converter;

    converter.convert( _labels, vol_out );
    gkg::Writer::getInstance().write( fname, vol_out, false, "gis" );

  }
  else
  {

    // check that file with same name does not exist and is unwritable
    std::string baseName = 
      gkg::GisDiskFormat< int16_t >::getInstance().getNameWithoutExtension(
                                                                        fname );

    gkg::File fileIma( baseName + ".ima" );
    gkg::File fileDim( baseName + ".dim" );
    gkg::File fileMinf( baseName + "." +
            gkg::GisDiskFormat< int16_t >::getInstance().getHeaderExtension() );

    if ( ( fileIma.isExisting() && !fileIma.isWritable() ) ||
         ( fileDim.isExisting() && !fileDim.isWritable() ) ||
         ( fileMinf.isExisting() && !fileMinf.isWritable() ) )
    {

      std::cout << "cannot write file due to bad file permissions"
                << std::endl;
      return;

    }

    // writing file
    gkg::Writer::getInstance().write( fname, _labels, false, "gis" );

  }
  
}


gkg::VoiLutInfo& gkg::VoiData::lutInfo( bool shift )
{ 

  return _baseFusion.getLut( shift );
  
}


gkg::VoiLabelInfo& gkg::VoiData::labelInfo()
{ 

  return labInfo; 
  
}


const gkg::VoiLabelInfo& gkg::VoiData::labelInfo() const 
{ 

  return labInfo; 
  
}


gkg::VoiEvolutionInfo& gkg::VoiData::evolInfo() 
{ 

  return evInfo; 
  
}


const gkg::VoiEvolutionInfo& gkg::VoiData::evolInfo() const 
{ 

  return evInfo; 
  
}


bool gkg::VoiData::dataLoaded() const 
{ 

  return dataOk; 
  
}


bool gkg::VoiData::labelLoaded() const 
{ 

  return labelOk; 
  
}

float gkg::VoiData::min() 
{ 

  return minimum; 
  
}


float gkg::VoiData::max() 
{ 

  return maximum; 
  
}


float gkg::VoiData::plage() 
{ 

  return pctPlage;
  
}


int32_t gkg::VoiData::fusionLabel() 
{ 

  return fusFactLabel; 
  
}


void gkg::VoiData::setFusionLabel( int32_t fusLabel )
{ 

  fusFactLabel = fusLabel;
  notifyData();

}


int32_t gkg::VoiData::fusionBaseVolume() 
{ 

  return fusFactBaseVolume; 
  
}


void gkg::VoiData::setFusionBaseVolume( int32_t fusBaseVolume )
{ 

  fusFactBaseVolume = fusBaseVolume;
  notifyData();

}


bool gkg::VoiData::empty() const 
{ 

  return !dataOk && !labelOk; 
  
}


void gkg::VoiData::setBrushSize( int32_t bs ) 
{

  int32_t i, j;
  int32_t b0 = bs / 2;
  int32_t b1 = b0 + ( bs % 2 );

  _brushOffsets.clear();

  for ( j = -b0; j < b1; j++ )
  {

    for ( i = -b0; i < b1; i++ )
    {

      _brushOffsets.push_back( gkg::Vector2d< int32_t >( i, j ) );

    }

  }
  
}


void gkg::VoiData::setCircleBrushSize( int32_t bs ) 
{

  int32_t i, j, b0 = bs / 2;

  _brushOffsets.clear();

  for ( j = -b0; j <= b0; j++ )
  {

    for ( i = -b0; i <= b0; i++ )
    {

      int32_t r = int32_t( std::sqrt( i * i + j * j ) + 0.5 );

      if ( r <= b0 )
      {

        _brushOffsets.push_back( gkg::Vector2d< int32_t >( i, j ) );

      }

    }

  }
  
}


void gkg::VoiData::modifyLabelInfo( int32_t index, const std::string& name, 
                                    int32_t r, int32_t g, int32_t b )
{

  labInfo.update( index, name, r, g, b );

  if ( dataOk || labelOk )
  {

    notifyPalette();

  }

}


void gkg::VoiData::setCurrentLabel( int32_t i ) 
{

  labInfo.setCurrent( i );  
  
}


float gkg::VoiData::grayLevel( gkg::Vector3d< int32_t >& pt, int32_t t )
{

  return strgy->grayLevel( _baseFusion.getT1(), pt, t );

}


void gkg::VoiData::setLabelPoint( gkg::Vector3d< int32_t >& pt, 
                                  int32_t t, int32_t lab )
{

  strgy->doit( this, _labels, pt, t, lab, _brushOffsets );

}


void gkg::VoiData::fillLabel( gkg::Vector3d< int32_t >& pt, 
                              int32_t /* t */, int32_t lab )
{

  int32_t nbPt = 0;
  int32_t fond = (int32_t)_labels( pt );
  strgy->fill( this, _labels, pt.x, pt.y, pt.z, fond, lab, &nbPt );

}


void gkg::VoiData::evolutionLabel( gkg::Vector3d< int32_t >& pt, 
                                   int32_t t, int32_t label )
{

  strgy->evolution( this, pt, t, label, evInfo );

}


void gkg::VoiData::splineToLabel( std::list< QPoint >& spline,
                                  int32_t slice, int32_t label )
{

  strgy->spline( this, _labels, spline, slice, label );

}


void gkg::VoiData::morphologyLabel( gkg::Volume< int16_t >& roi,
                                    int32_t label )
{

  strgy->morphology( this, roi, label );

}


void gkg::VoiData::undoEvolution()
{

  strgy->undo( this );

}


void gkg::VoiData::clearSelectedLabel()
{

  strgy->clearSelection( this, labInfo.currentLabel() );

}


void gkg::VoiData::clearAllLabels()
{

  strgy->clearAll( this );

}


void gkg::VoiData::setLabelStrategy( gkg::VoiLabelStrategy *stgy )
{

  if ( stgy->id() != strgy->id() )
    strgy = stgy;

}


gkg::Volume< int16_t >& gkg::VoiData::label()
{

  return _labels;

}

void gkg::VoiData::setMorphologyRadius( double radius )
{

  _morphoRadius = radius;

}


double gkg::VoiData::getMorphologyRadius()
{

  return _morphoRadius;

}


void gkg::VoiData::setMorphologyOperation( int32_t operation )
{

  _morphoOperation = operation;

}


int32_t gkg::VoiData::getMorphologyOperation()
{

  return _morphoOperation;

}

void gkg::VoiData::setCylinderDiameter( int32_t diameter )
{

  _cylinderDiameter = diameter;
  notifyRedraw();

}


int32_t gkg::VoiData::getCylinderDiameter()
{

  return _cylinderDiameter;

}


void gkg::VoiData::setCylinderOrientation( int32_t dir )
{

  _cylinderOrientation = dir;
  notifyRedraw();

}


int32_t gkg::VoiData::getCylinderOrientation()
{

  return _cylinderOrientation;

}


void gkg::VoiData::toggleContour()
{

  _isContour = !_isContour;
  notifyData();

}


bool gkg::VoiData::isContour()
{

  return _isContour;

}


gkg::Slicer& gkg::VoiData::getSlicer()
{

  return _slicer;

}
