#ifndef SEQUENTIALMODEL_H
#define SEQUENTIALMODEL_H

#include <memory>
#include <utility>
#include <iostream>

#include "../../launcher/launcher.hpp"
#include "../../parameterSelection/parameterSelection.hpp"
#include "../../rewardComputation/rewardComputation.hpp"
#include "../calculationModel.hpp"

template<class SOL> class SequentialModel : public CalculationModel {
  public:
	SequentialModel(std::unique_ptr<Launcher> launcher,
					std::unique_ptr<ParameterSelection> parameterSelection,
					std::unique_ptr<RewardComputation<SOL>> rewardComputation);

	virtual ~SequentialModel();

	void operator()();

  protected:
	std::unique_ptr<Launcher> _launcher;
	std::unique_ptr<ParameterSelection> _parameterSelection;
	std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
};

#endif