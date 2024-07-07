/**
 * @file eigen_interface.cpp
 * @author Roman Wallner-Silberhubere
 * @date 07.07.2024
 * @brief This file contains function definitions for calculating the eigen
 * decomposition of a matrix using the LAPACK library.
 *
 * This C++ interface file provides a wrapper around the Fortran LAPACK
 * library's `eigen_decomposition` routine. It works with the Eigen library for
 * matrix operations, and has error checking built in: if the LAPACK function
 * call fails, a runtime_error exception is thrown to indicate the failure.
 *
 * It comprises only a single function, `eigen_decomposition`, which takes as
 * input a Matrix of any dimensions and prepares it for the LAPACK call, then
 * throws an error if the operation is not successful.
 */

#include "eigen_interface.h"
#include <stdexcept>

void eigen_decomposition(const Eigen::MatrixXd &A, Eigen::VectorXd &W,
                         Eigen::MatrixXd &V)
{
    int n = A.rows();
    Eigen::MatrixXd A_copy = A;
    int info;
    W.resize(n);    // Ensure the output vector is resized
    V.resize(n, n); // Ensure the output matrix is resized

    // Call the Fortran subroutine
    eigen_decomposition(n, A_copy.data(), W.data(), V.data(), &info);

    if (info != 0)
    {
        throw std::runtime_error(
            "LAPACK eigen_decomposition failed with info code: " +
            std::to_string(info));
    }
}
