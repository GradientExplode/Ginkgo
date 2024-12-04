#ifndef _gkg_processing_machinelearning_MachineLearningData_h_
#define _gkg_processing_machinelearning_MachineLearningData_h_


#include <gkg-processing-container/Volume.h>

#include <vector>


namespace gkg
{


class MachineLearningDataImplementation;
class SparseMatrix;
class Matrix;
class MachineLearningScaler;
template < class T > class Volume;


class MachineLearningData
{

  public:

    MachineLearningData( SparseMatrix& data,
                         MachineLearningScaler& scaler );
    MachineLearningData( Matrix& data,
                         MachineLearningScaler& scaler );
    template < class T, class M >
    MachineLearningData( const Volume< T >& volume,
                         const Volume< M >& mask,
                         const std::vector< uint8_t >& selectedRank,
                         MachineLearningScaler& scaler );
    template < class T >
    MachineLearningData( const Volume< T >& volume,
                         const std::vector< Vector3d< int32_t > >& sites,
                         const std::vector< int16_t >& selectedRank,
                         MachineLearningScaler& scaler );
    virtual ~MachineLearningData();

    int32_t getMeasureCount() const;
    int32_t getMaximumIndex() const;

    MachineLearningDataImplementation* getImplementation() const;

  protected:

    MachineLearningDataImplementation* _machineLearningDataImplementation;

};


}


#endif
