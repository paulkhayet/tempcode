#include "types_p2.h"
#include "utils.h"

extern Restroom room;
void Person::set_gender(int data)
{
	gender = data;
	if (gender) 
	{
		str_gender = "Woman";
	} 
	else 
	{
		str_gender = "Man";
	}
}

int Person::get_gender(void)      { return gender; }
std::string Person::get_str_gender(void)      { return str_gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }
void Person::set_enqueue_time(void) { gettimeofday(&t_enqueue, NULL); }
void Person::set_dequeue_time(void) { gettimeofday(&t_dequeue, NULL);  }
long Person::get_time() { return time_to_stay_ms; }
long Person::get_queue_time() { return get_elapsed_time(t_enqueue, t_dequeue); }
int Person::ready_to_leave(void) 
{
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elapsed_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}

void Person::start(void) 
{
	gettimeofday(&t_start, NULL);
	printf("(%lu)th person enters the restroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elapsed_time(t_create, t_start));
}

void Person::complete(void) 
{
	gettimeofday(&t_end, NULL);
	printf("(%lu)th person comes out of the restroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elapsed_time(t_create, t_end));
	printf(" - (%lu) milliseconds after using the restroom\n", get_elapsed_time(t_start, t_end));
}

Person::Person() 
{
	gettimeofday(&t_create, NULL);
}

// Print the list of people in the room
void Restroom::print_room(void) 
{
	int	i;
	Person	*p;

	for (i = 0; i < room.vec.size(); i++) 
	{
		p = &room.vec[i];
		printf("Gender: %s, order: %ld, use_order: %ld, stay_time: %ld\n",
		p->get_str_gender().c_str(), p->get_order(),
		p->get_use_order(), p->get_time());
	}
}

int Restroom::get_status(void) 
{
	return status;
}

std::string Restroom::get_str_status(void)      { return str_status; }

// Room mutext must be locked prior to calling this.
void Restroom::add_person(Person& p) 
{
	int	gender;

	p.start();
	room.vec.push_back(p);
	gender = p.get_gender();
	if (gender == WOMAN) 
	{
		num_women++;
		status = WOMENPRESENT;
		str_status = "WomenPresent";
	} 
	else 
	{
		num_men++;
		status = MENPRESENT;
		str_status = "MenPresent";
	}
	printf("[%ld ms][Restroom] (%s) goes into the restroom, State is ", get_runtime(), p.get_str_gender().c_str());
	printf("(%s) Total: %d (Men: %d, Women: %d)\n", str_status.c_str(), num_women + num_men, num_men, num_women);
}

int Restroom::get_size(void) 
{
	return vec.size();
}

Person Restroom::get_person(int index) 
{
	return vec[index];
}

int Restroom::get_num_women(void) 
{
	return num_women;
}


int Restroom::get_num_men(void) 
{
	return num_men;
}

// Room mutext must be locked prior to calling this.
void Restroom::remove_person(int index) 
{
	int	gender;
	int status_changed = 0;

	gender = room.vec[index].get_gender();
	if (gender == WOMAN) 
	{
		if (num_women >= 1) 
		{
			num_women--;
		} 
		else 
		{
			fprintf(stderr, "num_women count is invalid (%d)\n", num_women);
		}
		if (num_women == 0) 
		{
			status = EMPTY;
			str_status = "Empty";
			status_changed++;
		}
	} 
	else 
	{
		if (num_men >= 1) 
		{
			num_men--;
		} 
		else 
		{
			fprintf(stderr, "num_men count is invalid (%d)\n", num_men);
		}
		if (num_men == 0) 
		{
			status = EMPTY;
			str_status = "Empty";
			status_changed++;
		}
	}
	printf("[%ld ms][Restroom] (%s) left the restroom. ", get_runtime(), room.vec[index].get_str_gender().c_str());
	if (status_changed) 
	{
		printf("status changed, ");
	} 
	else 
	{
		printf("status is unchanged, ");
	}
	printf("Status is (%s): Total: %d (Men: %d, Women: %d)\n", str_status.c_str(), num_men + num_women, num_men, num_women);
	room.vec[index].complete();
	room.vec.erase(room.vec.begin() + index);
}
