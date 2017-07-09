#ifndef _DRIVER_H
#define _DRIVER_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <iomanip> 
#include<math.h>

using namespace std;

struct TASKS{
	double LONG;
	double LAT;
	int hour;
	int min;
};

struct TIME_{
	int hour;
	int min;
};

class driver{
private:
	double degree2Radian(double degree);
	double cord2dist(double LAT_1, double LONG_1, double LAT_2, double LONG_2);
	int travel_time(double dist);	//gives the rounded off travel time
	TIME_ add2currentTime(int hour,int min);
	TIME_ addTime(TIME_ t1,TIME_ t2);
	TIME_ IsNextTask(int taskIndex_current, int taskIndex_next);
public:
	driver();
	TIME_ current_time;
	TIME_ working_time;
	double salary;
	void calSalary(int hour, double min);
	vector<TASKS> task;
	vector<int> completed_tasks;
	int loadTasks(string filename);
	void completeTasks();
};

#endif
