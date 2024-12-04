#include <gkg-simulation-neuron-population/Neuron.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-neuron-population/NeuronPopulation.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-container/DictionaryHelpers.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <iostream>
#include <iomanip>


gkg::Neuron::Neuron( int32_t id,
                     const NeuronPopulation* parent,
                     const gkg::VirtualTissue& neuronModel,
                     float neuronModelDiameter,
                     const gkg::Vector3d< float >& neuronModelCenter,
                     float neuronModelVolume,
                     const gkg::Vector3d< float >& randomRotationParameters,
                     float targetDiameter,
                     const gkg::Vector3d< float >& targetCenter )
            : gkg::Cell( id, parent ),
              _diameter( targetDiameter ),
              _center( targetCenter ),
              _volume( 0.0 )
{

  try
  {

    // collecting field of view
    const gkg::BoundingBox< float >&
      fieldOfView = parent->getParent()->getFieldOfView();

    // computing scaling factor
    float scaling = targetDiameter / neuronModelDiameter;

    // computing target volume
    _volume = scaling * scaling * scaling * neuronModelVolume;

    // accessing the original atoms of the unique cell populating the selected
    // virtual tissue
    const gkg::Cell& 
      neuron = *( neuronModel.getPopulations()[ 0 ]->getCells()[ 0 ] );

    const std::vector< gkg::Atom* >& atoms = neuron.getAtoms();

    // computing the composite transformation:
    //
    //                    AtomPrime.center =
    //                       T[targetNeuronCenter]
    //                       Rrandom x
    //                       S(scaling, scaling, scaling) x
    //                       T[-neuronModelCenter] x
    //                       Atom.center
    //
    // and in case if ellipsoid atoms:
    //
    //                    AtomPrime.axis1 =
    //                       Rrandom x
    //                       Atom.axis1
    //                    AtomPrime.axis2 =
    //                       Rrandom x
    //                       Atom.axis2
    //                    AtomPrime.axis2 =
    //                       Rrandom x
    //                       Atom.axis2


    gkg::Translation3d< float > translation3dFromNeuronModelCenter(
                                                           -neuronModelCenter );
    gkg::Scaling3d< float > scaling3d( scaling, scaling, scaling );
    gkg::Rotation3d< float > randomRotation3d( randomRotationParameters );
    gkg::Translation3d< float > translation3dToTargetNeuronCenter(
                                                                 targetCenter );

    gkg::CompositeTransform3d< float > compositeTransform3d;
    compositeTransform3d.compose( translation3dFromNeuronModelCenter );
    compositeTransform3d.compose( scaling3d );
    compositeTransform3d.compose( randomRotation3d );
    compositeTransform3d.compose( translation3dToTargetNeuronCenter );

    // at maximum we have the same number of output atoms (except those getting
    // out of the virtual tissue field of view
    _atoms.reserve( atoms.size() );

    gkg::Vector3d< float > center;
    float radius1 = 0.0f;
    float radius2 = 0.0f;
    float radius3 = 0.0f;
    gkg::Vector3d< float > axis1;
    gkg::Vector3d< float > axis2;
    gkg::Vector3d< float > axis3;

    gkg::Vector3d< float > translatedAtomCenter;
    gkg::Vector3d< float > rotatedAxis1;
    gkg::Vector3d< float > rotatedAxis2;
    gkg::Vector3d< float > rotatedAxis3;

    // looping over the neuron model's atom and applyhing them the 
    // transformation, then storing them into the target atom vector
    // (only if they belong to the field of view boundaries)
    std::vector< gkg::Atom* >::const_iterator
      ia = atoms.begin(),
      iae = atoms.end();
    while ( ia != iae )
    {

      if ( ( *ia )->getType() == 1U )
      {

        const gkg::SphereAtom* 
          sphereAtom = static_cast< const gkg::SphereAtom* >( *ia );

        compositeTransform3d.getComposition().getDirect(
                                                        sphereAtom->getCenter(),
                                                        translatedAtomCenter );
        if ( fieldOfView.contains( translatedAtomCenter ) )
        {

          _atoms.push_back( new gkg::SphereAtom(
                                          translatedAtomCenter,
                                          sphereAtom->getRadius() * scaling ) );

        }

      }
      else if ( ( *ia )->getType() == 2U )
      {

        const gkg::EllipsoidAtom* 
          ellipsoidAtom = static_cast< const gkg::EllipsoidAtom* >( *ia );

        ellipsoidAtom->getStdParameters( center, 
                                         radius1,
                                         radius2,
                                         radius3,
                                         axis1,
                                         axis2,
                                         axis3 );

        compositeTransform3d.getComposition().getDirect( center,
                                                         translatedAtomCenter );
        randomRotation3d.getDirect( axis1, rotatedAxis1 );
        randomRotation3d.getDirect( axis2, rotatedAxis2 );
        randomRotation3d.getDirect( axis3, rotatedAxis3 );

        if ( fieldOfView.contains( translatedAtomCenter ) )
        {

          _atoms.push_back( new gkg::EllipsoidAtom(
                                          translatedAtomCenter,
                                          radius1 * scaling,
                                          radius2 * scaling,
                                          radius3 * scaling,
                                          rotatedAxis1,
                                          rotatedAxis2,
                                          rotatedAxis3 ) );

        }

      }
      ++ ia;

    }

  }
  GKG_CATCH( "gkg::Neuron::Neuron( "
             "int32_t id, "
             "const NeuronPopulation* parent, "
             "const gkg::VirtualTissue& neuronModel, "
             "float neuronModelDiameter, "
             "const gkg::Vector3d< float >& neuronModelCenter, "
             "float neuronModelVolume, "
             "const gkg::Vector3d< float >& randomRotationParameters, "
             "float targetDiameter, "
             "const gkg::Vector3d< float >& targetCenter, "
             "bool verbose )" );


}


