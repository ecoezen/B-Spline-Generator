#include "basisfunctions.hpp"

#include <string>
#include <cmath>
#include <stdexcept>

namespace cie
{
namespace splinekernel
{

double evaluateBSplineBasis( double t, 
                             size_t i, 
                             size_t p, 
                             const std::vector<double>& knotVector )
{
    double tolerance = 1e-12;

    if( p == 0 )
    {
        return ( t >= knotVector[i] && t < knotVector[i + 1] ) ||
               ( std::abs( t - knotVector[i + 1] ) < tolerance && 
                 std::abs( t - knotVector.back( ) ) < tolerance );
    }
    else
    {
        double a1 = t - knotVector[i];
        double b1 = knotVector[i + p] - knotVector[i];

        double result = 0.0;

        if( std::abs( b1 ) > tolerance )
        {
            result += a1 / b1 * evaluateBSplineBasis( t, i, p - 1, knotVector );
        }

        double a2 = knotVector[i + p + 1] - t;
        double b2 = knotVector[i + p + 1] - knotVector[i + 1];

        if( std::abs( b2 ) > tolerance )
        {
            result += a2 / b2 * evaluateBSplineBasis( t, i + 1, p - 1, knotVector );
        }

        return result;
    }
}

} // splinekernel
} // cie
