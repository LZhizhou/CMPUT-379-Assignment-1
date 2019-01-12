#define _POSIX_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_TEXT 20
int communication_PID;
// FIFO file path 
char *myfifo = "/tmp/myfifo"; 
int fd;

void received_signal(int signum){
    signal(signum,received_signal);
    // Open FIFO for Read only 
    fd = open(myfifo, O_RDONLY|O_NONBLOCK); 
    char received_message[MAX_TEXT];
    // Read from FIFO 
    read(fd, received_message, sizeof(received_message)); 

    // Print the read message 
    printf("! %s\n", received_message); 
    close(fd); 
}


int main(int argc, char const *argv[])
{
    printf("Own PID: %d\n", getpid());

    scanf("%d", &communication_PID);
    
    signal(SIGUSR1,received_signal);
    signal(SIGUSR2,received_signal);

  

    // Creating the named file(FIFO) 
    // mkfifo(<pathname>, <permission>)
    umask(0);
    if (access(myfifo, F_OK) != -1){
        remove(myfifo);
    }

    if (mkfifo(myfifo,0777)<0){
        
        fprintf(stderr, "Could not create fifo %s\n", myfifo);
        exit(0);
    }

    
    while (1) 
    {   
        char sent_message[MAX_TEXT] ; 
        sent_message[0] = '\0';
        // Open FIFO for write only 
        fd = open(myfifo, O_RDWR|O_NONBLOCK); 
        // Take an input sent_messageing from user. 
        // 20 is maximum length 
        
        fgets(sent_message, MAX_TEXT, stdin); 
        
        // Write the input sent_messageing on FIFO 
        // and close it 
        
        if (sent_message[0]>=48 &&sent_message[0]<=126){
            //check if user enter anything
            write(fd, sent_message, strlen(sent_message)+1);
            close(fd); 
            #if defined(SINGLE)
                kill((pid_t)communication_PID,SIGUSR1);
            #else
                kill((pid_t)communication_PID,SIGUSR2);
            #endif
        }
            
        
        
  

    } 

    return 0;
}