PROGRAM project1
  implicit None

  integer :: n
  real :: h
  integer :: i
  real(DP), allocatable, dimension(:,:) :: x
  n = 10
  h = 1.0/(n+1)
  allocate (x(n,n))
  print *, "Solving ", n, "×", n, "with stepsize", h
  print *, x
end program project1
