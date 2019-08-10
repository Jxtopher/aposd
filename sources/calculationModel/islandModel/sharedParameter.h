#ifndef JXTOPHER_SHAREDPARAMETER_H
#define JXTOPHER_SHAREDPARAMETER_H

#include <mutex>
#include <thread>
#include <tuple>

#include "islandModel.h"

using namespace std;

template <class SOL>
class SharedParameter : public IslandModel<SOL> {
	public:
		SharedParameter(int argc, char** argv, 
                        Topologies &topologies, 
                        Launcher &launcher, 
                        ParameterSelection &parameterSelection, 
                        RewardComputation<SOL> &rewardComputation,
                        Selection<SOL> &selection) : 
            IslandModel<SOL>(argc, argv, topologies),
            _launcher(launcher),
            _parameterSelection(parameterSelection),
            _rewardComputation(rewardComputation),
            _selection(selection)  {
                continued = true;
                recv_msg_size = 10;   
                recv_msg = new char[recv_msg_size];
		}
		virtual ~SharedParameter() {
            delete recv_msg;
		}

        void operator()() {
            // Initialisation des varaibles
            auto neighbours = boost::adjacent_vertices(mpi_globals_rank, this->_topologies.graph());
            solutions.clear();
            _parameterSelection.reset();

            unsigned int iteration = 0;

            // Initialisation de la premiere solution
            solutions.push_back(SOL(_launcher.initSolution()));

            do {                
                // Selection solutions and parameter a utiliser
                unsigned int parameter = _parameterSelection.getParameter();
                SOL bestSolution = _selection(solutions);
                cout<<bestSolution<<endl;

                // Call solver
                string newSolution = _launcher.solve(bestSolution.str(), parameter);
                solutions.push_back(SOL(newSolution));
                
                // Compute reward
                pair<double, unsigned int> rewardOp = _rewardComputation(bestSolution, solutions.back(), parameter);

                // Send message : solution, RewardOp
                order = MPI_Order::COMPUTE_FITNESS;
                
                for (unsigned int neighbour : make_iterator_range(neighbours)) {
                    //std::cout << mpi_globals_rank<< " has adjacent vertex " << neighbour << "\n";
                    MPI_Isend(&order, 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);

                    // Send solution
                    MPI_Isend(static_cast<const char *>(newSolution.c_str()), newSolution.size(), MPI_CHAR, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    
                    // Send reward
                    MPI_Isend(&(rewardOp.first), 1, MPI_DOUBLE, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);

                    // Send parameter
                    MPI_Isend(&(rewardOp.second), 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                }

                // Reception des messages
                for (unsigned int neighbour : make_iterator_range(neighbours)) {
                    MPI_Recv(&order, 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &status);
                    switch (order) {
                        case MPI_Order::FINISH:
                            continued = false;
                            break;
                        case MPI_Order::COMPUTE_FITNESS: {
                            MPI_Probe(status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                            MPI_Get_count(&status, MPI_CHAR, &recv_sizeOfmessage);
                            
                            if ((recv_sizeOfmessage + 1) != recv_msg_size) {
                                delete recv_msg;
                                recv_msg = new char[recv_sizeOfmessage + 1]; 
                                recv_msg_size =  recv_sizeOfmessage + 1;
                            }
                            
                            MPI_Recv(recv_msg, recv_sizeOfmessage, MPI_CHAR, status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get solution

                            MPI_Recv(&recv_reward, 1, MPI_DOUBLE, status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get reward

                            MPI_Recv(&recv_parameter, 1, MPI_INT, status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get parameter
                                                        
                            // Update solutions and parameter
                            _parameterSelection.update(recv_reward, recv_parameter);
                            solutions.push_back(SOL(recv_msg));
                            
                            } break;
                        default:
                            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] Switch-case MPI_Order is not valide");
                            break;
                    }
                }

                // Stop criteria
                if (100 < iteration++ && continued != false) {
                    order = MPI_Order::FINISH;
                    for (unsigned int neighbour : make_iterator_range(neighbours))
                        MPI_Isend(&order, 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    continued = false;
                }
            } while(continued);
        }
        
	private:
        Launcher &_launcher;
        ParameterSelection &_parameterSelection;
        RewardComputation<SOL> &_rewardComputation;
        Selection<SOL> & _selection;

        vector<SOL> solutions;  // liste des solutions candidat
        
        int continued;          // loop
        int order;              // order to send

        MPI_Status status;
        MPI_Request request;

        double recv_reward;     // Get reward by recv
        int recv_parameter;     // Get parameter by recv
        int recv_sizeOfmessage; // Get size of string solution by recv
        char *recv_msg;         // Get string solution by recv
        int recv_msg_size;      // size of recv_msg
};

#endif