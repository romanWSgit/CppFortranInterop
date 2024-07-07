/**
 * @file eigen_interface.h
 * @author Roman Wallner-Silberhubere
 * @date 07.07.2024
 * @brief This header file defines the interface for eigenvalue decomposition.
 *
 * This file provides the function declaration for `eigen_decomposition` which
 * is used to calculate the eigenvalues and eigenvectors of a given square
 * matrix. It utilizes the LAPACK library's `dgeev` function for eigenvalue
 * decomposition. The interface works with the Eigen library for easy handling
 * of matrices and vectors in C++.
 */

#ifndef EIGEN_INTERFACE_H
#define EIGEN_INTERFACE_H

#include <Eigen/Dense>

extern "C"
{
    void eigen_decomposition(int n, double *A, double *W, double *V, int *info);
}

/**
 * @brief Compute the eigenvalues and eigenvectors of a square matrix.
 *
 * This function uses the LAPACK library's `dgeev` function to perform
 * eigenvalue decomposition. It takes a square matrix as input and outputs the
 * eigenvalues and the corresponding eigenvectors. If computation fails, an
 * error code is returned.
 *
 * @param A The input matrix for eigenvalue decomposition.
 * @param W The vector that will store the computed eigenvalues.
 * @param V The matrix that will store the computed eigenvectors.
 */
void eigen_decomposition(const Eigen::MatrixXd &A, Eigen::VectorXd &W,
                         Eigen::MatrixXd &V);

#endif // EIGEN_INTERFACE_H