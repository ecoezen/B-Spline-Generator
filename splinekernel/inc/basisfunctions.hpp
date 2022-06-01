#pragma once

#include <vector>
#include "stddef.h"

namespace cie
{
namespace splinekernel
{

double evaluateBSplineBasis( double t, size_t i, size_t p, const std::vector<double>& knotVector );

} // splinekernel
} // cie