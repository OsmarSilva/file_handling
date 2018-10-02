/*
 * parse.h
 *
 *  Created on: Sep 30, 2018
 *      Author: icts
 */

#ifndef DECODER_H_
#define DECODER_H_

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#ifdef __cplusplus
 extern "C" {
#endif

void handleData(char const *filename);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* DECODER_H_ */
