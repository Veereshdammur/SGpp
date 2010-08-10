#############################################################################
# This file is part of sgpp, a program package making use of spatially      #
# adaptive sparse grids to solve numerical problems                         #
#                                                                           #
# Copyright (C) 2010 Alexander Heinecke (Alexander.Heinecke@mytum.de)       #
#                                                                           #
# pysgpp is free software; you can redistribute it and/or modify            #
# it under the terms of the GNU Lesser General Public License as published  #
# by the Free Software Foundation; either version 3 of the License, or      #
# (at your option) any later version.                                       #
#                                                                           #
# pysgpp is distributed in the hope that it will be useful,                 #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU Lesser General Public License for more details.                       #
#                                                                           #
# You should have received a copy of the GNU Lesser General Public License  #
# along with pysgpp; if not, write to the Free Software                     #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA #
# or see <http://www.gnu.org/licenses/>.                                    #
#############################################################################

SRCDIR=./../../../src/sgpp
###################################################################
# Default Variables, overwirtten by CLI
###################################################################	
# use OpenMP Version 3
OMP=0
# use the TR1 Implementations for Hashmaps
TR1=0
# default compiler: g++; possible values: icpc (Intel Compiler) or xlc++_r (IBM Compiler)
CC=g++

###################################################################
# Compiler Flags
###################################################################	
CFLAGS_GCC=-Wall -pedantic -ansi -c -Wno-long-long -fno-strict-aliasing -O3 -funroll-loops -ffloat-store -I$(SRCDIR)
LFLAGS_GCC=-Wall -pedantic -ansi -O3
CFLAGS_GCC_TRONE=-Wall -pedantic -ansi -c -Wno-long-long -fno-strict-aliasing -O3 -funroll-loops -ffloat-store -I$(SRCDIR) -DUSETRONE -std=c++0x
LFLAGS_GCC_TRONE=-Wall -pedantic -ansi -O3

CFLAGS_GCC_OMP=-Wall -pedantic -ansi -c -Wno-long-long -fno-strict-aliasing -O3 -funroll-loops -ffloat-store -I$(SRCDIR) -fopenmp -DUSEOMP -DUSEOMPTHREE
LFLAGS_GCC_OMP=-Wall -pedantic -ansi -O3 -pthread -fopenmp
CFLAGS_GCC_OMP_TRONE=-Wall -pedantic -ansi -c -Wno-long-long -fno-strict-aliasing -O3 -funroll-loops -ffloat-store -I$(SRCDIR) -fopenmp -DUSEOMP -DUSEOMPTHREE -DUSETRONE -Isrc/sgpp/
LFLAGS_GCC_OMP_TRONE=-Wall -pedantic -ansi -O3 -fopenmp

CFLAGS_ICC=-Wall -ansi -c -fno-strict-aliasing -fno-alias -ansi-alias -O3 -funroll-loops -I$(SRCDIR) -DUSEICCINTRINSICS
LFLAGS_ICC=-Wall -ansi -O3 -static-intel
CFLAGS_ICC_TRONE=-Wall -ansi -c -fno-strict-aliasing -fno-alias -ansi-alias -O3 -funroll-loops -I$(SRCDIR) -DUSETRONE -DUSEICCINTRINSICS -std=c++0x
LFLAGS_ICC_TRONE=-Wall -ansi -O3 -static-intel

CFLAGS_ICC_OMP=-ansi -c -fno-strict-aliasing -fno-alias -ansi-alias -O3 -funroll-loops -I$(SRCDIR) -openmp -DUSEOMP -DUSEOMPTHREE -DUSEICCINTRINSICS
LFLAGS_ICC_OMP=-Wall -ansi -O3 -static-intel -openmp -openmp-link static
CFLAGS_ICC_OMP_TRONE=-Wall -ansi -c -fno-strict-aliasing -fno-alias -ansi-alias -O3 -funroll-loops -I$(SRCDIR) -openmp -DUSEOMP -DUSEOMPTHREE -DUSETRONE -DUSEICCINTRINSICS -std=c++0x
LFLAGS_ICC_OMP_TRONE=-Wall -ansi -O3 -static-intel -openmp -openmp-link static

