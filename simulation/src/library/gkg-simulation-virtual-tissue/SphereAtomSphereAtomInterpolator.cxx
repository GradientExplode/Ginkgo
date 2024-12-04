#include <gkg-simulation-virtual-tissue/AtomInterpolatorFactory.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-core-exception/Exception.h>



static void sphereAtomSphereAtomInterpolator(
                                             const gkg::Atom& atom1,
                                             const gkg::Atom& atom2,
                                             float oversamplingRatio,
                                             std::list< gkg::Atom* >& newAtoms )
{

  try
  {

    const float& radius1 = atom1.parameters[ 3 ];
    const float& radius2 = atom2.parameters[ 3 ];

    float minimumRadius = std::min( radius1, radius2 );

    float deltaLength = minimumRadius / oversamplingRatio;
    gkg::Vector3d< float > 
      unitVector( atom2.parameters[ 0 ] - atom1.parameters[ 0 ],
                  atom2.parameters[ 1 ] - atom1.parameters[ 1 ],
                  atom2.parameters[ 2 ] - atom1.parameters[ 2 ] );
    float distance = unitVector.getNorm();
    unitVector.normalize();

    gkg::Vector3d< float > displacement = unitVector * deltaLength;

    float radiusSlope = ( radius2 - radius1 ) / distance;

    gkg::Vector3d< float > currentCenter( atom1.parameters[ 0 ],
                                          atom1.parameters[ 1 ],
                                          atom1.parameters[ 2 ] );
    float currentRadius = radius1;
    float currentLength = deltaLength;
    while ( currentLength < distance )
    {
        
      currentCenter += displacement;
      currentRadius += deltaLength * radiusSlope;
      newAtoms.push_back( new gkg::SphereAtom( currentCenter, currentRadius ) );
          
      currentLength += deltaLength;
                
    }

  }
  GKG_CATCH( "static void sphereAtomSphereAtomInterpolator( "
             "const gkg::Atom& atom1, "
             "const gkg::Atom& atom2, "
             "float oversamplingRatio, "
             "std::list< gkg::Atom* >& newAtoms )" );


}


//
// adding static method to atom interpolator factory
//


static bool registerToAtomInterpolatorFactory __attribute__((unused)) = 
  gkg::AtomInterpolatorFactory::getInstance().registerAtomInterpolator(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getStaticType(),
    sphereAtomSphereAtomInterpolator );
