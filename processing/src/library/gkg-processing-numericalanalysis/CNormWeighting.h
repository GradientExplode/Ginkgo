#ifndef _gkg_processing_numericalanalysis_CNormWeighting_h_
#define _gkg_processing_numericalanalysis_CNormWeighting_h_


namespace gkg
{


class CMatrix;


class CNormWeighting
{

  public:

    virtual ~CNormWeighting();

    virtual const CMatrix& getWeighting() const = 0;

  protected:

    CNormWeighting();

};


}


#endif
