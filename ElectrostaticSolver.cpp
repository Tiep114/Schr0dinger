#include "ElectrostaticSolver.h"
#include <cmath>
#include <fstream>
#include <iostream>

int ElectrostaticSolver::coordToIndex(int i, int j, int nx) {
    return j * nx + i;
}

std::pair<int, int> ElectrostaticSolver::indexToCoord(int index, int nx) {
    int j = index / nx;
    int i = index % nx;
    return {i, j};
}

void ElectrostaticSolver::buildFDMSystem(
    int nx, int ny,
    double dx, double dy,
    const std::vector<double>& rho,
    double epsilon,
    MatrixXd& A,
    VectorXd& b,
    const std::vector<double>& boundaryValues) {
    
    int n = nx * ny;  // Total number of grid points
    
    if (rho.size() != (nx - 2) * (ny - 2)) {
        throw std::invalid_argument("Charge density size mismatch with interior grid points");
    }
    
    A = MatrixXd::Zero(n, n);
    b = VectorXd::Zero(n);
    
    // Finite difference coefficients
    double cx = 1.0 / (dx * dx);
    double cy = 1.0 / (dy * dy);
    double center = -2.0 * (cx + cy);
    
    // Fill the system for interior and boundary points
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            int idx = coordToIndex(i, j, nx);
            
            // Boundary points (Dirichlet boundary conditions)
            if (i == 0 || i == nx - 1 || j == 0 || j == ny - 1) {
                A(idx, idx) = 1.0;
                
                // Use provided boundary values or default to 0
                if (idx < static_cast<int>(boundaryValues.size())) {
                    b(idx) = boundaryValues[idx];
                } else {
                    b(idx) = 0.0;
                }
            }
            // Interior points
            else {
                int interior_idx = (i - 1) + (j - 1) * (nx - 2);
                
                // Center point
                A(idx, idx) = center;
                
                // Left neighbor
                A(idx, coordToIndex(i - 1, j, nx)) = cx;
                
                // Right neighbor
                A(idx, coordToIndex(i + 1, j, nx)) = cx;
                
                // Bottom neighbor
                A(idx, coordToIndex(i, j - 1, nx)) = cy;
                
                // Top neighbor
                A(idx, coordToIndex(i, j + 1, nx)) = cy;
                
                // Right-hand side: -ρ/ε
                b(idx) = -rho[interior_idx] / epsilon;
            }
        }
    }
}

MatrixSolver::MatrixXd ElectrostaticSolver::solvePotential(int nx, int ny, const MatrixSolver::VectorXd& phi) {
    MatrixSolver::MatrixXd phi_field(ny, nx);
    
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            int idx = coordToIndex(i, j, nx);
            phi_field(j, i) = phi(idx);
        }
    }
    
    return phi_field;
}

void ElectrostaticSolver::computeElectricField(
    const MatrixSolver::MatrixXd& phi_field,
    double dx, double dy,
    MatrixSolver::MatrixXd& Ex, MatrixSolver::MatrixXd& Ey) {
    
    int ny = phi_field.rows();
    int nx = phi_field.cols();
    
    Ex = MatrixSolver::MatrixXd::Zero(ny, nx);
    Ey = MatrixSolver::MatrixXd::Zero(ny, nx);
    
    // Compute E = -∇φ using central differences
    for (int j = 1; j < ny - 1; ++j) {
        for (int i = 1; i < nx - 1; ++i) {
            // Ex = -∂φ/∂x
            Ex(j, i) = -(phi_field(j, i + 1) - phi_field(j, i - 1)) / (2.0 * dx);
            
            // Ey = -∂φ/∂y
            Ey(j, i) = -(phi_field(j + 1, i) - phi_field(j - 1, i)) / (2.0 * dy);
        }
    }
}

MatrixSolver::MatrixXd ElectrostaticSolver::computeFieldMagnitude(const MatrixSolver::MatrixXd& Ex, const MatrixSolver::MatrixXd& Ey) {
    return (Ex.array().square() + Ey.array().square()).sqrt();
}

MatrixSolver::MatrixXd ElectrostaticSolver::computeEnergyDensity(
    const MatrixSolver::MatrixXd& Ex,
    const MatrixSolver::MatrixXd& Ey,
    double epsilon) {
    
    MatrixSolver::MatrixXd E_mag = computeFieldMagnitude(Ex, Ey);
    return 0.5 * epsilon * E_mag.array().square();
}

void ElectrostaticSolver::getGridCoordinates(
    int nx, int ny,
    double dx, double dy,
    std::vector<double>& x_coords,
    std::vector<double>& y_coords) {
    
    x_coords.resize(nx);
    y_coords.resize(ny);
    
    for (int i = 0; i < nx; ++i) {
        x_coords[i] = i * dx;
    }
    
    for (int j = 0; j < ny; ++j) {
        y_coords[j] = j * dy;
    }
}

bool ElectrostaticSolver::exportToCSV(
    const std::string& filename,
    const MatrixXd& matrix,
    const std::string& delimiter) {
    
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            file << matrix(i, j);
            if (j < matrix.cols() - 1) {
                file << delimiter;
            }
        }
        file << "\n";
    }
    
    file.close();
    std::cout << "✓ Exported to: " << filename << std::endl;
    return true;
}
