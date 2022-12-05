#include "timedifference.h"
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include "system.h"

using namespace std;

TimeDifference::TimeDifference():
start(0),
end(0){
}

void TimeDifference::startTime(){
    start = System::currentMicroseconds();
}

void TimeDifference::endTime(){
    end = System::currentMicroseconds();
}

const string TimeDifference::printTime(){
    return this->printTime("Function took");
}

unsigned long long int TimeDifference::getTime(){
    unsigned long long int g = end - start;
    return g;
}

static double roundit(double number, int digits){
    return (long long) (number * pow((double) 10.0, digits)) / pow((double) 10.0, digits);
}

const string TimeDifference::printTime(const string & s){

    double total = getTime();
    string units = "microseconds";

    int unit_times[] = {1000, 1000, 60};
    string unit_descriptions[] = {"milliseconds", "seconds", "minutes"};

    for (unsigned int index = 0; index < sizeof(unit_times) / sizeof(int); index++){
        if (total > unit_times[index]){
            total /= unit_times[index];
            units = unit_descriptions[index];
        } else {
            break;
        }
    }

    ostringstream o;
    o << s << " " << roundit(total, 3) << " " << units;
    return o.str();

    /*

	unsigned long long int micro = (end.tv_sec*1000000+end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec );
	unsigned long long int milli = micro / 1000;
	unsigned long long int sec = milli / 1000;

	//cout<<s<<" took "<<micro<<" microseconds / "<< milli << " milliseconds / " <<sec<< " seconds "<< endl;
	ostringstream o;
	o << s <<" took "<<micro<<" microseconds / "<< milli << " milliseconds / " <<sec<< " seconds";
	return o.str();
        */
}

TimeDifference::~TimeDifference(){
}
