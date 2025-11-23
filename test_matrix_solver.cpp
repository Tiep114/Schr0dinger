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

    // ========== Example 6: Conjugate Gradient Method ==========
    std::cout << "\n--- Example 6: Conjugate Gradient Method (Iterative) ---\n";
    
    // Use the symmetric matrix from Example 5 (which is SPD-like)
    // Create RHS that matches the problem structure
    Eigen::VectorXd b_cg(3);
    b_cg << 2, -4, 2;
    
    solver.printMatrix("SPD Matrix A", A_sym);
    solver.printVector("Right-hand side b", b_cg);
    
    // Solve using Conjugate Gradient
    Eigen::VectorXd x_cg = solver.solveConjugateGradient(A_sym, b_cg);
    solver.printVector("Solution x (using CG)", x_cg);
    
    // Solve same system using LU for comparison
    Eigen::VectorXd x_lu_cg = solver.solveLU(A_sym, b_cg);
    solver.printVector("Solution x (using LU)", x_lu_cg);
    
    // Compare solutions
    Eigen::VectorXd residual_cg = A_sym * x_cg - b_cg;
    std::cout << "Residual norm (CG): " << residual_cg.norm() << std::endl;
    
    Eigen::VectorXd residual_lu_cg = A_sym * x_lu_cg - b_cg;
    std::cout << "Residual norm (LU): " << residual_lu_cg.norm() << std::endl;
    
    std::cout << "Solution difference (CG vs LU): " << (x_cg - x_lu_cg).norm() << std::endl;

    // ========== Example 7: GMRES Method ==========
    std::cout << "\n--- Example 7: GMRES Method (Iterative, General) ---\n";
    
    // Create a general (non-symmetric) matrix to test GMRES
    Eigen::MatrixXd A_general(4, 4);
    A_general << 4, -1, 0.5, 1,
                 -1, 3, -0.5, -1,
                 0.5, -0.5, 2, 0.5,
                 1, -1, 0.5, 3;
    
    Eigen::VectorXd b_gmres(4);
    b_gmres << 10, 5, 7, 8;
    
    solver.printMatrix("General Matrix A", A_general);
    solver.printVector("Right-hand side b", b_gmres);
    
    // Solve using GMRES
    Eigen::VectorXd x_gmres = solver.solveGMRES(A_general, b_gmres);
    solver.printVector("Solution x (using GMRES)", x_gmres);
    
    // Solve same system using LU for comparison
    Eigen::VectorXd x_lu_gmres = solver.solveLU(A_general, b_gmres);
    solver.printVector("Solution x (using LU)", x_lu_gmres);
    
    // Compare solutions
    Eigen::VectorXd residual_gmres = A_general * x_gmres - b_gmres;
    std::cout << "Residual norm (GMRES): " << residual_gmres.norm() << std::endl;
    
    Eigen::VectorXd residual_lu_gmres = A_general * x_lu_gmres - b_gmres;
    std::cout << "Residual norm (LU): " << residual_lu_gmres.norm() << std::endl;
    
    std::cout << "Solution difference (GMRES vs LU): " << (x_gmres - x_lu_gmres).norm() << std::endl;

    std::cout << "\n=== All examples completed successfully! ===" << std::endl;

    return 0;
}
