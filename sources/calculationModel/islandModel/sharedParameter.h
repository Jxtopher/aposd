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
                                       vector<string> &neighboringSolutions, 
                                       std::mutex &_lock_mutex) {
            MPI_Status status;
            //MPI_Request request;
            int order;
            do {
                
                MPI_Recv(&order, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                switch (order) {
                    case MPI_Order::FINISH:

                        _lock_mutex.lock();
                        _continued_recv = false;
                        _lock_mutex.unlock();

                        break;
                    case MPI_Order::COMPUTE_FITNESS: {
                        cout<<"["<<mpi_globals_rank<<"]* | line : "<<__LINE__<<endl;
                        // Recv message : solution, RewardOp
                        int sizeOfmessage;  // Get size of string solution
                        int parameter;  // Get parameter
                        double reward;  // Get reward
                        cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                        MPI_Probe(MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                        MPI_Get_count(&status, MPI_CHAR, &sizeOfmessage);
                        cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<" | sizeOfmessage : "<<sizeOfmessage<<endl;
                        char *msg = new char[sizeOfmessage + 1];  // Get solution
                        MPI_Recv(msg, sizeOfmessage, MPI_CHAR, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                        cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                        MPI_Recv(&parameter, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get parameter
                        cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                        MPI_Recv(&reward, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status); // Get parameter
                        cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                        _lock_mutex.lock();
                        _rewardOp.insert(_rewardOp.begin(), std::tuple<string, double, unsigned int>(string(msg), reward, parameter));
                        _lock_mutex.unlock();
                        cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                        delete msg;
                    } break;
                    default:
                        assert("[-] MPI_Order is not valide" && false);
                        break;
                }
               
            } while(_continued_recv);
        }

        void operator()() {
            cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
            //MPI_Status status;
            MPI_Request request;
            vector<SOL> solutions;
            _parameterSelection.reset();

            thread t(recvRewardParamter, boost::ref(continued_recv), boost::ref(rewardOp), boost::ref(neighboringSolutions), boost::ref(lock_mutex));
            //unsigned int iteration = 0;

            // Initialisation de la premiere solution
            solutions.push_back(_launcher.initSolution());

            do {
                cout<<"["<<mpi_globals_rank<<"]* | line : "<<__LINE__<<endl;

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
                
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                // Selection solutions and parameter a utiliser
                unsigned int parameter = _parameterSelection.getParameter();
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                SOL bestSolution = _selection(solutions);
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                // Call solver
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                string newSolution = _launcher.solve(bestSolution.str(), parameter);
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                solutions.push_back(SOL(newSolution));
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                // Compute reward
                pair<double, unsigned int> rewardOp = _rewardComputation(bestSolution, SOL(string(newSolution)), parameter);

                // Send message : solution, RewardOp
                double reward = rewardOp.first;
                int mpi_parameter = rewardOp.second;
                int order = MPI_Order::COMPUTE_FITNESS;
                auto neighbours = boost::adjacent_vertices(mpi_globals_rank, this->_topologies.graph());
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                for (unsigned int neighbour : make_iterator_range(neighbours)) {
                    std::cout << mpi_globals_rank<< " has adjacent vertex " << neighbour << "\n";
                    cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                    MPI_Isend(&order, 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<" | newS.size() : "<<newSolution.size()<<endl;
                    MPI_Isend(static_cast<const char *>(newSolution.c_str()), newSolution.size(), MPI_CHAR, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                    MPI_Isend(&(mpi_parameter), 1, MPI_INT, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                    MPI_Isend(&(reward), 1, MPI_DOUBLE, neighbour, MPI_TAG, MPI_COMM_WORLD, &request);
                    cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
                }

                // Stop criteria
                /* if (3 < iteration++) {
                    continued_send = false;
                }*/
                cout<<"["<<mpi_globals_rank<<"] | line : "<<__LINE__<<endl;
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
        vector<string> neighboringSolutions;
        std::mutex lock_mutex;
        int continued_recv;
        int continued_send;      
};

#endif