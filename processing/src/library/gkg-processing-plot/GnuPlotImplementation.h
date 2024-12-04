#ifndef _gkg_processing_plot_GnuPlotImplementation_h_
#define _gkg_processing_plot_GnuPlotImplementation_h_


#include <gkg-processing-plot/CurvePlotterImplementation.h>
#include <gkg-core-pattern/Creator.h>
#include <cstdio>


namespace gkg
{


class GnuPlotImplementation : public CurvePlotterImplementation,
                              public CreatorNoArg< GnuPlotImplementation,
                                                   CurvePlotterImplementation >
{

  public:

    virtual ~GnuPlotImplementation();

    std::string getName() const;

    void begin( const std::string& );
    void end();

    double getWidth() const;
    double getHeight() const;
    double getCharacterWidth() const;
    double getCharacterHeight() const;
    double getTicWidth() const;
    double getTicHeight() const;

    void move( double x, double y );
    void drawVector( double x, double y );
    void drawText( double x, double y, const std::string& text );

    void setLineType( LineType lineType );

    void setLeftJustify();
    void setCenterJustify();
    void setRightJustify();

    static std::string getStaticName();

  private:

    friend class CreatorNoArg< GnuPlotImplementation,
                               CurvePlotterImplementation >;

    GnuPlotImplementation();

    FILE* _x11Ipc;

};


}


#endif
