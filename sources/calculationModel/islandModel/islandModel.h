#ifndef JXTOPHER_ISLANDMODEL_H
#define JXTOPHER_ISLANDMODEL_H

#include <mpi.h>

#include "../calculationModel.h"
#include "topologies/topologies.h"

using namespace std;


class IslandModel : public CalculationModel{
	public:
		IslandModel(int argc, char** argv, Topologies &topologies) : 
            _topologies(topologies) {
            // Variable MPI init
            MPI_Init(&argc, &argv);
            MPI_Comm_size(MPI_COMM_WORLD, &mpi_globals_nbnodes);
            MPI_Comm_rank(MPI_COMM_WORLD, &mpi_globals_rank);
            MPI_Get_processor_name(mpi_globals_name, &mpi_globals_namelen);
		}
		virtual ~IslandModel() {
			MPI_Finalize();
		}

        virtual void operator()() = 0;

	protected:
        Topologies & _topologies;
};


#endif