gkg::Neuron::Neuron( const gkg::Neuron& other )
            : gkg::Cell( other ),
              _diameter( other._diameter ),
              _center( other._center ),
              _volume( other._volume )
             
{
}


gkg::Neuron::~Neuron()
{
}


gkg::Neuron& gkg::Neuron::operator=( const gkg::Neuron& other )
{

  try
  {

    this->::gkg::Cell::operator=( other );

    _diameter = other._diameter;
    _center = other._center;
    _volume = other._volume;

    return *this;

  }
  GKG_CATCH( "gkg::Neuron& gkg::Neuron::operator=( "
             "const gkg::Neuron& other )" );


}


float gkg::Neuron::getDiameter() const
{

  try
  {

    return _diameter;

  }
  GKG_CATCH( "float gkg::Neuron::getDiameter() const" );

}


const gkg::Vector3d< float >& gkg::Neuron::getCenter() const
{

  try
  {

    return _center;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::Neuron::getCenter() const" );

}


float gkg::Neuron::getVolume() const
{

  try
  {

    return _volume;

  }
  GKG_CATCH( "float gkg::Neuron::getVolume() const" );

}


void gkg::Neuron::fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {

    ///////////////////// A REVOIR

    const gkg::SphereAtom* sphereAtom = static_cast< gkg::SphereAtom* >(
                                                                  _atoms[ 0 ] );

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      neuronMeshMap = gkg::MeshFactory::getInstance().getSphere(
                                                        sphereAtom->getCenter(),
                                                        sphereAtom->getRadius(),
                                                              60 );

    meshAccumulators[ 0 ].add( *neuronMeshMap );

  }
  GKG_CATCH( "void gkg::Neuron::fillMeshAccumulators( "
             "std::vector< MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


void gkg::Neuron::addDetails()
{

  try
  {

    // nothing to do

  }
  GKG_CATCH( "void gkg::Neuron::addDetails()" );

}


