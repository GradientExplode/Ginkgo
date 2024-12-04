#include <gkg-deep-nuclei-deepnuclei/LabelDependentTransform3dRelaxationFunction.h>
#include <gkg-processing-regiongrowing/RegionGrowingEnergy_i.h>
#include <gkg-core-exception/Exception.h>


template < class L >
inline
gkg::LabelDependentTransform3dRelaxationFunction< L >::
                                    LabelDependentTransform3dRelaxationFunction(
            gkg::RCPointer< gkg::ParameterizedTransform3d< float > >
                                                       parameterizedTransform3d,
            gkg::RCPointer< gkg::RegionGrowingEnergy< L > > regionGrowingEnergy,
            const std::string& contrastName,
            const L& label )
                                        : gkg::NelderMeadSimplexFunction( 9 ),
                                          _parameterizedTransform3d(
                                                     parameterizedTransform3d ),
                                          _regionGrowingEnergy(
                                                          regionGrowingEnergy ),
                                          _contrastName( contrastName ),
                                          _label( label )
{
}


template < class L >
inline
gkg::LabelDependentTransform3dRelaxationFunction< L >::
                                  ~LabelDependentTransform3dRelaxationFunction()
{
}


template < class L >
inline
double gkg::LabelDependentTransform3dRelaxationFunction< L >::getValueAt(
                                           const gkg::Vector& parameters ) const
{

  try
  {

    _parameterizedTransform3d->setDirectParameters( parameters );

    _regionGrowingEnergy->initializeForContrastAndPotentialAndLabel(
                                                _contrastName,
                                                "probabilistic_atlas_potential",
                                                _label );

    return _regionGrowingEnergy->
                                getWeightedValueForContrastAndPotentialAndLabel(
                                                _contrastName,
                                                "probabilistic_atlas_potential",
                                                _label );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::LabelDependentTransform3dRelaxationFunction< L >::"
             "getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


