#ifndef MATRIX_SOLVER_H
#define MATRIX_SOLVER_H

#include <Eigen/Dense>
#include <iostream>
#include <vector>

/**
 * @class MatrixSolver
 * @brief A class for solving systems of linear equations using Eigen library
 * 
 * This class provides methods for:
 * - Solving Ax = b systems
 * - Computing matrix determinants
 * - Computing matrix inverses
 * - Computing eigenvalues and eigenvectors
 */
class MatrixSolver {
public:
    using MatrixXd = Eigen::MatrixXd;
    using VectorXd = Eigen::VectorXd;

    /**
     * @brief Solve a linear system Ax = b using LU decomposition
     * @param A Coefficient matrix (n x n)
     * @param b Right-hand side vector (n x 1)
     * @return Solution vector x (n x 1)
     */
    VectorXd solveLU(const MatrixXd& A, const VectorXd& b);

    /**
     * @brief Solve a linear system Ax = b using QR decomposition
     * @param A Coefficient matrix (m x n)
     * @param b Right-hand side vector (m x 1)
     * @return Solution vector x (n x 1)
     */
    VectorXd solveQR(const MatrixXd& A, const VectorXd& b);

    /**
     * @brief Solve Ax = b using Conjugate Gradient (for symmetric positive-definite matrices)
     * 
     * Best for:
     * - Symmetric positive-definite matrices (Poisson equation, electrostatics)
     * - Large sparse matrices
     * - When memory is limited
     * 
     * @param A Coefficient matrix (must be SPD)
     * @param b Right-hand side vector
     * @param maxIterations Maximum iterations (default: automatic)
     * @param tolerance Convergence tolerance (default: 1e-6)
     * @return Solution vector x
     */
    VectorXd solveConjugateGradient(
        const MatrixXd& A,
        const VectorXd& b,
        int maxIterations = -1,
        double tolerance = 1e-6
    );

    /**
     * @brief Solve Ax = b using GMRES (for general matrices)
     * 
     * Best for:
     * - General non-symmetric matrices (Schrödinger equation)
     * - Indefinite matrices
     * - Problems where CG doesn't converge
     * 
     * @param A Coefficient matrix (any square matrix)
     * @param b Right-hand side vector
     * @param restart GMRES restart parameter (default: 30)
     * @param maxIterations Maximum iterations (default: automatic)
     * @param tolerance Convergence tolerance (default: 1e-6)
     * @return Solution vector x
     */
    VectorXd solveGMRES(
        const MatrixXd& A,
        const VectorXd& b,
        int restart = 30,
        int maxIterations = -1,
        double tolerance = 1e-6
    );

    /**
     * @brief Solve a linear system Ax = b using QR decomposition

    /**
     * @brief Compute the determinant of a matrix
     * @param A Input matrix
     * @return Determinant value
     */
    double determinant(const MatrixXd& A);

    /**
     * @brief Compute the inverse of a matrix
     * @param A Input matrix
     * @return Inverse matrix
     */
    MatrixXd inverse(const MatrixXd& A);

    /**
     * @brief Compute eigenvalues and eigenvectors
     * @param A Input matrix
     * @param eigenvalues Output eigenvalues
     * @param eigenvectors Output eigenvectors
     */
    void eigenDecomposition(const MatrixXd& A, VectorXd& eigenvalues, MatrixXd& eigenvectors);

    /**
     * @brief Print a matrix in a formatted way
     * @param name Name of the matrix
     * @param matrix Matrix to print
     */
    void printMatrix(const std::string& name, const MatrixXd& matrix);

    /**
     * @brief Print a vector in a formatted way
     * @param name Name of the vector
     * @param vector Vector to print
     */
    void printVector(const std::string& name, const VectorXd& vector);
};

#endif // MATRIX_SOLVER_H
