/*
 * tools.h
 *
 *  Created on: 2010.05.26.
 *      Author: Agoston
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <unistd.h>
#include <iostream>
#include <sys/time.h>

namespace tools {

int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);

}

#endif /* TOOLS_H_ */
