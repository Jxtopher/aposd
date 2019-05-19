#ifndef JXTOPHER_MASTER_H
#define JXTOPHER_MASTER_H


class MPI_Order {
	public:
	static const int FINISH = 0;
	static const int COMPUTE_FITNESS = 1;
	static const int INIT_SOLUTION = 2;
};

class Master {
	public:
		Master() {

		}
		virtual ~Master() {
			
		}
		virtual void operator()() = 0;
	protected:
};


#endif
