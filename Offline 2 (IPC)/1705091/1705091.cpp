#include<bits/stdc++.h>
#include<pthread.h>
#include <unistd.h>
#include<semaphore.h>
#include <chrono>
#include <ctime>
using namespace std;

#define global_variable 4
// default_random_engine generator;
// poisson_distribution<int> distribution(4.1);

std::random_device rd; 
std::mt19937 rng (rd ());
double averageArrival = 20;
double lamda = averageArrival/60;
std::exponential_distribution<double> expon (lamda);

time_t start_time;

int arrival_time, number_of_passengers = 10, M,N,P,w,x,y,z;
sem_t kiosk, *belt;
pthread_mutex_t *kiosk_mutex, boarding_mutex, vip_channel_mutex, special_kiosk_mutex, vip_channel_count, extra_mutex, global_var_mutex[global_variable];
int* kiosk_index, left_right = 0, right_left = 0;

time_t current_time(){
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void vip_channel_waiting(string msg, int vip, int id, time_t now_diff){
	pthread_mutex_lock(&extra_mutex);
		time_t updated_time = difftime(current_time(), start_time);
		if(updated_time > now_diff-1) now_diff = updated_time;

		msg = "Passenger " + to_string(id) + " (VIP) has started waiting for VIP channel at time " + to_string(now_diff) + "\n";
		cout << msg;
		pthread_mutex_lock(&global_var_mutex[2]);
			if(left_right == 0){
				pthread_mutex_lock(&vip_channel_mutex);
			}	
			left_right++;
		pthread_mutex_unlock(&global_var_mutex[2]);
	pthread_mutex_unlock(&extra_mutex);
}

time_t vip_channel_crossing(string msg, int id, time_t now_diff){
	time_t updated_time = difftime(current_time(), start_time);
	if(updated_time > now_diff-1) now_diff = updated_time;

	msg = "Passenger " + to_string(id) + " (VIP) has started walking through the VIP channel at time " + to_string(now_diff) + "\n";
	cout << msg;

	sleep(z);
	now_diff += z;

	pthread_mutex_lock(&vip_channel_count);
		msg = "Passenger " + to_string(id) + " (VIP) has crossed the VIP channel at time " + to_string(now_diff) + "\n";
		cout << msg;
		left_right--;
		if(left_right == 0){
			pthread_mutex_unlock(&vip_channel_mutex);
		}
	pthread_mutex_unlock(&vip_channel_count);
	return now_diff;
}


void* journey_by_air(void* arg)
{
	time_t now_diff;
	string msg;
	int sem_value, index;
	int id = *((int*) arg);

	//vip pasenger
	int vip = rand()%2;

	//passenge arrival
	now_diff = difftime(current_time(), start_time);

	if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has arrived at time " + to_string(now_diff) + "\n";
	else msg = "Passenger " + to_string(id) + " has arrived at time " + to_string(now_diff) + "\n";
	cout << msg;

	//kiosk check in
	sem_wait(&kiosk);
		// find out which kiosk is available()
		pthread_mutex_lock(&global_var_mutex[0]);
			for(int i = 0; i < M; i++){
				if(kiosk_index[i] == 0){
					index = i;
					kiosk_index[i] = 1;
					break;
				}
			}
		pthread_mutex_unlock(&global_var_mutex[0]);

		pthread_mutex_lock(&kiosk_mutex[index]);
			now_diff = difftime(current_time(), start_time);

			if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has started self-check in at kiosk " + to_string(index+1) + " at time " + to_string(now_diff) + "\n";
			else msg = "Passenger " + to_string(id) + " has started self-check in at kiosk " + to_string(index+1) + " at time " + to_string(now_diff) + "\n";
			cout << msg;

			sleep(w);
			now_diff += w;

			if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has finished check in at time " + to_string(now_diff) + "\n";
			else msg = "Passenger " + to_string(id) + " has finished check in at time " + to_string(now_diff) + "\n";
			cout << msg;

			// vip channel waiting
			if(vip == 1)	vip_channel_waiting(msg, vip, id, now_diff);

			kiosk_index[index] = 0;
		pthread_mutex_unlock(&kiosk_mutex[index]);
	sem_post(&kiosk);

	//vip channel
	if(vip == 1) now_diff = vip_channel_crossing(msg, id, now_diff);

	// security check
	else if(vip == 0){
        int belt_index = rand()%N + 1;
		msg = "Passenger " + to_string(id) + " has started waiting for security check at belt " + to_string(belt_index) + " from time " + to_string(now_diff) + "\n"; 
		cout << msg;
		sem_wait(&belt[belt_index-1]);
            time_t updated_time = difftime(current_time(), start_time);
            if(updated_time > now_diff-1) now_diff = updated_time;
            msg = "Passenger " + to_string(id) + " has started the security check at belt " + to_string(belt_index) + " at time " + to_string(now_diff) + "\n";
            cout << msg;

            sleep(x);
            now_diff += x;

            msg = "Passenger " + to_string(id) + " has crossed the security check at time " + to_string(now_diff) + "\n";
            cout << msg;
		sem_post(&belt[belt_index-1]);
	}

	//careless passenger
	while(rand()%2 == 1){  // if pass lost
		if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has lost the boarding pass\n";
		else msg = "Passenger " + to_string(id) + " has lost the boarding pass\n";
		cout << msg;

		//return to special kiosk
		if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has started waiting for the VIP channel right to left at time " + to_string(now_diff) + "\n";
		else msg = "Passenger " + to_string(id) + " has started waiting for the VIP channel right to left at time " + to_string(now_diff) + "\n";
		cout << msg;

		// right to left vip channel check
		pthread_mutex_lock(&extra_mutex);
			pthread_mutex_lock(&global_var_mutex[3]);
				if(right_left == 0){
					pthread_mutex_lock(&vip_channel_mutex);
				}
				right_left++;
			pthread_mutex_unlock(&global_var_mutex[3]);

			time_t updated_time = difftime(current_time(), start_time);
			if(updated_time > now_diff-1) now_diff = updated_time;

			if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has started walking through the VIP channel right to left at time " + to_string(now_diff) + "\n";
			else msg = "Passenger " + to_string(id) + " has started walking through the VIP channel right to left at time " + to_string(now_diff) + "\n";
			cout << msg;
		pthread_mutex_unlock(&extra_mutex);

		sleep(z);
		now_diff += z;

		pthread_mutex_lock(&vip_channel_count);
			if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has crossed the VIP channel right to left at time " + to_string(now_diff) + "\n";
			else msg = "Passenger " + to_string(id) + " has crossed the VIP channel right to left at time " + to_string(now_diff) + "\n";
			cout << msg;

			right_left--;
			if(right_left == 0){
				pthread_mutex_unlock(&vip_channel_mutex);
			}
		pthread_mutex_unlock(&vip_channel_count);

		//special kiosk service
		pthread_mutex_lock(&special_kiosk_mutex);
			if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has started self-check in at special kiosk at time " + to_string(now_diff) + "\n";
			else msg = "Passenger " + to_string(id) + " has started self-check in at special kiosk at time " + to_string(now_diff) + "\n";
			cout << msg;

			sleep(w);
			now_diff += w;

			if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has finished check in at special kiosk at time " + to_string(now_diff) + "\n";
			else msg = "Passenger " + to_string(id) + " has finished check in at special kiosk at time " + to_string(now_diff) + "\n";
			cout << msg;

			// vip channel waiting
			vip_channel_waiting(msg, vip, id, now_diff);

		pthread_mutex_unlock(&special_kiosk_mutex);

		//vip channel
		now_diff = vip_channel_crossing(msg, id, now_diff);
	}

	//boarding
	if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has started waiting to be boarded at time " + to_string(now_diff) + "\n";
	else msg = "Passenger " + to_string(id) + " has started waiting to be boarded at time " + to_string(now_diff) + "\n";
	cout << msg;

	pthread_mutex_lock(&boarding_mutex);
		time_t updated_time = difftime(current_time(), start_time);
		if(updated_time > now_diff-1) now_diff = updated_time;

		if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has started boarding the plane at time " + to_string(now_diff) + "\n";
		else msg = "Passenger " + to_string(id) + " has started boarding the plane at time " + to_string(now_diff) + "\n";
		cout << msg;

		sleep(y);
		now_diff += y;

		if(vip == 1) msg = "Passenger " + to_string(id) + " (VIP) has boarded the plane at time " + to_string(now_diff) + "\n";
		else msg = "Passenger " + to_string(id) + " has boarded the plane at time " + to_string(now_diff) + "\n";
		cout << msg;
	pthread_mutex_unlock(&boarding_mutex);
}

int main(void)
{
	start_time = current_time();
	srand(time(0));
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

	int return_value;
	string msg;

	cin >> M >> N >> P;
	cin >> w >> x >> y >> z;

	cout << M << " " << N << " " << P << endl;
	cout << w << " " << x << " " << y << " " << z << endl;

	// cout << "input taken\n";

	//global variable initilization
	//kiosk index
	kiosk_index = new int[M];
	for(int i = 0; i < M; i++){
		kiosk_index[i] = 0;
	}

	// semaphore initialization
	// kiosk
	return_value = sem_init(&kiosk,0,M);
	if(return_value != 0) {
		cout << "kiosk semaphore initialization failed\n";
	}
	//belt
	belt = new sem_t[N];
    for(int i = 0; i < N; i++){
        return_value = sem_init(&belt[i],0,P);
        if(return_value != 0) {
            cout << "belt semaphore initialization failed\n";
        }
    }
	
	// mutex initialization
	// kiosk mutex
	kiosk_mutex = new pthread_mutex_t[M];
	for(int i = 0; i < M; i++){
		return_value = pthread_mutex_init(&kiosk_mutex[i],NULL);
		if(return_value != 0) {
			msg = "kiosk mutex " + to_string(i) + " initialization failed\n";
			cout << msg;
		}
	}
	//boarding mutex
	return_value = pthread_mutex_init(&boarding_mutex, NULL);
	if(return_value != 0) {
		cout << "boarding mutex initialization failed\n";
	}
	//vip channel mutex
	return_value = pthread_mutex_init(&vip_channel_mutex, NULL);
	if(return_value != 0) {
		cout << "vip channel mutex initialization failed\n";
	}
	//special kiosk mutex
	return_value = pthread_mutex_init(&special_kiosk_mutex, NULL);
	if(return_value != 0) {
		cout << "special kiosk mutex initialization failed\n";
	}
	//vip channel count
	return_value = pthread_mutex_init(&vip_channel_count, NULL);
	if(return_value != 0) {
		cout << "vip channel count mutex initialization failed\n";
	}
	//extra mutex
	return_value = pthread_mutex_init(&extra_mutex, NULL);
	if(return_value != 0) {
		cout << "extra mutex initialization failed\n";
	}
	//global variable mutex
	for(int i = 0; i < global_variable; i++){
		return_value = pthread_mutex_init(&global_var_mutex[i],NULL);
		if(return_value != 0) {
			msg = "global variable mutex " + to_string(i) + " initialization failed\n";
			cout << msg;
		}
	}

	// initializing passenger thread
	pthread_t passengers[number_of_passengers];
	for(int i = 0; i < number_of_passengers; i++){
		//arrival_time = distribution(generator);
		arrival_time=  expon.operator() (rng);
		sleep(arrival_time);

		int* id = new int(i+1);
		msg = "thread " + to_string(i+1) + " created after " + to_string(arrival_time) + "\n";
		cout << msg;

		return_value = pthread_create(&passengers[i], NULL, journey_by_air, (void*)id);
		if(return_value != 0) {
			msg = "passenger " + to_string(i+1) + " thread creation failed\n";
			cout << msg;
		}
	}

	// wait for passenger thread to finish
	for(int i = 0; i < number_of_passengers; i++) {
		void *result;
        pthread_join(passengers[i],&result);
		printf("%s", (char*)result);
	}

	// semaphore destruction
	//kiosk
	return_value = sem_destroy(&kiosk);
	if(return_value != 0) {
		cout << "kiosk semaphore destruction failed\n";
	}
	//belt
	for(int i = 0; i < N; i++){
        return_value = sem_destroy(&belt[i]);
        if(return_value != 0) {
            cout << "belt semaphore initialization failed\n";
        }
    }
	
	// mutex destruction
	// kiosk mutex
	for(int i = 0; i < M; i++){
		return_value = pthread_mutex_destroy(&kiosk_mutex[i]);
		if(return_value != 0) {
			cout << "kiosk mutex destruction failed\n";
		}
	}
	//boarding mutex
	return_value = pthread_mutex_destroy(&boarding_mutex);
	if(return_value != 0) {
		cout << "boarding mutex destruction failed\n";
	}
	//vip channel mutex
	return_value = pthread_mutex_destroy(&vip_channel_mutex);
	if(return_value != 0) {
		cout << "vip channel mutex destruction failed\n";
	}
	//special kiosk mutex
	return_value = pthread_mutex_destroy(&special_kiosk_mutex);
	if(return_value != 0) {
		cout << "special kiosk mutex destruction failed\n";
	}
	//vip channel count
	return_value = pthread_mutex_destroy(&vip_channel_count);
	if(return_value != 0) {
		cout << "vip channel count mutex destruction failed\n";
	}
	//extra mutex
	return_value = pthread_mutex_destroy(&vip_channel_count);
	if(return_value != 0) {
		cout << "extra mutex destruction failed\n";
	}
	//global variable mutex
	for(int i = 0; i < global_variable; i++){
		return_value = pthread_mutex_destroy(&global_var_mutex[i]);
		if(return_value != 0) {
			msg = "global variable mutex " + to_string(i) + " destruction failed\n";
			cout << msg;
		}
	}
	return 0;
}
