#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>

#define NUM 2
#define NAMESIZE 7

struct Cars {
	char model[NAMESIZE];
	char type[NAMESIZE];
	int cost;
};

int main(int argc, char* argv[]){
	if (argc != NUM){
		printf("please 2 arguments\n");
		exit(1);
	}
	
	int structnum = atoi(argv[1]);
	int pid[NUM], i;
	char buffer[1024], buffer_online[1024];
	buffer[0] = '\0';
	struct Cars *c = malloc(sizeof(struct Cars)*structnum);
	struct Cars cc;
	
	for (int j=0; j<structnum; j++){
		printf("Enter Car model name: ");
		scanf("%s", c[j].model);
		printf("Enter Car type name: ");
		scanf(" %s", c[j].type);
		printf("Enter Car cost: ");
		scanf(" %d", &c[j].cost);
		
		sprintf(buffer_online, "model: %s\t type: %s\t cost: %d\n", c[j].model, c[j].type, c[j].cost);
		strcat(buffer, buffer_online);
	}
	
	for (i=0; i<NUM; i++){
		pid[i] = fork();
		
		if (pid[i]==0){
			break;
		}
	}
	
	if(i == 0) /*child 1*/
	{
		int fd = open("struct_cars.txt", O_WRONLY | O_CREAT | O_APPEND, 0664);
		if (fd == -1){
			perror("Open\n");
			exit(1);
		}
		write(fd, (struct Cars*)c, sizeof(struct Cars)*structnum);
		close(fd);
		exit(0);
	}
	else if(i == 1) /*child 2*/
	{
		int fd = open("string_cars.txt", O_WRONLY | O_CREAT | O_APPEND, 0664);
		if (fd == -1){
			perror("Open\n");
			exit(1);
		}
		write(fd, buffer, strlen(buffer));
		close(fd);
		exit(1);
	}
	
	while(wait(NULL) != -1);
	
	int PID = fork();
	
	if (PID == 0) /*child*/
	{
		printf("\n======== EXEC Cat string_cars.txt ========\n");
		execlp("cat", "cat", "string_cars.txt", NULL); //exec friends -> automatically exit(0); 
	}
	else if (PID > 0) /*parent*/
	{
		int status;
		wait(&status);
		
		printf("\n======== PARENT: struct_cars.txt READ ========\n");
		int fd = open("struct_cars.txt", O_RDONLY);
		if (fd == -1){
			perror("Open\n");
			exit(1);
		}
		
		while ((read(fd, &cc, sizeof(cc))) > 0)
		{
			printf("model: %s\t type: %s\t cost: %d\n", cc.model, cc.type, cc.cost);
		}
		printf("BYE::%d\n", status>>8);
	}
	else{
		printf("fail to fork\n");
		exit(1);
	}
	free(c);
	
	return 0;
}
