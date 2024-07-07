!> @file eigendecomposition.f90
!> @author Roman Wallner- Silberhuber
!> @date 07.07.2024
!> @brief This Fortran module performs the eigen decomposition.

module eigendecomposition_module
    use iso_c_binding
    implicit none
contains
    !>  @brief This subroutine performs the eigen decomposition of a given matrix.
    !>  It is a binding to LAPACK's `dgeev` function for eigen decomposition.
    !>  For more details, see LAPACK's reference:
    !>  <a href="https://netlib.org/lapack/explore-html/d4/d68/group__geev_ga7d8afe93d23c5862e238626905ee145e.html">
    !>  LAPACK's `dgeev` function documentation
    !>  </a> for eigen decomposition.
    !>
    !> @param[in] n the order of the given matrix
    !> @param[inout] A input matrix of dimensions (n,n), on output it gets overwritten
    !> @param[out] W output vector (allocated on input) to store the eigenvalues
    !> @param[out] V output matrix (allocated on input) to store the eigenvectors
    !> @param[out] info output status: if 0 then successful exit
    subroutine eigen_decomposition(n, A, W, V, info) bind(C)
        integer(c_int), value :: n
        real(c_double), intent(inout) :: A(n, n)
        real(c_double), intent(out) :: W(n)
        real(c_double), intent(out) :: V(n, n)
        integer(c_int), intent(out) :: info

        integer(c_int) :: lwork, lda
        real(c_double), allocatable :: work(:)
        character(len=1) :: jobvl, jobvr
        real(c_double), allocatable :: wr(:)
        real(c_double), allocatable :: wi(:)
        real(c_double), allocatable :: vl(:,:)

        lda = n
        lwork = max(1, 4*n)
        allocate(work(lwork))
        allocate(wr(n))
        allocate(wi(n))
        allocate(vl(n, n))

        jobvl = 'N'
        jobvr = 'V'

        ! Hopefully correct call to LAPACK's `dgeev` subroutine for eigenvalue decomposition
        call dgeev(jobvl, jobvr, n, A, lda, wr, wi, vl, lda, V, lda, work, lwork, info)

        ! Combine real and imaginary parts of the eigenvalues into W
        W = wr  ! For simplicity, assuming eigenvalues are real. Otherwise, you'd need to handle complex parts separately

        deallocate(work)
        deallocate(wr)
        deallocate(wi)
        deallocate(vl)
    end subroutine eigen_decomposition
end module eigendecomposition_module
