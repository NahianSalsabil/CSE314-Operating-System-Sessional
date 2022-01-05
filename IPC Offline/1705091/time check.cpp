#include<bits/stdc++.h>
#include<unistd.h>
#include <chrono>
#include <ctime>

using namespace std;

#define time 15
#define no_of_passengers 10
default_random_engine generator;
poisson_distribution<int> distribution(4);


time_t cur_time;

time_t current_time(){
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void* journey_by_air(void* arg){
    time_t local_time;
    int id = *((int*) arg);

    string msg = "I am thread " + to_string(id) + " local time: ";
    local_time = current_time();
    double start = difftime(local_time, cur_time);
    msg += to_string(start) + "\n";
    cout << msg;
}

int main(){
    cur_time = current_time();
    cout << std::ctime(&cur_time) << endl;

    int return_value, number;
    string msg;
    
	for(int j = 0; j < no_of_passengers; j++){
        number = distribution(generator);
        pthread_t passengers;
        cout << number << endl;
        sleep(number);
        int* id = new int(j+1);
        return_value = pthread_create(&passengers, NULL, journey_by_air, (void*)id);
       
	}

}