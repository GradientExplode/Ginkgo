#include <gkg-simulation-soma-population/Soma.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-soma-population/SomaPopulation.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Soma::Soma( int32_t id,
                 const SomaPopulation* parent,
                 float diameter,
                 const gkg::Vector3d< float >& center )
           : gkg::Cell( id, parent ),
             _diameter( diameter )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing atom radius
    ////////////////////////////////////////////////////////////////////////////

    float atomRadius = diameter / 2.0f;


    ////////////////////////////////////////////////////////////////////////////
    // creating the unique atom
    ////////////////////////////////////////////////////////////////////////////

    _atoms.push_back( new gkg::SphereAtom( center, atomRadius ) );

  }
  GKG_CATCH( "gkg::Soma::Soma( "
             "int32_t id, "
             "float diameter, "
             "const gkg::Vector3d< float >& center )" );


}


gkg::Soma::Soma( const gkg::Soma& other )
           : gkg::Cell( other ),
             _diameter( other._diameter )
             
{
}


gkg::Soma::~Soma()
{
}


gkg::Soma& gkg::Soma::operator=( const gkg::Soma& other )
{

  try
  {

    this->::gkg::Cell::operator=( other );

    _diameter = other._diameter;

    return *this;

  }
  GKG_CATCH( "gkg::Soma& gkg::Soma::operator=( const gkg::Soma& other )" );


}


float gkg::Soma::getDiameter() const
{

  try
  {

    return _diameter;

  }
  GKG_CATCH( "float gkg::Soma::getDiameter() const" );

}


gkg::Vector3d< float > gkg::Soma::getCenter() const
{

  try
  {

    return _atoms[ 0 ]->getCenter();

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::Soma::getCenter() const" );

}


float gkg::Soma::getVolume() const
{

  try
  {

    return M_PI * _diameter * _diameter * _diameter / 6.0f;

  }
  GKG_CATCH( "float gkg::Soma::getVolume() const" );

}


void gkg::Soma::fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {

    const gkg::SphereAtom* sphereAtom = static_cast< gkg::SphereAtom* >(
                                                                  _atoms[ 0 ] );

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      somaMeshMap = gkg::MeshFactory::getInstance().getSphere(
                                                        sphereAtom->getCenter(),
                                                        sphereAtom->getRadius(),
                                                              60 );

    meshAccumulators[ 0 ].add( *somaMeshMap );

  }
  GKG_CATCH( "void gkg::Soma::fillMeshAccumulators( "
             "std::vector< MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


void gkg::Soma::addDetails()
{

  try
  {

    // nothing to do

  }
  GKG_CATCH( "void gkg::Soma::addDetails()" );


}