CFLAGS_XLC=-O3 -qstrict -qtune=auto -qarch=auto -qansialias -c -I$(SRCDIR) -DAIX_XLC -DUSETRONE
LFLAGS_XLC=

CFLAGS_XLC_OMP=-O3 -qstrict -qtune=auto -qarch=auto -qansialias -c -I$(SRCDIR) -DAIX_XLC -DUSETRONE -DUSEOMP -DUSEOMPTHREE -qsmp=omp
LFLAGS_XLC_OMP=-qsmp=omp

###################################################################
# Builds a lib containing all SG Algorithms
###################################################################	
default:
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/sgpplib_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a"
else
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a"
else
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a"
endif
endif
endif
ifeq ($(CC),icpc)
	mkdir -p tmp/build_native/sgpplib_icc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC)" "LFLAGS=$(LFLAGS_ICC)" "LIBNAME=libsgpp_icc.a"
else
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_TRONE)" "LFLAGS=$(LFLAGS_ICC_TRONE)" "LIBNAME=libsgpp_icc.a"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP)" "LFLAGS=$(LFLAGS_ICC_OMP)" "LIBNAME=libsgpp_icc.a"
else	
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP_TRONE)" "LFLAGS=$(LFLAGS_ICC_OMP_TRONE)" "LIBNAME=libsgpp_icc.a"
endif	
endif
endif
ifeq ($(CC),xlc++_r)
	mkdir -p tmp/build_native/sgpplib_xlc
ifeq ($(OMP),0)
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC)" "LFLAGS=$(LFLAGS_XLC)" "LIBNAME=libsgpp_xlc.a"
else
	make -f ./../../../src/makefileSGppLIB --directory=./tmp/build_native/sgpplib_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC_OMP)" "LFLAGS=$(LFLAGS_XLC_OMP)" "LIBNAME=libsgpp_xls.a"
endif
endif

###################################################################
# Builds a Balck Scholes Solver
###################################################################	
BSSolver: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/BSSolver_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=BSSolver_GCC"
else
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=BSSolver_GCC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=BSSolver_GCC"
else
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=BSSolver_GCC"
endif
endif
endif
ifeq ($(CC),icpc)
	mkdir -p tmp/build_native/BSSolver_icc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC)" "LFLAGS=$(LFLAGS_ICC)" "LIBNAME=libsgpp_icc.a" "BINNAME=BSSolver_ICC"
else
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_TRONE)" "LFLAGS=$(LFLAGS_ICC_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=BSSolver_ICC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP)" "LFLAGS=$(LFLAGS_ICC_OMP)" "LIBNAME=libsgpp_icc.a" "BINNAME=BSSolver_ICC"
else	
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP_TRONE)" "LFLAGS=$(LFLAGS_ICC_OMP_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=BSSolver_ICC"
endif	
endif
endif
ifeq ($(CC),xlc++_r)
	mkdir -p tmp/build_native/BSSolver_xlc
ifeq ($(OMP),0)
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC)" "LFLAGS=$(LFLAGS_XLC)" "LIBNAME=libsgpp_xlc.a" "BINNAME=BSSolver_XLC"
else
	make -f ./../../../src/makefileNativeBlackScholesSolver --directory=./tmp/build_native/BSSolver_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC_OMP)" "LFLAGS=$(LFLAGS_XLC_OMP)" "LIBNAME=libsgpp_xlc.a" "BINNAME=BSSolver_XLC"
endif
endif

###################################################################
# Builds a Hull White Solver
###################################################################	
HWSolver: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/HWSolver_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HWSolver_GCC"
else
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HWSolver_GCC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HWSolver_GCC"
else
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HWSolver_GCC"
endif
endif
endif
ifeq ($(CC),icpc)
	mkdir -p tmp/build_native/HWSolver_icc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC)" "LFLAGS=$(LFLAGS_ICC)" "LIBNAME=libsgpp_icc.a" "BINNAME=HWSolver_ICC"
