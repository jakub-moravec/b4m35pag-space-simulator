#!/bin/bash
#PBS -S /bin/bash
#PBS -N stars
#PBS -l walltime=01:59:00
#PBS -l select=1:ncpus=32:mem=200mb:scratch_local=10mb

DATADIR="$PBS_O_WORKDIR"
cp $DATADIR/*.cpp $SCRATCHDIR
cp $DATADIR/*.h $SCRATCHDIR
cp $DATADIR/input/* $SCRATCHDIR
cd $SCRATCHDIR || exit 1

source /software/modules/init
module load intelcdk-17.1

icpc -fopenmp -fopt-info-vec -std=c++11 -march=native -Ofast -qopt-report=1 -qopt-report-file:stdout -qopt-report-phase=vec -o stars main.cpp

echo -e "\nBenchmark started..."

for thread in 1 2 4 8 16 32 
do 
    export OMP_NUM_THREADS="$thread"
    echo "$thread"
    ./stars $DATADIR/input/1000
done

echo "Benchmark finished..."

