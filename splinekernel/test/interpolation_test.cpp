#include "catch.hpp"
#include "interpolation.hpp"
#include <algorithm>

namespace cie
{
namespace splinekernel
{


TEST_CASE( "centripetalParameterPositions_test" )
{
    // Define points to be interpolated
    ControlPoints2D interpolationPoints;
    interpolationPoints[0] = { 0.0, 15.0, 171.0, 307.0, 907.0 }; // x-coordinates
    interpolationPoints[1] = { 0.0, 20.0, 85.0, 340.0, 515.0 };  // y-coordinates

    // Get parameter positions
    std::vector<double> parameterPositions( interpolationPoints[0].size( ) );

    REQUIRE_NOTHROW(parameterPositions = centripetalParameterPositions( interpolationPoints ) );

    // Check parameter positions
    double d = 5.0 + 13.0 + 17.0 + 25.0; // d = 60

    REQUIRE( parameterPositions.size( ) == interpolationPoints[0].size( ) );

    CHECK( parameterPositions[0] == Approx( 0.0 ) );
    CHECK( parameterPositions[1] == Approx( 5.0 / d ) );
    CHECK( parameterPositions[2] == Approx( ( 5.0 + 13.0 ) / d ) );
    CHECK( parameterPositions[3] == Approx( ( 5.0 + 13.0 + 17.0 ) / d ) );
    CHECK( parameterPositions[4] == Approx( 1.0 ) );
}

TEST_CASE( "knotVectorUsingAveraging_test" )
{

    // Define parameter positions
    std::vector<double> parameterPositions{ 0.0, 1.0 / 2.0, 2.0 / 3.0, 3.0 / 4.0, 1.0 };
    std::vector<double> knotVector;

    // Test for p = 2
    size_t p = 2;

    REQUIRE_NOTHROW( knotVector = knotVectorUsingAveraging( parameterPositions, p ) );

    REQUIRE( knotVector.size( ) == parameterPositions.size( ) + p + 1 );

    CHECK( knotVector[0] == Approx( 0.0 ) );
    CHECK( knotVector[1] == Approx( 0.0 ) );
    CHECK( knotVector[2] == Approx( 0.0 ) );
    CHECK( knotVector[3] == Approx( 7.0 / 12.0 ) );
    CHECK( knotVector[4] == Approx( 17.0 / 24.0 ) );
    CHECK( knotVector[5] == Approx( 1.0 ) );
    CHECK( knotVector[6] == Approx( 1.0 ) );
    CHECK( knotVector[7] == Approx( 1.0 ) );

    // Test for p = 3
    p = 3;

    REQUIRE_NOTHROW( knotVector = knotVectorUsingAveraging( parameterPositions, p ) );

    REQUIRE( knotVector.size( ) == parameterPositions.size( ) + p + 1 );

    CHECK( knotVector[0] == Approx( 0.0 ) );
    CHECK( knotVector[1] == Approx( 0.0 ) );
    CHECK( knotVector[2] == Approx( 0.0 ) );
    CHECK( knotVector[3] == Approx( 0.0 ) );
    CHECK( knotVector[4] == Approx( 23.0 / 36.0 ) );
    CHECK( knotVector[5] == Approx( 1.0 ) );
    CHECK( knotVector[6] == Approx( 1.0 ) );
    CHECK( knotVector[7] == Approx( 1.0 ) );
    CHECK( knotVector[8] == Approx( 1.0 ) );

    // Test for p = 4 (no internal knots)
    p = 4;

    REQUIRE_NOTHROW( knotVector = knotVectorUsingAveraging( parameterPositions, p ) );

    REQUIRE( knotVector.size( ) == parameterPositions.size( ) + p + 1 );

    CHECK( knotVector[0] == Approx( 0.0 ) );
    CHECK( knotVector[1] == Approx( 0.0 ) );
    CHECK( knotVector[2] == Approx( 0.0 ) );
    CHECK( knotVector[3] == Approx( 0.0 ) );
    CHECK( knotVector[4] == Approx( 0.0 ) );
    CHECK( knotVector[5] == Approx( 1.0 ) );
    CHECK( knotVector[6] == Approx( 1.0 ) );
    CHECK( knotVector[7] == Approx( 1.0 ) );
    CHECK( knotVector[8] == Approx( 1.0 ) );
    CHECK( knotVector[9] == Approx( 1.0 ) );

    CHECK_THROWS( knotVectorUsingAveraging( parameterPositions, 5 ) );
}

TEST_CASE("interpolateWithBSplineCurve_test")
{
    ControlPoints2D interpolationPoints;
    ControlPointsAndKnotVector result;

    // Define points to be interpolated
    interpolationPoints[0] = { 0.0, 15.0, 171.0, 307.0,907.0 }; // x-coordinates
    interpolationPoints[1] = { 0.0, 20.0, 85.0, 340.0, 515.0 }; // y-coordinates

    double d = 5.0 + 13.0 + 17.0 + 25.0; // d = 60

    // Check p = 1
    size_t p = 1;

    // Perform interpolation
    REQUIRE_NOTHROW( result = interpolateWithBSplineCurve( interpolationPoints, p ) );

    // Check control points (should be the same as the interpolation points for p = 1)
    REQUIRE( result.first[0].size( ) == interpolationPoints[0].size( ) ); // Check # of x-components
    REQUIRE( result.first[1].size( ) == interpolationPoints[1].size( ) ); // Check # of y-components

    for( size_t i = 0; i < result.first[0].size( ); ++i )
    {
        CHECK( result.first[0][i] == interpolationPoints[0][i] ); // Check x component
        CHECK( result.first[1][i] == interpolationPoints[1][i] ); // Check y component
    }

    // Check knot vector
    REQUIRE( result.second.size( ) == interpolationPoints[0].size( ) + p + 1 );

    CHECK( result.second[0] == Approx( 0.0 ) );
    CHECK( result.second[1] == Approx( 0.0 ) );
    CHECK( result.second[2] == Approx( 5.0 / d ) );
    CHECK( result.second[3] == Approx( ( 5.0 + 13 ) / d ) );
    CHECK( result.second[4] == Approx( ( 5.0 + 13 + 17 ) / d ) );
    CHECK( result.second[5] == Approx( 1.0 ) );
    CHECK( result.second[6] == Approx( 1.0 ) );

    // Check p = 3
    p = 3;

    // Perform interpolation
    REQUIRE_NOTHROW( result = interpolateWithBSplineCurve( interpolationPoints, p ) );

    // Check control points
    REQUIRE( result.first[0].size( ) == interpolationPoints[0].size( ) ); // Check # of x-components
    REQUIRE( result.first[1].size( ) == interpolationPoints[1].size( ) ); // Check # of y-components

    // Check x components
    CHECK( result.first[0][0] == Approx( 0.0 ) );
    CHECK( result.first[0][1] == Approx( -12.5687 ) );
    CHECK( result.first[0][2] == Approx( 383.886 ) );
    CHECK( result.first[0][3] == Approx( 236.859 ) );
    CHECK( result.first[0][4] == Approx( 907.0 ) );

    // Check y components
    CHECK( result.first[1][0] == Approx( 0.0 ) );
    CHECK( result.first[1][1] == Approx( 31.9598 ) );
    CHECK( result.first[1][2] == Approx( 30.3872 ) );
    CHECK( result.first[1][3] == Approx( 672.775 ) );
    CHECK( result.first[1][4] == Approx( 515.0 ) );

    // Check knot vector
    REQUIRE( result.second.size( ) == interpolationPoints[0].size( ) + p + 1 );

    CHECK( result.second[0] == Approx( 0.0 ) );
    CHECK( result.second[1] == Approx( 0.0 ) );
    CHECK( result.second[2] == Approx( 0.0 ) );
    CHECK( result.second[3] == Approx( 0.0 ) );
    CHECK( result.second[4] == Approx( ( 3 * 5.0 + 2 * 13.0 + 17.0) / 3 / d ) );
    CHECK( result.second[5] == Approx( 1.0 ) );
    CHECK( result.second[6] == Approx( 1.0 ) );
    CHECK( result.second[7] == Approx( 1.0 ) );
    CHECK( result.second[8] == Approx( 1.0 ) );

    // Check p = 4 (no inner knots )
    p = 4;

    // Perform interpolation
    REQUIRE_NOTHROW( result = interpolateWithBSplineCurve( interpolationPoints, p ) );

    // Check control points
    REQUIRE( result.first[0].size( ) == interpolationPoints[0].size( ) ); // Check # of x-components
    REQUIRE( result.first[1].size( ) == interpolationPoints[1].size( ) ); // Check # of y-components

    // Check x components
    CHECK( result.first[0][0] == Approx( 0.0 ) );
    CHECK( result.first[0][1] == Approx( -38.5422077922 ) );
    CHECK( result.first[0][2] == Approx( 718.2705627706 ) );
    CHECK( result.first[0][3] == Approx( -139.3798701299 ) );
    CHECK( result.first[0][4] == Approx( 907.0 ) );

    // Check y components
    CHECK( result.first[1][0] == Approx( 0.0 ) );
    CHECK( result.first[1][1] == Approx( 80.2976190476 ) );
    CHECK( result.first[1][2] == Approx( -71.8650793651 ) );
    CHECK( result.first[1][3] == Approx( 883.5119047619 ) );
    CHECK( result.first[1][4] == Approx( 515.0 ) );

    // Check knot vector
    REQUIRE( result.second.size( ) == interpolationPoints[0].size( ) + p + 1 );

    CHECK( result.second[0] == Approx( 0.0 ) );
    CHECK( result.second[1] == Approx( 0.0 ) );
    CHECK( result.second[2] == Approx( 0.0 ) );
    CHECK( result.second[3] == Approx( 0.0 ) );
    CHECK( result.second[4] == Approx( 0.0 ) );
    CHECK( result.second[5] == Approx( 1.0 ) );
    CHECK( result.second[6] == Approx( 1.0 ) );
    CHECK( result.second[7] == Approx( 1.0 ) );
    CHECK( result.second[8] == Approx( 1.0 ) );
    CHECK( result.second[9] == Approx( 1.0 ) );

    // Check for inconsistent input component sizes
    interpolationPoints[0] = { 0, 1, 2, 3, 4, 5 };
    interpolationPoints[1] = { 0, 1, 2, 3, 4 };

    REQUIRE_THROWS( result = interpolateWithBSplineCurve( interpolationPoints, p ) );

    interpolationPoints[0] = { 0, 1, 2, 3, 4 };
    interpolationPoints[1] = { 0, 1, 2, 3, 4 };

    // Check for invalid polynomial degrees
    REQUIRE_THROWS( result = interpolateWithBSplineCurve( interpolationPoints, 0 ) );
    REQUIRE_THROWS( result = interpolateWithBSplineCurve( interpolationPoints, 5 ) );

    // Check if parameter positions and knot vectors are sorted
    std::vector<double> parameterPositions, knotVector;

    REQUIRE_NOTHROW( parameterPositions = centripetalParameterPositions( interpolationPoints ) );

    CHECK( is_sorted( parameterPositions.begin( ), parameterPositions.end( ) ) );

    REQUIRE_NOTHROW( knotVector = knotVectorUsingAveraging( parameterPositions, p ) );

    CHECK( is_sorted( knotVector.begin( ), knotVector.end( ) ) );
}



} // namespace splinekernel
} // namespace cie
