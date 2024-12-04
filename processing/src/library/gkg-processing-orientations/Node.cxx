#include <gkg-processing-orientations/Node.h>
#include <gkg-processing-orientations/TreeOrientation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/StringConverter.h>


static int getRandomNumber( int32_t maximum )
{

  // getting numerical analysis implementation factory
  static gkg::NumericalAnalysisImplementationFactory*
    factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
  static gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

  return ( int32_t )factory->getUniformRandomNumber( randomGenerator,
                                                     0.0,
                                                     ( float )maximum );

}


gkg::Node::Node( int32_t orientationCount,
                 bool avoidSymmetricOrientations )
          : _ownWeight( 0 ),
            _childrenWeight( 0 )
{

  int32_t o;
  for ( o = 0; o < orientationCount; o++ )
  {

    _orientations.push_back( new gkg::TreeOrientation(
                                                 avoidSymmetricOrientations ) );

  }

}


gkg::Node::Node( const std::string& pattern,
                 bool avoidSymmetricOrientations )
          : _ownWeight( 0 ),
            _childrenWeight( 0 )
{

  analyzePattern( pattern, avoidSymmetricOrientations );

}


gkg::Node::~Node()
{

  std::list< gkg::Node* >::iterator n = _children.begin(), ne = _children.end();
  while ( n != ne )
  {

    delete *n;
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::iterator o = _orientations.begin(),
                                               oe = _orientations.end();
  while ( o != oe )
  {

    delete *o;
    ++ o;

  }

}


const std::list< gkg::Node* >& gkg::Node::getChildren() const
{

  return _children;

}


const std::list< gkg::TreeOrientation* >& gkg::Node::getOrientations() const
{

  return _orientations;

}


void gkg::Node::addChild( gkg::Node* node )
{

  _children.push_back( node );

}


void gkg::Node::addOrientation( gkg::TreeOrientation* orientation )
{

  _orientations.push_back( orientation );

}


int32_t gkg::Node::getOrientationCount() const
{

  int32_t orientationCount = ( int32_t )_orientations.size();

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    orientationCount += ( *n )->getOrientationCount();
    ++ n;

  }

  return orientationCount;

}


int32_t gkg::Node::setWeight( int32_t depth, int32_t probabilityFactor )
{

  _ownWeight = depth;
  _childrenWeight = 0;

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    _childrenWeight += ( *n )->setWeight( depth * probabilityFactor,
                                          probabilityFactor );
    ++ n;

  }
  return ( _ownWeight + _childrenWeight );

}


int32_t gkg::Node::getWeight() const
{

  return ( _ownWeight + _childrenWeight ); 

}


// global energy of a node
float gkg::Node::getGlobalEnergy( float multiplicator ) const
{

  float energy = 0.0;

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();

  while ( n != ne )
  {

    energy += multiplicator * ( *n )->getGlobalEnergy( multiplicator );

    std::list< gkg::Node* >::const_iterator nprime = n;
    ++ nprime;
    while ( nprime != ne )
    {

      energy += ( *n )->getEnergy( **nprime );
      ++ nprime;

    }

    o = _orientations.begin();
    while ( o != oe )
    {

      energy += ( *n )->getEnergy( **o );
      ++ o;

    }

    ++ n;

  }

  o = _orientations.begin();
  while ( o != oe )
  {

    std::list< gkg::TreeOrientation* >::const_iterator oprime = o;
    ++ oprime;
    while ( oprime != oe )
    {

      energy += ( *o )->gkg::Orientation::getEnergy( **oprime );
      ++ oprime;

    }
    ++ o;

  }

  energy /= ( float )( getOrientationCount() ) *
            ( float )( getOrientationCount() - 1 );

  return energy;

}


// energy between two nodes
float gkg::Node::getEnergy( const gkg::Node& other ) const
{

  float energy = 0.0;

  std::list< gkg::Node* >::const_iterator n1 = _children.begin(),
                                          n1e = _children.end();
  std::list< gkg::TreeOrientation* >::const_iterator o1 = _orientations.begin(),
                                                     o1e = _orientations.end();
  while ( n1 != n1e )
  {

    std::list< gkg::Node* >::const_iterator n2 = other.getChildren().begin(),
                                            n2e = other.getChildren().end();
    while ( n2 != n2e )
    {

      energy += ( *n1 )->getEnergy( **n2 );
      ++ n2;

    }

    o1 = _orientations.begin();
    while ( o1 != o1e )
    {

      energy += ( *n1 )->getEnergy( **o1 );
      ++ o1;

    }
    ++ n1;

  }
  
  o1 = _orientations.begin();
  while ( o1 != o1e )
  {

    std::list< gkg::Node* >::const_iterator n2 = other.getChildren().begin(),
                                            n2e = other.getChildren().end();
    while ( n2 != n2e )
    {

      energy += ( *n2 )->getEnergy( **o1 );
      ++ n2;

    }

    std::list< gkg::TreeOrientation* >::const_iterator
      o2 = other.getOrientations().begin(),
      o2e = other.getOrientations().end();
    while ( o2 != o2e )
    {

      energy += ( *o1 )->getEnergy( **o2 );
      ++ o2;

    }

    ++ o1;

  }

  return energy;

}


