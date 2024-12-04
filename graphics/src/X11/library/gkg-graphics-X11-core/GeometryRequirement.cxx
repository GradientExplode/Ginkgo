#include <gkg-graphics-X11-core/GeometryRequirement.h>
#include <gkg-processing-algobase/Math.h>


gkg::GeometryRequirement::GeometryRequirement()
                         : _natural( -fil ),
                           _stretch( 0 ),
                           _shrink( 0 ),
                           _alignment( 0 )
{
}


gkg::GeometryRequirement::GeometryRequirement(
                                         const gkg::GeometryRequirement& other )
                         : _natural( other._natural ),
                           _stretch( other._stretch ),
                           _shrink( other._shrink ),
                           _alignment( other._alignment )
{
}


gkg::GeometryRequirement::GeometryRequirement( gkg::FloatCoordinate natural )
                            : _natural( natural ),
                              _stretch( 0 ),
                              _shrink( 0 ),
                              _alignment( 0 )
{
}


gkg::GeometryRequirement::GeometryRequirement( gkg::FloatCoordinate natural,
                                               gkg::FloatCoordinate stretch,
                                               gkg::FloatCoordinate shrink,
                                               float alignment )
                         : _natural( natural ),
                           _stretch( stretch ),
                           _shrink( shrink ),
                           _alignment( alignment )
{
}


gkg::GeometryRequirement::GeometryRequirement(
                                             gkg::FloatCoordinate naturalLead,
                                             gkg::FloatCoordinate maximumLead,
                                             gkg::FloatCoordinate minimumLead,
                                             gkg::FloatCoordinate naturalTrail,
                                             gkg::FloatCoordinate maximumTrail,
                                             gkg::FloatCoordinate minimumTrail )
{

  naturalLead = std::max( minimumLead, std::min( maximumLead, naturalLead ) );
  maximumLead = std::max( maximumLead, naturalLead );
  minimumLead = std::min( minimumLead, naturalLead );
  naturalTrail = std::max( minimumTrail,std::min( maximumTrail,naturalTrail ) );
  maximumTrail = std::max( maximumTrail, naturalTrail );
  minimumTrail = std::min( minimumTrail, naturalTrail );
  _natural = naturalLead + naturalTrail;
  if ( naturalLead == 0 )
  {

    _shrink = naturalTrail - minimumTrail;
    _stretch = maximumTrail - naturalTrail;
    _alignment = 0;

  }
  else if ( naturalTrail == 0 )
  {

    _shrink = naturalLead - minimumLead;
    _stretch = maximumLead - naturalLead;
    _alignment = 1;

  }
  else
  {

    float shrink = std::max( ( float )minimumLead / ( float )naturalLead,
                             ( float )minimumTrail / ( float )naturalTrail );
    _shrink = gkg::FloatCoordinate( _natural * ( 1 - shrink ) );
    float stretch = std::min( ( float )maximumLead / ( float )naturalLead,
                             ( float )maximumTrail / ( float )naturalTrail );
    _stretch = gkg::FloatCoordinate( _natural * ( stretch - 1 ) );
    if ( _natural == 0 )
    {

      _alignment = 0;

    }
    else
    {

      _alignment = ( float )naturalLead / ( float )_natural;

    }

  }
  
}


gkg::GeometryRequirement& gkg::GeometryRequirement::operator = (
                                         const gkg::GeometryRequirement& other )
{

  _natural = other._natural;
  _stretch = other._stretch;
  _shrink = other._shrink;
  _alignment = other._alignment;
  
  return *this;

}


bool gkg::GeometryRequirement::isEqualTo( const gkg::GeometryRequirement& other,
                                          float epsilon ) const
{

  if ( !gkg::equal( _natural, other._natural, epsilon ) )
  {

    return false;

  }
  else if ( !gkg::equal( _stretch, other._stretch, epsilon ) )
  {

    return false;

  }
  else if ( !gkg::equal( _shrink, other._shrink, epsilon ) )
  {

    return false;

  }
  else if ( !gkg::equal( _alignment, other._alignment, epsilon ) )
  {

    return false;

  }
  else
  {

    return true;

  }

}


bool gkg::GeometryRequirement::isDefined() const
{

  return _natural != -fil;

}


void gkg::GeometryRequirement::setNatural( gkg::FloatCoordinate natural )
{

  _natural = natural;

}


gkg::FloatCoordinate gkg::GeometryRequirement::getNatural() const
{

  return _natural;

}


void gkg::GeometryRequirement::setStretch( gkg::FloatCoordinate stretch )
{

  _stretch = stretch;

}


gkg::FloatCoordinate gkg::GeometryRequirement::getStretch() const
{

  return _stretch;

}


void gkg::GeometryRequirement::setShrink( gkg::FloatCoordinate shrink )
{

  _shrink = shrink;

}


gkg::FloatCoordinate gkg::GeometryRequirement::getShrink() const
{

  return _shrink;

}


void gkg::GeometryRequirement::setAlignment( float alignment )
{

  _alignment = alignment;

}


float gkg::GeometryRequirement::getAlignment() const
{

  return _alignment;

}


