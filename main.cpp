#include "driver.h"

using namespace std;

/*******************************  Assumptions  ******************************
input data is in the format:  Longitude Latitude hour minutes
Driver is starting towards task 1 exactly @8:30
Travel time in minutes is rounded to int like 6.75 min = 7min
Time is provided in 24 hour format
No realtime delays and traffic
Ideal conditions
****************************************************************************/

int main(int argc, char* argv[]) {
	string filename;
	if(argc!=2) {
		cout<<"Invalid format!!\nProper way to use is:\n./main input_data.txt"<<endl;
		return 1;
	} else {
		filename=argv[1];
	}
	driver Driver;
	int success=Driver.loadTasks(filename);
	if(success==-1) {cout<<"unable to read file!! either file not exists or corrupted"<<endl;return 1;}
	Driver.completeTasks();
	return 0;
}
