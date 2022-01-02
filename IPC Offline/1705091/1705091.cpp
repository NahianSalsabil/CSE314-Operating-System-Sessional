#include<bits/stdc++.h>
#include<pthread.h>
#include <unistd.h>
#include<semaphore.h>

using namespace std;

#define global_variable 1
#define number_of_passengers 10

int M,N,P,w,x,y,z;
sem_t kiosk, *belt;
pthread_mutex_t *kiosk_mutex, **belt_mutex, boarding_mutex, vip_channel_mutex, global_var_mutex[global_variable];
int* kiosk_index, **belt_index;


void* journey_by_air(void* arg)
{
	string msg;
	int sem_value, index;
	int id = *((int*) arg);

	//vip pasenger
	int vip = rand()%2;

	//passenge arrival
	if(vip == 1){
		msg = "Passenger " + to_string(id) + " (VIP) has arrived\n";
		cout << msg;
	}
	else{
		msg = "Passenger " + to_string(id) + " has arrived\n";
		cout << msg;
	}
	
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
	msg = "Passenger " + to_string(id) + " has started self-check in at kiosk " + to_string(index+1) + " at time\n";
	cout << msg;
	usleep(w);
	msg = "Passenger " + to_string(id) + " has finished check in at time\n";
	cout << msg;

	// vip channel waiting
	// if(vip == 1){
	// 	msg = "Passenger " + to_string(id) + " (VIP) has started waiting for VIP channel at time\n";
	// 	pthread_mutex_lock(&vip_channel_mutex);
	// }

	kiosk_index[index] = 0;
	pthread_mutex_unlock(&kiosk_mutex[index]);
	sem_post(&kiosk);

	//security check or vip channel
	// if(vip == 1){
	// 	msg = "Passenger " + to_string(id) + " (VIP) has started walking through the VIP channel\n";
	// 	cout << msg;
	// 	usleep(z);
	// 	msg = "Passenger " + to_string(id) + " (VIP) has crossed the VIP channel\n";
	// 	cout << msg;
	// }

	//security check in belt
	
	//careless passenger
	// int lost_pass = rand()%2;
	// if(lost_pass == 1){
	// 	msg = "Passenger " + to_string(id) + " has lost the boarding pass\n";
	// 	cout << msg;

	// }
	//boarding
	msg = "Passenger " + to_string(id) + " has started waiting to be boarded at time\n";
	cout << msg;
	pthread_mutex_lock(&boarding_mutex);
	msg = "Passenger " + to_string(id) + " has started boarding the plane at time\n";
	cout << msg;
	usleep(y);
	msg = "Passenger " + to_string(id) + " has boarded the plane at time\n";
	cout << msg;
	pthread_mutex_unlock(&boarding_mutex);
}

int main(void)
{
	srand(time(0));
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

	int return_value;
	string msg;

	cin >> M >> N >> P;
	cin >> w >> x >> y >> z;

	cout << M << " " << N << " " << P << endl;
	cout << w << " " << x << " " << y << " " << z << endl;

	cout << "input taken\n";

	//global variable initilization
	//kiosk index
	kiosk_index = new int[M];
	for(int i = 0; i < M; i++){
		kiosk_index[i] = 0;
	}
	
	//belt index
	belt_index = new int*[N];
	for(int i = 0; i < N; i++){
		belt_index[i] = new int[P];
	}
	for(int i = 0; i < N; i++){
		for(int j = 0; j < P; j++){
			belt_index[i][j] = 0;
		}
	}

	// semaphore initialization
	return_value = sem_init(&kiosk,0,M);
	if(return_value != 0) {
		cout << "kiosk semaphore initialization failed\n";
	}

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

	//belt mutex
	belt_mutex = new pthread_mutex_t*[N];
	for(int i = 0; i < N; i++){
		belt_mutex[i] = new pthread_mutex_t[P];
	}
	for(int i = 0; i < N; i++){
		for(int j = 0; j < P; j++){
			return_value = pthread_mutex_init(&belt_mutex[i][j],NULL);
			if(return_value != 0) {
				msg = "security check mutex[" + to_string(i) + "][" + to_string(j) + " initialization failed\n";
				cout << msg;
			}
		}
	}

	//boarding mutex
	return_value = pthread_mutex_init(&boarding_mutex, NULL);
	if(return_value != 0) {
		cout << "boarding mutex initialization failed\n";
	}

	//vip channel mutex
	// return_value = pthread_mutex_init(&vip_channel_mutex, NULL);
	// if(return_value != 0) {
	// 	cout << "vip channel mutex initialization failed\n";
	// }

	//global variable mutex
	for(int i = 0; i < global_variable; i++){
		return_value = pthread_mutex_init(&global_var_mutex[i],NULL);
		if(return_value != 0) {
			msg = "global variable mutex " + to_string(i) + " initialization failed\n";
			cout << msg;
		}
	}

	cout << "mutex initialized\n";

	// initializing passenger thread
	pthread_t passengers[number_of_passengers];
	for(int i = 0; i < number_of_passengers; i++){
		int* id = new int(i+1);

		msg = "thread " + to_string(i+1) + " created\n";
		cout << msg;
		return_value = pthread_create(&passengers[i], NULL, journey_by_air, (void*)id);
		if(return_value != 0) {
			msg = "passenger " + to_string(i) + " thread creation failed\n";
			cout << msg;
		}
	}

	cout << "initialized\n";

	// wait for passenger thread to finish
	for(int i = 0; i < number_of_passengers; i++) {
		void *result;
        pthread_join(passengers[i],&result);
        printf("%s",(char*)result);
	}

	// semaphore destruction
	return_value = sem_destroy(&kiosk);
	if(return_value != 0) {
		cout << "kiosk semaphore destruction failed\n";
	}

	for(int i = 0; i < N; i++){
		return_value = sem_destroy(&belt[i]);
		if(return_value != 0) {
			cout << "belt semaphore destruction failed\n";
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

	//belt mutex
	for(int i = 0; i < N; i++){
		for(int j = 0; j < P; j++){
			return_value = pthread_mutex_destroy(&belt_mutex[i][j]);
			if(return_value != 0) {
				msg = "belt mutex[" + to_string(i) + "][" + to_string(j) + " destruction failed\n";
				cout << msg;
			}
		}
	}

	//boarding mutex
	return_value = pthread_mutex_destroy(&boarding_mutex);
	if(return_value != 0) {
		cout << "boarding mutex destruction failed\n";
	}

	//
	//vip channel mutex
	// return_value = pthread_mutex_destroy(&vip_channel_mutex);
	// if(return_value != 0) {
	// 	cout << "vip channel mutex destruction failed\n";
	// }

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
