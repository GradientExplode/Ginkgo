#ifndef _gkg_processing_registration_SimilarityMeasureFactory_h_
#define _gkg_processing_registration_SimilarityMeasureFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-registration/SimilarityMeasure.h>
#include <vector>
#include <set>
#include <string>


namespace gkg
{


template < class T1, class T2 >
class SimilarityMeasureFactory : public 
                                 Singleton< SimilarityMeasureFactory< T1, T2 > >
{

  public:

    ~SimilarityMeasureFactory();

    const std::set< std::string >& getSimilarityMeasureNames() const;

    RCPointer< SimilarityMeasure< T1, T2 > >
      create( const std::string& similarityMeasureName,
              RCPointer< Volume< T1 > > reference,
              RCPointer< Volume< T2 > > floating,
              const T1& referenceLowerThreshold,
              const T2& floatingLowerThreshold,
              int32_t resamplingOrder,
              int32_t subSamplingMaximumSize,
              const std::vector< double >& scalarParameters,
              bool verbose ) const;


    void checkSimilarityMeasureName(
                               const std::string& similarityMeasureName ) const;
    void checkScalarParameters( const std::string& similarityMeasureName,
                                std::vector< double >& scalarParameters ) const;

  protected:

    friend class Singleton< SimilarityMeasureFactory< T1, T2 > >;

    SimilarityMeasureFactory();

    std::set< std::string > _similarityMeasureNames;

};


}


#endif
