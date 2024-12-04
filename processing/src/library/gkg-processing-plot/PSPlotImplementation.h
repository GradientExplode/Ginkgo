#ifndef _gkg_processing_plot_PSPlotImplementation_h_
#define _gkg_processing_plot_PSPlotImplementation_h_


#include <gkg-processing-plot/CurvePlotterImplementation.h>
#include <gkg-core-pattern/Creator.h>
#include <fstream>


namespace gkg
{


class PSPlotImplementation : public CurvePlotterImplementation,
                             public CreatorNoArg< PSPlotImplementation,
                                                  CurvePlotterImplementation >
{

  public:

    virtual ~PSPlotImplementation();

    std::string getName() const;

    void begin( const std::string& name );

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

    friend class CreatorNoArg< PSPlotImplementation,
                               CurvePlotterImplementation >;

    PSPlotImplementation();


    std::string getNameWoSpace( const std::string& name ) const;

    std::ofstream _os;
    double _x, _y;
    double _lineWidth;
    double _red, _green, _blue;

};


}


#endif
