#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MEM_ALLOC_FAILIURE 30
#define TOO_FEW_ARGS 31
#define NS_FILE_ERR 32
#define NS_ERR 100
#define MTU 1500

int main(int argc, char* argv[]){
int net_ns_fd;
struct sockaddr_in lsock;
char buffer[MTU + 1];
int lsock_fd;

if(argc < 2){
fprintf(stderr, "Too few arguments.\n");
fprintf(stdout, "Syntax:\n");
fprintf(stdout, "nstest pid\n");
exit(TOO_FEW_ARGS);
}

char *fname;
fname = (char *) malloc(PATH_MAX);
if(fname == NULL){
 fprintf(stderr, "Memory allocation failed. Exiting.\n");
 exit(MEM_ALLOC_FAILIURE);
}
strcpy(fname, "/proc/");
strcat(fname, argv[1]);
strcat(fname, "/ns/net");

//printf("%s\n", fname);
net_ns_fd = open(fname, O_RDONLY);
if(net_ns_fd == -1){
  fprintf(stderr, "Cannot open network namespace file.\n");
  exit(NS_FILE_ERR);
}
if(setns(net_ns_fd, CLONE_NEWNET) == -1){
 fprintf(stderr, "Namespace assignment failed!");
 exit(NS_ERR);
}

lsock_fd = socket(AF_INET, SOCK_DGRAM, 0);

memset(&lsock, 0, sizeof(lsock));
lsock.sin_family = AF_INET;
lsock.sin_addr.s_addr = htonl(INADDR_ANY);
lsock.sin_port = htons(12345);

bind(lsock_fd, (struct sockaddr *) &lsock, sizeof(lsock));


setns( open("/proc/1/ns/net", O_RDONLY), CLONE_NEWNET );
free(fname);
close(net_ns_fd);
sleep(3600); /*sleep for an hour */
return 0;
}

