#ifndef _gkg_processing_numericalanalysis_SVDInverser_h_
#define _gkg_processing_numericalanalysis_SVDInverser_h_


#include <gkg-processing-numericalanalysis/IllMatrixInverser.h>


namespace gkg
{


class FilterFactor;


class SVDInverser : public IllMatrixInverser
{

  public:

    SVDInverser( const FilterFactor& filterFactor );
    virtual ~SVDInverser();

    const FilterFactor& getFilterFactor() const;

    virtual void inverse( const CMatrix& A,
                          const CVector& b,
                          CVector& x ) const;

  protected:

    const FilterFactor& _filterFactor;

};


}


#endif
