#ifndef _gkg_processing_registration_NormalizedMutualInformationSimilarityMeasure_h_
#define _gkg_processing_registration_NormalizedMutualInformationSimilarityMeasure_h_


#include <gkg-processing-registration/SimilarityMeasure.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <vector>


namespace gkg
{


template < class T > class DericheGaussianCoefficients;
class NumericalAnalysisImplementationFactory;


template < class T1, class T2 >
class NormalizedMutualInformationSimilarityMeasure :
                                              public SimilarityMeasure< T1, T2 >
{

  public:

    NormalizedMutualInformationSimilarityMeasure(
                     RCPointer< Volume< T1 > > reference,
                     RCPointer< Volume< T2 > > floating,
                     const T1& referenceLowerThreshold,
                     const T2& floatingLowerThreshold,
                     int32_t resamplingOrder,
                     int32_t subSamplingMaximumSize,
                     const std::vector< double >& scalarParameters,
                     bool verbose );
    virtual ~NormalizedMutualInformationSimilarityMeasure();

    int32_t getLevelCount() const;
    bool isSmoothingApplied() const;

    double getValueAt( const Transform3d< float >& transform3d ) const;

    static void checkScalarParameters( 
                                      std::vector< double >& scalarParameters );

  protected:

    NumericalAnalysisImplementationFactory* _factory;

    int32_t _levelCount;
    bool _isSmoothingApplied;
    mutable Matrix _joinProbabilities;
    mutable Vector _referenceMarginalProbabilities;
    mutable Vector _floatingMarginalProbabilities;
    DericheGaussianCoefficients< double >*
                                          _dericheGaussianSmoothingCoefficients;
    mutable Matrix* _smoothedJoinProbabilities;

};


}


#endif
