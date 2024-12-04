#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfo.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>


gkg::LabelInformation::LabelInformation( const std::string& theName,
                                         gkg::RGBComponent theRgb,
                                         bool theShow )
                     : name( theName ),
                       rgb( theRgb ),
                       show( theShow )
{
}


gkg::LabelInformation::LabelInformation( const LabelInformation& other )
                     : name( other.name ),
                       rgb( other.rgb ),
                       show( other.show )
{
}


gkg::VoiLabelInfo::VoiLabelInfo() : QObject()
{

  bOrder = 0x00081018; // 00, 08, 16, et 24
  current = -1;

  if ( !read( "default.labels" ) ) 
  {

    init();

  }

}


gkg::VoiLabelInfo::~VoiLabelInfo()
{
}


std::map< int32_t, gkg::LabelInformation >& gkg::VoiLabelInfo::info()
{ 

  return infos;
  
}


const std::map< int32_t, gkg::LabelInformation >& 
gkg::VoiLabelInfo::info() const 
{ 

  return infos;
  
}


uint8_t* gkg::VoiLabelInfo::byteOrder()
{ 

  return (uint8_t *)&bOrder;
  
}


int32_t gkg::VoiLabelInfo::currentLabel()
{ 

  return current;
  
}


void gkg::VoiLabelInfo::init()
{

  infos.clear();

  infos.insert( std::make_pair( 0, 
                gkg::LabelInformation( "Background\0", 
                                       gkg::RGBComponent(   0,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 1, 
                gkg::LabelInformation( "Skin-Fat\0", 
                                       gkg::RGBComponent( 255, 255, 255 ) ) ) );
  infos.insert( std::make_pair( 4,
                gkg::LabelInformation( "Bone\0", 
                                       gkg::RGBComponent( 254,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 8,
                gkg::LabelInformation( "Csf\0", 
                                       gkg::RGBComponent( 255,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 22,
                gkg::LabelInformation( "Gray Matter\0", 
                                       gkg::RGBComponent( 127, 255, 127 ) ) ) );
  infos.insert( std::make_pair( 23,
                gkg::LabelInformation( "White Matter\0", 
                                       gkg::RGBComponent( 127,  63, 127 ) ) ) );
  infos.insert( std::make_pair( 24,
                gkg::LabelInformation( "Blood\0", 
                                       gkg::RGBComponent( 127, 127, 255 ) ) ) );
  infos.insert( std::make_pair( 77,
                gkg::LabelInformation( "Cerebellum\0", 
                                       gkg::RGBComponent( 194, 153,  95 ) ) ) );

  infos.insert( std::make_pair( 11,
                gkg::LabelInformation( "Lesion 1\0", 
                                       gkg::RGBComponent( 255, 128,   0 ) ) ) );
  infos.insert( std::make_pair( 12,
                gkg::LabelInformation( "Lesion 2\0", 
                                       gkg::RGBComponent(   0, 255, 128 ) ) ) );
  infos.insert( std::make_pair( 13,
                gkg::LabelInformation( "Lesion 3\0", 
                                       gkg::RGBComponent( 128,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 14,
                gkg::LabelInformation( "Lesion 4\0", 
                                       gkg::RGBComponent( 255,   0, 128 ) ) ) );
  infos.insert( std::make_pair( 15,
                gkg::LabelInformation( "Lesion 5\0", 
                                       gkg::RGBComponent( 128, 200, 128 ) ) ) );
  infos.insert( std::make_pair( 16,
                gkg::LabelInformation( "Lesion 6\0", 
                                       gkg::RGBComponent( 127, 199, 127 ) ) ) );
  infos.insert( std::make_pair( 17,
                gkg::LabelInformation( "Lesion 7\0", 
                                       gkg::RGBComponent(   0,  48, 128 ) ) ) );
  infos.insert( std::make_pair( 18,
                gkg::LabelInformation( "Lesion 8\0", 
                                       gkg::RGBComponent( 255, 128, 128 ) ) ) );
  infos.insert( std::make_pair( 19,
                gkg::LabelInformation( "Lesion 9\0", 
                                       gkg::RGBComponent( 192, 192, 192 ) ) ) );
  infos.insert( std::make_pair( 31,
                gkg::LabelInformation( "Lesion 10\0", 
                                       gkg::RGBComponent( 120, 210, 220 ) ) ) );
  infos.insert( std::make_pair( 32,
                gkg::LabelInformation( "Lesion 11\0", 
                                       gkg::RGBComponent( 130, 230, 250 ) ) ) );
  infos.insert( std::make_pair( 33,
                gkg::LabelInformation( "Lesion 12\0", 
                                       gkg::RGBComponent( 140, 250,   5 ) ) ) );
  infos.insert( std::make_pair( 34,
                gkg::LabelInformation( "Lesion 13\0", 
                                       gkg::RGBComponent( 150,  10,  10 ) ) ) );
  infos.insert( std::make_pair( 35,
                gkg::LabelInformation( "Lesion 14\0", 
                                       gkg::RGBComponent( 160,  30,  40 ) ) ) );
  infos.insert( std::make_pair( 36,
                gkg::LabelInformation( "Lesion 15\0", 
                                       gkg::RGBComponent( 170,  50,  70 ) ) ) );
  infos.insert( std::make_pair( 37,
                gkg::LabelInformation( "Lesion 16\0", 
                                       gkg::RGBComponent( 180,  70, 100 ) ) ) );
  infos.insert( std::make_pair( 38,
                gkg::LabelInformation( "Lesion 17\0", 
                                       gkg::RGBComponent( 190,  90, 130 ) ) ) );
  infos.insert( std::make_pair( 39,
                gkg::LabelInformation( "Lesion 18\0", 
                                       gkg::RGBComponent( 200, 110, 160 ) ) ) );
  infos.insert( std::make_pair( 41,
                gkg::LabelInformation( "Lesion 19\0", 
                                       gkg::RGBComponent( 210, 130, 190 ) ) ) );
  infos.insert( std::make_pair( 42,
                gkg::LabelInformation( "Lesion 20\0", 
                                       gkg::RGBComponent( 220, 150, 210 ) ) ) );

  infos.insert( std::make_pair( 10,
                gkg::LabelInformation( "Topo A\0", 
                                       gkg::RGBComponent( 127, 127, 127 ) ) ) );
  infos.insert( std::make_pair( 20,
                gkg::LabelInformation( "Topo B\0", 
                                       gkg::RGBComponent(   0,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 30,
                gkg::LabelInformation( "Topo C\0", 
                                       gkg::RGBComponent( 255,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 40,
                gkg::LabelInformation( "Topo D\0", 
                                       gkg::RGBComponent( 217, 217, 217 ) ) ) );
  infos.insert( std::make_pair( 50,
                gkg::LabelInformation( "Topo E\0", 
                                       gkg::RGBComponent(   0, 255, 255 ) ) ) );
  infos.insert( std::make_pair( 60,
                gkg::LabelInformation( "Topo F\0", 
                                       gkg::RGBComponent(   0, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 70,
                gkg::LabelInformation( "Topo G\0", 
                                       gkg::RGBComponent( 255, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 80,
                gkg::LabelInformation( "Topo H\0", 
                                       gkg::RGBComponent(  76,  76,  76 ) ) ) );
  infos.insert( std::make_pair( 90,
                gkg::LabelInformation( "Topo I\0", 
                                       gkg::RGBComponent( 255,   0, 255 ) ) ) );

  infos.insert( std::make_pair( 115,
                gkg::LabelInformation( "3V\0", 
                                       gkg::RGBComponent( 255,  63, 127 ) ) ) );
  infos.insert( std::make_pair( 125,
                gkg::LabelInformation( "4V\0", 
                                       gkg::RGBComponent( 255,  63,   0 ) ) ) );
  infos.insert( std::make_pair( 135,
                gkg::LabelInformation( "cc\0", 
                                       gkg::RGBComponent( 255,   0, 127 ) ) ) );

  infos.insert( std::make_pair( 145,
                gkg::LabelInformation( "Left Cd\0", 
                                       gkg::RGBComponent( 255, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 155,
                gkg::LabelInformation( "Left Acb\0", 
                                       gkg::RGBComponent( 127,   0, 127 ) ) ) );
  infos.insert( std::make_pair( 165,
                gkg::LabelInformation( "Left LV\0", 
                                       gkg::RGBComponent(   0, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 175,
                gkg::LabelInformation( "Left Pu\0", 
                                       gkg::RGBComponent( 255,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 185,
                gkg::LabelInformation( "Left GP\0", 
                                       gkg::RGBComponent(   0, 127,   0 ) ) ) );
  infos.insert( std::make_pair( 195,
                gkg::LabelInformation( "Left Th\0", 
                                       gkg::RGBComponent( 127, 127,   0 ) ) ) );
  infos.insert( std::make_pair( 205,
                gkg::LabelInformation( "Right LV\0", 
                                       gkg::RGBComponent( 255,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 215,
                gkg::LabelInformation( "Right Acb\0", 
                                       gkg::RGBComponent(   0, 127, 127 ) ) ) );
  infos.insert( std::make_pair( 225,
                gkg::LabelInformation( "Right Cd\0", 
                                       gkg::RGBComponent(   0,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 235,
                gkg::LabelInformation( "Right Pu\0", 
                                       gkg::RGBComponent(   0, 255, 255 ) ) ) );
  infos.insert( std::make_pair( 245,
                gkg::LabelInformation( "Right GP\0", 
                                       gkg::RGBComponent( 127,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 250,
                gkg::LabelInformation( "Right Th\0", 
                                       gkg::RGBComponent(   0,   0, 127 ) ) ) );

  infos.insert( std::make_pair( 1000,
                gkg::LabelInformation( "Left LV\0", 
                                       gkg::RGBComponent(   0, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 1001,
                gkg::LabelInformation( "Left Cd\0", 
                                       gkg::RGBComponent( 255, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 1002,
                gkg::LabelInformation( "Left Pu\0", 
                                       gkg::RGBComponent( 255,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 1003,
                gkg::LabelInformation( "Left GP\0", 
                                       gkg::RGBComponent(   0, 127,   0 ) ) ) );
  infos.insert( std::make_pair( 1004,
                gkg::LabelInformation( "Left Th\0", 
                                       gkg::RGBComponent( 127, 127,   0 ) ) ) );
  infos.insert( std::make_pair( 1005,
                gkg::LabelInformation( "Left Amg\0", 
                                       gkg::RGBComponent( 127,   0, 127 ) ) ) );
  infos.insert( std::make_pair( 1006,
                gkg::LabelInformation( "Left Hi\0", 
                                       gkg::RGBComponent( 127, 127, 255 ) ) ) );
  infos.insert( std::make_pair( 1007,
                gkg::LabelInformation( "Left R\0", 
                                       gkg::RGBComponent( 239,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 1008,
                gkg::LabelInformation( "Left SN\0", 
                                        gkg::RGBComponent( 85,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 1009,
                gkg::LabelInformation( "Left STN\0", 
                                      gkg::RGBComponent( 255,   85,   0 ) ) ) );

  infos.insert( std::make_pair( 2000,
                gkg::LabelInformation( "Right LV\0", 
                                       gkg::RGBComponent( 255,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 2001,
                gkg::LabelInformation( "Right Cd\0", 
                                       gkg::RGBComponent(   0,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 2002,
                gkg::LabelInformation( "Right Pu\0", 
                                       gkg::RGBComponent(   0, 255, 255 ) ) ) );
  infos.insert( std::make_pair( 2003,
                gkg::LabelInformation( "Right GP\0", 
                                       gkg::RGBComponent( 127,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 2004,
                gkg::LabelInformation( "Right Th\0", 
                                       gkg::RGBComponent(   0,   0, 127 ) ) ) );
  infos.insert( std::make_pair( 2005,
                gkg::LabelInformation( "Right Amg\0", 
                                       gkg::RGBComponent(   0, 127, 127 ) ) ) );
  infos.insert( std::make_pair( 2006,
                gkg::LabelInformation( "Right Hi\0", 
                                       gkg::RGBComponent( 127, 255, 127 ) ) ) );
  infos.insert( std::make_pair( 2007,
                gkg::LabelInformation( "Right R\0", 
                                       gkg::RGBComponent( 102,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 2008,
                gkg::LabelInformation( "Right SN\0", 
                                        gkg::RGBComponent( 85,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 2009,
                gkg::LabelInformation( "Right STN\0", 
                                      gkg::RGBComponent( 255,   85,   0 ) ) ) );

  infos.insert( std::make_pair( 3000,
                gkg::LabelInformation( "3V\0", 
                                       gkg::RGBComponent( 255,  63, 127 ) ) ) );
  infos.insert( std::make_pair( 3001,
                gkg::LabelInformation( "4V\0", 
                                       gkg::RGBComponent( 255,  63,   0 ) ) ) );

  infos.insert( std::make_pair( 4000,
                gkg::LabelInformation( "BS\0", 
                                       gkg::RGBComponent( 150, 255, 127 ) ) ) );
				  
  infos.insert( std::make_pair( 2,
                gkg::LabelInformation( "Atlas 2\0", 
                                       gkg::RGBComponent(   0,   0, 255 ) ) ) );
  infos.insert( std::make_pair( 3,
                gkg::LabelInformation( "Atlas 3\0", 
                                       gkg::RGBComponent( 255,   0,   0 ) ) ) );
  infos.insert( std::make_pair( 5,
                gkg::LabelInformation( "Atlas 5\0", 
                                       gkg::RGBComponent(   0, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 6,
                gkg::LabelInformation( "Atlas 6\0", 
                                       gkg::RGBComponent(   0, 254,   0 ) ) ) );
  infos.insert( std::make_pair( 7,
                gkg::LabelInformation( "Atlas 7\0", 
                                       gkg::RGBComponent( 255, 255,   0 ) ) ) );
  infos.insert( std::make_pair( 9,
                gkg::LabelInformation( "Atlas 9\0", 
                                       gkg::RGBComponent( 254,   0, 254 ) ) ) );

}


void gkg::VoiLabelInfo::setCurrent( int i )
{

  if ( i < gkg::VoiLabelInfo::labelMax )
  {

    current = i;
    emit currentChanged( i );

  }

}


int32_t gkg::VoiLabelInfo::count()
{

  return (int32_t)infos.size();

}


int32_t gkg::VoiLabelInfo::getMax()
{

  std::map< int32_t, gkg::LabelInformation >::reverse_iterator
    last = infos.rbegin();

  if ( last != infos.rend() )
  {

    return last->first;

  }

  return -1;

}


int32_t gkg::VoiLabelInfo::getFirstAvailable()
{

  std::map< int32_t, gkg::LabelInformation >::iterator
    i = infos.begin(),
    ie = infos.end();
  int32_t l = i->first + 1;

  ++i;

  while ( ( i != ie ) && ( l == i->first ) )
  {

    l++;
    ++i;

  }

  if ( l < gkg::VoiLabelInfo::labelMax )
  {

    infos.insert( std::make_pair( l, 
                              gkg::LabelInformation( "",
                                                     gkg::RGBComponent() ) ) );

    return l;

  }

  return -1;

}


void gkg::VoiLabelInfo::removeLabel( int32_t index )
{

  std::map< int32_t, gkg::LabelInformation >::iterator
    i = infos.find( index );

  if ( i != infos.end() )
  {

    infos.erase( i );

  }

}


std::string gkg::VoiLabelInfo::getName( int32_t index )
{

  if ( index < gkg::VoiLabelInfo::labelMax )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.find( index );

    if ( i != infos.end() )
    {

      return i->second.name;

    }

  }

  return "Unknown";

}


void gkg::VoiLabelInfo::toggleShow( int32_t index )
{

  if ( index < gkg::VoiLabelInfo::labelMax )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.find( index );

    if ( i != infos.end() )
    {

      i->second.show = !i->second.show;

    }

  }

}

void gkg::VoiLabelInfo::setShow( int32_t index, bool state )
{

  if ( index < gkg::VoiLabelInfo::labelMax )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.find( index );

    if ( i != infos.end() )
    {

      i->second.show = state;

    }

  }

}


bool gkg::VoiLabelInfo::getShow( int32_t index )
{

  if ( index < gkg::VoiLabelInfo::labelMax )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.find( index );

    if ( i != infos.end() )
    {

      return i->second.show;

    }

  }

  return false;

}


bool gkg::VoiLabelInfo::getColor( int32_t index, gkg::RGBComponent& color )
{

  if ( ( index > 0 ) && ( index < gkg::VoiLabelInfo::labelMax ) )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.find( index );

    if ( i != infos.end() )
    {

      if ( i->second.show )
      {

        color = i->second.rgb;

        return true;

      }

    }

  }

  return false;

}


void gkg::VoiLabelInfo::update( int32_t index, const std::string& name,
                                int32_t r, int32_t g, int32_t b )
{

  if ( index < gkg::VoiLabelInfo::labelMax )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.find( index );

    if ( i != infos.end() )
    {

      i->second.name = name;
      i->second.rgb.r = (uint8_t)r;
      i->second.rgb.g = (uint8_t)g;
      i->second.rgb.b = (uint8_t)b;

    }

  }

}


void gkg::VoiLabelInfo::clear()
{

  init();

  emit listCleared();
  
}


bool gkg::VoiLabelInfo::read( const std::string& fileName )
{

  std::ifstream file( fileName.c_str() );

  if ( file.good() ) 
  {

    int16_t label, r, g, b;
    std::string buf;

    infos.clear();

    while ( !file.eof() )
    {
  
      file >> label;
      file >> buf;
      file >> r;
      file >> g;
      file >> b;

      for ( int32_t pos = buf.find_first_of( "_" ); 
            pos != (int32_t)std::string::npos; )
      {

        buf.replace( pos, 1, " " );
        pos = buf.find_first_of( "_" );

      }

      infos.insert( std::make_pair( label,
                   gkg::LabelInformation( buf, 
                                          gkg::RGBComponent( (uint8_t)r,
                                                             (uint8_t)g, 
                                                             (uint8_t)b ) ) ) );
    
    }

    file.close();

    return true;

  }

  return false;

}


void gkg::VoiLabelInfo::write( const std::string& fileName )
{

  std::ofstream file( fileName.c_str() );

  if ( file.good() )
  {

    std::map< int32_t, gkg::LabelInformation >::iterator
      i = infos.begin(),
      ie = infos.end();

    while ( i != ie )
    {

      std::string buf = i->second.name;

      for ( int32_t pos = buf.find_first_of( " " ); 
            pos != (int)std::string::npos; )
      {
      
        buf.replace( pos, 1, "_" );
        pos = buf.find_first_of( " " );
          
      }

      file << i->first << " "
           << buf << " "
           << (int16_t)i->second.rgb.r << " "
           << (int16_t)i->second.rgb.g << " "
           << (int16_t)i->second.rgb.b << " "
           << std::endl;

      ++i;
      
    }

    file.close();
    
  }

}


void gkg::VoiLabelInfo::readHeader( gkg::HeaderedObject& header )
{

  if ( header.getHeader().hasAttribute( "voi" ) )
  {

    gkg::GenericObjectList palette;
    int32_t label, r, g, b;
    std::string name;

    infos.clear();
    header.getHeader().getAttribute( "voi", palette );

    gkg::GenericObjectList::iterator
      o = palette.begin(),
      oe = palette.end();

    while ( o != oe )
    {

      gkg::Dictionary d = (*o)->getValue< gkg::Dictionary >();
      name = d[ "name" ]->getString();
      label = d[ "label" ]->getScalar();
      r = d[ "red" ]->getScalar();
      g = d[ "green" ]->getScalar();
      b = d[ "blue" ]->getScalar();
      infos.insert( std::make_pair( label,
                   gkg::LabelInformation( name, 
                                          gkg::RGBComponent( (uint8_t)r,
                                                             (uint8_t)g, 
                                                             (uint8_t)b ) ) ) );

      ++o;

    }

    emit listCleared();

  }

}


void gkg::VoiLabelInfo::writeHeader( gkg::HeaderedObject& header )
{

  gkg::GenericObjectList palette;
  std::map< int32_t, gkg::LabelInformation >::iterator
    i = infos.begin(),
    ie = infos.end();

  while ( i != ie )
  {

    gkg::Dictionary p;
    p.insert( std::make_pair( "label", i->first ) );
    p.insert( std::make_pair( "name", i->second.name ) );
    p.insert( std::make_pair( "red", i->second.rgb.r ) );
    p.insert( std::make_pair( "green", i->second.rgb.g ) );
    p.insert( std::make_pair( "blue", i->second.rgb.b ) );
    palette.push_back( p );
    ++i;

  }

  header.getHeader().addAttribute( "voi", palette );

}
