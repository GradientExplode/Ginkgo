#ifndef _gkg_processing_plot_CurvePlotterImplementation_h_
#define _gkg_processing_plot_CurvePlotterImplementation_h_


#include <vector>
#include <string>


namespace gkg
{


class CurvePlotterImplementation
{

  public:

    enum TickType
    {

      PointTick = 0,
      DiamondTick,
      PlusTick,
      BoxTick,
      CrossTick,
      TriangleTick,
      StarTick

    };

    enum LineType
    {

      BorderLine = 0,
      AxisLine,
      RunLine1,
      RunLine2,
      RunLine3,
      RunLine4,
      RunLine5,
      RunLine6,
      RunLine7

    };

    enum JustifyType
    {

      LeftJustify = 0,
      CenterJustify,
      RightJustify

    };

    virtual ~CurvePlotterImplementation();

    virtual std::string getName() const = 0;

    virtual void begin( const std::string& name = "" ) = 0;
    virtual void end() = 0;

    virtual double getWidth() const = 0;
    virtual double getHeight() const = 0;
    virtual double getCharacterWidth() const = 0;
    virtual double getCharacterHeight() const = 0;
    virtual double getTicWidth() const = 0;
    virtual double getTicHeight() const = 0;

    virtual void move( double x, double y ) = 0;
    virtual void drawVector( double x, double y ) = 0;
    virtual void drawText( double x, double y, const std::string& text ) = 0;


    void drawPoint( double x, double y, TickType tickType );
    void drawArrow( double startX, double startY,
                    double endX, double endY,
                    bool hasHead );
    void drawPoly( const std::vector< double >& x,
                   const std::vector< double >& y );

    virtual void setLineType( LineType lineType ) = 0;

    virtual void setLeftJustify() = 0;
    virtual void setCenterJustify() = 0;
    virtual void setRightJustify() = 0;

  protected:

    CurvePlotterImplementation();

    TickType _tickType;
    LineType _lineType;
    JustifyType _justifyType;

};


}


#endif
