# Adaptive Portfolio Selection Distributed (aposd)

#### Branch Master 
- **Linux Build Status** : [![Build Status](https://travis-ci.org/Jxtopher/aposd.svg?branch=master)](https://travis-ci.org/Jxtopher/aposd)
- **lgtm** : --
- **Doc** : --

# Installation

	
~~~~
apt-get install cmake libpcre3-dev zlib1g-dev libgcrypt11-dev libicu-dev python openmpi-bin
git clone https://github.com/artyom-beilis/cppcms.git cppcms
cd cppcms
mkdir build
cd build
cmake ..
make
make test
make install
export LD_LIBRARY_PATH=/usr/local/lib
~~~~
