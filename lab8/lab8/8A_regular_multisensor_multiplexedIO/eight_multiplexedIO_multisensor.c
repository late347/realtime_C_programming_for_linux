#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/time.h>
#include "MultiSensorSimulator.h"




int main(int argc, char const *argv[])
{
	/* code */
	const int totalSensors = 10;
	int sensorReadResults[10] = {1}; //contains results from each sensorsRead, so you can see who ended sending
	int measurementCount = 0; //for debugging the count of measurements
    
    Tmeas measurement;
	int sensorDescriptors[10];	//sensorDescriptors for each sensor
	StartSimulator(sensorDescriptors, 5); //start simulation
	ssize_t sensorRead; //temp variable to store one readResult from a sensor
	
	int keepReadingSensors = 1;
	int guardVal = 0;
	fd_set fdset;	//descriptors for select()
	fd_set writefdset; //descriptors for select
	int n;	//how many descriptors did something with select()
	int maxFD = sensorDescriptors[0];

	struct timespec theTotalDelay; //temp variable to hold theDelay data from timespec from sensor
	struct timespec deltaTime;
	struct timespec curTime;

	theTotalDelay.tv_sec=0;
	theTotalDelay.tv_nsec=0;
	deltaTime.tv_sec=0;
	deltaTime.tv_nsec=0;
	curTime.tv_sec=0;
	curTime.tv_nsec=0;



/*check maxFD*/
	for (int i = 1; i < totalSensors; ++i){
		if (maxFD < sensorDescriptors[i])
		{
			maxFD = sensorDescriptors[i];
		}
	}

	if (maxFD < STDOUT_FILENO)
	{
		maxFD = STDOUT_FILENO; //unlikely to happen but anyways...
	}


	while(keepReadingSensors){

			/*initialize fdset in the beginning, with known activeSensorFDs*/

		FD_ZERO(&fdset);
		FD_ZERO(&writefdset); //put STDOUT into writefdset???
		for (int i = 0; i < totalSensors; ++i){
			if(sensorDescriptors[i] != -1) {
				FD_SET(sensorDescriptors[i], &fdset);
			}
		}
			//FD_ZERO(&writefdset);
		FD_SET(STDOUT_FILENO, &writefdset );

		n = select( maxFD+1, &fdset, &writefdset, NULL, NULL );

		if( n > 0 ) {

			int activeSensorCount = totalSensors;
			/*print out the values from all active Sensors*/
			for (int i = 0; i < totalSensors; ++i){
				if ( FD_ISSET(sensorDescriptors[i], &fdset) ){
					sensorRead = read( sensorDescriptors[i], &measurement, sizeof(Tmeas) );
					//sensorReadResults[i] = sensorRead; //put the readREsults into storage		

					if (sensorRead == -1){
						printf("error occured at sensorRead\n");
						exit(1);
					} else if (sensorRead > 0){
						++measurementCount;	//for debugging print the counts
						clock_gettime(CLOCK_REALTIME, &curTime);
						deltaTime = diff_timespec(&measurement.moment, &curTime);
						increment_timespec(&theTotalDelay, &deltaTime);
						printf("sensorIndex %d, measurementValue %d, totalMeasurements %d\n", i, measurement.value, measurementCount );

					} else if (sensorRead == 0){ // zero indicates an inactive sensor existed
						--activeSensorCount;
					}
				}
			}

			/* prepare to end the mainloop, if all sensors were inactive*/			
			
			if (activeSensorCount == 0){
					keepReadingSensors = 0; 
					printf("all sensors Inactive, ending program...\n");
					printf("nanosec delay was %ld", theTotalDelay.tv_nsec);
			}


				

		} else if (n == -1){
			printf("error in select happened\n");
			exit(1);
		} else if(n == 0){
			printf("no descriptors were ready\n");
		}

	}


		return 0;
}
