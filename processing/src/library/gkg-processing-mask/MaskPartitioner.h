#ifndef _gkg_processing_mask_MaskPartitioner_h_
#define _gkg_processing_mask_MaskPartitioner_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <list>
#include <string>


namespace gkg
{
  


template < class T > class Volume;

template < class IN, class OUT >
class MaskPartitioner
{

  public:

    struct PartitionStatistics
    {

      PartitionStatistics( float theImbalance, float theCost )
      {

         imbalance = theImbalance;
         cost = theCost;

      }
      float imbalance;
      int32_t cost;

    };

    MaskPartitioner(
                  const std::list< Vector3d< int32_t > >& neighborhoodOffsets,
                  const Volume< IN >& mask );
    virtual ~MaskPartitioner();

    std::string getName() const;

    PartitionStatistics 
      computePartition( Volume< OUT >& outputVolume,
                        float& loadImbalanceTolerance,
                        int32_t& partCount,
                        int32_t& runCount,
                        bool& totalCommunicationVolumeMinimization = false,
                        int32_t& startingRandomSeed = 0 ) const;

    static std::string getStaticName();

  private:

    Volume< IN > _mask;
    Vector3d< int32_t > _maskSize;
    Vector3d< double > _maskResolution;

    int32_t _vertexCount;

    // forced to be mutable since the METIS library does not specificy the
    // input variables to be const!
    mutable std::vector< int32_t > _adjTab;
    mutable std::vector< int32_t > _adjncyTab;

};


}


#endif
