#ifndef _gkg_communication_algorithm_Algorithm_h_
#define _gkg_communication_algorithm_Algorithm_h_


#include <gkg-communication-algorithm/AlgorithmParameter.h>
#include <string>
#include <vector>


namespace gkg
{


class AlgorithmParameter;


class Algorithm
{

  public:

    virtual ~Algorithm();

    const std::string& getName() const;

    virtual bool check() = 0;
    virtual void run() = 0;

    AlgorithmParameter* getParameter( const std::string& name ) const;


  protected:

    Algorithm( const std::string& name );

    void addParameter( AlgorithmParameter* algorithmParameter );

    std::string _name;
    std::vector< AlgorithmParameter* > _parameters;

};




}


#endif
