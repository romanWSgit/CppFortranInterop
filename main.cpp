/**
 * @file eigen_interface.cpp
 * @author Roman Wallner-Silberhubere
 * @date 07.07.2024
 * @brief This file contains function definitions for the eigen_decomposition
 * method in the Eigen Interface.
 *
 * This file provides an interface between the Eigen library in C++ and the
 * Lapack's `dgeev` function for eigen decomposition. It includes the definition
 * of the function `eigen_decomposition`. The function uses the Eigen library to
 * abstract the complexity of matrix operations and uses Lapack's function
 * `dgeev` to perform eigen decomposition. The function interface has been
 * defined in eigen_interface.h.
 *
 * If `dgeev` fails it throws a runtime_error exception with the error code
 * returned by `dgeev`.
 */

#include "eigen_interface.h"
#include <Eigen/Dense>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

/**
 * @brief Check the relative reconstruction error of a matrix decomposition.
 *
 * This method takes in a matrix A, a vector W, a matrix V, a string method, an
 * output file stream and a double variable for relative error. It reconstructs
 * the matrix A using the decomposition given by the input W and V matrices, and
 * calculates the relative reconstruction error as the norm of the difference
 * between A and the reconstructed matrix divided by the norm of A. It then
 * prints the method and relative reconstruction error to the console, and
 * writes them to the output file.
 *
 * @param A The input matrix for decomposition.
 * @param W The input vector representing part of the decomposition.
 * @param V The input matrix representing part of the decomposition.
 * @param method The name of the decomposition method.
 * @param outfile An output file stream to write the results to.
 * @param relative_error The double variable to store the calculated relative
 * error.
 */
void check_decomposition(const Eigen::MatrixXd &A, const Eigen::VectorXd &W,
                         const Eigen::MatrixXd &V, const std::string &method,
                         std::ofstream &outfile, double &relative_error)
{
    Eigen::MatrixXd A_reconstructed = V * W.asDiagonal() * V.inverse();
    relative_error = (A - A_reconstructed).norm() / A.norm();
    std::cout << method << " relative reconstruction error: " << relative_error
              << std::endl;
    outfile << method << " relative reconstruction error: " << relative_error
            << std::endl;
}

/**
 * @brief Compute the condition number of a matrix.
 *
 * This method takes in a matrix A and computes its condition number. The
 * condition number of a matrix is the ratio of the largest singular value to
 * the smallest singular value.
 *
 * @param A The input matrix for which to compute the condition number.
 *
 * @return The computed condition number.
 */
double compute_condition_number(const Eigen::MatrixXd &A)
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
    double cond_number =
        svd.singularValues()(0) / svd.singularValues().tail(1)(0);
    return cond_number;
}

