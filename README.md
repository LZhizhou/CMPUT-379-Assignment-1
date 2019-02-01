# CMPUT-379-Assignment-1

name		ccid
Zhizhou Lu	zhizhou
Ruoqi Yang	ry
Design:
	Our goal is to transmit the message using signals. For two signal option, we think about using SIGUSR1 and SIGUSR2 to represent “1” and “0” since every character on the ASCII table can be transform into binary number. Similarly, for one signal option, we only have one signal. For this situation we consider of using time interval with different lengths to distinguish “1” and “0”.
	We want message sent after the user pressed the “Enter”, so we use fgets() function to detect the end of the message and write a function in receiver to detected the “\n”.
Implementation:
	For the double signal option, our code follows and executes what we desired in our design part. For the single signal option, since the time interval is unstable due to CPU and many other factors, some errors can happen due to the overlong short-time-interval. We can successfully detect the errors caused by unstable signal for the ASCII number out of range. 
Questions:
1.	How do you code messages for the one signal option (only SIGUSR1 used)?
Each character (alphabets, number 0-9, and space) in the message from left to right can be encoded into an 8-bit binary number according to ASCII table. Thus, we need to use one signal, SIGUSR1, to represent “1” and “0” to transmit the information. 
Since each message user entered is ended with “\n” so we use fgets () function to get the message. After we decode it into amounts of 8-bit number using handler(), the message entered will be traced from left to right bit by bit. After that the sender will send the “SIGUSR1” twice with a short interval (150 microsecond) if the bit is “0” and with a long “interval” (1500 microsecond) if the bit is “1”. The range of interval we use to determine the “long” or “short” is 1300 microsecond.  
The first signal received is used to identify which mode we use. For one signal option, the sender is under “single” mode, and the first signal received is SIGUSR1. It starts decode the signals into bits of “0” or “1” after the mode is determined. Every 8-bit number received can be decode into a character according to ASCII. The decoded character will be appended into a character array called “res”. It will repeats receiving, decoding and adding until the character “\n” is received. After that, it done the reception of this message. After the message is printed out, the stored array “res” will be clear up. 
2.	How do you code messages for the two signals option (both SIGUSR1 and SIGUSR2 used)?
It’s similar as how we code messages for only SIGUSR1 used. For this option we still use fgets() to read the message and encode it into amounts of 8-bit binary numbers from left to right. For this mode we send SIGUSR1 if the bit read is “0” and send SIGUSR2 if the bit is “1”.
The send and receive processes are the same as the single mode.
3.	How do you represent message boundaries?
The message boundary is “\n” which can be detected by fgets() and the receiver: break if it detects “\n”.
4.	How do you check for errors?
If the ASCII number we get is out of the range, i.e. [32, 126], it will be detected as an error and it will be printout starting with a question mark.
5.	What errors may you not be able to catch?
For the single mode, since the “1” and “0” bit is determined by time interval between the signal and the time interval can be affected by various factors to become longer unpredictable. We can not catch this kind of error and it will seldom happen.
6.	How do you handle the interleaving of input/output?
The fgets() we use can detect the end of a message. It sends the message only after the user pressed “Enter”, no matter if it prints out the message received from the other program.
The message received from the other send will be printed out when you are entering.
7.	Is there any other signal you had to use (and why)? -- such signal is of course not allowed for communication
We only used SIGUSR1 and SIGUSR2 as required.
8.	How do you ensure that you do not spend unnecessary CPU cycles?
It will decode and print out the message only after the user entered something, or it will keep waiting.


For the working process, we worked together and discuss different ideas and methods we thought. One of us put the method into implementation and the other test the code and found the errors.
We run and test the code on official VM.
