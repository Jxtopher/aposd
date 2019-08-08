#ifndef JXTOPHER_WORKERS
#define JXTOPHER_WORKERS

class Workers {
	public:
	Workers() {

	}
	virtual ~Workers() {

	}

	virtual void operator()() = 0;

	protected:
};

#endif
