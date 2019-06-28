#ifndef JXTOPHER_SHAREDPARAMETER_H
#define JXTOPHER_SHAREDPARAMETER_H

#include <mutex>
#include <thread>

#include "islandModel.h"

using namespace std;

class SharedParameter : public IslandModel {
	public:
		SharedParameter(int argc, char** argv, Topologies &topologies) : 
            IslandModel(argc, argv, topologies) {
            this->operator()();
            
		}
		virtual ~SharedParameter() {
			
		}

        static void recvRewardParamter(vector<pair<double, unsigned int>> &_rewardOp, vector<string> &neighboringSolutions, std::mutex &_lock_mutex) {
            
            while(true) {
                sleep(1);
                cout<<"x"<<endl;
                _lock_mutex.lock();
                _rewardOp.insert(_rewardOp.begin(), pair<double, unsigned int>(5.4, 22));
                _lock_mutex.unlock();
            }
        }

        void operator()() {
            cout<<mpi_globals_rank<<endl;
            thread t1(recvRewardParamter, boost::ref(rewardOp), boost::ref(neighboringSolutions), boost::ref(lock_mutex));
            while(true) {
                sleep(3);
                lock_mutex.lock();
                cout<<"y"<<rewardOp.size()<<endl;
                while (!rewardOp.empty())
                    rewardOp.pop_back();
                lock_mutex.unlock();
            }
            
        }
        
	protected:
        vector<pair<double, unsigned int>> rewardOp;
        vector<string> neighboringSolutions;
        std::mutex lock_mutex;
        
};

#endif