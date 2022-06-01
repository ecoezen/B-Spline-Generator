#include "interpolation.hpp"
#include "basisfunctions.hpp"
#include "linalg.hpp"
#include <cmath>
#include <exception>


 
namespace cie{ 
    namespace splinekernel{ 
        namespace splinehelper{

            void runtime_error_handler(bool isSafe, const char* errMessage){

                if(!isSafe)
                {
                    throw std::runtime_error{errMessage};
                }
            }

        } // namespace splinehelper



ControlPointsAndKnotVector interpolateWithBSplineCurve(const ControlPoints2D& interpolationPoints,
                                                                           size_t polynomialDegree){   
   
    // Reparameterizes by using Centripetal Method.
    auto paramPositon = centripetalParameterPositions(interpolationPoints);

    // Calculates Knot Vector with averaging approach. To be returned (return.second).
    std::vector<double> knotVec = knotVectorUsingAveraging(paramPositon, polynomialDegree);

    // Size for containers, e.g. shapeFunctions, sendToSolver, and for loop iterators.
    const int sizeCommon = paramPositon.size();

    // Contains shape functions for each amaket parameter positions.
    std::vector<std::vector<double>> shapeFunctions(sizeCommon, std::vector<double>(sizeCommon));

   /* // Resizing vector of rows. 
    for ( int i = 0 ; i < sizeCommon ; i++ ){    
        shapeFunctions[i].resize(sizeCommon);
    }
    */
    // Calculated control points. To be returned (return.first).
    ControlPoints2D cPoints;

    // Throw exception if number of x-values is not equal number of y-values
     splinehelper::runtime_error_handler(interpolationPoints[0].size() == interpolationPoints[1].size(), "Missing data: Number of x-values is not equal number of y-values!" );
    
    // Calculation procedure for Control Points.
     if(polynomialDegree==1){
        cPoints[0] = interpolationPoints[0]; 
        cPoints[1] = interpolationPoints[1]; 
     }

     else{
        for(int j = 0; j<sizeCommon; j++){
            for(int i = 0; i < sizeCommon; i++){
                shapeFunctions[j][i] = evaluateBSplineBasis(paramPositon[j], (size_t)i, polynomialDegree, knotVec);
            }
        }

        // Forming new container with linalg::Matrix class that linalg::solve() can handle. 
        linalg::Matrix sendToSolver(shapeFunctions);

        // Solving linear system of equations once for x-coordinates(cPoints[0]) and for y-coordinates(cPoints[1]).
        cPoints[0] = linalg::solve(sendToSolver, interpolationPoints[0]);
        cPoints[1] = linalg::solve(sendToSolver, interpolationPoints[1]);
        
     } 
    
    return { cPoints, knotVec };
}

std::vector<double> centripetalParameterPositions(const ControlPoints2D& interpolationPoints){

    // sum of root of each pre-parameter position.
    double dTotal = 0.0;

    // Number of control points.
    int nPoints = (interpolationPoints[0]).size();

    // Container of pre-parameter positions.
    std::vector<double> dPre(nPoints-1);

    // Container of final parameter positions. To be returned.
    std::vector<double> tHat(nPoints, 0.0);

    // Forming initial configuration of parameter positing vector. e.g. [0.0 0.0 ... 0.0 1.0]
    tHat[nPoints-1] = 1.0;

    
    // Calculates pre-parameter positions.
    for(int i=0; i<nPoints-1; i++){
        dPre[i] = std::sqrt(std::pow((interpolationPoints[0][i+1] - interpolationPoints[0][i]), 2)
                +  std::pow((interpolationPoints[1][i+1] - interpolationPoints[1][i]), 2));
    }

    // Calculates dTotal.
    for(double d: dPre){
        dTotal += std::sqrt(d);
    }

    // Calculates each parameter position. 
    for(int i=1; i<nPoints-1; i++){
        tHat[i] = tHat[i-1] + std::sqrt(dPre[i-1]) / dTotal;
    }
 
    return tHat;
}

std::vector<double> knotVectorUsingAveraging( const std::vector<double>& parameterPositions,
                                                                     size_t polynomialDegree){

    // Size of Knot vector.                                                                     
    int knotSize = parameterPositions.size() + polynomialDegree + 1;

    // Knot Vector. To be returned.
    std::vector<double> knotVector(knotSize, 0.0);

    // The number of inner nodes. Will also be used for error check.    
    int mInner = knotSize - 2*(polynomialDegree + 1);

    // Throw exception if polynomial degree is too high for given number of points. Error in case of mInner < 0.
    splinehelper::runtime_error_handler( mInner >= 0, "Given polynomial degree is too high to proceed!" );
    
    // Calculates the knot vector.
    for(int i = 0; i < mInner; i++){

        if(polynomialDegree==1){
            knotVector[polynomialDegree + 1 + i] = parameterPositions[polynomialDegree + i];       
        } 

        else if(polynomialDegree==2){
            knotVector[polynomialDegree + 1 + i] = ((parameterPositions[polynomialDegree + i] + parameterPositions[polynomialDegree - 1 + i]))/(double)polynomialDegree;
        } 

        else{
            knotVector[polynomialDegree + 1 + i] = ((parameterPositions[polynomialDegree + i-2])+(parameterPositions[polynomialDegree + i] + parameterPositions[polynomialDegree - 1 + i]))/(double)polynomialDegree;
        }
    }

    // Final configuration of knot vector with respect to open-knot vector approach by filling the last (polynomialDegree + 1) elements with 1.0. 
    for(int i=0; i<(int)polynomialDegree+1; i++){
        knotVector[polynomialDegree + 1 + i + mInner] = 1.0; 
    }  

    return knotVector;
}

    } // namespace splinekernel
} // namespace cie
