#include "MatrixSolver.h"

MatrixSolver::VectorXd MatrixSolver::solveLU(const MatrixXd& A, const VectorXd& b) {
    // LU decomposition and back substitution
    return A.lu().solve(b);
}

MatrixSolver::VectorXd MatrixSolver::solveQR(const MatrixXd& A, const VectorXd& b) {
    // QR decomposition (useful for overdetermined systems)
    return A.colPivHouseholderQr().solve(b);
}

double MatrixSolver::determinant(const MatrixXd& A) {
    if (A.rows() != A.cols()) {
        throw std::invalid_argument("Matrix must be square to compute determinant");
    }
    return A.determinant();
}

MatrixSolver::MatrixXd MatrixSolver::inverse(const MatrixXd& A) {
    if (A.rows() != A.cols()) {
        throw std::invalid_argument("Matrix must be square to compute inverse");
    }
    return A.inverse();
}

void MatrixSolver::eigenDecomposition(const MatrixXd& A, VectorXd& eigenvalues, MatrixXd& eigenvectors) {
    if (A.rows() != A.cols()) {
        throw std::invalid_argument("Matrix must be square for eigenvalue decomposition");
    }
    
    Eigen::EigenSolver<MatrixXd> solver(A);
    eigenvalues = solver.eigenvalues().real();
    eigenvectors = solver.eigenvectors().real();
}

void MatrixSolver::printMatrix(const std::string& name, const MatrixXd& matrix) {
    std::cout << "\n" << name << ":\n" << matrix << "\n";
}

void MatrixSolver::printVector(const std::string& name, const VectorXd& vector) {
    std::cout << "\n" << name << ":\n" << vector << "\n";
}
