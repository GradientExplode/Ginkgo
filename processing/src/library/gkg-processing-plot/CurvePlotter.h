#ifndef _gkg_processing_plot_CurvePlotter_h_
#define _gkg_processing_plot_CurvePlotter_h_


#include <list>
#include <vector>
#include <string>
#include <complex>


namespace gkg
{


class CurvePlotterImplementation;


class CurvePlotter
{

  public:

    CurvePlotter();
    virtual ~CurvePlotter();

    bool plot( const std::vector< double >& x,
               const std::list< std::vector< double > >& yList,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;
    bool plot( const std::list< std::vector< double > >& yList,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;

    bool plot( const std::vector< double >& x,
               const std::vector< double >& y,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;
    bool plot( const std::vector< double >& y,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;


    bool plot( const std::vector< double >& x,
               const std::list< std::vector< std::complex< double > > >& yList,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;
    bool plot( const std::list< std::vector< std::complex< double > > >& yList,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;

    bool plot( const std::vector< double >& x,
               const std::vector< std::complex< double > >& y,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;
    bool plot( const std::vector< std::complex< double > >& y,
               const std::string& xTitle,
               const std::string& yTitle,
               const std::string& title = "" ) const;

/*
    bool plot( const std::string& fileName,
               const std::string& xTitle,
               const std::string& yTitle );
*/
  protected:

    void plotAxis( double xMinimum, double xMaximum,
                   double yMinimum, double yMaximum,
                   const std::string& xTitle,
                   const std::string& yTitle,
                   const std::string& title ) const;

    CurvePlotterImplementation* _cpi;

    double _originX;
    double _originY;
    double _width;
    double _height;
    double _arrowWidth;
    double _arrowHeight;

};


}


#endif
