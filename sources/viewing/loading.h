
#ifndef LOADING_H
#define LOADING_H

#include <ostream>

using namespace std;

class Loading {
    public:
        Loading() {
            state = 0;
        }
        

        void setState(unsigned int _state) {
            state = _state;
        }

        unsigned int getState() {
            return state;
        }

		friend std::ostream & operator << (std::ostream &out,Loading &l) {
            switch (l.getState()) {
                case 0:
                    out<<"[-]";
                    l.setState(l.getState() + 1);
                    break;
                case 1:
                    out<<"[/]";
                    l.setState(l.getState() + 1);
                    break;
                case 2:
                    out<<"[-]";
                    l.setState(l.getState() + 1);
                    break;
                case 3:
                    out<<"[\\]";
                    l.setState(0);
                    break;
                default:
                    break;
            }
			return out;
		}

    protected:
        unsigned int state;
};

#endif