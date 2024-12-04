#include <gkg-simulation-fiber-population/Fiber.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-fiber-population/FiberPopulation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Fiber::Fiber( int32_t id,
                   const gkg::FiberPopulation* parent,
                   const gkg::BoundingBox< float >& fieldOfView,
                   const gkg::Vector3d< float >& startingAtomCenter,
                   float radiusOfSphereContainingFieldOfView,
                   float atomOversamplingRatio,
                   const gkg::Vector3d< float >& meanOrientation,
                   float axonDiameter,
                   float outerMyelinSheathDiameter,
                   float internodalLength,
                   float ranvierNodeWidth,
                   float ranvierNodeOffset,
                   bool hasTortuosity,
                   bool hasBeading,
                   bool hasMyelinSheath,
                   bool hasRanvierNodes )
           : gkg::Cell( id, parent ),
             _atomOversamplingRatio( atomOversamplingRatio ),
             _meanOrientation( meanOrientation ),
             _axonDiameter( axonDiameter ),
             _outerMyelinSheathDiameter( outerMyelinSheathDiameter ),
             _internodalLength( internodalLength ),
             _ranvierNodeWidth( ranvierNodeWidth ),
             _ranvierNodeOffset( ranvierNodeOffset ),
             _hasTortuosity( hasTortuosity ),
             _hasBeading( hasBeading ),
             _hasMyelinSheath( hasMyelinSheath ),
             _hasRanvierNodes( hasRanvierNodes )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing atom radius
    ////////////////////////////////////////////////////////////////////////////

    float atomRadius = outerMyelinSheathDiameter / 2.0f;


    ////////////////////////////////////////////////////////////////////////////
    // computing atom radius
    ////////////////////////////////////////////////////////////////////////////

    int32_t maximumAtomCount = 
         ( int32_t )( radiusOfSphereContainingFieldOfView * 2.0f /
                      ( atomRadius / _atomOversamplingRatio ) ) + 1;

    gkg::Vector3d< float > displacement = _meanOrientation *
                                          ( atomRadius /
                                            _atomOversamplingRatio );

    gkg::Vector3d< float > nextAtomCenter = startingAtomCenter;

    _atoms.reserve( maximumAtomCount );


    do
    {

      if ( fieldOfView.contains( nextAtomCenter ) )
      {

        _atoms.push_back( new gkg::SphereAtom( nextAtomCenter, atomRadius ) );

      }
      nextAtomCenter += displacement;

    }
    while ( nextAtomCenter.getNorm() <=
            radiusOfSphereContainingFieldOfView );

    _lastEnvelopeIndex = this->getAtomCount() - 1;
    
    
    ////////////////////////////////////////////////////////////////////////////
    // adding information to regularize branches a posteriori
    ////////////////////////////////////////////////////////////////////////////
    
    _branches.resize( 1U );
    _branches[ 0 ].first = 0;
    _branches[ 0 ].second = _lastEnvelopeIndex;
    

  }
  GKG_CATCH( "gkg::Fiber::Fiber( "
             "int32_t id, "
             "const gkg::BoundingBox< float >& fieldOfView, "
             "const gkg::Vector3d< float >& startingAtomCenter, "
             "float radiusOfSphereContainingFieldOfView, "
             "float atomOversamplingRatio, "
             "const gkg::Vector3d< float >& meanOrientation, "
             "float axonDiameter, "
             "float outerMyelinSheathDiameter, "
             "float internodalLength, "
             "float ranvierNodeWidth, "
             "float ranvierNodeOffset, "
             "bool hasTortuosity, "
             "bool hasBeading, "
             "bool hasMyelinSheath, "
             "bool hasRanvierNodes )" );


}


gkg::Fiber::Fiber( const Fiber& other )
           : gkg::Cell( other ),
             _meanOrientation( other._meanOrientation ),
             _axonDiameter( other._axonDiameter ),
             _outerMyelinSheathDiameter( other._outerMyelinSheathDiameter ),
             _hasTortuosity( other._hasTortuosity ),
             _hasBeading( other._hasBeading ),
             _hasMyelinSheath( other._hasMyelinSheath ),
             _hasRanvierNodes( other._hasRanvierNodes )
             
