#ifndef _gkg_processing_plugin_svm_LibsvmMachineLearningDataImplementation_h_
#define _gkg_processing_plugin_svm_LibsvmMachineLearningDataImplementation_h_


#include <gkg-processing-machinelearning/MachineLearningDataImplementation.h>

#include <libsvm/svm.h>


namespace gkg
{


class LibsvmMachineLearningDataImplementation : 
                                        public MachineLearningDataImplementation
{

  public:

    LibsvmMachineLearningDataImplementation(
                                       MachineLearningData* machineLearningData,
                                       SparseMatrix& data );
    LibsvmMachineLearningDataImplementation(
                                       MachineLearningData* machineLearningData,
                                       Matrix& data );
    virtual ~LibsvmMachineLearningDataImplementation();

    int32_t getMeasureCount() const;
    int32_t getMaximumIndex() const;

    svm_node** getLibsvmData() const;

    static LibsvmMachineLearningDataImplementation* getImplementation( 
                                     MachineLearningData* machineLearningData );

  private:

    int32_t _measureCount;
    int32_t _maximumIndex;
    svm_node** _libsvmData;

};


}


#endif
