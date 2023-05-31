#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2

#define MAN	         0
#define WOMAN	     1

class Person
{

	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_enqueue;
	struct timeval t_dequeue;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;

	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);
	std::string get_str_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	void set_enqueue_time(void);
	void set_dequeue_time(void);
	long get_time(void);
	long get_queue_time(void);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
};


// Class for the restroom
// You may need to add more class member variables and functions
class Restroom 
{
	int status;
	std::string str_status;
	std::vector<Person> vec;
	int num_women;
	int num_men;

public:
	Restroom()
	{
		status = EMPTY;
		num_women = 0;
		num_men = 0;
	}

	// You need to use this function to print the Restroom's status
	void print_status(void);
	void print_room(void);
	int get_status(void);
	std::string get_str_status(void);
	int get_num_women(void);
	int get_num_men(void);
	int get_size(void);
	Person get_person(int);

	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);
	void remove_person(int index);
};
#endif