#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "basisfunctions.hpp"
#include "curve.hpp"
#include "interpolation.hpp"

PYBIND11_MODULE( pysplinekernel, m ) 
{
    m.doc( ) = "spline computation kernel"; // optional module docstring

    m.def( "evaluateBSplineBasis", &cie::splinekernel::evaluateBSplineBasis, "Evaluates single b-spline basis function." );
    m.def( "evaluate2DCurve", &cie::splinekernel::evaluate2DCurve, "Evaluates single b-spline basis function." );
    m.def( "interpolateWithBSplineCurve", &cie::splinekernel::interpolateWithBSplineCurve, "interpolateWithBSplineCurve." );

}
