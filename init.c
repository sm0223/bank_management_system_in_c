#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "string.h"
#include "./structures.c"



int main(int argc, char const *argv[])
{

	struct account acc1;
	struct transaction trans;
	acc1.acc_no = 11111;  
	memcpy(acc1.password, "admin", 20); 
	
	acc1.usertype=1; 
	int fd = open("account", O_CREAT|O_TRUNC|O_RDWR, 0644) ;
	write(fd, &acc1, sizeof(acc1)); 


	close(fd);
	
	return 0;
}
