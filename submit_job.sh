#PBS -q batch
#PBS -N test_scheduler
#PBS -l nodes=16:ppn=2
#PBS -M mneira011@gmail.com
#PBS -m abe

module load openmpi/1.8.5
cd $PBS_O_WORKDIR
NPROCS=`wc -l < $PBS_NODEFILE`
mpiexec -v -n $NPROCS ./placas.x
