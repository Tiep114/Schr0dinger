#include "MatrixSolver.h"
#include <iostream>

int main() {
    std::cout << "=== C++ Matrix Solver using Eigen ===" << std::endl;

    MatrixSolver solver;

    // ========== Example 1: Solve Ax = b ==========
    std::cout << "\n--- Example 1: Solving Linear System Ax = b ---\n";
    
    // Create a 3x3 coefficient matrix A
    Eigen::MatrixXd A(3, 3);
    A << 2, 1, -1,
         -3, -1, 2,
         -2, 1, 2;
    
    // Create right-hand side vector b
    Eigen::VectorXd b(3);
    b << 8, -11, -3;
    
    solver.printMatrix("Coefficient Matrix A", A);
    solver.printVector("Right-hand side b", b);
    
    // Solve using LU decomposition
    Eigen::VectorXd x = solver.solveLU(A, b);
    solver.printVector("Solution x (using LU)", x);
    
    // Verify: A*x should equal b
    Eigen::VectorXd verification = A * x;
    solver.printVector("Verification: A*x", verification);

    // ========== Example 2: Matrix Determinant ==========
    std::cout << "\n--- Example 2: Computing Determinant ---\n";
    
    double det = solver.determinant(A);
    std::cout << "Determinant of A: " << det << std::endl;

    // ========== Example 3: Matrix Inverse ==========
    std::cout << "\n--- Example 3: Computing Matrix Inverse ---\n";
    
    Eigen::MatrixXd A_inv = solver.inverse(A);
    solver.printMatrix("Inverse of A", A_inv);
    
    // Verify: A * A_inv should be identity
    Eigen::MatrixXd identity = A * A_inv;
    solver.printMatrix("Verification: A * A_inv (should be I)", identity);

    // ========== Example 4: Overdetermined System (QR) ==========
    std::cout << "\n--- Example 4: Overdetermined System (using QR) ---\n";
    
    // Create a 4x2 matrix (more equations than unknowns)
    Eigen::MatrixXd A_over(4, 2);
    A_over << 1, 1,
              2, 1,
              3, 1,
              4, 1;
    
    Eigen::VectorXd b_over(4);
    b_over << 2, 3, 4, 5;
    
    solver.printMatrix("Overdetermined Matrix A", A_over);
    solver.printVector("Right-hand side b", b_over);
    
    Eigen::VectorXd x_least_squares = solver.solveQR(A_over, b_over);
    solver.printVector("Least squares solution x (using QR)", x_least_squares);

    // ========== Example 5: Eigenvalue Decomposition ==========
    std::cout << "\n--- Example 5: Eigenvalue Decomposition ---\n";
    
    // Create a symmetric matrix for eigenvalue decomposition
    Eigen::MatrixXd A_sym(3, 3);
    A_sym << 4, -2, 0,
             -2, 4, -2,
             0, -2, 4;
    
    solver.printMatrix("Symmetric Matrix", A_sym);
    
    Eigen::VectorXd eigenvalues;
    Eigen::MatrixXd eigenvectors;
    solver.eigenDecomposition(A_sym, eigenvalues, eigenvectors);
    
    solver.printVector("Eigenvalues", eigenvalues);
    solver.printMatrix("Eigenvectors", eigenvectors);

    std::cout << "\n=== All examples completed successfully! ===" << std::endl;

    return 0;
}
