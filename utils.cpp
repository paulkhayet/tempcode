#include "utils.h"

extern struct timeval t_global_start;

// return the elapsed time between the two timvals
long get_elapsed_time(struct timeval& start, struct timeval& end)
{

	long mtime = 0;
	long seconds  = end.tv_sec  - start.tv_sec;
	long useconds = end.tv_usec - start.tv_usec;

	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	return mtime;
}

// generate a random number between min and max
int gen_random(int min, int max)
{
    int rnum;
    int fd;
    int seed;

    if ((fd = open("/dev/urandom", O_RDONLY)) < 0) 
	{
		fprintf(stderr, "Failed to open random device\n");
        return -1;
    }
    if (read(fd, &seed, sizeof (int)) != sizeof (int)) 
	{
        fprintf(stderr, "Failed to read from random device\n");
		close(fd);
		return -1;
    }
	close(fd);

    srand(seed);
    rnum = (rand() % (max - min)) + min + 1;
    return rnum;
}

// display the line, for debugging
void show_line(std::queue<Person> line)
{
	int	i;

	for (i = 0; i < line.size(); i++) 
	{
		printf("Order: %ld, Gender (%d): %s, Stay_time: %ld\n",
		line.front().get_order(),
		line.front().get_gender(),
		line.front().get_str_gender().c_str(),
		line.front().get_time());
		line.push(line.front());
		line.pop();
	}
}

// return the elapsed time since the progran started
long get_runtime(void)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return get_elapsed_time(t_global_start, now);
}
