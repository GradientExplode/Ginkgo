#ifndef _gkg_processing_registration_NelderMeadSimilarityMeasureFunction_i_h_
#define _gkg_processing_registration_NelderMeadSimilarityMeasureFunction_i_h_


#include <gkg-processing-registration/NelderMeadSimilarityMeasureFunction.h>
#include <gkg-processing-registration/SimilarityMeasure_i.h>



template < class T1, class T2 >
inline
gkg::NelderMeadSimilarityMeasureFunction< T1, T2 >::
                                            NelderMeadSimilarityMeasureFunction(
    const gkg::RCPointer< gkg::SimilarityMeasure< T1, T2 > >& similarityMeasure,
    const gkg::RCPointer< gkg::ParameterizedTransform3d< float > >&
                                                      parameterizedTransform3d,
    typename gkg::RegistrationAlgorithm< T1, T2 >::TunedParameters 
                                                               tunedParameters )
                                         : gkg::NelderMeadSimplexFunction(
                                                      parameterizedTransform3d->
                                                          getParameterCount() ),
                                           _similarityMeasure(
                                                            similarityMeasure ),
                                           _parameterizedTransform3d(
                                                     parameterizedTransform3d ),
                                           _tunedParameters( tunedParameters )
{
}


template < class T1, class T2 >
inline
gkg::NelderMeadSimilarityMeasureFunction< T1, T2 >::
                                          ~NelderMeadSimilarityMeasureFunction()
{
}


template < class T1, class T2 >
inline
double gkg::NelderMeadSimilarityMeasureFunction< T1, T2 >::getValueAt(
                                           const gkg::Vector& parameters ) const
{

  try
  {


    if ( this->_tunedParameters ==
                                  gkg::RegistrationAlgorithm< T1, T2 >::Direct )
    {

      _parameterizedTransform3d->setDirectParameters( parameters );

    }
    else
    {

      _parameterizedTransform3d->setInverseParameters( parameters );

    }

    return _similarityMeasure->getValueAt( *_parameterizedTransform3d );

  }
  GKG_CATCH( "double gkg::NelderMeadSimilarityMeasureFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}




#endif
