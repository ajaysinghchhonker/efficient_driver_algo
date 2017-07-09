#include<sstream>
#include<stdlib.h>

#include "driver.h"

using namespace std;

const int R=6373;	//Earth radius in Km
const int SPEED=50; //km/h
const int SAL=30; //$30/h
const int STOP_TIME=15;	//minutes

driver::driver() {
	cout<<"driver Created"<<endl;
	current_time.hour=8;
	current_time.min=30;
}

double driver::degree2Radian(double degree) { 
    return degree * M_PI/180; 
}

int driver::loadTasks(string filename) {
  ifstream file_stream(filename.c_str());
  if(!file_stream) {
    cout<<"File with filename: "<<filename<<" doesn't exists"<<endl;
    return -1;
  }
  //loading tasks in the data structure
	//first index is pickup location and last index is dropoff location
  string line;
  while(getline(file_stream,line)) {
		TASKS t;
		istringstream iss(line);
    iss >> t.LONG >> t.LAT >> t.hour >> t.min;
		//cout<< t.LAT << " " << t.LONG << " " << t.hour << " " << t.min <<endl;
		task.push_back(t);
	}
	//removing garbage info of pickup and drop off location
	task[0].hour=task[0].min=0;
	task[task.size()-1].hour=task[task.size()-1].min=0;
	for(int i=0;i<task.size();i++) {
		for(int j=0;j<task.size();j++) {
		cout<<"location index: "<<i<<" to "<<j<<endl;
		travel_time(cord2dist(task[i].LAT, task[i].LONG, task[j].LAT, task[j].LONG));
		}
	}
  
  file_stream.close();
	return 0;
}

double driver::cord2dist(double LAT_1, double LONG_1, double LAT_2, double LONG_2) {
	//Haversine Formula
	double dlon = degree2Radian(LONG_2 - LONG_1);
	double dlat = degree2Radian(LAT_2 - LAT_1);
	double a = pow((sin(dlat/2)),2) + cos(degree2Radian(LAT_1)) * cos(degree2Radian(LAT_2)) * pow((sin(dlon/2)),2);
	double c = 2 * atan2( sqrt(a), sqrt(1-a) );
	double d = R * c;
	cout << setprecision(10) << "distance is: " << d << " km\n";
	return d;
}

int driver::travel_time(double dist) {
	//time in minutes rounded to nearest minute
	double t = dist / SPEED; //in hour
	t*=60;	//in minutes
	int t1 = round(t);
	cout << setprecision(10) << "travel_time is: " << t1 << " minutes" <<endl;
	return t1;
}

TIME_ driver::add2currentTime(int hour,int min) {
	TIME_ ct=current_time;
	ct.min+=min;
	while(ct.min>=60) {
		ct.hour++;
		ct.min-=60;
	}
	ct.hour+=hour;
	while(ct.hour>=24) {
		ct.hour=0;
	}
	//cout<<"time: "<<ct.hour<<":"<<ct.min<<endl;
	return ct;
}

TIME_ driver::addTime(TIME_ t1,TIME_ t2) {
	TIME_ ct;
	ct.min=t1.min+t2.min;
	ct.hour=t1.hour+t2.hour;
	while(ct.min>=60) {
		ct.hour++;
		ct.min-=60;
	}
	while(ct.hour>=24) {
		ct.hour=0;
	}
	//cout<<"time: "<<ct.hour<<":"<<ct.min<<endl;
	return ct;
}

TIME_ driver:: IsNextTask(int taskIndex_current, int taskIndex_next) {
	//calculates the time for next task and decides whether to go to that or not
	cout<<"\nCurrrent task index: "<<taskIndex_current<<" to index : "<<taskIndex_next<<" ; current time: "<<current_time.hour<<" hr "<<current_time.min<<" min"<<endl;	//print current time and task
	double minutes=travel_time(cord2dist(task[taskIndex_current].LAT, task[taskIndex_current].LONG, task[taskIndex_next].LAT, task[taskIndex_next].LONG));
	TIME_ t=add2currentTime(0,minutes);
	cout<<"Time to reach index "<<taskIndex_next<<"  : "<<t.hour<<" hr "<<t.min<<" min"<<" ; deadline: "<<task[taskIndex_next].hour<<" hr "<<task[taskIndex_next].min<<" min ; ";
	//Compare time 
	if(task[taskIndex_next].hour<t.hour) {
		cout<<"\nCannot go to this location\n";
		t.hour=t.min=-1;
		return t;
	} else if(task[taskIndex_next].hour==t.hour) {
			if(task[taskIndex_next].min<t.min) {
				cout<<"\nCannot go to this location\n";
				t.hour=t.min=-1;
				return t;
			}
	}
	TIME_ delay;
	delay.hour=0;delay.min=STOP_TIME;
	t=addTime(t,delay);
	cout<<" Time to leave the location: "<<t.hour<<" hr "<<t.min<<" min"<<endl;
	return t;	//returns time of leaving this next location
}

void driver::completeTasks() {
	TIME_ t;
	//int flag=0;	//raised if no more task to execute
	int next_task=1;
	for(int i=0;i<task.size()-2;) {
			t=IsNextTask(i,next_task);
			while(t.hour==-1) {
				next_task++;
				if(next_task>=task.size()-1) {break;}
				t=IsNextTask(i,next_task);
			}
			if(t.hour==-1) {next_task=i;break;}
			current_time=t;
			completed_tasks.push_back(next_task);	//saving this index becoz we can go to this location
			i=next_task;
			if(next_task<task.size()-2) {next_task=i+1;}
	}

	
	//time from last location to Drop off location
	cout<<"\nCurrrent task index: "<<next_task<<" to dropoff location ; current time: "<<current_time.hour<<" hr "<<current_time.min<<" min"<<endl;
	double minutes=travel_time(cord2dist(task[next_task].LAT, task[next_task].LONG, task[task.size()-1].LAT, task[task.size()-1].LONG));
	current_time=add2currentTime(0,minutes);

	//Show completed tasks
	cout<<"\nCompleted Tasks:\nPickup\n";
	for(int i=0;i<completed_tasks.size();i++) {
		cout<<completed_tasks[i]<<endl;
	}
	cout<<"Drop off location"<<endl;
	working_time.hour=current_time.hour-8;
	working_time.min=current_time.min-30;
	if(working_time.min<0) {
		working_time.min=60+working_time.min;
		working_time.hour--;
	}
	cout<<"Driver's total working time = "<<working_time.hour<<" hr "<<working_time.min<<" min"<<endl;
	calSalary(working_time.hour,working_time.min);
}

void driver::calSalary(int hour, double min) {
	cout<<hour<<" hr "<<min<<" min"<<endl;
	salary = (hour + (double)(min/60)) * SAL;
	cout<<"Amount paid to driver = $"<<salary<<endl;
	//Save salary to a file
	ofstream file_stream("output_data.txt");
	file_stream << salary;
	file_stream.close();
}
