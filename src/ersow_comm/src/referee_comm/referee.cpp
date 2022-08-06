#include <stdio.h>
#include <ros/ros.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>

#include <unistd.h>
#include <time.h>

#include <sys/time.h>

#include <stdlib.h>

// Include message library  
#include "ersow_comm/data_agent0.h"
#include "ersow_comm/strategyMsg.h"
#include "ersow_comm/refereeMsg.h"
#include "ersow_comm/data_gui.h"

#define PERRNO(txt) \
	printf("ERROR: (%s / %s): " txt ": %s\n", __FILE__, __FUNCTION__, strerror(errno))

#define PERR(txt, par...) \
	printf("ERROR: (%s / %s): " txt "\n", __FILE__, __FUNCTION__, ## par)

#ifdef DEBUG
#define PDEBUG(txt, par...) \
	printf("DEBUG: (%s / %s): " txt "\n", __FILE__, __FUNCTION__, ## par)
#else
#define PDEBUG(txt, par...)
#endif

#define CONNECTED 		1
#define DISCONNECTED	0

#define READY           1
#define NOT_READY       0    

// #define DEBUG_AGENT1
// #define DEBUG_AGENT2
// #define DEBUG_AGENT3
// #define DEBUG_REFEREE

typedef struct _ref
{
	int socket;
	int state;
	char isConnected;
	char* address;
	int port;
    int last_state;
    char ready;
} _referee;

_referee referee;

// Global var
int end;
int timer;
double delay;

// declaration of message object
ersow_comm::refereeMsg PubRefereeMsg;
ersow_comm::data_agent0 data_agent0;


//	*************************
//  Signal catch
//
static void signal_catch(int sig)
{
	if (sig == SIGINT)
		end = 1;
	else
		if (sig == SIGALRM)
			timer++;
}

void callbackGUI(const ersow_comm::data_gui& msgHandler)
{
    std::string str = msgHandler.ipaddr;
    
    referee.address = &str[0];
    referee.port = msgHandler.port;        
}


// *************************
//   sleep function
// *************************
void sleep(double delay)
{
    time_t timer0, timer1;

    time(&timer0);

    do
    {
        time(&timer1);
    } while(difftime(timer1, timer0)<delay);
}

void *listenReferee(void*)
{	
	int new_conn_ref;
	char echobuffer[2] = "S";
        
    while(!end)
    {            
        if (referee.isConnected == CONNECTED)
        {
            // just receive data
            int retval;
            int error;
            struct timeval tv;
            int nbytes; 

            /* -------------- checking file descriptor ------------- */
            socklen_t len = sizeof(error);
            if((retval = getsockopt(referee.socket, SOL_SOCKET, SO_ERROR, &error, &len)) != 0) 
            {
                fprintf(stderr, "getting error socket code: %s\n", strerror(retval));
                exit(1);
            }

            if(error != 0) {
                fprintf(stderr, "socket error: %s\n", strerror(error));
            }


            /* -------------- Set timeout ----------------- */
            tv.tv_sec  = 0;          /* Secs Timeout */
            tv.tv_usec = 10E3;     /*  set timeout 10ms */

            if (setsockopt(referee.socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) != 0) 
            {                
                perror("setsockopt: rcvtimeo");
                exit(1);
            }

            /* -------------- Start receiving data from referee ---------- */
            int size_to_recv = 2;

            if((nbytes = recv(referee.socket, echobuffer, size_to_recv, 0)) > 0) 
            // {
            //     fprintf(stderr, "[Error] Not receiving data from refbox\n");    
            // } else 
            {
                echobuffer[nbytes] = '\0';
                fprintf(stdout, "connection: %d, State %s, %d Bytes\n",referee.socket, echobuffer,nbytes);    
                fprintf(stdout, "state refbox: %d\n", int(echobuffer[nbytes-1]));
                referee.state = int(echobuffer[nbytes-1]);


                referee.last_state = referee.state;
            } 
            else 
                referee.state = referee.last_state;
            
    
            fflush(stdout);         

        } else if (referee.isConnected == DISCONNECTED) {
            
            // create socket connection
            referee.socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

            /* ------------- Set-up referee address from struct ---------------- */ 
            struct sockaddr_in referee_address;
            referee_address.sin_family = AF_INET;
            
            referee_address.sin_addr.s_addr = inet_addr(referee.address);
            referee_address.sin_port = htons(referee.port);
        
            /* --------------- Create connection ------------------   */    
            new_conn_ref = connect(referee.socket,(struct sockaddr *)&referee_address, sizeof(referee_address));
            
            /* ---------------- Set flag connection --------------- */ 
            if(new_conn_ref < 0) 
            {
                referee.isConnected = DISCONNECTED;
                close(referee.socket);            
            } 
            else referee.isConnected = CONNECTED;
                // ROS_INFO("Connected to refbox");    
        }
    }

}

int main(int argc, char *argv[])
{
	pthread_t refereeThread;
	pthread_attr_t thread_attr;

    // initial ros 
    ros::init(argc, argv, "referee_node");
    ros::NodeHandle referee_node;

    // Publish data strategy
    ros::Publisher PubReferee = referee_node.advertise<ersow_comm::refereeMsg>("data/referee", 100);
    ros::Subscriber SubGUI = referee_node.subscribe("data/gui", 100, &callbackGUI);

	if(signal(SIGINT, signal_catch) == SIG_ERR)
	{
		PERRNO("signal");
		return -1;
	}

	// initial referee variable
    referee.address = "NULL";
    referee.port = 0;

	referee.isConnected = DISCONNECTED;
    referee.ready = NOT_READY;

    // init global var
    end = 0;
    timer = 0;
    delay = 0.1;

#ifndef DEBUG_REFEREE
  	/* referee thread */
  	pthread_attr_init (&thread_attr);
	pthread_attr_setinheritsched (&thread_attr, PTHREAD_INHERIT_SCHED);
	if ((pthread_create(&refereeThread, &thread_attr, listenReferee, NULL)) != 0)
	{
		PERRNO("pthread_create");
		close(referee.socket);
		return -1;
	}
#endif

    // set speed program 
    ros::Rate loop_rate(50);    // 50 Hz => 20 ms

    while(!end) 
    {
    	// TODO   
        // nothing.

        // isi data publish data referee
        PubRefereeMsg.state = referee.state;
        PubRefereeMsg.connected = referee.isConnected;

#ifndef DEBUG_REFEREE
       printf("referee state, connected = %d\t%d\n", 
           PubRefereeMsg.state,
           PubRefereeMsg.connected);
#endif        

        // finally publish referee data
        PubReferee.publish(PubRefereeMsg);      

        ros::spinOnce();
        loop_rate.sleep();        
    }


    pthread_join(refereeThread, NULL);
    close(referee.socket);

	return 0;
}