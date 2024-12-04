#include <gkg-deep-nuclei-deepnuclei/LabelIndependentTransform3dRelaxationFunction.h>
#include <gkg-processing-regiongrowing/RegionGrowingEnergy_i.h>
#include <gkg-core-exception/Exception.h>


template < class L >
inline
gkg::LabelIndependentTransform3dRelaxationFunction< L >::
                                  LabelIndependentTransform3dRelaxationFunction(
            gkg::RCPointer< gkg::ParameterizedTransform3d< float > >
                                                       parameterizedTransform3d,
            gkg::RCPointer< gkg::RegionGrowingEnergy< L > > regionGrowingEnergy,
            const std::string& contrastName )
                                        : gkg::NelderMeadSimplexFunction( 9 ),
                                          _parameterizedTransform3d(
                                                     parameterizedTransform3d ),
                                          _regionGrowingEnergy(
                                                          regionGrowingEnergy ),
                                          _contrastName( contrastName )
{
}


template < class L >
inline
gkg::LabelIndependentTransform3dRelaxationFunction< L >::
                                ~LabelIndependentTransform3dRelaxationFunction()
{
}


template < class L >
inline
double gkg::LabelIndependentTransform3dRelaxationFunction< L >::getValueAt(
                                           const gkg::Vector& parameters ) const
{

  try
  {

    _parameterizedTransform3d->setDirectParameters( parameters );

    _regionGrowingEnergy->initializeForContrastAndPotential(
                                              _contrastName,
                                              "probabilistic_atlas_potential" );

    return _regionGrowingEnergy->getWeightedValueForContrastAndPotential(
                                              _contrastName,
                                              "probabilistic_atlas_potential" );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::LabelIndependentTransform3dRelaxationFunction< L >::"
             "getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


