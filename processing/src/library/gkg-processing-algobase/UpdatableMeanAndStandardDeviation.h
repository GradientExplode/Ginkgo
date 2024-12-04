#ifndef _gkg_processing_algobase_UpdatableMeanAndStandardDeviation_h_
#define _gkg_processing_algobase_UpdatableMeanAndStandardDeviation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{

class UpdatableMeanAndStandardDeviation
{

  public:
  
    UpdatableMeanAndStandardDeviation();
    UpdatableMeanAndStandardDeviation(
                               const UpdatableMeanAndStandardDeviation& other );
    virtual ~UpdatableMeanAndStandardDeviation();

    UpdatableMeanAndStandardDeviation& operator=( 
                               const UpdatableMeanAndStandardDeviation& other ); 

    void add( double value );

    const double& getMean() const;
    const double& getStandardDeviation() const;

    const double& getSum() const;
    const double& getSumOfSquare() const;
    const int32_t& getCount() const;

  private:
  
    double _mean;
    double _standardDeviation;
  
    double _sum;
    double _sumOfSquare;
    int32_t _count;
    
};

}
  
#endif