else
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_TRONE)" "LFLAGS=$(LFLAGS_ICC_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=HWSolver_ICC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP)" "LFLAGS=$(LFLAGS_ICC_OMP)" "LIBNAME=libsgpp_icc.a" "BINNAME=HWSolver_ICC"
else	
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP_TRONE)" "LFLAGS=$(LFLAGS_ICC_OMP_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=HWSolver_ICC"
endif	
endif
endif
ifeq ($(CC),xlc++_r)
	mkdir -p tmp/build_native/HWSolver_xlc
ifeq ($(OMP),0)
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC)" "LFLAGS=$(LFLAGS_XLC)" "LIBNAME=libsgpp_xlc.a" "BINNAME=HWSolver_XLC"
else
	make -f ./../../../src/makefileNativeHullWhiteSolver --directory=./tmp/build_native/HWSolver_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC_OMP)" "LFLAGS=$(LFLAGS_XLC_OMP)" "LIBNAME=libsgpp_xlc.a" "BINNAME=HWSolver_XLC"
endif
endif

###################################################################
# Builds a simple Heat Equation Solver
###################################################################	
HESolver: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/HESolver_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HESolver_GCC"
else
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HESolver_GCC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HESolver_GCC"
else
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=HESolver_GCC"
endif
endif
endif
ifeq ($(CC),icpc)
	mkdir -p tmp/build_native/HESolver_icc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC)" "LFLAGS=$(LFLAGS_ICC)" "LIBNAME=libsgpp_icc.a" "BINNAME=HESolver_ICC"
else
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_TRONE)" "LFLAGS=$(LFLAGS_ICC_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=HESolver_ICC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP)" "LFLAGS=$(LFLAGS_ICC_OMP)" "LIBNAME=libsgpp_icc.a" "BINNAME=HESolver_ICC"
else
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP_TRONE)" "LFLAGS=$(LFLAGS_ICC_OMP_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=HESolver_ICC"
endif	
endif
endif
ifeq ($(CC),xlc++_r)
	mkdir -p tmp/build_native/HESolver_xlc
ifeq ($(OMP),0)
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC)" "LFLAGS=$(LFLAGS_XLC)" "LIBNAME=libsgpp_xlc.a" "BINNAME=HESolver_XLC"
else
	make -f ./../../../src/makefileNativeHeatEquationSolver --directory=./tmp/build_native/HESolver_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC_OMP)" "LFLAGS=$(LFLAGS_XLC_OMP)" "LIBNAME=libsgpp_xlc.a" "BINNAME=HESolver_XLC"
endif
endif

###################################################################
# Builds a (very) simple Classifier
###################################################################
Classifier: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/Classifier_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=Classifier_GCC"
else
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=Classifier_GCC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=Classifier_GCC"
else
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=Classifier_GCC"
endif
endif
endif
ifeq ($(CC),icpc)
	mkdir -p tmp/build_native/Classifier_icc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC)" "LFLAGS=$(LFLAGS_ICC)" "LIBNAME=libsgpp_icc.a" "BINNAME=Classifier_ICC"
else
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_TRONE)" "LFLAGS=$(LFLAGS_ICC_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=Classifier_ICC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP)" "LFLAGS=$(LFLAGS_ICC_OMP)" "LIBNAME=libsgpp_icc.a" "BINNAME=Classifier_ICC"
else	
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP_TRONE)" "LFLAGS=$(LFLAGS_ICC_OMP_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=Classifier_ICC"
endif	
endif
endif
ifeq ($(CC),xlc++_r)
	mkdir -p tmp/build_native/Classifier_xlc
ifeq ($(OMP),0)
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC)" "LFLAGS=$(LFLAGS_XLC)" "LIBNAME=libsgpp_xlc.a" "BINNAME=Classifier_XLC"
else
	make -f ./../../../src/makefileNativeClassifier --directory=./tmp/build_native/Classifier_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC_OMP)" "LFLAGS=$(LFLAGS_XLC_OMP)" "LIBNAME=libsgpp_xlc.a" "BINNAME=Classifier_XLC"
endif
endif