{
}


gkg::Fiber::~Fiber()
{
}


gkg::Fiber& gkg::Fiber::operator=( const gkg::Fiber& other )
{

  try
  {

    this->::gkg::Cell::operator=( other );

    _meanOrientation = other._meanOrientation;
    _axonDiameter = other._axonDiameter;
    _outerMyelinSheathDiameter = other._outerMyelinSheathDiameter;
    _hasTortuosity = other._hasTortuosity;
    _hasBeading = other._hasBeading;
    _hasMyelinSheath = other._hasMyelinSheath;
    _hasRanvierNodes = other._hasRanvierNodes;

    return *this;

  }
  GKG_CATCH( "gkg::Fiber& gkg::Fiber::operator=( const gkg::Fiber& other )" );


}


float gkg::Fiber::getLength() const
{

  try
  {

    float length = 0.0f;
    int32_t s = 0;
    for ( s = 0; s < _lastEnvelopeIndex; s++ )
    {

      length += ( _atoms[ s + 1 ]->getCenter() -
                  _atoms[ s ]->getCenter() ).getNorm();

    }

    return length;

  }
  GKG_CATCH( "float gkg::Fiber::getLength() const" );

}


float gkg::Fiber::getVolume() const
{

  try
  {

    return M_PI * _outerMyelinSheathDiameter * _outerMyelinSheathDiameter *
           this->getLength() / 4.0f;

  }
  GKG_CATCH( "float gkg::Fiber::getVolume() const" );

}


