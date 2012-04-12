#include <iostream>
#include <time.h>
using namespace std;

class Clock{		
	//struct timeval s, e;
    timespec t1, t2;

    double diff(timespec& start, timespec& end){
        timespec temp;
        if ((end.tv_nsec - start.tv_nsec)<0){
            temp.tv_sec = end.tv_sec - start.tv_sec - 1;
            temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        }else{
            temp.tv_sec = end.tv_sec - start.tv_sec;
            temp.tv_nsec = end.tv_nsec - start.tv_nsec;
        }
        return temp.tv_sec * 1000 + temp.tv_nsec/1000000.0;
    }

public:
	void start(){
		//gettimeofday(&s, NULL);		
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
	}

	void end(){
		//gettimeofday(&e, NULL);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
	}	

	double elapsed(){
		//int seconds = e.tv_sec - s.tv_sec;
		//int usedconds = e.tv_usec - s.tv_usec;
		//double d =  ((seconds) * 1000 + usedconds/1000.0);
		//return d/(CLOCKS_PER_SEC);
        return diff(t1, t2);
	}
};
