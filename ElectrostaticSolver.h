#ifndef ELECTROSTATIC_SOLVER_H
#define ELECTROSTATIC_SOLVER_H

#include "MatrixSolver.h"
#include <Eigen/Dense>
#include <vector>
#include <stdexcept>

/**
 * @class ElectrostaticSolver
 * @brief Solves 2D electrostatic problems using Finite Difference Method (FDM)
 * 
 * This class extends MatrixSolver to solve Poisson's equation on a 2D rectangular grid:
 * ∇²φ = -ρ/ε₀
 * 
 * Where:
 * - φ is the electric potential
 * - ρ is the charge density
 * - ε₀ is the permittivity
 */
class ElectrostaticSolver : public MatrixSolver {
public:
    /**
     * @brief Build FDM system for 2D Poisson equation
     * 
     * Discretizes the domain into a grid and builds the linear system Ax = b
     * using the finite difference method with 5-point stencil.
     * 
     * @param nx Number of grid points in x-direction
     * @param ny Number of grid points in y-direction
     * @param dx Grid spacing in x-direction (m)
     * @param dy Grid spacing in y-direction (m)
     * @param rho Charge density at each interior grid point (C/m³)
     * @param epsilon Permittivity (F/m) - typically ε₀ = 8.854e-12
     * @param A Output: coefficient matrix (nx*ny x nx*ny)
     * @param b Output: right-hand side vector (nx*ny)
     * @param boundaryValues Boundary potential values (Dirichlet conditions)
     */
    void buildFDMSystem(
        int nx, int ny,
        double dx, double dy,
        const std::vector<double>& rho,
        double epsilon,
        MatrixXd& A,
        VectorXd& b,
        const std::vector<double>& boundaryValues
    );

    /**
     * @brief Solve for electric potential on 2D grid
     * 
     * @param nx Number of grid points in x-direction
     * @param ny Number of grid points in y-direction
     * @param phi Solution vector (potential at each grid point)
     * @return 2D potential field as matrix (ny x nx)
     */
    MatrixXd solvePotential(int nx, int ny, const VectorXd& phi);

    /**
     * @brief Compute electric field from potential using finite differences
     * 
     * E = -∇φ, so:
     * Ex = -∂φ/∂x
     * Ey = -∂φ/∂y
     * 
     * @param phi_field 2D potential field
     * @param dx Grid spacing in x-direction
     * @param dy Grid spacing in y-direction
     * @param Ex Output: x-component of electric field
     * @param Ey Output: y-component of electric field
     */
    void computeElectricField(
        const MatrixXd& phi_field,
        double dx, double dy,
        MatrixXd& Ex, MatrixXd& Ey
    );

    /**
     * @brief Compute electric field magnitude
     * 
     * @param Ex x-component of electric field
     * @param Ey y-component of electric field
     * @return Electric field magnitude |E| = sqrt(Ex² + Ey²)
     */
    MatrixXd computeFieldMagnitude(const MatrixXd& Ex, const MatrixXd& Ey);

    /**
     * @brief Compute electrostatic energy density
     * 
     * Energy density: u = (1/2) * ε * E²
     * 
     * @param Ex x-component of electric field
     * @param Ey y-component of electric field
     * @param epsilon Permittivity
     * @return Energy density field
     */
    MatrixXd computeEnergyDensity(
        const MatrixXd& Ex,
        const MatrixXd& Ey,
        double epsilon
    );

    /**
     * @brief Get grid coordinates for visualization
     * 
     * @param nx Number of grid points in x
     * @param ny Number of grid points in y
     * @param dx Grid spacing in x
     * @param dy Grid spacing in y
     * @param x_coords Output: x-coordinates
     * @param y_coords Output: y-coordinates
     */
    void getGridCoordinates(
        int nx, int ny,
        double dx, double dy,
        std::vector<double>& x_coords,
        std::vector<double>& y_coords
    );

    /**
     * @brief Export matrix to CSV file
     * 
     * @param filename Output CSV file path
     * @param matrix Matrix to export
     * @param delimiter CSV delimiter (default: comma)
     * @return True if export successful
     */
    bool exportToCSV(
        const std::string& filename,
        const MatrixXd& matrix,
        const std::string& delimiter = ","
    );

    /**
     * @brief Convert 1D index to 2D grid coordinates
     * 
     * @param index 1D linear index
     * @param nx Number of grid points in x
     * @return {i, j} where i is x-index, j is y-index
     */
    std::pair<int, int> indexToCoord(int index, int nx);

    /**
     * @brief Convert 2D grid coordinates to 1D index
     * 
     * @param i x-grid index
     * @param j y-grid index
     * @param nx Number of grid points in x
     * @return Linear 1D index
     */
    int coordToIndex(int i, int j, int nx);
};

#endif // ELECTROSTATIC_SOLVER_H
