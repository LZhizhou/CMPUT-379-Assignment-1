#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT 200

int ascii, element_pos = 0, start = 0, sender_mode = 0, head = 0;
// ascii: the ascii got according current element_pos
// element_pos: number of bit got for signal
// send_mode: 1 single, 2 double
// head: is 1 if received first signal, if sender_mode is single 
char res[MAX_TEXT];
// temp store the message
struct timespec start_time, end_time;
unsigned long elapsed_time;

//return start_time - end_time/ 10e-6
long elapsed_time_ms(const struct timespec *start_time, const struct timespec *end_time)
{
    unsigned long sec, nsec;
    if (start_time == NULL || end_time == NULL)
    {
        return 0;
    }
    sec = end_time->tv_sec - start_time->tv_sec;
    nsec = end_time->tv_nsec - start_time->tv_nsec;
    if (sec < 0)
    {
        return 0;
    }
    if (nsec < 0)
    {
        sec--;
        nsec += 1000000000;
    }
    return sec * 1000000 + nsec / 1000;
}

//ascii is the ascii number of the letter sent though signals, 8 bit
//element_pos points the 8 bit of binary ascii from left to right
void handler(int signal)
{

    // get the mode of the sender
    switch (sender_mode)
    {
    case 0:
        switch (signal)
        {
        case SIGUSR1:
            sender_mode = 1;
            return;
        case SIGUSR2:
            sender_mode = 2;
            return;
        }

    case 1:
        switch (head)
        {
        case 0:
            head = 1;
            // record the time of the first signal
            clock_gettime(CLOCK_MONOTONIC, &start_time);
            return;
        case 1:
            head = 0;
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            elapsed_time = elapsed_time_ms(&start_time, &end_time);
            // time interval of two signals
            //printf("time is %ld\n", elapsed_time);
            if (elapsed_time > 1300)
            {
                ascii += 1 << (7 - element_pos);
            }
            element_pos = (element_pos + 1) % 8;
            //printf("element pos is %d\n", element_pos);
            //clock_gettime(CLOCK_MONOTONIC, &start_time);
        }
        break;

        //printf("time is %ld\n", elapsed_time);
        //printf("space is %d\n",space);

        //ascii += 1 << (7 - element_pos);
        //printf("ascii is %d\n",ascii);

    case 2:
        switch (signal)
        {
        case SIGUSR2:

            ascii += 1 << (7 - element_pos);

        case SIGUSR1:
        default:
            element_pos = (element_pos + 1) % 8;
            break;
        }
        //decode singals for mode double
    }
    if (element_pos == 0)
    {
        // received 8 bit, can get the corresponding char 
        res[start] = ascii;
        start++;
        int err = 0;
        if (ascii == 10)
        {
            for (int i = 0; i < strlen(res); i++)
            {
                if (res[i] != 10 && (res[i] <= 31 || res[i] >= 127))
                {
                    err = 1;
                    break;
                    // error if there is a character is not printable
                }
            }
            if (err == 0)
            {
                printf("!");
            }
            else
            {
                printf("?");
            }
            for (int i = 0;; i++)
            {
                printf("%c", res[i]);
                //print message
                if (res[i] == '\n')
                {
                    break;
                }
            }
            //printf("!%s\n", res);
            start = 0;
            for (int i = 0; i < strlen(res) + 1; i++)
            {
                res[i] = 0;
                //clear res
            }
        }
        ascii = 0;
    }
}

int main(int argc, char const *argv[])
{

    printf("Own PID: %d\n", getpid());
    // print pid of this process

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_flags = SA_RESTART;
    //copy form lab

#if defined(SINGLE)
    int mode = 1;
    sigaction(SIGUSR1, &sa, NULL);
#else
    int mode = 2;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
#endif

    pid_t pid;
    scanf("%d", &pid);
    //get the pid want to commnicate with
    switch (mode)
    {
    case 1:
        kill(pid, SIGUSR1);
        break;

    case 2:
        kill(pid, SIGUSR2);
        break;
    }
    usleep(200);
    while (1)
    {

        char sent_message[MAX_TEXT];

        fgets(sent_message, MAX_TEXT-1, stdin);

        //check if user enter a word
        if (sent_message[0] >= 33)
        {
            
            if (sent_message[MAX_TEXT-2] >= 33 && sent_message[MAX_TEXT-1] == '\n'){
                sent_message[MAX_TEXT-1] = '\n';
            }
            for (int i = 0; i < strlen(sent_message); i++)

            //for each letter in the message
            {

                for (int j = 7; j >= 0; --j)
                //encode letter into binary, order of bit is from left to right
                {
                    switch (mode)
                    {
                    case 1:
                        if (sent_message[i] & (1 << j))
                        {
                            //printf("1");
                            kill(pid, SIGUSR1);
                            usleep(1500);
                            kill(pid, SIGUSR1);
                        }
                        else
                        {
                            //printf("0");

                            kill(pid, SIGUSR1);
                            usleep(150);
                            kill(pid, SIGUSR1);
                        }
                        break;

                    case 2:
                        if (sent_message[i] & (1 << j))
                        {
                            kill(pid, SIGUSR2);
                        }
                        else
                        {

                            kill(pid, SIGUSR1);
                        }
                        break;
                    }

                    usleep(150);
                }
            }
        }

        for (int i = 0; i < strlen(sent_message); i++)
        {
            sent_message[i] = 0;
        }
    }

    return 0;
}
