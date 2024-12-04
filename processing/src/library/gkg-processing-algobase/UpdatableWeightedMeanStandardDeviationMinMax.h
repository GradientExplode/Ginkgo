#ifndef _gkg_processing_algobase_UpdatableWeightedMeanStandardDeviationMinMax_h_
#define _gkg_processing_algobase_UpdatableWeightedMeanStandardDeviationMinMax_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{

class UpdatableWeightedMeanStandardDeviationMinMax
{

  public:
  
    UpdatableWeightedMeanStandardDeviationMinMax();
    UpdatableWeightedMeanStandardDeviationMinMax(
                    const UpdatableWeightedMeanStandardDeviationMinMax& other );
    virtual ~UpdatableWeightedMeanStandardDeviationMinMax();

    
    UpdatableWeightedMeanStandardDeviationMinMax& operator=( 
                    const UpdatableWeightedMeanStandardDeviationMinMax& other ); 

    void add( double value, double weight );

    const double& getMean() const;
    const double& getStandardDeviation() const;
    const double& getMinimum() const;
    const double& getMaximum() const;

    const double& getValueSum() const;
    const double& getValueSumOfSquare() const;
    const double& getWeightSum() const;
    const double& getWeightSumOfSquare() const;
    const int32_t& getCount() const;

  private:
  
    double _mean;
    double _standardDeviation;
    double _minimum;
    double _maximum;

    double _valueSum;
    double _weightSum;

    double _valueSumOfSquare;
    double _weightSumOfSquare;

    int32_t _count;
    
};

}
  
#endif
