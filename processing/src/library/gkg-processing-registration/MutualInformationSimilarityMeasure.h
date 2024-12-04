#ifndef _gkg_processing_registration_MutualInformationSimilarityMeasure_h_
#define _gkg_processing_registration_MutualInformationSimilarityMeasure_h_


#include <gkg-processing-registration/SimilarityMeasure.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>


namespace gkg
{


template < class T > class DericheGaussianCoefficients;


template < class T1, class T2 >
class MutualInformationSimilarityMeasure : public SimilarityMeasure< T1, T2 >
{

  public:

    MutualInformationSimilarityMeasure(
                     RCPointer< Volume< T1 > > reference,
                     RCPointer< Volume< T2 > > floating,
                     const T1& referenceLowerThreshold,
                     const T2& floatingLowerThreshold,
                     int32_t resamplingOrder,
                     int32_t subSamplingMaximumSize,
                     const std::vector< double >& scalarParameters,
                     bool verbose );
    virtual ~MutualInformationSimilarityMeasure();

    int32_t getLevelCount() const;
    bool isSmoothingApplied() const;

    double getValueAt( const Transform3d< float >& transform3d ) const;

    static void checkScalarParameters( 
                                      std::vector< double >& scalarParameters );

  protected:

    int32_t _levelCount;
    bool _isSmoothingApplied;
    mutable Matrix _joinProbabilities;
    mutable Vector _referenceMarginalProbabilities;
    mutable Vector _floatingMarginalProbabilities;
    DericheGaussianCoefficients< double >* _dericheGaussianCoefficients;
    mutable Matrix* _smoothedJoinProbabilities;

};


}


#endif
