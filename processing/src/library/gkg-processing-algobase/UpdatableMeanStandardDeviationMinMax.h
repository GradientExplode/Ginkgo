#ifndef _gkg_processing_algobase_UpdatableMeanStandardDeviationMinMax_h_
#define _gkg_processing_algobase_UpdatableMeanStandardDeviationMinMax_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{

class UpdatableMeanStandardDeviationMinMax
{

  public:
  
    UpdatableMeanStandardDeviationMinMax();
    UpdatableMeanStandardDeviationMinMax(
                            const UpdatableMeanStandardDeviationMinMax& other );
    virtual ~UpdatableMeanStandardDeviationMinMax();

    
    UpdatableMeanStandardDeviationMinMax& operator=( 
                            const UpdatableMeanStandardDeviationMinMax& other ); 

    void add( double value );

    const double& getMean() const;
    const double& getStandardDeviation() const;
    const double& getMinimum() const;
    const double& getMaximum() const;

    const double& getSum() const;
    const double& getSumOfSquare() const;
    const int32_t& getCount() const;

  private:
  
    double _mean;
    double _standardDeviation;
    double _minimum;
    double _maximum;

    double _sum;
    double _sumOfSquare;
    int32_t _count;
    
};

}
  
#endif
