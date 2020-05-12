#ifndef WORKERS_SYNCHRONOUS_H
#define WORKERS_SYNCHRONOUS_H

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include <memory>
#include <regex>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include "../../launcher/launcher.hpp"
#include "../calculationModel.hpp"
#include "../mpiConfig.hpp"

#include "workers.hpp"

class WorkersSynchronous : public Workers {
  public:
	WorkersSynchronous(std::unique_ptr<Launcher> launcher);
	virtual ~WorkersSynchronous();

	void operator()();

  protected:
	std::unique_ptr<Launcher> _launcher;
	MPI_Status status;
	MPI_Request request;
};

#endif