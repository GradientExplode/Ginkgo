#include <gkg-graphics-X11-core/Transformer.h>
#include <cmath>


static const double RADIAN_PER_DEGREE = M_PI / 180.0;


gkg::Transformer::Transformer()
                 : gkg::RCGUIObject(),
                   _isIdentity( true ),
                   _mat00( 1 ), _mat01( 0 ),
                   _mat10( 0 ), _mat11( 1 ),
                   _mat20( 0 ), _mat21( 0 )
{
}


gkg::Transformer::Transformer( const gkg::Transformer& other )
                 : gkg::RCGUIObject(),
                   _mat00( other._mat00 ), _mat01( other._mat01 ),
                   _mat10( other._mat10 ), _mat11( other._mat11 ),
                   _mat20( other._mat20 ), _mat21( other._mat21 )
{

  update();
  addReference();

}


gkg::Transformer::Transformer( float a00, float a01,
                               float a10, float a11,
                               float a20, float a21 )
                 : gkg::RCGUIObject(),
                   _mat00( a00 ), _mat01( a01 ),
                   _mat10( a10 ), _mat11( a11 ),
                   _mat20( a20 ), _mat21( a21 )
{

  update();
  addReference();

}


gkg::Transformer::~Transformer()
{
}


gkg::Transformer& 
gkg::Transformer::operator =( const gkg::Transformer& other )
{

  _mat00 = other._mat00;
  _mat01 = other._mat01;
  _mat10 = other._mat10;
  _mat11 = other._mat11;
  _mat20 = other._mat20;
  _mat21 = other._mat21;
  update();
  return *this;

}


bool gkg::Transformer::operator==( const gkg::Transformer& other ) const
{

  if ( _isIdentity )
  {

    return other._isIdentity;

  }
  if ( other._isIdentity )
  {

    return false;

  }
  return ( _mat00 == other._mat00 ) && ( _mat01 == other._mat01 ) &&
         ( _mat10 == other._mat10 ) && ( _mat11 == other._mat11 ) &&
         ( _mat20 == other._mat20 ) && ( _mat21 == other._mat21 );

}


bool gkg::Transformer::operator!=( const gkg::Transformer& other ) const
{

  if ( _isIdentity )
  {

    return !other._isIdentity;

  }
  if ( other._isIdentity )
  {

    return true;

  }
  return ( _mat00 != other._mat00 ) || ( _mat01 != other._mat01 ) ||
         ( _mat10 != other._mat10 ) || ( _mat11 != other._mat11 ) ||
         ( _mat20 != other._mat20 ) || ( _mat21 != other._mat21 );

}


void gkg::Transformer::setIdentity()
{

  _mat00 = 1;
  _mat01 = 0;
  _mat10 = 0;
  _mat11 = 1;
  _mat20 = 0;
  _mat21 = 0;
  update();

}


bool gkg::Transformer::isIdentity() const
{

  return _isIdentity;

}


bool gkg::Transformer::isInvertible() const
{

  return getDeterminant() != 0;

}


void gkg::Transformer::preMultiply( const gkg::Transformer& other )
{

  float tmp1 = _mat00;
  float tmp2 = _mat10;

  _mat00  = other._mat00 * tmp1 + other._mat01 * tmp2;
  _mat10  = other._mat10 * tmp1 + other._mat11 * tmp2;
  _mat20 += other._mat20 * tmp1 + other._mat21 * tmp2;

  tmp1 = _mat01;
  tmp2 = _mat11;

  _mat01  = other._mat00 * tmp1 + other._mat01 * tmp2;
  _mat11  = other._mat10 * tmp1 + other._mat11 * tmp2;
  _mat21 += other._mat20 * tmp1 + other._mat21 * tmp2;

  update();

}


void gkg::Transformer::postMultiply( const gkg::Transformer& other )
{

  float tmp = _mat00 * other._mat01 + _mat01 * other._mat11;

  _mat00 = _mat00 * other._mat00 + _mat01 * other._mat10;
  _mat01 = tmp;

  tmp = _mat10 * other._mat01 + _mat11 * other._mat11;
  _mat10 = _mat10 * other._mat00 + _mat11 * other._mat10;
  _mat11 = tmp;

  tmp = _mat20 * other._mat01 + _mat21 * other._mat11;
  _mat20 = _mat20 * other._mat00 + _mat21 * other._mat10;
  _mat21 = tmp;

  _mat20 += other._mat20;
  _mat21 += other._mat21;

  update();

}


void gkg::Transformer::invert()
{

  float d = getDeterminant();
  float t00 = _mat00;
  float t20 = _mat20;

  _mat20 = ( _mat10 * _mat21 - _mat11 * _mat20 ) / d;
  _mat21 = ( _mat01 * t20 - _mat00 * _mat21 ) / d;
  _mat00 = _mat11 / d;
  _mat11 = t00 / d;
  _mat10 = -_mat10 / d;
  _mat01 = -_mat01 / d;

  update();

}


void gkg::Transformer::translate( float dx, float dy )
{

  _mat20 += dx;
  _mat21 += dy;

  update();

}


