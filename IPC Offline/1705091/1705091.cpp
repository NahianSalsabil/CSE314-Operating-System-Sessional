#include<bits/stdc++.h>
#include<pthread.h>
#include <unistd.h>
#include<semaphore.h>

using namespace std;

#define global_variable 1
#define number_of_passengers 10
#define number_of_belt 2

int M,N,P,w,x,y,z;
sem_t kiosk, belt[number_of_belt];
pthread_mutex_t check_in, *kiosk_mutex, global_var_mutex[global_variable];
int* kiosk_index;


void* journey_by_air(void* arg)
{
	string msg;
	int sem_value, index;
	int id = *((int*) arg);

	msg = "Passenger " + to_string(id) + " has arrived\n";
	cout << msg;
	
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
	cout << "Passenger " << id << " has started self-check in at kiosk " << index+1 << " at time\n";
	usleep(w);
	cout << "Passenger " << id << " has finished check in at time\n";
	kiosk_index[index] = 0;
	pthread_mutex_unlock(&kiosk_mutex[index]);
	sem_post(&kiosk);

	
}

int main(void)
{
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

	int return_value;
	string msg;

	cin >> M >> N >> P;
	cin >> w >> x >> y >> z;

	cout << M << " " << N << " " << P << endl;
	cout << w << " " << x << " " << y << " " << z << endl;

	cout << "input taken\n";

	//glbal variable initilization
	kiosk_index = new int[M];
	for(int i = 0; i < M; i++){
		kiosk_index[i] = 0;
	}

	// semaphore initialization
	return_value = sem_init(&kiosk,0,M);
	if(return_value != 0) {
		cout << "kiosk semaphore initialization failed\n";
	}

	for(int i = 0; i < N; i++){
		return_value = sem_init(&belt[i],0,P);
		if(return_value != 0) {
			cout << "belt semaphore initialization failed\n";
		}
	}
	
	// mutex initialization
	return_value = pthread_mutex_init(&check_in,NULL);
	if(return_value != 0) {
		cout << "boarding mutex initialization failed\n";
	}
	
	kiosk_mutex = new pthread_mutex_t[M];
	for(int i = 0; i < M; i++){
		return_value = pthread_mutex_init(&kiosk_mutex[i],NULL);
		if(return_value != 0) {
			cout << "kiosk mutex initialization failed\n";
		}
	}
	
	for(int i = 0; i < global_variable; i++){
		return_value = pthread_mutex_init(&global_var_mutex[i],NULL);
		if(return_value != 0) {
			cout << "global variable mutex initialization failed\n";
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
	for(int i = 0; i < M; i++){
		return_value = pthread_mutex_destroy(&kiosk_mutex[i]);
		if(return_value != 0) {
			cout << "kiosk mutex destruction failed\n";
		}
	}

	return_value = pthread_mutex_destroy(&check_in);
	if(return_value != 0) {
		cout << "boarding mutex destruction failed\n";
	}


	return 0;
}
