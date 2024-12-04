#include <gkg-processing-orientations/Orientation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>


static float getRandomNumber()
{

  // getting numerical analysis implementation factory
  static gkg::NumericalAnalysisImplementationFactory*
    factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
  static gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

  return ( float )factory->getUniformRandomNumber( randomGenerator,-1.0, +1.0 );

}


gkg::Orientation::Orientation( const gkg::Vector3d< float >& other,
                               bool avoidSymmetricOrientations )
                 : _avoidSymmetricOrientations( avoidSymmetricOrientations ),
                   _unitVector( other )
{

  try
  {

    _unitVector.normalize();

  }
  GKG_CATCH( "gkg::Orientation::Orientation( "
             "const gkg::Vector3d< float >& other, "
             "bool avoidSymmetricOrientations )" );

}


gkg::Orientation::Orientation( bool avoidSymmetricOrientations )
                 : _avoidSymmetricOrientations( avoidSymmetricOrientations )
{

  try
  {


    do 
    {

      _unitVector.x = getRandomNumber();
      _unitVector.y = getRandomNumber();
      _unitVector.z = getRandomNumber();

    }
    while ( _unitVector.getNorm() == 0.0 );
    _unitVector.normalize();

  }
  GKG_CATCH( "gkg::Orientation::Orientation( "
             "bool avoidSymmetricOrientations )" );

}


gkg::Orientation::~Orientation()
{
}


const gkg::Vector3d< float >& gkg::Orientation::getUnitVector() const
{

  return _unitVector;

}


float gkg::Orientation::getEnergy( const gkg::Orientation& other ) const
{

  float energy = 0.0f;
  if ( _avoidSymmetricOrientations )
  {

    energy = 1 / ( _unitVector + other.getUnitVector() ).getNorm() +
             1 / ( _unitVector - other.getUnitVector() ).getNorm();

  }
  else
  {

    energy = 1 / ( _unitVector - other.getUnitVector() ).getNorm();

  }
  return energy;
                                       

}


void gkg::Orientation::addForce( const gkg::Orientation& other, float weight )
{

  if ( _avoidSymmetricOrientations )
  {

    gkg::Vector3d< float >
      sum( _unitVector.x + other.getUnitVector().x,
           _unitVector.y + other.getUnitVector().y,
           _unitVector.z + other.getUnitVector().z );
    gkg::Vector3d< float > 
      difference( _unitVector.x - other.getUnitVector().x,
                  _unitVector.y - other.getUnitVector().y,
                  _unitVector.z - other.getUnitVector().z );


    float norm = difference.getNorm();
    _force += difference / ( norm * norm * norm ) * weight;
  
    norm = sum.getNorm();
    _force += sum / ( norm * norm * norm ) * weight;

  }
  else
  {

    gkg::Vector3d< float > 
      difference( _unitVector.x - other.getUnitVector().x,
                  _unitVector.y - other.getUnitVector().y,
                  _unitVector.z - other.getUnitVector().z );


    float norm = difference.getNorm();
    _force += difference / ( norm * norm * norm ) * weight;

  }

}


void gkg::Orientation::resetForce()
{

  _force.x = _force.y = _force.z = 0.0;

}


void gkg::Orientation::move( float step )
{

  _force -= _unitVector * _force.dot( _unitVector );
  _savedUnitVector = _unitVector;
  _unitVector += _force * step;
  _unitVector.normalize();

}


void gkg::Orientation::undo()
{

  _unitVector = _savedUnitVector;

}


std::ostream& std::operator << ( std::ostream& os,
                                 const gkg::Orientation& thing )
{

  os << thing.getUnitVector().x << ' ' 
     << thing.getUnitVector().y << ' ' 
     << thing.getUnitVector().z << std::endl;
  return os;

}


