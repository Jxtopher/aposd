# Adaptive Portfolio Selection Distributed (aposd)

APOSD sélectionne de manière automatique les paramètres d'un solver d'optimisation. Il est conçu pour les environnements de cluster de calcul HPC, il support plusieurs de modèle de calcul distribué en fonction des besoins. Conçus étroitement avec le solver [STOCOS](https://github.com/Jxtopher/stocos), il s'inter-connecter facilement à d'autres solver ou simulateur par le biais d'une communication websocket ou commnucation inter-processus avec une description en JSON. 

#### Branch Master 
- **Linux Build Status** : [![Build Status](https://travis-ci.org/Jxtopher/aposd.svg?branch=master)](https://travis-ci.org/Jxtopher/aposd)
- **lgtm** : --
- **Doc** : --

# Installation

	
~~~~
apt-get install cmake libpcre3-dev zlib1g-dev libgcrypt11-dev libicu-dev python
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