// energy between a node and an orientation
float gkg::Node::getEnergy( const gkg::TreeOrientation& orientation ) const
{

  float energy = 0;

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    energy += ( *n )->getEnergy( orientation );
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();
  while ( o != oe )
  {

    energy += ( *o )->getEnergy( orientation );
    ++ o;

  }

  return energy;

}


// total forces on a node
void gkg::Node::setForces( float multiplicator, float weight )
{

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();

  while ( n != ne )
  {

    ( *n )->setForces( multiplicator, multiplicator * weight );

    std::list< gkg::Node* >::const_iterator nprime = _children.begin();
    while ( nprime != ne )
    {

      if ( nprime != n )
      {

        ( *n )->addForces( **nprime, weight );

      }

      ++ nprime;

    }

    o = _orientations.begin();
    while ( o != oe )
    {

      ( *n )->addForces( **o, weight );
      ++ o;

    }

    ++ n;

  }

  o = _orientations.begin();
  while ( o != oe )
  {

    n = _children.begin();
    while ( n != ne )
    {

      ( *o )->addForce( **n, weight );
      ++ n;

    }

    std::list< gkg::TreeOrientation* >::const_iterator
      oprime = _orientations.begin();
    while ( oprime != oe )
    {

      if ( oprime != o )
      {

        ( *o )->gkg::Orientation::addForce( **oprime, weight * weight );

      }
      ++ oprime;

    }

    ++ o;

  }

}


// forces with another node
void gkg::Node::addForces( const gkg::Node& other, float weight )
{

  std::list< gkg::Node* >::const_iterator n1 = _children.begin(),
                                          n1e = _children.end();

    std::list< gkg::Node* >::const_iterator n2 = other.getChildren().begin(),
                                            n2e = other.getChildren().end();

  std::list< gkg::TreeOrientation* >::const_iterator o1 =_orientations.begin(),
                                                     o1e = _orientations.end();
  std::list< gkg::TreeOrientation* >::const_iterator
    o2 = other.getOrientations().begin(),
    o2e = other.getOrientations().end();

  while ( n1 != n1e )
  {

    n2 = other.getChildren().begin();
    while ( n2 != n2e )
    {

      ( *n1 )->addForces( **n2, weight );
      ++ n2;

    }

    o2 = other.getOrientations().begin();
    while ( o2 != o2e )
    {

      ( *n1 )->addForces( **o2, weight );
      ++ o2;

    }

    ++ n1;

  }

  o1 = _orientations.begin();
  while ( o1 != o1e )
  {

    n2 = other.getChildren().begin();
    while ( n2 != n2e )
    {

      ( *o1 )->addForce( **n2, weight );
      ++ n2;

    }

    o2 = other.getOrientations().begin();
    while ( o2 != o2e )
    {

      ( *o1 )->gkg::Orientation::addForce( **o2, weight * weight );
      ++ o2;

    }

    ++ o1;

  }

}


// forces with an orientation
void gkg::Node::addForces( const gkg::TreeOrientation& orientation,
                           float weight )
{

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();

  while ( n != ne )
  {

    ( *n )->addForces( orientation, weight );
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();
  while ( o != oe )
  {

    ( *o )->gkg::Orientation::addForce( orientation, weight * weight );
    ++ o;

  }

}


void gkg::Node::resetForce()
{

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    ( *n )->resetForce();
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();
  while ( o != oe )
  {

    ( *o )->resetForce();
    ++ o;

  }

}


// choose randomly a node to move
gkg::Node* gkg::Node::getRandomNode()
{

  int32_t rank = getRandomNumber( _ownWeight + _childrenWeight );
  if ( rank <= _ownWeight )
  {

    return this;

  }

  rank -= _ownWeight;

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    int32_t childWeight = ( *n )->getWeight();
    if ( rank <= childWeight )
    {

      return ( *n )->getRandomNode();

    }  
    rank -= childWeight ;

    ++ n;

  }

  return 0;

}


// move all node orientations
void gkg::Node::move( float step )
{

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    ( *n )->move( step );
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();
  while ( o != oe )
  {

    ( *o )->move( step );
    ++ o;

  }

}


