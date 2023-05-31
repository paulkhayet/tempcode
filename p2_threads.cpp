#include "p2_threads.h"
#include "utils.h"

using namespace std;

extern void show_line(queue<Person>);

pthread_cond_t  input_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t input_mutex = PTHREAD_MUTEX_INITIALIZER;
queue<Person>   line;
int		input_done;

pthread_cond_t  room_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t room_mutex = PTHREAD_MUTEX_INITIALIZER;
Restroom room = Restroom();
int		queue_done;

// Thread to add persons to the line
void *input_thread(void *args)
{
	int	persons = *(int *) args;
	long stay_time;
	int	gender;
	int	men = 0;
	int	women = 0;
	int	i;

	// Set up the line of people waiting to be processed.
	//printf("Input thread started\n");
	input_done = 0;
	persons *= 2;
	for (i = 0; i < persons; i++) 
	{
		Person pers;
		gender = gen_random(0, 2) - 1;
		if (gender) 
		{
			if (women >= (persons / 2)) 
			{
				gender = 0;
			} 
			else 
			{
				women++;
			}
		} 
		else 
		{
			if (men >= (persons / 2)) 
			{
				gender = 1;
			} else {
				men++;
			}
		}
		stay_time = gen_random(3, 10);
		pers.set_order(i + 1);
		pers.set_gender(gender);
		pers.set_time(stay_time);
		printf("[%ld ms][Input] A person (%s) goes into the queue\n", get_runtime(), pers.get_str_gender().c_str());
		pthread_mutex_lock(&input_mutex);
		pers.set_enqueue_time();
		line.push(pers);
		//printf("Sending signal\n");
		pthread_mutex_unlock(&input_mutex);
		pthread_cond_signal(&input_cond);
	}
	//show_line(line);
	input_done = 1;
	//printf("Input thread completed\n");
	pthread_exit(0);
}

// Thread to remove persons from the line and place them in the restroom
void *queue_thread(void *args)
{
	Person	pers;
	int	delay = 0;

	//printf("Queue thread started\n");
	pthread_mutex_lock(&input_mutex);
	queue_done = 0;
	while (!line.empty() || !input_done) 
	{
		if (!input_done) 
		{
			//printf("Queue thread waiting on signal\n");
			pthread_cond_wait(&input_cond, &input_mutex);
		}
		pers = line.front();
		line.pop();
		pers.set_dequeue_time();
		pthread_mutex_unlock(&input_mutex);
		if (pers.get_gender()) 
		{
			woman_wants_to_enter(pers);
		} 
		else 
		{
			man_wants_to_enter(pers);
		}
		pthread_mutex_lock(&input_mutex);
	}
	queue_done = 1;
	pthread_mutex_unlock(&input_mutex);
	//printf("Queue thread completed\n");
	pthread_exit(0);
}

// Thread to remove persons from the restroom
void *room_thread(void *args)
{
	Person	pers;
	int	i;

	//printf("Room thread started\n");
	pthread_mutex_lock(&room_mutex);
	while (!queue_done || room.get_size()) 
	{
		for (i = 0; i < room.get_size(); i++) 
		{
			pers = room.get_person(i);
			if (pers.ready_to_leave()) 
			{
				if (pers.get_gender()) 
				{
					woman_leaves(i);
				} 
				else 
				{
					man_leaves(i);
				}
			}
		}
		pthread_mutex_unlock(&room_mutex);
		usleep(MSEC(1));
		pthread_mutex_lock(&room_mutex);
	}
	//printf("Room thread completed\n");
	pthread_exit(0);
}

// A woman wishes to enter the restroom
void woman_wants_to_enter(Person& pers)
{
	int	room_status;

	pthread_mutex_lock(&room_mutex);
	room_status = room.get_status();
	while (room_status != EMPTY && room_status != WOMENPRESENT) 
	{
		pthread_mutex_unlock(&room_mutex);
		usleep(MSEC(1));
		pthread_mutex_lock(&room_mutex);
		room_status = room.get_status();
	}
	room.add_person(pers);
	pthread_mutex_unlock(&room_mutex);
	printf("[%ld ms][Queue] Send (%s) into the restroom (Stay %ld ms), ", get_runtime(), pers.get_str_gender().c_str(), pers.get_time());
	printf("Status: Total: %d (Men: %d, Women: %d)\n", room.get_num_men() + room.get_num_women(), room.get_num_men(), room.get_num_women());
	usleep(MSEC(gen_random(1, 5)));
	return;

}

// A man wishes to enter the restroom
void man_wants_to_enter(Person& pers)
{
	int	room_status;

	pthread_mutex_lock(&room_mutex);
	room_status = room.get_status();
	while (room_status != EMPTY && room_status != MENPRESENT) 
	{
		pthread_mutex_unlock(&room_mutex);
		usleep(MSEC(1));
		pthread_mutex_lock(&room_mutex);
		room_status = room.get_status();
	}
	room.add_person(pers);
	pthread_mutex_unlock(&room_mutex);
	printf("[%ld ms][Queue] Send (%s) into the restroom (Stay %ld ms), ", get_runtime(), pers.get_str_gender().c_str(), pers.get_time());
	printf("Status: Total: %d (Men: %d, Women: %d)\n", room.get_num_men() + room.get_num_women(), room.get_num_men(), room.get_num_women());
	usleep(MSEC(gen_random(1, 5)));
	return;
}

// A man is about to leave the restroom.
// room mutex must be locked
void woman_leaves(int index)
{
	room.remove_person(index);
}

// A man is about to leave the restroom.
// room mutex must be locked
void man_leaves(int index)
{
	room.remove_person(index);
}
