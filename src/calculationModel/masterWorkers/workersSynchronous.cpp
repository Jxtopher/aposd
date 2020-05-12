#include "calculationModel/masterWorkers/workersSynchronous.hpp"

WorkersSynchronous::WorkersSynchronous(std::unique_ptr<Launcher> launcher) : _launcher(std::move(launcher)) {}
WorkersSynchronous::~WorkersSynchronous() {}

void WorkersSynchronous::operator()() {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " Workers";

	int continued = true;
	int order;

	while(continued) {
		MPI_Recv(&order, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
		switch(order) {
			case MPI_Order::INIT_SOLUTION: {
				std::string s = _launcher->initSolution();
				MPI_Isend((char*)s.c_str(), s.size(), MPI_CHAR, MPI_MASTER, MPI_TAG,
							MPI_COMM_WORLD, &request);
			} break;
			case MPI_Order::COMPUTE_FITNESS: {
				// Recv
				int sizeOfmessage; // Get size of string solution
				int parameter;	 // Get parameter

				MPI_Probe(MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
				MPI_Get_count(&status, MPI_CHAR, &sizeOfmessage);

				char* msg = new char[sizeOfmessage + 1]; // Get solution
				MPI_Recv(msg, sizeOfmessage, MPI_CHAR, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD,
							&status);

				MPI_Recv(&parameter, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD,
							&status);

				// Call solver
				std::string s = _launcher->solve(msg, parameter);
				delete msg;

				// Send message
				MPI_Isend((char*)s.c_str(), s.size(), MPI_CHAR, MPI_MASTER, MPI_TAG,
							MPI_COMM_WORLD, &request);
			} break;
			case MPI_Order::FINISH: continued = false; break;
			default:
				throw std::runtime_error(std::string(__FILE__) + ":" +
											std::to_string(__LINE__) +
											" [-] Switch-case MPI_Order is not valide");
				break;
		}
	}
}