void gkg::Node::undo()
{

  std::list< gkg::Node* >::const_iterator n = _children.begin(),
                                          ne = _children.end();
  while ( n != ne )
  {

    ( *n )->undo();
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator o = _orientations.begin(),
                                                     oe = _orientations.end();
  while ( o != oe )
  {

    ( *o )->undo();
    ++ o;

  }

}


void gkg::Node::analyzePattern( const std::string& pattern,
                                bool avoidSymmetricOrientations )
{

  std::string subPattern;

  // remove any space
  std::string patternWoSpace;
  size_t c;
  for ( c = 0; c < pattern.length(); c++ )
  {

    if ( ( pattern[ c ] != ' ' ) &&
         ( pattern[ c ] != '\t' ) &&
         ( pattern[ c ] != '\n' ) )
    {

      patternWoSpace.push_back( pattern[ c ] );

    }

  }

  int32_t orientationCount = 0;
  while ( patternWoSpace.length() )
  {

    if ( firstElementIsNode( patternWoSpace, subPattern ) )
    {

      addChild( new gkg::Node( subPattern, avoidSymmetricOrientations ) );

    }
    else if ( firstElementIsOrientation( patternWoSpace ) )
    {

      addOrientation( new gkg::TreeOrientation( avoidSymmetricOrientations ) );

    }
    else if ( firstElementIsDigit( patternWoSpace, orientationCount ) )
    {

      addChild( new gkg::Node( orientationCount, avoidSymmetricOrientations ) );

    }

  }

}


bool gkg::Node::firstElementIsNode( std::string& pattern,
                                    std::string& subPattern ) const
{

  if ( pattern[ 0 ] != '[' )
  {

    return false;

  }

  int32_t endPosition = 1;
  int32_t openingBraketCount = 0;
  bool ok = false;
  while ( !ok )
  {

    if ( ( pattern[ endPosition ] == ']' ) && ( openingBraketCount == 0 ) )
    {

      ok = true;

    }
    else
    {

      if ( pattern[ endPosition ] == '[' )
      {

        ++ openingBraketCount;

      }
      else if ( pattern[ endPosition ] == ']' )
      {

        -- openingBraketCount;

      }
      ++ endPosition;

    }

  }

  subPattern = pattern.substr( 1, endPosition - 1 );
  if ( ( pattern.length() - endPosition ) > 1 )
  {

    pattern = pattern.substr( endPosition + 2, pattern.length() - endPosition );

  }
  else
  {

    pattern = "";

  }
  return true;
  
}


bool gkg::Node::firstElementIsOrientation( std::string& pattern ) const
{

  if ( pattern[ 0 ] != 'd' )
  {

    return false;

  }
  
  if ( pattern.length() > 1 )
  {

    pattern = pattern.substr( 2, pattern.length() - 2 );

  }
  else
  {

    pattern = "";

  }
  return true;

}


bool gkg::Node::firstElementIsDigit( std::string& pattern,
                                     int32_t& orientationCount ) const
{

  if ( !isdigit( pattern[ 0 ] ) )
  {

    return false;

  }
  int32_t endPosition = 1;
  while ( isdigit( pattern[ endPosition ] ) )
  {

    ++ endPosition;

  }
  gkg::StringConverter::toScalar( pattern.substr( 0, endPosition ),
                                  orientationCount );
  if ( ( pattern.length() - endPosition ) > 1 )
  {

    pattern = pattern.substr( endPosition + 1,
                              pattern.length() - endPosition - 1 );

  }
  else
  {

    pattern = "";

  }
  return true;

}


std::ostream& std::operator << ( std::ostream& os, const gkg::Node& thing )
{

  std::list< gkg::Node* >::const_iterator n = thing.getChildren().begin(),
                                          ne = thing.getChildren().end();
  while ( n != ne )
  {

    os << **n;
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator
    o = thing.getOrientations().begin(),
    oe = thing.getOrientations().end();
  while ( o != oe )
  {

    os << **o;
    ++ o;

  }
  return os;

}

  
  
std::ostream& gkg::printCpp( std::ostream& os, const gkg::Node& thing )
{

  std::list< gkg::Node* >::const_iterator n = thing.getChildren().begin(),
                                          ne = thing.getChildren().end();
  while ( n != ne )
  {

    printCpp( os, **n );
    ++ n;

  }

  std::list< gkg::TreeOrientation* >::const_iterator
    o = thing.getOrientations().begin(),
    oe = thing.getOrientations().end();
  while ( o != oe )
  {

    os << "  { "
       << ( *o )->getUnitVector().x
       << ", "
       << ( *o )->getUnitVector().y
       << ", "
       << ( *o )->getUnitVector().z
       << " },"
       << std::endl;
    ++ o;

  }
  return os;

}

  
