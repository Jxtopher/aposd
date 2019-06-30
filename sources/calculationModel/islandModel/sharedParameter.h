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
                continued_recv = true;
                continued_send = true;         
		}
		virtual ~SharedParameter() {
            
		}

        static void recvRewardParamter(int &_continued_recv,
                                       vector<std::tuple<string, double, unsigned int>> &_rewardOp,
                                       std::mutex &_lock_mutex) {
            // MPI_Request request
            MPI_Status status;
            int order;

            do {
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                MPI_Recv(&order, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<" | order :"<<order<<endl;
                switch (order) {
                    case MPI_Order::FINISH:

                        _lock_mutex.lock();
                        _continued_recv = false;
                        _lock_mutex.unlock();

                        break;
                    case MPI_Order::COMPUTE_FITNESS: {
                        // Recv message : solution, RewardOp
                        int sizeOfmessage;  // Get size of string solution
                        double reward;  // Get reward
                        int parameter;  // Get parameter
                        
                        MPI_Probe(status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                        MPI_Get_count(&status, MPI_CHAR, &sizeOfmessage);
                        
                        char *msg = new char[sizeOfmessage + 1];  // Get solution
                        MPI_Recv(msg, sizeOfmessage, MPI_CHAR, status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);

                        MPI_Recv(&reward, 1, MPI_DOUBLE, status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get reward

                        MPI_Recv(&parameter, 1, MPI_INT, status.MPI_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get parameter

                        _lock_mutex.lock();
                        _rewardOp.push_back(std::tuple<string, double, unsigned int>(string(msg), reward, parameter));
                        _lock_mutex.unlock();

                        delete msg;
                    } break;
                    default:
                        assert("[-] Switch-case MPI_Order is not valide" && false);
                        break;
                }
               
            } while(_continued_recv);
        }

        void operator()() {
            //MPI_Status status;
            MPI_Request request;
            vector<SOL> solutions;
            _parameterSelection.reset();

            thread t(recvRewardParamter, boost::ref(continued_recv), boost::ref(rewardOp), boost::ref(lock_mutex));
            //unsigned int iteration = 0;

            // Initialisation de la premiere solution
            solutions.push_back(SOL(_launcher.initSolution()));

            do {
                // Update solutions and parameter
                lock_mutex.lock();
                if (!rewardOp.empty()) {
                    for (unsigned int i = 0 ; i < rewardOp.size() ; i++) {
                        _parameterSelection.update(std::get<1>(rewardOp[i]), std::get<2>(rewardOp[i]));
                        solutions.push_back(SOL(static_cast<string>(std::get<0>(rewardOp[i]))));
                    }
                    rewardOp.clear();
                }
                lock_mutex.unlock();
                
                // Selection solutions and parameter a utiliser
                unsigned int parameter = _parameterSelection.getParameter();
                SOL bestSolution = _selection(solutions);
                
                // Call solver
                string newSolution = string("0 : 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");//_launcher.solve(bestSolution.str(), parameter);
                solutions.push_back(SOL(newSolution));

                // Compute reward
                pair<double, unsigned int> rewardOp = _rewardComputation(bestSolution, solutions.back(), parameter);

                // Send message : solution, RewardOp
                int order = MPI_Order::COMPUTE_FITNESS;
                auto neighbours = boost::adjacent_vertices(mpi_globals_rank, this->_topologies.graph());
                for (unsigned int neighbour : make_iterator_range(neighbours)) {
                    //std::cout << mpi_globals_rank<< " has adjacent vertex " << neighbour << "\n";
                    MPI_Isend(&order, 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    
                    MPI_Isend(static_cast<const char *>(newSolution.c_str()), newSolution.size(), MPI_CHAR, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    
                    // Reward
                    MPI_Isend(&(rewardOp.first), 1, MPI_DOUBLE, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);

                    // Parameter
                    MPI_Isend(&(rewardOp.second), 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                }

                // Stop criteria
                /* if (3 < iteration++) {
                    continued_send = false;
                }*/
            } while(continued_recv && continued_send);

            // Send FINISH
            if (continued_recv == true || continued_send == false) {
                cout<<"-----------END---------"<<endl;
                int order = MPI_Order::FINISH;
                lock_mutex.lock();
                continued_recv = false;
                lock_mutex.unlock();
                for (int i = 0; i < mpi_globals_nbnodes; i++)
                    MPI_Isend(&order, 1, MPI_INT, i, MPI_TAG, MPI_COMM_WORLD, &request);
            }

            // Wait de thread
            t.join();
        }
        
	protected:
        Launcher &_launcher;
        ParameterSelection &_parameterSelection;
        RewardComputation<SOL> &_rewardComputation;
        Selection<SOL> & _selection;

        vector<std::tuple<string, double, unsigned int>> rewardOp;
        std::mutex lock_mutex;
        int continued_recv;
        int continued_send;      
};

#endif