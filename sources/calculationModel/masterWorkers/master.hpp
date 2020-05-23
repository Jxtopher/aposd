#ifndef JXTOPHER_MASTER_H
#define JXTOPHER_MASTER_H

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
