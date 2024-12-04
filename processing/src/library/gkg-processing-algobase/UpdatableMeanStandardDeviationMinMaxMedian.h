#ifndef _gkg_processing_algobase_UpdatableMeanStandardDeviationMinMaxMedian_h_
#define _gkg_processing_algobase_UpdatableMeanStandardDeviationMinMaxMedian_h_


#include <gkg-core-cppext/StdInt.h>
#include <list>


namespace gkg
{

class UpdatableMeanStandardDeviationMinMaxMedian
{

  public:
  
    UpdatableMeanStandardDeviationMinMaxMedian();
    UpdatableMeanStandardDeviationMinMaxMedian(
                      const UpdatableMeanStandardDeviationMinMaxMedian& other );
    virtual ~UpdatableMeanStandardDeviationMinMaxMedian();

    
    UpdatableMeanStandardDeviationMinMaxMedian& operator=( 
                      const UpdatableMeanStandardDeviationMinMaxMedian& other ); 

    void add( double value );
   
    const double& getMean() const;
    const double& getStandardDeviation() const;
    const double& getMinimum() const;
    const double& getMaximum() const;
    const double& getMedian() const;

    const double& getSum() const;
    const double& getSumOfSquare() const;
    const int32_t& getCount() const;

    const std::list< double >& getValues() const;

  private:
  
    double _mean;
    double _standardDeviation;
    double _minimum;
    double _maximum;
    std::list< double > _values;

    double _sum;
    double _sumOfSquare;
    int32_t _count;
    
};

}
  
#endif
