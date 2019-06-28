#ifndef CALCULATIONMODEL_H
#define	CALCULATIONMODEL_H

class MPI_Order {
	public:
	static const int FINISH = 0;
	static const int COMPUTE_FITNESS = 1;
	static const int INIT_SOLUTION = 2;
};

class CalculationModel {
    public:
        CalculationModel() {

        }

        virtual ~CalculationModel() {

        }

        virtual void operator()() = 0;

    private:
};

#endif