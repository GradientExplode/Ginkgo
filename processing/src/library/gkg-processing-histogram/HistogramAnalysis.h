#ifndef _gkg_processing_histogram_HistogramAnalysis_h_
#define _gkg_processing_histogram_HistogramAnalysis_h_

#include <gkg-core-object/HeaderedObject.h>
#include <utility>

namespace gkg
{


template < class T > class Volume;


class HistogramAnalysis : public HeaderedObject
{

  public:
  
    struct Statistics
    {

      Statistics( double theMean = 0.0, double theStandardDeviation = 0.0 );
      Statistics( const Statistics& other );

      Statistics& operator=( const Statistics& other );

      double mean;
      double standardDeviation;

    };
  
    HistogramAnalysis();
    HistogramAnalysis( const Statistics& whiteMatter,
                       const Statistics& grayMatter,
		       const Statistics& cerebroSpinalFluid );
    template < class T >
    HistogramAnalysis( const Volume< T >& volume );
    virtual ~HistogramAnalysis();
    
    HistogramAnalysis& operator=( const HistogramAnalysis& other );
   
    const Statistics& getWhiteMatterStatistics() const;
    const Statistics& getGrayMatterStatistics() const;
    const Statistics& getCerebroSpinalFluidStatistics() const;

    void addToHeaderedObject( HeaderedObject& other ) const;
    
    static void addSemantic( HeaderedObject& other );

  protected:

    friend class HistogramAnalysisDiskFormat;
    
    Statistics _whiteMatterStatistics;
    Statistics _grayMatterStatistics;
    Statistics _cerebroSpinalFluidStatistics;
    
};

}

#endif
