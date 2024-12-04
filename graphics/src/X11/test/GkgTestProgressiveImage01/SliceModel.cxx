#include "SliceModel.h"
#include "BrainSlice.h"
#include <fstream>
#include <iostream>
#include <cstring>


SliceModel::SliceModel()
           : gkg::ProgressiveModel( 163, 202 )
{

  _data = new float[ 256 * 256 ];
  memcpy( _data, BrainSlice, 256 * 256 * sizeof( float ) );

}


SliceModel::~SliceModel()
{

  delete [] _data;

}


bool SliceModel::getValueAt( gkg::PixelCoordinate x,
                             gkg::PixelCoordinate y,
                             gkg::FloatColorIntensity& red,
                             gkg::FloatColorIntensity& green,
                             gkg::FloatColorIntensity& blue,
                             float& alpha ) const
{


  red = green = blue = _data[ x + y * 256 ];
  alpha = 1.0;
  return true;

}