float gkg::Fiber::addTortuosity(
                    const gkg::BoundingBox< float >& /*fieldOfView*/,
                    float magnitude,
                    float waveLength,
                    const gkg::RandomGenerator& randomGenerator,
                    const gkg::NumericalAnalysisImplementationFactory* factory )
{

  try
  {

    float inducedTortuosity = 0.0f;

    ////////////////////////////////////////////////////////////////////////////
    // computing the neighborhood size
    ////////////////////////////////////////////////////////////////////////////

    int32_t neighborhoodSize = ( int32_t )( 
                  ( 2.0f * _atomOversamplingRatio * waveLength ) / 
                  static_cast< gkg::SphereAtom* >( _atoms[ 0 ] )->getRadius() );
    

    ////////////////////////////////////////////////////////////////////////////
    // randomly selecting the sphere atoms around which tortuosity is induced
    ////////////////////////////////////////////////////////////////////////////

    int32_t atomCount = this->getAtomCount();

    int32_t selectedAtomIndex = ( int32_t )factory->getUniformRandomUInt32(
                                  randomGenerator,
                                  ( uint32_t )atomCount );

    // computing lower and upper bounds of sphere atom indices to be moved
    int32_t lowerAtomIndex = std::max( selectedAtomIndex -
                                       neighborhoodSize,
                                       0 );
    int32_t upperAtomIndex = std::min( selectedAtomIndex +
                                       neighborhoodSize,
                                       atomCount - 1 );


    // computing lower and upper indices of all impacted atom(s)
    int32_t lowerStartingAtomIndex = std::max( lowerAtomIndex - 1, 0 );
    int32_t upperStartingAtomIndex = upperAtomIndex;
    //if ( upperAtomIndex == atomCount - 1 )
    //{

    //  -- upperAtomIndex;

    //}

    ////////////////////////////////////////////////////////////////////////////
    // computing the old angular deviation of the selected fiber portion
    ////////////////////////////////////////////////////////////////////////////

    float oldAngularDeviation = computeLocalAngularDeviation( 
                                                       lowerStartingAtomIndex, 
                                                       upperStartingAtomIndex );


    ////////////////////////////////////////////////////////////////////////////
    // computing the orthonormal trieder at the selected atom
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > firstAxis;
    if ( selectedAtomIndex < atomCount - 1 )
    {

      firstAxis = _atoms[ selectedAtomIndex + 1 ]->getCenter() -
                  _atoms[ selectedAtomIndex ]->getCenter();
      firstAxis.normalize();


    }
    else 
    {

      firstAxis = _atoms[ selectedAtomIndex ]->getCenter() -
                  _atoms[ selectedAtomIndex - 1 ]->getCenter();
      firstAxis.normalize();

    }
    gkg::Vector3d< float > secondAxis;
    gkg::Vector3d< float > thirdAxis;
    gkg::getRandomOrthonormalTriederFromFirstAxis( firstAxis,
                                                   secondAxis,
                                                   thirdAxis );


    ////////////////////////////////////////////////////////////////////////////
    // computing displacement along the second and third axis
    ////////////////////////////////////////////////////////////////////////////

    float displacementAlongSecondAxis =
                     ( float )factory->getGaussianRandomNumber( randomGenerator,
                                                                0.0,
                                                                magnitude );
    float displacementAlongThirdAxis =
                     ( float )factory->getGaussianRandomNumber( randomGenerator,
                                                                0.0,
                                                                magnitude );


    ////////////////////////////////////////////////////////////////////////////
    // applying local deformation
    ////////////////////////////////////////////////////////////////////////////

    double offset = 0.0f;
    bool exitingFieldOfView = false;
    gkg::Vector3d< float > displacement;
    int32_t neighborAtomIndex = 0;

    std::vector< gkg::Vector3d< float > > 
      displacements( upperAtomIndex - lowerAtomIndex + 1 );
    int32_t displacementIndex = 0;

    for ( neighborAtomIndex = lowerAtomIndex;
          neighborAtomIndex <= upperAtomIndex;
          neighborAtomIndex++ )
    {

      //const gkg::Atom*
      //  neighborAtom = _atoms[ neighborAtomIndex ];

      offset = ( double )std::abs( neighborAtomIndex - 
                                   selectedAtomIndex );
      displacement = 
                  ( secondAxis * displacementAlongSecondAxis +
                    thirdAxis * displacementAlongThirdAxis ) *
                  ( float )std::exp( -( offset * offset ) /
                                     ( double )neighborhoodSize );


      //if ( !fieldOfView.contains( neighborAtom->getCenter() + displacement ) )
      //{

      //  exitingFieldOfView = true;
      //  break;

      //}
      displacements[ displacementIndex ] = displacement;
      ++ displacementIndex;

    }

    if ( !exitingFieldOfView )
    {

      displacementIndex = 0;
      for ( neighborAtomIndex = lowerAtomIndex;
            neighborAtomIndex <= upperAtomIndex;
            neighborAtomIndex ++ )
      {

        gkg::Atom*
          neighborAtom = _atoms[ neighborAtomIndex ];

        // moving each atom
        neighborAtom->setCenter( neighborAtom->getCenter() +
                                 displacements[ displacementIndex ] );
        
        ++ displacementIndex;

      }


      //////////////////////////////////////////////////////////////////////////
      // computing the old angular deviation of the selected fiber portion
      //////////////////////////////////////////////////////////////////////////

      float newAngularDeviation = computeLocalAngularDeviation( 
                                                         lowerAtomIndex, 
                                                         upperAtomIndex );

      inducedTortuosity = std::abs( newAngularDeviation - oldAngularDeviation ) /
                          atomCount;

    }

    return inducedTortuosity;

  }
  GKG_CATCH( "float gkg::Fiber::addTortuosity( "
             "const gkg::BoundingBox< float >& fieldOfView, "
             "float magnitude, "
             "float waveLength, "
             "const gkg::RandomGenerator& randomGenerator, "
             "const gkg::NumericalAnalysisImplementationFactory* factory )" );

}


