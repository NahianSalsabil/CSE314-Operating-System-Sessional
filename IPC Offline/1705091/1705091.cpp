#include<bits/stdc++.h>
#include<pthread.h>
#include <unistd.h>
#include<semaphore.h>

using namespace std;

#define number_of_passengers 10
#define number_of_belt 3



sem_t kiosk, belt[number_of_belt];
pthread_mutex_t boarding;
//char message[100];



void* journey_by_air(void* arg)
{
	int id = *((int*) arg);
	
	while(true)
	{
		

	}

}

int main(void)
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

	pthread_t passengers[number_of_passengers];
	pthread_t ta;

	int return_value;

	int K,N,P;
	cin >> K >> N >> P;

	// semaphore initialization
	return_value = sem_init(&kiosk,0,K);
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
	return_value = pthread_mutex_init(&boarding,NULL);
	if(return_value != 0) {
		cout << "boarding mutex initialization failed\n";
	}

	//pthread_attr_init(&ta);
	//pthread_attr_setschedpolicy(&ta,SCHED_RR);


	// initializing passenger thread
	for(int i = 0; i < number_of_passengers; i++){
		pthread_create(&passengers[i],NULL,journey_by_air,NULL);
		int* id = new int(i+1);

		return_value = pthread_create(&passengers[i], NULL, journey_by_air, (void*)id);
		if(return_value != 0) {
			cout << "passenger " << i << " thread creation failed\n";
		}
	}


	// wait for passenger thread to finish
	for(int i = 0; i < number_of_passengers; i++){
        void *result;
        pthread_join(passengers[i],&result);
        cout << (char*)result << endl;
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
	return_value = pthread_mutex_destroy(&boarding);
	if(return_value != 0) {
		cout << "boarding mutex destruction failed\n";
	}
}
