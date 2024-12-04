#ifndef _gkg_processing_statistics_MannWhitneyWilcoxonUTest_h_
#define _gkg_processing_statistics_MannWhitneyWilcoxonUTest_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>
#include <list>


namespace gkg
{

// we reject the null hypothesis when p-value < significance level=0.05
// Requirements:
// 1) the two samples x and y have to be independent
// 2) x and y distributions are continuous or discrete with continuous approx.
// 3) null hypothesis: two populations have the same shape; the only difference
//                     can be the value of their median
// 4) the number of elements should be greater or equal to 5


class MannWhitneyWilcoxonUTest
{

  public:

    struct Result
    {

      double twoTailedTestPValue;    // null hypothesis + same median values
      double leftTailedTestPValue;   // null hypothesis + the median of the
                                     // first sample is greater than or equal to
                                     // the median of the second sample
      double rightTailedTestPValue;  // null hypothesis + the median of the
                                     // first sample is less than or equal to
                                     // the median of the second sample

    };

    MannWhitneyWilcoxonUTest();
    virtual ~MannWhitneyWilcoxonUTest();

    template < class T >
    Result test( const std::vector< T >& x,
                 const std::vector< T >& y ) const;
    template < class T >
    Result test( const std::list< T >& x,
                 const std::list< T >& y ) const;
    Result test( const Vector& x,
                 const Vector& y ) const;

  protected:

    static double usigma( double s, int32_t n1, int32_t n2 );
    static void ucheb( double x, double c, double& tj, double& tj1, double& r );
    static double uninterpolate( double p1, double p2, double p3, int32_t n );

    static double usigma000( int32_t n1, int32_t n2 );
    static double usigma075( int32_t n1, int32_t n2 );
    static double usigma150( int32_t n1, int32_t n2 );
    static double usigma225( int32_t n1, int32_t n2 );
    static double usigma300( int32_t n1, int32_t n2 );
    static double usigma333( int32_t n1, int32_t n2 );
    static double usigma367( int32_t n1, int32_t n2 );
    static double usigma400( int32_t n1, int32_t n2 );

    static double utbln5n5( double s );
    static double utbln5n6( double s );
    static double utbln5n7( double s );
    static double utbln5n8( double s );
    static double utbln5n9( double s );
    static double utbln5n10( double s );
    static double utbln5n11( double s );
    static double utbln5n12( double s );
    static double utbln5n13( double s );
    static double utbln5n14( double s );
    static double utbln5n15( double s );
    static double utbln5n16( double s );
    static double utbln5n17( double s );
    static double utbln5n18( double s );
    static double utbln5n19( double s );
    static double utbln5n20( double s );
    static double utbln5n21( double s );
    static double utbln5n22( double s );
    static double utbln5n23( double s );
    static double utbln5n24( double s );
    static double utbln5n25( double s );
    static double utbln5n26( double s );
    static double utbln5n27( double s );
    static double utbln5n28( double s );
    static double utbln5n29( double s );
    static double utbln5n30( double s );
    static double utbln5n100( double s );
    static double utbln6n6( double s );
    static double utbln6n7( double s );
    static double utbln6n8( double s );
    static double utbln6n9( double s );
    static double utbln6n10( double s );
    static double utbln6n11( double s );
    static double utbln6n12( double s );
    static double utbln6n13( double s );
    static double utbln6n14( double s );
    static double utbln6n15( double s );
    static double utbln6n30( double s );
    static double utbln6n100( double s );
    static double utbln7n7( double s );
    static double utbln7n8( double s );
    static double utbln7n9( double s );
    static double utbln7n10( double s );
    static double utbln7n11( double s );
    static double utbln7n12( double s );
    static double utbln7n13( double s );
    static double utbln7n14( double s );
    static double utbln7n15( double s );
    static double utbln7n30( double s );
    static double utbln7n100( double s );
    static double utbln8n8( double s );
    static double utbln8n9( double s );
    static double utbln8n10( double s );
    static double utbln8n11( double s );
    static double utbln8n12( double s );
    static double utbln8n13( double s );
    static double utbln8n14( double s );
    static double utbln8n15( double s );
    static double utbln8n30( double s );
    static double utbln8n100( double s );
    static double utbln9n9( double s );
    static double utbln9n10( double s );
    static double utbln9n11( double s );
    static double utbln9n12( double s );
    static double utbln9n13( double s );
    static double utbln9n14( double s );
    static double utbln9n15( double s );
    static double utbln9n30( double s );
    static double utbln9n100( double s );
    static double utbln10n10( double s );
    static double utbln10n11( double s );
    static double utbln10n12( double s );
    static double utbln10n13( double s );
    static double utbln10n14( double s );
    static double utbln10n15( double s );
    static double utbln10n30( double s );
    static double utbln10n100( double s );
    static double utbln11n11( double s );
    static double utbln11n12( double s );
    static double utbln11n13( double s );
    static double utbln11n14( double s );
    static double utbln11n15( double s );
    static double utbln11n30( double s );
    static double utbln11n100( double s );
    static double utbln12n12( double s );
    static double utbln12n13( double s );
    static double utbln12n14( double s );
    static double utbln12n15( double s );
    static double utbln12n30( double s );
    static double utbln12n100( double s );
    static double utbln13n13( double s );
    static double utbln13n14( double s );
    static double utbln13n15( double s );              
    static double utbln13n30( double s );
    static double utbln13n100( double s );
    static double utbln14n14( double s );
    static double utbln14n15( double s );
    static double utbln14n30( double s );
    static double utbln14n100( double s );

};


}


#endif