void gkg::Fiber::addBeading(
                    const gkg::RCPointer< gkg::ParameterDistribution >&
                                                 interbeadingLengthDistribution,
                    const gkg::RCPointer< gkg::ParameterDistribution >&
                                                       beadingWidthDistribution,
                    const gkg::RCPointer< gkg::ParameterDistribution >&
                                              beadingMagnitudeRatioDistribution,
                    const gkg::RandomGenerator& randomGenerator,
                    const gkg::NumericalAnalysisImplementationFactory* factory )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting distance between atom centers
    ////////////////////////////////////////////////////////////////////////////

    float distanceBetweenAtomCenters = _outerMyelinSheathDiameter / 2.0f /
                                       _atomOversamplingRatio;


    ////////////////////////////////////////////////////////////////////////////
    // initializing inter-beading length, beading width and beading magnitude 
    // ratio
    ////////////////////////////////////////////////////////////////////////////

    float interbeadingLength = interbeadingLengthDistribution->getRandomValue();
    float beadingWidth = 0.0f;
    float beadingMagnitudeRatio = 0.0f;

    ////////////////////////////////////////////////////////////////////////////
    // initializing the curvilinear abscissa with a random offset
    ////////////////////////////////////////////////////////////////////////////

    float offset = ( float )factory->getUniformRandomNumber(
                                           randomGenerator,
                                           0.0,
                                           ( double )interbeadingLength / 2.0 );

    int32_t firstAtomIndex = (int32_t)( offset / 
                                        distanceBetweenAtomCenters + 
                                        0.5 ); 

    float curvilinearAbscissa = offset;

    float accumulatedLength = offset;

    std::vector< gkg::Atom* >::iterator
      a = _atoms.begin() + firstAtomIndex,
      ae = _atoms.end();
    std::vector< gkg::Atom* >::iterator
      nextA = a;
    ++ nextA;

    while ( nextA != ae )
    {

      if ( curvilinearAbscissa >= accumulatedLength )
      {

        interbeadingLength = interbeadingLengthDistribution->getRandomValue();
        beadingWidth = beadingWidthDistribution->getRandomValue();
        beadingMagnitudeRatio =
                            beadingMagnitudeRatioDistribution->getRandomValue();

        accumulatedLength += beadingWidth + interbeadingLength;

      }

      gkg::SphereAtom* atom = static_cast< gkg::SphereAtom* >( *a );
      gkg::SphereAtom* nextAtom = static_cast< gkg::SphereAtom* >( *nextA );
      gkg::Vector3d< float > atomCenter = atom->getCenter();
      gkg::Vector3d< float > nextAtomCenter = nextAtom->getCenter();

      curvilinearAbscissa += ( nextAtomCenter - atomCenter ).getNorm();

      nextAtom->setRadius(
                         nextAtom->getRadius() *
                         this->getBeadingRadiusRatio( curvilinearAbscissa,
                                                      interbeadingLength,
                                                      beadingWidth,
                                                      beadingMagnitudeRatio ) );

      ++ a;
      ++ nextA;

    }

  }
  GKG_CATCH( "void gkg::Fiber::addBeading( "
             "float interBeadingLength, "
             "float beadingWidth, "
             "float beadingMagnitudeRatio, "
             "const gkg::RandomGenerator& randomGenerator, "
             "const gkg::NumericalAnalysisImplementationFactory* factory )" );

}



