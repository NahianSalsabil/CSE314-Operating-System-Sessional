#include<bits/stdc++.h>
#include<unistd.h>
#include <chrono>
#include <ctime>

using namespace std;

#define time 15
default_random_engine generator;
poisson_distribution<int> distribution(4);


time_t cur_time;

time_t current_time(){
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void* journey_by_air(void* arg){
    time_t local_time;
    int id = *((int*) arg);

    string msg = "I am thread " + to_string(id) + " " + to_string(difftime(current_time(), cur_time)) + "\n";
    // local_time = current_time();
    // double start = difftime(local_time, cur_time);
    // msg += to_string(start) + "\n";
    cout << msg;
}

int main(){
    cur_time = current_time();
    cout << std::ctime(&cur_time) << endl;

    int return_value, number;
    string msg;
    
	for(int j = 0; j < time; j++){
        number = distribution(generator);
        pthread_t passengers[number];
        cout << number << endl;
        for(int i = 0; i < number; i++){
            int* id = new int(i+1);
            return_value = pthread_create(&passengers[i], NULL, journey_by_air, (void*)id);
        }
        sleep(1);
	}

}