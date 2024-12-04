#include <gkg-graphics-QtGL-plugin-functors/Voi/RoiCache.h>


gkg::RoiCache::RoiCache()
              : _label( -1 )
{
}


gkg::RoiCache::~RoiCache()
{
}


void gkg::RoiCache::setLabel( int32_t label )
{

  _roi.clear();
  _label = label;

}


bool gkg::RoiCache::isValid( int32_t label )
{

  return ( label == _label ) ? true : false;

}


bool gkg::RoiCache::empty()
{

  return _roi.empty();

}


void gkg::RoiCache::add( gkg::Vector3d< int32_t >& roiPoint )
{

  _roi.push_back( roiPoint );

}


std::list< gkg::Vector3d< int32_t > >& gkg::RoiCache::getRoi()
{

  return _roi;

}