void gkg::Fiber::fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {

    int32_t axonAtomCount = this->getAtomCount();

    if ( _hasMyelinSheath )
    {

      axonAtomCount /= 2;

    }

    int32_t subSamplingFactor = 1;
    if ( axonAtomCount > 100 )
    {

      subSamplingFactor = 10;

    }

    gkg::LightCurve3d< float >
      lightCurve3d = this->getLightCurve3d( subSamplingFactor );

    std::vector< float >
      axonRadii = this->getAxonRadii( subSamplingFactor, 0.95f );

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      axonMeshMap = gkg::MeshFactory::getInstance().getFiber( lightCurve3d,
                                                              axonRadii,
                                                              36,
                                                              false,
                                                              0 );

    meshAccumulators[ 0 ].add( *axonMeshMap );

    if ( _hasMyelinSheath )
    {

      std::vector< float >
        myelinSheathRadii = this->getMyelinSheathRadii( subSamplingFactor );
      gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
        myelinSheathMeshMap = gkg::MeshFactory::getInstance().getFiber(
                                                            lightCurve3d,
                                                            myelinSheathRadii,
                                                            36,
                                                            false,
                                                            0 );
      meshAccumulators[ 1 ].add( *myelinSheathMeshMap );

    }

  }
  GKG_CATCH( "void gkg::Fiber::fillMeshAccumulators( "
             "std::vector< MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


void gkg::Fiber::addDetails()
{

  try
  {

    // collecting the current atom count before adding axon membrane
    int32_t originalAtomCount = this->getAtomCount();

    //
    // in this case, we create both axon and outer myelin sheath atom(s)
    // axon atoms are first listed, followed by the outer myelin atoms
    //
    if ( _hasMyelinSheath )
    {

      // cloning sphere atoms to have axon sphere atoms first and
      // myelin sheath sphere atoms after

      std::vector< gkg::Atom* > furtherAtoms( originalAtomCount );

      std::vector< gkg::Atom* >::iterator
        fa = furtherAtoms.begin(),
        fae = furtherAtoms.end();
      std::vector< gkg::Atom* >::const_iterator
        oa = _atoms.begin();
      while ( fa != fae )
      {

        *fa = ( *oa )->clone();
        ++ oa;
        ++ fa;

      }

      _atoms.insert( _atoms.end(),
                     furtherAtoms.begin(),
                     furtherAtoms.end() );

      // modify the diameter of axon sphere atoms from 
      // _outerMyelinSheathDiameter to _axonDiameter
      std::vector< gkg::Atom* >::iterator
        a = _atoms.begin(),
        ae = _atoms.begin() + originalAtomCount;
      while ( a != ae )
      {

        static_cast< gkg::SphereAtom* >( *a )->setRadius( _axonDiameter /
                                                          2.0f );
        ++ a;

      }

    }
    //
    // in this case, we replace the outer myelin sheath by the inner axon
    // we only have to reduce the actual diameter originally set to the outer
    // myelin sheath diameter to the axon diameter
    //
    else
    {

      std::vector< gkg::Atom* >::iterator
        a = _atoms.begin(),
        ae = _atoms.end();
      while ( a != ae )
      {

        static_cast< gkg::SphereAtom* >( *a )->setRadius( _axonDiameter /
                                                          2.0f );
        ++ a;

      }

    }


    //
    // creating the Ranvier nodes on demand
    //
    if ( _hasMyelinSheath && _hasRanvierNodes )
    {

      int32_t atomOffset = ( int32_t )( _ranvierNodeOffset /
                                        ( _axonDiameter / 2.0f ) );
      gkg::Vector3d< float > previousRanvierNodePosition( 0.0f, 0.0f, 0.0f );
      bool offsetApplied = false;

      // looping over outer myelin sheath sphere atoms
      int32_t atomIndex = 0;

      std::vector< gkg::Atom* >::iterator
        a = _atoms.begin() + originalAtomCount,
        ae = _atoms.end();
      while ( a != ae )
      {

        gkg::SphereAtom* sphereAtom = static_cast< gkg::SphereAtom* >( *a );

        if ( offsetApplied )
        {

          if ( ( sphereAtom->getCenter() -
                 previousRanvierNodePosition ).getNorm() >=
               _internodalLength )
          {

            sphereAtom->setRadius( _axonDiameter / 2.0f );
            previousRanvierNodePosition = sphereAtom->getCenter();

          }
          if ( ( sphereAtom->getCenter() - 
                 previousRanvierNodePosition ).getNorm() <=
               _ranvierNodeWidth )
          {

            sphereAtom->setRadius( _axonDiameter / 2.0f );

          }

        }
        else if ( atomIndex == atomOffset )
        {

          sphereAtom->setRadius( _axonDiameter / 2.0f );
          previousRanvierNodePosition = sphereAtom->getCenter();
          offsetApplied = true;

        }

        ++ atomIndex;
        ++ a;

      }

    }

  }
  GKG_CATCH( "void gkg::Fiber::addDetails()" );


}


gkg::LightCurve3d< float > 
gkg::Fiber::getLightCurve3d( int32_t subSamplingFactor ) const
{

  try
  {

    int32_t axonAtomCount = this->getAtomCount();

    if ( _hasMyelinSheath )
    {

      axonAtomCount /= 2;

    }

    gkg::LightCurve3d< float > lightCurve3d;
    lightCurve3d.reserve( axonAtomCount / subSamplingFactor );

    int32_t i = 0;
    for ( i = 0; i < axonAtomCount; i+= subSamplingFactor )
    {

      lightCurve3d.addPoint( _atoms[ i ]->getCenter() );

    }

    return lightCurve3d;

  }
  GKG_CATCH( "gkg::LightCurve3d< float > "
             "gkg::Fiber::getLightCurve3d( int32_t subSamplingFactor ) const" );

}


std::vector< float > 
gkg::Fiber::getAxonRadii( int32_t subSamplingFactor,
                          float scaling ) const
{

  try
  {

    int32_t axonAtomCount = this->getAtomCount();

    if ( _hasMyelinSheath )
    {

      axonAtomCount /= 2;

    }

    std::vector< float > axonRadii;
    axonRadii.reserve( axonAtomCount / subSamplingFactor );


    int32_t i = 0;
    for ( i = 0; i < axonAtomCount; i+= subSamplingFactor )
    {

      axonRadii.push_back(
        static_cast< gkg::SphereAtom* >( _atoms[ i ] )->getRadius() * scaling );

    }


    return axonRadii;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::Fiber::getAxonRadii( int32_t subSamplingFactor, "
             "float scaling ) const" );

}


std::vector< float > 
gkg::Fiber::getMyelinSheathRadii( int32_t subSamplingFactor,
                                  float scaling ) const
{

  try
  {

    int32_t axonAtomCount = this->getAtomCount();
    int32_t myelinSheathAtomCount = 0;

    if ( _hasMyelinSheath )
    {

      axonAtomCount /= 2;
      myelinSheathAtomCount = axonAtomCount;

    }

    std::vector< float > myelinSheathRadii;
    myelinSheathRadii.reserve( myelinSheathAtomCount /
                               subSamplingFactor );

    int32_t i = 0;
    for ( i = axonAtomCount;
          i < axonAtomCount + myelinSheathAtomCount;
          i+= subSamplingFactor )
    {

      myelinSheathRadii.push_back(
        static_cast< gkg::SphereAtom* >( _atoms[ i ] )->getRadius() * scaling );

    }

    return myelinSheathRadii;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::Fiber::getMyelinSheathRadii( int32_t subSamplingFactor, "
             "float scaling ) const" );

}


float gkg::Fiber::computeLocalAngularDeviation( int32_t lowerAtomIndex,
                                                int32_t upperAtomIndex ) const
{

  try
  {

    float localAngularDeviation = 0.0f;
    int32_t i = 0;
    for ( i = lowerAtomIndex; i < upperAtomIndex; i++ )
    {

      const gkg::Vector3d< float >& p1 = 
        static_cast< gkg::SphereAtom* >( _atoms[ i ] )->getCenter();
      const gkg::Vector3d< float >& p2 =
        static_cast< gkg::SphereAtom* >( _atoms[ i + 1 ] )->getCenter();

      localAngularDeviation += gkg::getLineAngles( p2 - p1, _meanOrientation );

    }

    return localAngularDeviation;

  }
  GKG_CATCH( "float gkg::Fiber::computeLocalAngularDeviation( "
             "int32_t lowerAtomIndex, "
             "int32_t upperAtomIndex ) const" );

}


float gkg::Fiber::getBeadingRadiusRatio( float curvilinearAbscissa,
                                         float interbeadingLength,
                                         float beadingWidth,
                                         float beadingMagnitudeRatio ) const
{

  try
  {

    float ratio = 1.0f;
    float modulo = ::fmodf( curvilinearAbscissa, interbeadingLength );
    float halfBeadingWidth = beadingWidth / 2.0f;

    if ( modulo <= halfBeadingWidth )
    {

      ratio = 1.0f + ( beadingMagnitudeRatio - 1.0f ) * 
                     std::sin( M_PI / 2.0 * modulo / halfBeadingWidth );      

    }
    else if ( modulo < beadingWidth )
    {

      ratio = 1.0f + ( beadingMagnitudeRatio - 1.0f ) * 
                     std::sin( M_PI / 2.0 * ( beadingWidth - modulo ) /
                               halfBeadingWidth ); 

    }

    return ratio;

  }
  GKG_CATCH( "float gkg::Fiber::getBeadingRadiusRatio( "
             "float curvilinearAbscissa, "
             "float interbeadingLength, "
             "float beadingWidth, "
             "float beadingMagnitudeRatio ) const" );

}
