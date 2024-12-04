#ifndef _gkg_processing_registration_NelderMeadSimilarityMeasureFunction_h_
#define _gkg_processing_registration_NelderMeadSimilarityMeasureFunction_h_


#include <gkg-processing-registration/SimilarityMeasure.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-transform/ParameterizedTransform3d.h>


namespace gkg
{


template < class T1, class T2 >
class NelderMeadSimilarityMeasureFunction : public NelderMeadSimplexFunction
{

  public:

    NelderMeadSimilarityMeasureFunction(
             const RCPointer< SimilarityMeasure< T1, T2 > >& similarityMeasure,
             const RCPointer< ParameterizedTransform3d< float > >&
                                                     parameterizedTransform3d,
             typename RegistrationAlgorithm< T1, T2 >::TunedParameters
               tunedParameters = gkg::RegistrationAlgorithm< T1, T2 >::Direct );
    virtual ~NelderMeadSimilarityMeasureFunction();
    
    double getValueAt( const Vector& parameters ) const;

  protected:

    RCPointer< SimilarityMeasure< T1, T2 > > _similarityMeasure;
    RCPointer< ParameterizedTransform3d< float > > _parameterizedTransform3d;
    typename RegistrationAlgorithm< T1, T2 >::TunedParameters _tunedParameters;

};


}


#endif
