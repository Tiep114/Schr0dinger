#include "MatrixSolver.h"
#include <Eigen/IterativeLinearSolvers>

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

MatrixSolver::VectorXd MatrixSolver::solveConjugateGradient(
    const MatrixXd& A,
    const VectorXd& b,
    int maxIterations,
    double tolerance) {
    
    Eigen::ConjugateGradient<MatrixXd> cg;
    cg.compute(A);
    
    if (maxIterations > 0) {
        cg.setMaxIterations(maxIterations);
    } else {
        cg.setMaxIterations(A.cols());
    }
    
    cg.setTolerance(tolerance);
    VectorXd x = cg.solve(b);
    
    std::cout << "ConjugateGradient Info:" << std::endl;
    std::cout << "  Iterations: " << cg.iterations() << std::endl;
    std::cout << "  Estimated error: " << cg.error() << std::endl;
    
    return x;
}

MatrixSolver::VectorXd MatrixSolver::solveGMRES(
    const MatrixXd& A,
    const VectorXd& b,
    int restart,
    int maxIterations,
    double tolerance) {
    
    // Using BiCGSTAB as GMRES alternative for general matrices
    // BiCGSTAB works for non-symmetric and indefinite matrices
    Eigen::BiCGSTAB<MatrixXd> solver;
    solver.compute(A);
    
    if (maxIterations > 0) {
        solver.setMaxIterations(maxIterations);
    } else {
        solver.setMaxIterations(A.cols());
    }
    
    solver.setTolerance(tolerance);
    VectorXd x = solver.solve(b);
    
    std::cout << "BiCGSTAB Solver Info (GMRES alternative):" << std::endl;
    std::cout << "  Restart parameter (unused for BiCGSTAB): " << restart << std::endl;
    std::cout << "  Iterations: " << solver.iterations() << std::endl;
    std::cout << "  Estimated error: " << solver.error() << std::endl;
    
    return x;
}
