#include <gkg-processing-orientations/SlidingWindowOrientationSet.h>
#include <gkg-processing-orientations/Orientation.h>
#include <cmath>


gkg::SlidingWindowOrientationSet::SlidingWindowOrientationSet(
                                               int32_t windowSize,
                                               int32_t orientationCount,
                                               bool avoidSymmetricOrientations )
                                 : _windowSize( windowSize )
{

  int32_t o;
  for ( o = 0; o < orientationCount; o++ )
  {

    _orientations.push_back(
                           new gkg::Orientation( avoidSymmetricOrientations ) );

  }

}


gkg::SlidingWindowOrientationSet::~SlidingWindowOrientationSet()
{

  std::list< gkg::Orientation* >::const_iterator o = _orientations.begin(),
                                                 oe = _orientations.end();
  while ( o != oe )
  {

    delete *o;

    ++ o;

  }

}


int32_t gkg::SlidingWindowOrientationSet::getWindowSize() const
{

  return _windowSize;

}


int32_t gkg::SlidingWindowOrientationSet::getOrientationCount() const
{

  return ( int32_t )_orientations.size();

}


const std::list< gkg::Orientation* >&
gkg::SlidingWindowOrientationSet::getOrientations() const
{

  return _orientations;

}


float gkg::SlidingWindowOrientationSet::getGlobalEnergy( float threshold,
                                                         float power ) const
{

  std::list< gkg::Orientation* >::const_iterator o = _orientations.begin(),
                                                 oe = _orientations.end();
  std::list< gkg::Orientation* >::const_iterator oprime;

  int32_t o1 = 0, o2 = 0;
  float weight = 0.0;
  float energy = 0.0;
  while ( o != oe )
  {

    oprime = o;
    ++ oprime;
    o2 = o1 + 1;
    while ( oprime != oe )
    {

      if ( ( o2 - o1 ) < _windowSize )
      {

        weight = 1.0;

      }
      else
      {

        weight = 1 + ( 1 - threshold ) *
                      std::pow( ( _orientations.size() - _windowSize + 1 ) ,
                                power ) /
                      ( std::pow( ( _orientations.size() - _windowSize + 1 ) ,
                                  power ) -1 ) *
                      ( 1 / std::pow( ( o2 - o1 - _windowSize + 2 ) ,
                                       power ) - 1 );

      }
      energy += weight * ( *o )->getEnergy( **oprime );

      ++ oprime;
      ++ o2;

    }

    ++ o;
    ++ o1;

  }

  energy /= ( float )( _orientations.size() ) *
            ( float )( _orientations.size() - 1 );

  return energy;

}


void gkg::SlidingWindowOrientationSet::setForces( float threshold, float power )
{

  std::list< gkg::Orientation* >::const_iterator o = _orientations.begin(),
                                                 oe = _orientations.end();
  std::list< gkg::Orientation* >::const_iterator oprime;

  int32_t o1 = 0, o2 = 0, deltaOrientation = 0;
  float weight = 0.0;
  while ( o != oe )
  {

    ( *o )->resetForce();

    oprime = _orientations.begin();
    o2 = 0;
    while ( oprime != oe )
    {

      if ( oprime != o )
      {

        deltaOrientation = o2 - o1;

        if ( deltaOrientation < 0 )
        {

          deltaOrientation = -deltaOrientation;

        }

        if ( deltaOrientation < _windowSize )
        {

          weight = 1.0;

        }
        else
        {

          weight = 1 + ( 1 - threshold ) *
          		std::pow( ( _orientations.size() - _windowSize + 1 ) ,
          			  power ) /
          		( std::pow( ( _orientations.size() - _windowSize + 1 ) ,
          			    power ) -1 ) *
          		( 1 / std::pow( ( deltaOrientation - _windowSize + 2 ) ,
          				 power ) - 1 );

        }
        ( *o )->addForce( **oprime, weight * weight );

      }

      ++ oprime;
      ++ o2;

    }

    ++ o;
    ++ o1;

  }

}


void gkg::SlidingWindowOrientationSet::move( float step )
{

  std::list< gkg::Orientation* >::const_iterator o = _orientations.begin(),
                                                 oe = _orientations.end();
  while ( o != oe )
  {

    ( *o )->move( step );
    ++ o;

  }

}


void gkg::SlidingWindowOrientationSet::undo()
{

  std::list< gkg::Orientation* >::const_iterator o = _orientations.begin(),
                                                 oe = _orientations.end();
  while ( o != oe )
  {

    ( *o )->undo();
    ++ o;

  }

}


int32_t gkg::SlidingWindowOrientationSet::getSubSetCount() const 
{

  return getOrientationCount() - _windowSize + 1;

}


void gkg::SlidingWindowOrientationSet::moveSubSet( int32_t rank, float step )
{

  std::list< gkg::Orientation* >::const_iterator
    o = _orientations.begin(), oe = _orientations.begin();

  int32_t r;
  for ( r = 0; r < rank; r++ )
  {

    ++ o;
    ++ oe;

  }

  for ( r = 0; r < _windowSize; r++ )
  {

    ++ oe;

  }

  while ( o != oe )
  {

    ( *o )->move( step );
    ++ o;

  }

}


void gkg::SlidingWindowOrientationSet::undoSubSet( int32_t rank )
{

  std::list< gkg::Orientation* >::const_iterator
    o = _orientations.begin(), oe = _orientations.begin();

  int32_t r;
  for ( r = 0; r < rank; r++ )
  {

    ++ o;
    ++ oe;

  }

  for ( r = 0; r < _windowSize; r++ )
  {

    ++ oe;

  }

  while ( o != oe )
  {

    ( *o )->undo();
    ++ o;

  }

}


std::ostream& std::operator << ( std::ostream& os,
                                 const gkg::SlidingWindowOrientationSet& thing )
{

  std::list< gkg::Orientation* >::const_iterator
    o = thing.getOrientations().begin(),
    oe = thing.getOrientations().end();
  while ( o != oe )
  {

    os << **o;
    ++ o;

  }
  return os;

}

  