void gkg::Transformer::scale( float sx, float sy )
{

  _mat00 *= sx;
  _mat01 *= sy;
  _mat10 *= sx;
  _mat11 *= sy;
  _mat20 *= sx;
  _mat21 *= sy;

  update();

}


void gkg::Transformer::rotate( float angle )
{

  float tmp1, tmp2, m00, m01, m10, m11, m20, m21;

  angle *= RADIAN_PER_DEGREE;
  tmp1 = std::cos( angle );
  tmp2 = std::sin( angle );
    
  m00 = _mat00 * tmp1;
  m01 = _mat01 * tmp2;
  m10 = _mat10 * tmp1;
  m11 = _mat11 * tmp2;
  m20 = _mat20 * tmp1;
  m21 = _mat21 * tmp2;

  _mat01 = _mat00 * tmp2 + _mat01 * tmp1;
  _mat11 = _mat10 * tmp2 + _mat11 * tmp1;
  _mat21 = _mat20 * tmp2 + _mat21 * tmp1;
  _mat00 = m00 - m01;
  _mat10 = m10 - m11;
  _mat20 = m20 - m21;

  update();

}


void gkg::Transformer::skew( float sx, float sy )
{

  _mat01 += _mat00 * sy;
  _mat10 += _mat11 * sx;

  update();

}


void gkg::Transformer::transform( float& x, float& y ) const
{

  float tx = x;
  x = tx * _mat00 + y * _mat10 + _mat20;
  y = tx * _mat01 + y * _mat11 + _mat21;

}


void 
gkg::Transformer::transform( float x, float y, float& tx, float& ty ) const
{

  tx = x * _mat00 + y * _mat10 + _mat20;
  ty = x * _mat01 + y * _mat11 + _mat21;

}


void gkg::Transformer::inverseTransform( float& tx, float& ty ) const
{

  float d = getDeterminant();
  float a = ( tx - _mat20 ) / d;
  float b = ( ty - _mat21 ) / d;

  tx = a * _mat11 - b * _mat10;
  ty = b * _mat00 - a * _mat01;

}


void gkg::Transformer::inverseTransform( float tx, float ty,
                                            float& x, float& y ) const
{

  float d = getDeterminant();
  float a = ( tx - _mat20 ) / d;
  float b = ( ty - _mat21 ) / d;

  x = a * _mat11 - b * _mat10;
  y = b * _mat00 - a * _mat01;

}


float gkg::Transformer::getDeterminant() const
{

  return _mat00 * _mat11 - _mat01 * _mat10;

}


void gkg::Transformer::getMatrix( float& a00, float& a01,
                                     float& a10, float& a11,
                                     float& a20, float& a21 ) const
{

  a00 = _mat00;
  a01 = _mat01;
  a10 = _mat10;
  a11 = _mat11;
  a20 = _mat20;
  a21 = _mat21;

}


static const float smoothness = 10.0f;

bool 
gkg::Transformer::isStraight( float x0, float y0, float x1, float y1,
                                 float x2, float y2, float x3, float y3 ) const
{

  float tx0, tx1, tx2, tx3;
  float ty0, ty1, ty2, ty3;

  transform( x0, y0, tx0, ty0 );
  transform( x1, y1, tx1, ty1 );
  transform( x2, y2, tx2, ty2 );
  transform( x3, y3, tx3, ty3 );
  float f = ( tx1 + tx2 ) * ( ty0 - ty3 ) +
            ( ty1 + ty2 ) * ( tx3 - tx0 ) +
            2 * ( tx0 * ty3 - ty0 * tx3 );
  return ( f * f ) < smoothness;

}


int32_t gkg::Transformer::getKey( float x, float y ) const
{

  float x1, y1, x2, y2, x3, y3;
  transform( 0, 0, x1, y1 );
  transform( 0, y, x2, y2 );
  transform( x, 0, x3, y3 );
  int32_t k1 = key( ( int32_t )( x2 - x1 ) ) & 0xff;
  int32_t k2 = key( ( int32_t )( y2 - y1 - y ) ) & 0xff;
  int32_t k3 = key( ( int32_t )( x3 - x1 - x ) ) & 0xff;
  int32_t k4 = key( ( int32_t )( y3 - y1 ) ) & 0xff;
  return ( k1 << 24 ) + ( k2 << 16 ) + ( k3 << 8 ) + k4;

}


void gkg::Transformer::update()
{

  _isIdentity = ( _mat00 == 1 && _mat11 == 1 &&
                  _mat01 == 0 && _mat10 == 0 &&
                  _mat20 == 0 && _mat21 == 0 );

}


int32_t gkg::Transformer::key( int32_t i )
{

  if ( i >= 0 )
  {

    return i < 32 ? i :
                    i < 160 ? 24 + ( i >> 2 ) :
                              i < 672 ? 54 + ( i >> 4 ) :
                                        127;

  }
  else
  {

    return i > -32 ? i :
                     i > -160 ? -24 - ( i >> 2 ) :
                                i > -672 ? -54 - ( i >> 4 ) :
                                           -127;

  }

}
