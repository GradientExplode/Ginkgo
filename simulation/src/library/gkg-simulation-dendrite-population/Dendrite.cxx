#include <gkg-simulation-dendrite-population/Dendrite.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-dendrite-population/DendritePopulation.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Dendrite::Dendrite( int32_t id,
                         const gkg::DendritePopulation* parent,
                         const gkg::Vector3d< float >& startingAtomCenter,
                         const gkg::Vector3d< float >& endingAtomCenter,
                         float atomOversamplingRatio,
                         float radius,
                         float hasTortuosity,
                         float tortuosityMagnitude,
                         float tortuosityAngularDispersionInDegrees,
                         float tortuosityWaveLength )
              : gkg::Cell( id, parent ),
                _atomOversamplingRatio( atomOversamplingRatio ),
                _radius( radius ),
                _hasTortuosity( hasTortuosity ),
                _tortuosityMagnitude( tortuosityMagnitude ),
                _tortuosityAngularDispersionInDegrees(
                                         tortuosityAngularDispersionInDegrees ),
                _tortuosityWaveLength( tortuosityWaveLength )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting the virtual tissue field of view
    ////////////////////////////////////////////////////////////////////////////

    const gkg::BoundingBox< float >& 
      fieldOfView = _parent->getParent()->getFieldOfView();


    ////////////////////////////////////////////////////////////////////////////
    // creating a 3D light curve from the two extremities
    ////////////////////////////////////////////////////////////////////////////

    int32_t pointCount =
                 std::max( ( int32_t )( _atomOversamplingRatio *
                                        ( endingAtomCenter -
                                          startingAtomCenter ).getNorm() /
                                        _radius + 0.5 ),
                           2 );

    gkg::LightCurve3d< float > dendriteCurve3d( 2 );
    dendriteCurve3d.setPoint( 0, startingAtomCenter );
    dendriteCurve3d.setPoint( 1, endingAtomCenter );
    dendriteCurve3d.setEquidistantCurve( pointCount );


    ////////////////////////////////////////////////////////////////////////////
    // adding tortuosity to the dendrite
    ////////////////////////////////////////////////////////////////////////////

    if ( _hasTortuosity )
    {

      dendriteCurve3d.addTortuosity( _tortuosityMagnitude,
                                     _tortuosityAngularDispersionInDegrees,
                                     _tortuosityWaveLength,
                                     ( endingAtomCenter -
                                       startingAtomCenter ).normalize(),
                                     fieldOfView,
                                     0,
                                     100000 );

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding tortuosity to the dendrite
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Atom* > dendriteAtoms;
    gkg::LightCurve3d< float >::const_iterator
      p = dendriteCurve3d.begin(),
      pe = dendriteCurve3d.end();
    while ( p != pe )
    {

      if ( fieldOfView.contains( *p ) )
      {

        dendriteAtoms.push_back( new gkg::SphereAtom( *p, _radius ) );

      }
      ++ p;

    }

    ////////////////////////////////////////////////////////////////////////////
    // adding the dendrite sphere atom(s) to the cell structure
    ////////////////////////////////////////////////////////////////////////////

    _atoms.reserve( dendriteAtoms.size() );
    
    std::list< gkg::Atom* >::const_iterator
      a = dendriteAtoms.begin(),
      ae = dendriteAtoms.end();
    while ( a != ae )
    {

      _atoms.push_back( *a );
      ++ a;

    }

    ////////////////////////////////////////////////////////////////////////////
    // adding information to regularize branches a posteriori
    ////////////////////////////////////////////////////////////////////////////
    
    _branches.resize( 1U );
    _branches[ 0 ].first = 0;
    _branches[ 0 ].second = ( int32_t )dendriteAtoms.size() - 1;

  }
  GKG_CATCH( "gkg::Dendrite::Dendrite( "
             "int32_t id, "
             "const gkg::DendritePopulation* parent, "
             "const gkg::Vector3d< float >& startingAtomCenter, "
             "const gkg::Vector3d< float >& endingAtomCenter, "
             "float atomOversamplingRatio, "
             "float radius, "
             "float hasTortuosity, "
             "float tortuosityMagnitude, "
             "float tortuosityAngularDispersionInDegrees, "
             "float tortuosityWaveLength )" );


}


gkg::Dendrite::Dendrite( const gkg::Dendrite& other )
              : gkg::Cell( other ),
                _atomOversamplingRatio( other._atomOversamplingRatio ),
                _radius( other._radius ),
                _hasTortuosity( other._hasTortuosity ),
                _tortuosityMagnitude( other._tortuosityMagnitude ),
                _tortuosityAngularDispersionInDegrees(
                                  other._tortuosityAngularDispersionInDegrees ),
                _tortuosityWaveLength( other._tortuosityWaveLength )
             
