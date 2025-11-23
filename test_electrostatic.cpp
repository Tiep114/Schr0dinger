#include "ElectrostaticSolver.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== Electrostatic Solver - FDM Example ===" << std::endl;
    std::cout << "Problem: Parallel Plate Capacitor\n" << std::endl;

    ElectrostaticSolver solver;

    // ========== Problem Setup ==========
    // Grid dimensions
    int nx = 25;  // points in x: 0 to 2.0 m
    int ny = 25;  // points in y: 0 to 2.0 m
    double dx = 0.1;  // 0.1 m spacing
    double dy = 0.1;  // 0.1 m spacing

    // Physical constants
    const double epsilon_0 = 8.854e-12;  // F/m (vacuum permittivity)
    double epsilon = epsilon_0;

    std::cout << "Grid Configuration:" << std::endl;
    std::cout << "  Grid points: " << nx << " x " << ny << std::endl;
    std::cout << "  Domain size: " << (nx-1)*dx << " x " << (ny-1)*dy << " m\n" << std::endl;

    // ========== Boundary Conditions ==========
    // Parallel plate capacitor: two plates at constant potential
    // Left plate (x=0): V = 100V (entire edge)
    // Right plate (x=2.0): V = 0V (entire edge)
    // Top and bottom: Neumann boundary (dV/dn = 0) - natural condition
    
    int n_total = nx * ny;
    std::vector<double> boundaryValues(n_total, 0.0);

    // Set left plate to +100V (x=0, all y)
    for (int j = 0; j < ny; ++j) {
        int idx = solver.coordToIndex(0, j, nx);
        boundaryValues[idx] = 100.0;
    }

    // Set right plate to 0V (x=nx-1, all y)
    for (int j = 0; j < ny; ++j) {
        int idx = solver.coordToIndex(nx - 1, j, nx);
        boundaryValues[idx] = 0.0;
    }
    
    // Top and bottom edges: allow solution to determine values
    // (This implements approximate Neumann condition via FDM)

    std::cout << "Boundary Conditions (Parallel Plate Capacitor):" << std::endl;
    std::cout << "  Left plate (x=0): V = 100 V (entire edge)" << std::endl;
    std::cout << "  Right plate (x=" << (nx-1)*dx << "): V = 0 V (entire edge)" << std::endl;
    std::cout << "  Top/Bottom edges: Free (Neumann boundary condition)\n" << std::endl;

    // ========== Charge Distribution ==========
    // For parallel plate capacitor in free space, no charge density
    std::vector<double> rho((nx - 2) * (ny - 2), 0.0);

    std::cout << "Charge Distribution: None (free space problem)\n" << std::endl;

    // ========== Build and Solve System ==========
    std::cout << "Building FDM system..." << std::endl;
    Eigen::MatrixXd A;
    Eigen::VectorXd b;

    solver.buildFDMSystem(nx, ny, dx, dy, rho, epsilon, A, b, boundaryValues);

    std::cout << "System size: " << A.rows() << " x " << A.cols() << std::endl;
    std::cout << "Solving Ax = b using LU decomposition...\n" << std::endl;

    Eigen::VectorXd phi = solver.solveLU(A, b);

    // ========== Extract and Display Results ==========
    Eigen::MatrixXd phi_field = solver.solvePotential(nx, ny, phi);

    std::cout << "Potential Field φ (selected points):" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    // Show potential at y = 1.0m (middle row)
    int j_middle = ny / 2;
    std::cout << "\nPotential along y = " << j_middle * dy << " m:" << std::endl;
    for (int i = 0; i < nx; i += 4) {
        std::cout << "  x = " << std::setw(4) << i * dx << " m: φ = " << std::setw(7) << phi_field(j_middle, i) << " V" << std::endl;
    }

    // ========== Compute Electric Field ==========
    std::cout << "\nComputing electric field E = -∇φ..." << std::endl;
    Eigen::MatrixXd Ex, Ey;
    solver.computeElectricField(phi_field, dx, dy, Ex, Ey);

    Eigen::MatrixXd E_mag = solver.computeFieldMagnitude(Ex, Ey);

    std::cout << "\nElectric Field Magnitude |E| (selected points):" << std::endl;
    std::cout << "\nField magnitude along y = " << j_middle * dy << " m:" << std::endl;
    for (int i = 1; i < nx - 1; i += 4) {
        std::cout << "  x = " << std::setw(4) << i * dx << " m: |E| = " << std::setw(10) << E_mag(j_middle, i) << " V/m" << std::endl;
    }

    // ========== Compute Energy Density ==========
    std::cout << "\nComputing energy density u = (1/2)εE²..." << std::endl;
    Eigen::MatrixXd u = solver.computeEnergyDensity(Ex, Ey, epsilon);

    std::cout << "\nEnergy Density (selected points):" << std::endl;
    std::cout << "\nEnergy density along y = " << j_middle * dy << " m:" << std::endl;
    for (int i = 1; i < nx - 1; i += 4) {
        std::cout << "  x = " << std::setw(4) << i * dx << " m: u = " << std::setw(10) << std::scientific << u(j_middle, i) << " J/m³" << std::endl;
    }

    // ========== Summary Statistics ==========
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n--- Summary Statistics ---" << std::endl;
    std::cout << "Potential:" << std::endl;
    std::cout << "  Min: " << phi_field.minCoeff() << " V" << std::endl;
    std::cout << "  Max: " << phi_field.maxCoeff() << " V" << std::endl;
    std::cout << "  Mean: " << phi_field.mean() << " V" << std::endl;

    std::cout << "\nElectric Field Magnitude:" << std::endl;
    std::cout << "  Max: " << E_mag.maxCoeff() << " V/m" << std::endl;
    std::cout << "  Mean: " << E_mag.mean() << " V/m" << std::endl;

    std::cout << "\nTotal Energy Density:" << std::endl;
    double total_energy = u.sum() * dx * dy;
    std::cout << "  Total (approx): " << std::scientific << total_energy << " J" << std::endl;

    std::cout << "\n=== Simulation Complete ===" << std::endl;
    std::cout << "\nPhysical Insight:" << std::endl;
    std::cout << "- The electric field between the plates is approximately uniform" << std::endl;
    std::cout << "- Field strength ≈ ΔV / d = 100V / 2.0m = 50 V/m" << std::endl;
    std::cout << "- The potential varies linearly in the gap (ideal capacitor behavior)" << std::endl;

    // ========== Export Results to CSV ==========
    std::cout << "\n--- Exporting results to CSV files ---\n" << std::endl;
    
    solver.exportToCSV("potential.csv", phi_field);
    solver.exportToCSV("Ex.csv", Ex);
    solver.exportToCSV("Ey.csv", Ey);
    solver.exportToCSV("E_magnitude.csv", E_mag);
    solver.exportToCSV("energy_density.csv", u);
    
    std::cout << "\nTo visualize, run: python visualize_electrostatic.py" << std::endl;

    return 0;
}
