#pragma once

#include <array>
#include <vector>

namespace cie
{
namespace splinekernel
{

using ControlPoints2D = std::array<std::vector<double>, 2>;
using ControlPointsAndKnotVector = std::pair<ControlPoints2D, std::vector<double>>;

//! Returns the control points for a b-spline curve with given degree that interpolates the given points.
ControlPointsAndKnotVector interpolateWithBSplineCurve( const ControlPoints2D& interpolationPoints,
                                                        size_t polynomialDegree );

//! Computes the parameter positions for the given global interpolation points
std::vector<double> centripetalParameterPositions( const ControlPoints2D& interpolationPoints );

//! Computes the knot vector for the given parameter positions using the averaging technique
std::vector<double> knotVectorUsingAveraging( const std::vector<double>& parameterPositions,
                                              size_t polynomialDegree );

} // namespace splinekernel
} // namespace cie
