#include <unistd.h>
#include <sys/time.h>

class Clock{		
	struct timeval s, e;

public:
	void start(){
		gettimeofday(&s, NULL);		
	}

	void end(){
		gettimeofday(&e, NULL);
	}	

	double elapsed(){
		int seconds = e.tv_sec - s.tv_sec;
		int usedconds = e.tv_usec - s.tv_usec;
		double d =  ((seconds) * 1000 + usedconds/1000.0);
		//return d/(CLOCKS_PER_SEC);
		return d;
	}
};
