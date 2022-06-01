#include "basisfunctions.hpp"
#include "curve.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <string>

namespace cie
{
namespace splinekernel
{

std::array<std::vector<double>, 2> evaluate2DCurve( const std::vector<double>& tCoordinates, 
                                                    const std::vector<double>& xCoordinates,
                                                    const std::vector<double>& yCoordinates, 
                                                    const std::vector<double>& knotVector )
{
    size_t numberOfSamples = tCoordinates.size( );
    size_t numberOfPoints = xCoordinates.size( );

    size_t m = knotVector.size( );
    size_t p = m - numberOfPoints - 1;

    std::vector<double> curveX( numberOfSamples, 0.0 );
    std::vector<double> curveY( numberOfSamples, 0.0 );

    for( size_t iSample = 0; iSample < numberOfSamples; ++iSample )
    {
        for( size_t j = 0; j < numberOfPoints; ++j )
        {
            double N = evaluateBSplineBasis( tCoordinates[iSample], j, p, knotVector );

            curveX[iSample] += N * xCoordinates[j];
            curveY[iSample] += N * yCoordinates[j];
        }
    }

    return { curveX, curveY };
}

} // namespace splinekernel
} // namespace cie
