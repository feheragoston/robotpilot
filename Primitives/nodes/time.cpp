/*
 * time.cpp
 *
 *  Created on: 2011.05.28.
 *      Author: triszalo
 */



#include "include/time.h"




///////////////////////////////////////////////////////////////////
void TimeMeasure(struct timeval *time_start, struct timeval *time_elapsed) {
			// Eltelt ido meresere szolgalo fuggveny.
			// Megmeri, hogy mennyi ido telt el time_start ota,
			// es beleirja time_elapsed-be.
			// Legegyszerubb hasznalata a kovetkezo:
			//
			//	struct timeval time_start, time_elapsed;
			//	gettimeofday(&time_start, NULL) {
			//  TimeMeasure(&time_start, &time_elapsed) {
			//

	struct timeval	time_now;
	struct timeval	time_temp;

	time_temp.tv_sec = time_start->tv_sec;
	time_temp.tv_usec = time_start->tv_usec;

	gettimeofday(&time_now, NULL);


	// Perform the carry for the later subtraction by updating time_start.
	if (time_now.tv_usec < time_temp.tv_usec) {
	int nsec = (time_temp.tv_usec - time_temp.tv_usec) / 1000000 + 1;
	time_temp.tv_usec -= 1000000 * nsec;
	time_temp.tv_sec += nsec;
	}
	if (time_now.tv_usec - time_temp.tv_usec > 1000000) {
	int nsec = (time_now.tv_usec - time_temp.tv_usec) / 1000000;
	time_temp.tv_usec += 1000000 * nsec;
	time_temp.tv_sec -= nsec;
	}

	// Compute the time remaining to wait. tv_usec is certainly positive.
	time_elapsed->tv_sec = time_now.tv_sec - time_temp.tv_sec;
	time_elapsed->tv_usec = time_now.tv_usec - time_temp.tv_usec;
}