{
}


gkg::Dendrite::~Dendrite()
{
}


gkg::Dendrite& gkg::Dendrite::operator=( const gkg::Dendrite& other )
{

  try
  {

    this->::gkg::Cell::operator=( other );

    _atomOversamplingRatio = other._atomOversamplingRatio;
    _radius = other._radius;
    _hasTortuosity = other._hasTortuosity;
    _tortuosityMagnitude = other._tortuosityMagnitude;
    _tortuosityAngularDispersionInDegrees =
                                    other._tortuosityAngularDispersionInDegrees;
    _tortuosityWaveLength = other._tortuosityWaveLength;

    return *this;

  }
  GKG_CATCH( "gkg::Dendrite& gkg::Dendrite::operator=( "
             "const gkg::Dendrite& other )" );


}


float gkg::Dendrite::getAtomOversamplingRatio() const
{

  try
  {

    return _atomOversamplingRatio;

  }
  GKG_CATCH( "float gkg::Dendrite::getAtomOversamplingRatio() const" );

}


float gkg::Dendrite::getRadius() const
{

  try
  {

    return _radius;

  }
  GKG_CATCH( "float gkg::Dendrite::getRadius() const" );

}


bool gkg::Dendrite::getHasTortuosity() const
{

  try
  {

    return _hasTortuosity;

  }
  GKG_CATCH( "bool gkg::Dendrite::getHasTortuosity() const" );

}


float gkg::Dendrite::getTortuosityMagnitude() const
{

  try
  {

    return _tortuosityMagnitude;

  }
  GKG_CATCH( "float gkg::Dendrite::getTortuosityMagnitude() const" );

}


float gkg::Dendrite::getTortuosityAngularDispersionInDegrees() const
{

  try
  {

    return _tortuosityAngularDispersionInDegrees;

  }
  GKG_CATCH( "float "
             "gkg::Dendrite::getTortuosityAngularDispersionInDegrees() const" );

}


float gkg::Dendrite::getTortuosityWaveLength() const
{

  try
  {

    return _tortuosityWaveLength;

  }
  GKG_CATCH( "float gkg::Dendrite::getTortuosityMagnitude() const" );

}




float gkg::Dendrite::getLength() const
{

  try
  {

    int32_t segmentCount = ( int32_t )_atoms.size() - 1;
    float length = 0.0f;
    int32_t s = 0;
    for ( s = 0; s < segmentCount; s++ )
    {

      length += ( _atoms[ s + 1 ]->getCenter() -
                  _atoms[ s ]->getCenter() ).getNorm();

    }

    return length;

  }
  GKG_CATCH( "float gkg::Dendrite::getLength() const" );

}


float gkg::Dendrite::getVolume() const
{

  try
  {

    return M_PI * _radius * _radius * this->getLength();

  }
  GKG_CATCH( "float gkg::Dendrite::getVolume() const" );

}


void gkg::Dendrite::fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {

    int32_t dendriteAtomCount = this->getAtomCount();

    int32_t subSamplingFactor = 1;
    if ( dendriteAtomCount > 100 )
    {

      subSamplingFactor = 10;

    }

    gkg::LightCurve3d< float >
      lightCurve3d = this->getLightCurve3d( subSamplingFactor );

    std::vector< float > dendriteRadii( subSamplingFactor, _radius );

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      dendriteMeshMap = gkg::MeshFactory::getInstance().getFiber( lightCurve3d,
                                                                  dendriteRadii,
                                                                  36,
                                                                  false,
                                                                  0 );
    meshAccumulators[ 0 ].add( *dendriteMeshMap );

  }
  GKG_CATCH( "void gkg::Dendrite::fillMeshAccumulators( "
             "std::vector< MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


void gkg::Dendrite::addDetails()
{

  // nothing to do

}


gkg::LightCurve3d< float > 
gkg::Dendrite::getLightCurve3d( int32_t subSamplingFactor ) const
{

  try
  {

    int32_t dendriteAtomCount = this->getAtomCount();

    gkg::LightCurve3d< float > lightCurve3d;
    lightCurve3d.reserve( dendriteAtomCount / subSamplingFactor );

    int32_t i = 0;
    for ( i = 0; i < dendriteAtomCount; i+= subSamplingFactor )
    {

      lightCurve3d.addPoint( _atoms[ i ]->getCenter() );

    }

    return lightCurve3d;

  }
  GKG_CATCH( "gkg::LightCurve3d< float > "
             "gkg::Dendrite::getLightCurve3d( "
             "int32_t subSamplingFactor ) const" );

}


