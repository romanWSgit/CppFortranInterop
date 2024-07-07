# Compiler and linker settings
FC = gfortran
CXX = g++
FFLAGS = -O2 -fPIC
CXXFLAGS = -O2 -std=c++17 -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -DEIGEN_USE_BLAS
LDFLAGS = -framework Accelerate -lgfortran

# Files
FORTRAN_SRC = eigendecomposition.f90
FORTRAN_OBJ = eigendecomposition.o
CPP_SRC = eigen_interface.cpp main.cpp
CPP_OBJ = eigen_interface.o main.o
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(FORTRAN_OBJ): $(FORTRAN_SRC)
	$(FC) $(FFLAGS) -c $< -o $@

$(CPP_OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(FORTRAN_OBJ) $(CPP_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(FORTRAN_OBJ) $(CPP_OBJ) $(TARGET)
