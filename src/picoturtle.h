/*
* picoturtle: picoturtle.h
* Created on: 19-Feb-2019
* 
* picoturtle
* Copyright (C) 2018 Abhishek Mishra <abhishekmishra3@gmail.com>
*/

#ifndef SRC_PICOTURTLE_H_
#define SRC_PICOTURTLE_H_

#include <stdint.h>

void forward(uint64_t distance);

void backward(uint64_t distance);

void left(double angle_deg);

void right(double angle_deg);

void clear();

void penup();

void pendown();


#endif /* SRC_PICOTURTLE_H_ */
