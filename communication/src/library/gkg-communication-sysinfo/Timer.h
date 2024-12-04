#ifndef _gkg_communication_sysinfo_Timer_h_
#define _gkg_communication_sysinfo_Timer_h_


#include <ctime>
#include <list>


namespace gkg
{

class Timer
{

  public:

    Timer();
    virtual ~Timer();

    void storeTime();


    std::list< double > getTimesInMilliseconds() const;
    std::list< double > getTimesInSeconds() const;


    std::list< double > getTimeLapsInMilliseconds() const;
    std::list< double > getTimeLapsInSeconds() const;

    void printTimesInMilliseconds() const;
    void printTimesInSeconds() const;
    void printLapsInMilliseconds() const;
    void printLapsInSeconds() const;

  private:

    double getClockDifferenceInMilliseconds( clock_t clock1, 
                                             clock_t clock2 ) const;
    double getClockDifferenceInSeconds( clock_t clock1, 
                                        clock_t clock2 ) const;

    std::list< clock_t > _times;

};


}


#endif