/**
 * @brief The main entry point of the program.
 *
 * This method takes in the command line arguments, reads a matrix size from the
 * user, generates a random matrix of the given size, and performs matrix
 * decomposition using two different methods - Fortran LAPACK and Eigen Library.
 * It compares the results and outputs the duration, condition number, maximum
 * differences, relative reconstruction errors, and the faster method to the
 * console and an output file.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of strings representing the command line arguments.
 * @return Returns 0 if the program executed successfully, 1 otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <use_lapack_in_eigen: 0 or 1>"
                  << std::endl;
        return 1;
    }

    bool use_lapack_in_eigen = std::stoi(argv[1]);
    std::ofstream outfile("results.txt");

    int size;
    std::cout << "Enter the size of the matrix: ";
    std::cin >> size;

    Eigen::MatrixXd A =
        Eigen::MatrixXd::Random(size, size); // Generate a random matrix

    // Check condition number and prompt user if it's poor
    const double condition_number_threshold = 1e6;
    double cond_number = compute_condition_number(A);
    std::cout << "Condition number of the matrix: " << cond_number << std::endl;
    outfile << "Condition number of the matrix: " << cond_number << std::endl;

    char regenerate;
    if (cond_number > condition_number_threshold)
    {
        std::cout << "Condition number is poor. Regenerate matrix for better "
                     "condition number? (y/n): ";
        std::cin >> regenerate;

        while (regenerate == 'y' || regenerate == 'Y')
        {
            A = Eigen::MatrixXd::Random(size, size);
            cond_number = compute_condition_number(A);
            std::cout << "Condition number of the new matrix: " << cond_number
                      << std::endl;
            outfile << "Condition number of the new matrix: " << cond_number
                    << std::endl;
            if (cond_number <= condition_number_threshold)
                break;
            std::cout << "Regenerate again? (y/n): ";
            std::cin >> regenerate;
        }
    }

    Eigen::VectorXd W_fortran(size);
    Eigen::MatrixXd V_fortran(size, size);

    Eigen::VectorXd W_eigen(size);
    Eigen::MatrixXd V_eigen(size, size);

    double duration_fortran, duration_eigen;
    double relative_error_fortran, relative_error_eigen;
    double max_diff_values, max_diff_vectors;

    // Fortran LAPACK Eigenvalue Decomposition
    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        eigen_decomposition(A, W_fortran, V_fortran);
        auto end = std::chrono::high_resolution_clock::now();
        duration_fortran = std::chrono::duration<double>(end - start).count();
        std::cout << "Fortran LAPACK Duration: " << duration_fortran
                  << " seconds" << std::endl;
        outfile << "Fortran LAPACK Duration: " << duration_fortran << " seconds"
                << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in Fortran LAPACK eigenvalue decomposition: "
                  << e.what() << std::endl;
        return 1;
    }
    check_decomposition(A, W_fortran, V_fortran, "Fortran LAPACK", outfile,
                        relative_error_fortran);

    // Eigen Library Eigenvalue Decomposition
    if (use_lapack_in_eigen)
    {
        // With LAPACK
        try
        {
            auto start = std::chrono::high_resolution_clock::now();
            Eigen::EigenSolver<Eigen::MatrixXd> solver(A);
            W_eigen = solver.eigenvalues().real();
            V_eigen = solver.eigenvectors().real();
            auto end = std::chrono::high_resolution_clock::now();
            duration_eigen = std::chrono::duration<double>(end - start).count();
            std::cout << "Eigen Library (with LAPACK) Duration: "
                      << duration_eigen << " seconds" << std::endl;
            outfile << "Eigen Library (with LAPACK) Duration: "
                    << duration_eigen << " seconds" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in Eigen library eigenvalue decomposition: "
                      << e.what() << std::endl;
            return 1;
        }
    }
    else
    {
        // Without LAPACK
        try
        {
            auto start = std::chrono::high_resolution_clock::now();
            Eigen::EigenSolver<Eigen::MatrixXd> solver(A);
            W_eigen = solver.eigenvalues().real();
            V_eigen = solver.eigenvectors().real();
            auto end = std::chrono::high_resolution_clock::now();
            duration_eigen = std::chrono::duration<double>(end - start).count();
            std::cout << "Eigen Library (without LAPACK) Duration: "
                      << duration_eigen << " seconds" << std::endl;
            outfile << "Eigen Library (without LAPACK) Duration: "
                    << duration_eigen << " seconds" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in Eigen library eigenvalue decomposition: "
                      << e.what() << std::endl;
            return 1;
        }
    }
    check_decomposition(A, W_eigen, V_eigen, "Eigen Library", outfile,
                        relative_error_eigen);

    max_diff_values = (W_fortran - W_eigen).cwiseAbs().maxCoeff();
    max_diff_vectors = (V_fortran - V_eigen).cwiseAbs().maxCoeff();

    // Output final comparison
    std::cout << "\nSummary:\n";
    std::cout << "Chosen method: "
              << (use_lapack_in_eigen ? "Eigen with LAPACK"
                                      : "Eigen without LAPACK")
              << "\n";
    std::cout << "Matrix size: " << size << "\n";
    std::cout << "Fortran LAPACK Duration: " << duration_fortran
              << " seconds\n";
    std::cout << "Eigen Library Duration: " << duration_eigen << " seconds\n";
    std::cout
        << "Maximum difference between Fortran LAPACK and Eigen eigenvalues: "
        << max_diff_values << "\n";
    std::cout
        << "Maximum difference between Fortran LAPACK and Eigen eigenvectors: "
        << max_diff_vectors << "\n";
    std::cout << "Fortran LAPACK relative reconstruction error: "
              << relative_error_fortran << "\n";
    std::cout << "Eigen Library relative reconstruction error: "
              << relative_error_eigen << "\n";
    std::cout << (duration_fortran < duration_eigen ? "Fortran LAPACK"
                                                    : "Eigen Library")
              << " was faster by "
              << std::abs(duration_eigen - duration_fortran) << " seconds\n";

    outfile << "\nSummary:\n";
    outfile << "Chosen method: "
            << (use_lapack_in_eigen ? "Eigen with LAPACK"
                                    : "Eigen without LAPACK")
            << "\n";
    outfile << "Matrix size: " << size << "\n";
    outfile << "Fortran LAPACK Duration: " << duration_fortran << " seconds\n";
    outfile << "Eigen Library Duration: " << duration_eigen << " seconds\n";
    outfile
        << "Maximum difference between Fortran LAPACK and Eigen eigenvalues: "
        << max_diff_values << "\n";
    outfile
        << "Maximum difference between Fortran LAPACK and Eigen eigenvectors: "
        << max_diff_vectors << "\n";
    outfile << "Fortran LAPACK relative reconstruction error: "
            << relative_error_fortran << "\n";
    outfile << "Eigen Library relative reconstruction error: "
            << relative_error_eigen << "\n";
    outfile << (duration_fortran < duration_eigen ? "Fortran LAPACK"
                                                  : "Eigen Library")
            << " was faster by " << std::abs(duration_eigen - duration_fortran)
            << " seconds\n";

    outfile.close();
    return 0;
}