###################################################################
# Builds a VTuneTest Application
###################################################################	
VTuneTest: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/VTuneTest_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=VTuneTest_GCC"
else
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=VTuneTest_GCC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=VTuneTest_GCC"
else
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=VTuneTest_GCC"
endif
endif
endif
ifeq ($(CC),icpc)
	mkdir -p tmp/build_native/VTuneTest_icc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC)" "LFLAGS=$(LFLAGS_ICC)" "LIBNAME=libsgpp_icc.a" "BINNAME=VTuneTest_ICC"
else
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_TRONE)" "LFLAGS=$(LFLAGS_ICC_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=VTuneTest_ICC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP)" "LFLAGS=$(LFLAGS_ICC_OMP)" "LIBNAME=libsgpp_icc.a" "BINNAME=VTuneTest_ICC"
else	
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_icc "CC=$(CC)" "CFLAGS=$(CFLAGS_ICC_OMP_TRONE)" "LFLAGS=$(LFLAGS_ICC_OMP_TRONE)" "LIBNAME=libsgpp_icc.a" "BINNAME=VTuneTest_ICC"
endif	
endif
endif
ifeq ($(CC),xlc++_r)
	mkdir -p tmp/build_native/VTuneTest_xlc
ifeq ($(OMP),0)
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC)" "LFLAGS=$(LFLAGS_XLC)" "LIBNAME=libsgpp_xlc.a" "BINNAME=VTuneTest_XLC"
else
	make -f ./../../../src/makefileNativeVTuneTest --directory=./tmp/build_native/VTuneTest_xlc "CC=$(CC)" "CFLAGS=$(CFLAGS_XLC_OMP)" "LFLAGS=$(LFLAGS_XLC_OMP)" "LIBNAME=libsgpp_xlc.a" "BINNAME=VTuneTest_XLC"
endif
endif

###################################################################
# Builds a Up/Down Test Application
###################################################################	
UpDownTest: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/UpDownTest_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileUpDownTest --directory=./tmp/build_native/UpDownTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=UpDownTest_GCC"
else
	make -f ./../../../src/makefileUpDownTest --directory=./tmp/build_native/UpDownTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=UpDownTest_GCC"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileUpDownTest --directory=./tmp/build_native/UpDownTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=UpDownTest_GCC"
else
	make -f ./../../../src/makefileUpDownTest --directory=./tmp/build_native/UpDownTest_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=UpDownTest_GCC"
endif
endif
endif

###################################################################
# Builds a Refine/Coarsen Test Application
###################################################################	
RefineCoarsenTest: default
ifeq ($(CC),g++)
	mkdir -p tmp/build_native/RefineCoarsen_gcc
ifeq ($(OMP),0)
ifeq ($(TR1),0)
	make -f ./../../../src/makefileRefineCoarsenTest --directory=./tmp/build_native/RefineCoarsen_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC)" "LFLAGS=$(LFLAGS_GCC)" "LIBNAME=libsgpp_gcc.a" "BINNAME=RefineCoarsen_gcc"
else
	make -f ./../../../src/makefileRefineCoarsenTest --directory=./tmp/build_native/RefineCoarsen_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_TRONE)" "LFLAGS=$(LFLAGS_GCC_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=RefineCoarsen_gcc"
endif
else
ifeq ($(TR1),0)
	make -f ./../../../src/makefileRefineCoarsenTest --directory=./tmp/build_native/RefineCoarsen_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP)" "LFLAGS=$(LFLAGS_GCC_OMP)" "LIBNAME=libsgpp_gcc.a" "BINNAME=RefineCoarsen_gcc"
else
	make -f ./../../../src/makefileRefineCoarsenTest --directory=./tmp/build_native/RefineCoarsen_gcc "CC=$(CC)" "CFLAGS=$(CFLAGS_GCC_OMP_TRONE)" "LFLAGS=$(LFLAGS_GCC_OMP_TRONE)" "LIBNAME=libsgpp_gcc.a" "BINNAME=RefineCoarsen_gcc"
endif
endif
endif
		
clean:
	rm -rdfv tmp/build_native
