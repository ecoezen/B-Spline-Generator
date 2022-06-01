#ifndef CIE_LINALG_HPP
#define CIE_LINALG_HPP

#include <vector>
#include <iostream>
#include <array>

namespace cie
{
namespace linalg
{

// Represents a full double vector
using Vector = std::vector<double>;

class Matrix
{
public:
    explicit Matrix( );

    Matrix( size_t size1, size_t size2 );
    Matrix( size_t size1, size_t size2, double value );

    Matrix( const std::vector<double>& rowMajorData, size_t size1 );
    Matrix( const std::vector<Vector>& vectorOfRows );

    size_t size1( ) const;
    size_t size2( ) const;

    std::array<size_t, 2> sizes( ) const;

    double& operator()( size_t i, size_t j );
    double operator()( size_t i, size_t j ) const;

    void fill( double value );

private:
    size_t size1_, size2_;
    std::vector<double> data_;
};

// Write to an ostream, default argument is std::cout
void write( const Vector& vector, std::ostream& out = std::cout );
void write( const Matrix& matrix, std::ostream& out = std::cout );

// Euclidean norms
double norm( const Vector& vector );
double norm( const Matrix& matrix );

// Solve linear system of equations
Vector solve( const Matrix& matrix,
              const Vector& vector );

} // namespace linalg
} // namespace cie

#include "linalg_impl.hpp"

#endif // CIE_LINALG_HPP
