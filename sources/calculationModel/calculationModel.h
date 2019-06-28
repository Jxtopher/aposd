#ifndef CALCULATIONMODEL_H
#define	CALCULATIONMODEL_H


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