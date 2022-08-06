/* =======================================
    Author : Khoirul Anwar
	NRP    : 2210171032 
	Class  : 3-D4-Computer Engineering-B 
	BATCH  : 2017
	Email  : khoirlnwar@gmail.com
	Update : 23 December 2019
	Version: RA-TDMA 1.0
========================================= */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <stdlib.h>

#include <ros/ros.h>

#include "multicast.h"

// Include message library  
#include "ersow_comm/strategyMsg.h"
#include "ersow_comm/dataAgentMsg.h"

// 75 ms / 4 agents = 18.75 ms 

#define BUFFER_SIZE 		5000
#define TTUP_US 			60E3
#define COMM_DELAY_US 		4000
#define MIN_UPDATE_DELAY_US 1000

// #define DEBUG_STATE_AGENTS
// #define DEBUG_BALL
// #define DEBUG
// #define FILE_DEBUG
// #define UNSYNC
// #define BELUM_DEFINED
// #define DEBUG_STRATEGY
// #define DEBUG_HENDRO

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

#ifdef FILE_DEBUG
	FILE *filedebug;
#define FDEBUG(txt, par...) \
  fprintf(filedebug, txt , ## par)
#else
#define FDEBUG(file, txt, par...)
#endif

#define NOT_RUNNING			0
#define RUNNING				1
#define INSERT				2
#define REMOVE				3
#define MAX_REMOVE_TICKS	10

#define NO	0
#define YES	1

#define ERSOW_JAMIL		3
#define ERSOW_HENDRO	2
#define ERSOW_OKTO 		1
#define ERSOW_BASE 		0

#define POSX 0
#define POSY 1
#define POST 2  

#define SELF 0
#define MAX_AGENTS 5


int end;
int timer_sig;
int timer_signal;

int MAX_DELTA;

struct timeval lastSendTimeStamp;
int delay;

int lostPackets[MAX_AGENTS];
int totalLostPackets[MAX_AGENTS];
float totalDelay[MAX_AGENTS], averageDelay[MAX_AGENTS], cntDelay[MAX_AGENTS];

struct _agent
{
	char state;							          // current state
	char dynamicID;					          // position in frame
	char received;						        // received from agent in the last Ttup?
	struct timeval receiveTimeStamp;	// last receive time stamp
	int delta;							          // delta
	unsigned int lastFrameCounter;		// frame number
	char stateTable[MAX_AGENTS];		  // vision of agents state
  	int removeCounter;                // counter to move agent to not_running state
};


// struct header frames 
struct _header 
{
	unsigned int counterFrame;
	char stateTable[MAX_AGENTS];
	unsigned short int dataSize;
	unsigned char indexAgent;
};


// struct data for sending to robot
struct _robotData 
{
	short int position[3];
	short int value;

	unsigned char ball_detect;
	unsigned short int ball_position[2];
};

struct _baseData 
{
	unsigned char state[3];	
	unsigned char mode[3];

	short int target_jamil[3];
	short int target_hendro[3];
	short int target_okto[3];

	unsigned short int ball_position[2];
	char ball_detect;

	char localization_flag;
	short int localization[3];
};

int myNumber = SELF;

struct _agent agent[MAX_AGENTS];

int RUNNING_AGENTS;

// initial data untuk dikirim
struct _header header;
struct _baseData baseData;


////////////////////////// Global variable msg object ///////////////////////
ersow_comm::dataAgentMsg pubAgent1Data;
ersow_comm::dataAgentMsg pubAgent2Data;
ersow_comm::dataAgentMsg pubAgent3Data;


//	*************************
//  Signal catch
//
static void signal_catch(int sig)
{
	if (sig == SIGINT)
		end = 1;
	else
		if (sig == SIGALRM) {
			timer_sig ++;
			timer_signal ++;

			printf("timer_signal: %d\n", timer_signal);
		}
}


void setPublish_data(unsigned char indexAgent, struct _robotData &robotData, int &lostPackets)
{
	switch(indexAgent) 
	{
		case ERSOW_OKTO: 
		{
				// memcpy(&pubAgent1Data, &robotData, sizeof(robotData)); break;
				pubAgent1Data.positionx = robotData.position[POSX];
				pubAgent1Data.positiony = robotData.position[POSY];
				pubAgent1Data.positiont = robotData.position[POST];

				pubAgent1Data.ballposx = robotData.ball_position[POSX];
				pubAgent1Data.ballposy = robotData.ball_position[POSY];

				pubAgent1Data.balldetect = robotData.ball_detect;
				pubAgent1Data.value = robotData.value;

				pubAgent1Data.lostPackets = lostPackets;
			break;
		}

		case ERSOW_HENDRO: 
		{
				// memcpy(&pubAgent2Data, &robotData, sizeof(robotData)); break;
				pubAgent2Data.positionx = robotData.position[POSX];
				pubAgent2Data.positiony = robotData.position[POSY];
				pubAgent2Data.positiont = robotData.position[POST];

				pubAgent2Data.ballposx = robotData.ball_position[POSX];
				pubAgent2Data.ballposy = robotData.ball_position[POSY];

				pubAgent2Data.balldetect = robotData.ball_detect;
				pubAgent2Data.value = robotData.value;

				pubAgent2Data.lostPackets = lostPackets;

#ifdef DEBUG_BALL
				printf("Data ball agent2 = %d | %d\n", 
					pubAgent2Data.ballposx,
					pubAgent2Data.ballposy);
#endif

			break;			
		}
		
		case ERSOW_JAMIL:
		{
				// memcpy(&pubAgent3Data, &robotData, sizeof(robotData)); break;
				pubAgent3Data.positionx = robotData.position[POSX];
				pubAgent3Data.positiony = robotData.position[POSY];
				pubAgent3Data.positiont = robotData.position[POST];

				pubAgent3Data.ballposx = robotData.ball_position[POSX];
				pubAgent3Data.ballposy = robotData.ball_position[POSY];

				pubAgent3Data.balldetect = robotData.ball_detect;
				pubAgent3Data.value = robotData.value;

				pubAgent3Data.lostPackets = lostPackets;
			break;			
		} 
	}
}

void callbackStrategy(const ersow_comm::strategyMsg& strategyErsow)
{	
	// isi data base state READY or NOT_READY
	// state robot 
	baseData.state[ERSOW_OKTO]   = strategyErsow.state_agent1;
	baseData.state[ERSOW_HENDRO] = strategyErsow.state_agent2;
	baseData.state[ERSOW_JAMIL]  = strategyErsow.state_agent3;
	
	// printf("TEST\n");

	// Callback target posisi 
	baseData.target_okto[POSX] = strategyErsow.targetx_agent1;
	baseData.target_okto[POSY] = strategyErsow.targety_agent1;
	baseData.target_okto[POST] = strategyErsow.targett_agent1;
	printf("baseData.target_okto[POSX]: %d\n",baseData.target_okto[POSX]);
	printf("baseData.target_okto[POSY]: %d\n",baseData.target_okto[POSY]);
	printf("baseData.target_okto[POST]: %d\n",baseData.target_okto[POST]);

	baseData.target_hendro[POSX] = strategyErsow.targetx_agent2;
	baseData.target_hendro[POSY] = strategyErsow.targety_agent2;
	baseData.target_hendro[POST] = strategyErsow.targett_agent2;

	baseData.target_jamil[POSX] = strategyErsow.targetx_agent3;
	baseData.target_jamil[POSY] = strategyErsow.targety_agent3;
	baseData.target_jamil[POST] = strategyErsow.targett_agent3;




#ifdef BELUM_DEFINED
	for(int i=0; i<3; i++)  {
		baseData.target_okto[i] = 0;
		baseData.target_hendro[i] = 0;
		baseData.target_jamil[i] = 0;
		baseData.localization[i] = 0;
	}

	baseData.localization_flag = 0;
#endif

	// isi data mode robot
	// FSM_STRIKER
	// FSM_DEFENDER
	// FSM_KEEPER
	baseData.mode[ERSOW_OKTO]   = strategyErsow.mode_agent1;
	baseData.mode[ERSOW_HENDRO] = strategyErsow.mode_agent2;
	baseData.mode[ERSOW_JAMIL]  = strategyErsow.mode_agent3;

	// posisi bola global
	baseData.ball_position[POSX] = strategyErsow.global_ballposx;
	baseData.ball_position[POSY] = strategyErsow.global_ballposy;
	baseData.ball_detect = strategyErsow.ball_detect;

	baseData.localization[POSX] = strategyErsow.localization_posx;
	baseData.localization[POSY] = strategyErsow.localization_posy;
	baseData.localization[POST] = strategyErsow.localization_post;

	baseData.localization_flag = strategyErsow.flaglocalization;


#ifdef DEBUG_STRATEGY
	printf("bola %d\t%d\t%d\n", 
		baseData.ball_position[POSX],
		baseData.ball_position[POSY],
		baseData.ball_detect);
#endif


}


// Fungsi sinkronisasi schedule
int sync_ratdma(unsigned char indexAgent)
{	
	int realDiff, expectedDiff;
	struct itimerval timer;

	// update state dari robot 
	agent[indexAgent].received = YES;

	// hitung realDiff
	// algoritma = waktuKirimTerakhir - waktuTerimaDataRobotIndexAgent
	realDiff = (int)((agent[indexAgent].receiveTimeStamp.tv_sec - lastSendTimeStamp.tv_sec)*1E6 + (agent[indexAgent].receiveTimeStamp.tv_usec - lastSendTimeStamp.tv_usec));
	realDiff -= (int)COMM_DELAY_US; // travel time  	
  	if (realDiff < 0)
	{
		printf("*****  realDiff to agent %d = %d  *****\n", indexAgent, realDiff);
		return (2);
	}
	// printf("realDiff = %d\n", realDiff);

	// hitung expectedDiff 
	// algoritma = (dynamicIDAgent - dynamicIDmyNumberAgent) * TTUP/RUNNING_AGENTS 
	expectedDiff = (int)((agent[indexAgent].dynamicID - agent[myNumber].dynamicID) * TTUP_US/RUNNING_AGENTS);
	if (expectedDiff < 0)
		expectedDiff += (int)TTUP_US;
	// printf("expectedDiff = %d\n", expectedDiff);	

	// selisih waktu antara waktu real dan perhitungan
	agent[indexAgent].delta = realDiff - expectedDiff;
	// printf("realDiff | expectedDiff = %d | %d\n", realDiff, expectedDiff);

	// Opening file
	FILE* fptr;

	fptr = fopen("logfile.ersow", "w");
	if (fptr == NULL)
	{
		printf("Error opening file.\n");
		exit(1);
	}

	// kondisional sync apabila myNumberagent adalah 0 atau tidak
	if (agent[myNumber].dynamicID == 0)
	{
		if ((agent[indexAgent].delta > delay) && (agent[indexAgent].delta < MAX_DELTA))
		{
			if (agent[indexAgent].delta > (int)MIN_UPDATE_DELAY_US) 
			{
				delay = agent[indexAgent].delta;
				totalDelay[indexAgent]	+= delay;

				cntDelay[indexAgent] ++;


        		printf("delay between %d(%d) and %d(%d) -> %d", myNumber, agent[myNumber].dynamicID, indexAgent, agent[indexAgent].dynamicID, delay);
        		fprintf(fptr, "delay between %d(%d) and %d(%d) -> %d", myNumber, agent[myNumber].dynamicID, indexAgent, agent[indexAgent].dynamicID, delay);				
			}
		}
	} 
	else 
	{	
		// jika menerima data dari agent dynamicID 0 
		if(agent[indexAgent].dynamicID == 0)
		{
			printf("sync with agent 0 \t");
			expectedDiff = (int)(TTUP_US - expectedDiff);
			expectedDiff -= (int)COMM_DELAY_US; //travel time
			printf("expectedDiff = %d\n", expectedDiff);
			timer.it_value.tv_usec = (long int)(expectedDiff % (int)1E6);
			timer.it_value.tv_sec =(long int)(expectedDiff / (int)1E6);
			timer.it_interval.tv_usec = (__suseconds_t)(TTUP_US);
			timer.it_interval.tv_sec =0;
			setitimer(ITIMER_REAL, &timer, NULL); 
		}	
	}

	fclose(fptr);
	return (0);
}




void update_stateTable(void)
{
  	int i, j;

	for (i=0; i<MAX_AGENTS; i++)
	{
		if ( i != myNumber)
		{
			switch (agent[i].state)
			{
				case RUNNING:
					if (agent[i].received == NO)
						agent[i].state = REMOVE;
					break;
				case NOT_RUNNING:
					if (agent[i].received == YES)
						agent[i].state = INSERT;
					break;
				case INSERT:
					if (agent[i].received == NO)
						agent[i].state = NOT_RUNNING;
					else
					{
						for (j = 0; j < MAX_AGENTS; j++)
							if ((agent[j].state == RUNNING) &&
									((agent[j].stateTable[i] == NOT_RUNNING) || (agent[j].stateTable[i] == REMOVE)))
								break;
						agent[i].state = RUNNING;
					}
					break;
				case REMOVE:
					if (agent[i].received == YES)
          			{
            			agent[i].removeCounter = 0;
						agent[i].state = RUNNING;
          			}
					else
					{
						for (j = 0; j < MAX_AGENTS; j++)
							if ((agent[j].state == RUNNING) &&
									((agent[j].stateTable[i] == RUNNING) || (agent[j].stateTable[i] == INSERT)))
								break;
             			agent[i].removeCounter ++;
			            if (agent[i].removeCounter >= MAX_REMOVE_TICKS)
			            {
			               agent[i].state = NOT_RUNNING;
			               agent[i].removeCounter = 0;
			            }
					}
					break;
			}
		}
  }

  	// my state
	agent[myNumber].state = RUNNING;

	// update state age
	pubAgent1Data.state = agent[ERSOW_OKTO].state;
	pubAgent2Data.state = agent[ERSOW_HENDRO].state;
	pubAgent3Data.state = agent[ERSOW_JAMIL].state;

#ifdef DEBUG_STATE_AGENTS
	printf("Publish state = %d\t%d\t%d\n", 
		pubAgent1Data.state,
		pubAgent2Data.state,
		pubAgent3Data.state);
#endif			
}


// Threading receiver
void *recvThread(void *arg)
{
	// buffer tampung data terima
	char recvBuffer[BUFFER_SIZE];
	int indexBuffer =0;

	// object struct terima data
	struct _header header;
	struct _robotData robotData;

	// variable panjang data terima
	int recvLen;

	// index agent variable
	unsigned char indexAgent;
	while (!end)
	{
		// reset isi buffer 
		bzero(recvBuffer, BUFFER_SIZE);
		indexBuffer =0;

		// start terima data 
		if ((recvLen = receiveData(*(int*)arg, recvBuffer, BUFFER_SIZE)) > 0)
		{
			// parsing data header
			memcpy(&header, recvBuffer + indexBuffer, sizeof(header));
			indexBuffer += sizeof(header);

			indexAgent = header.indexAgent;
			// printf("indexAgent = %d\n", indexAgent);

			// perhitungan lost packets
			if ((agent[indexAgent].lastFrameCounter + 1) != (header.counterFrame))
			{
				// lost packets
				lostPackets[indexAgent] = header.counterFrame - (agent[indexAgent].lastFrameCounter + 1);
				PERR("lostPackets[%d] = %d\n", indexAgent, lostPackets[indexAgent]);
			} 
			agent[indexAgent].lastFrameCounter = header.counterFrame;

			// update data stateTable
			for(int i=0; i<MAX_AGENTS; i++)
				agent[indexAgent].stateTable[i] = header.stateTable[i];

			// catat waktu terima data
			gettimeofday(&(agent[indexAgent].receiveTimeStamp), NULL);

			// parsing data main
			memcpy(&robotData, recvBuffer + indexBuffer, sizeof(robotData));
			indexBuffer += sizeof(robotData);

			sync_ratdma(indexAgent);

			// finally publish data apa adanya
			setPublish_data(indexAgent, robotData, lostPackets[indexAgent]);
		
		// end looping while

		} 

	}

}


//	*************************
//  Main
//
int main(int argc, char *argv[])
{
	// ROS initial
	ros::init(argc, argv, "ratdma_node");
	ros::NodeHandle ratdma_node;

	// subscribe data dari strategy_node
	ros::Subscriber SubDataStrategy = ratdma_node.subscribe("data/Strategy", 100, &callbackStrategy);
	
	// publish data robot 
	ros::Publisher PubAgent1 = ratdma_node.advertise<ersow_comm::dataAgentMsg>("data/agent1", 100);
	ros::Publisher PubAgent2 = ratdma_node.advertise<ersow_comm::dataAgentMsg>("data/agent2", 100);
	ros::Publisher PubAgent3 = ratdma_node.advertise<ersow_comm::dataAgentMsg>("data/agent3", 100);


	// create socket connection
	int socket;
	std::string dev = "wlan0";  // wlp2s0 for wireless, enp3s0 for ethernet
	char *device = &dev[0];

	// open socket connection
	if((socket = openSocket(device)) == -1)
	{
		PERRNO("openSocket");
		return -1;
	} 

	// initial signal signal alarm dan signal interrupt
	if ((signal(SIGALRM, signal_catch)) == SIG_ERR)
	{
		PERRNO("signal");
		closeSocket(socket);
		return -1;
	}

	if ((signal(SIGINT, signal_catch)) == SIG_ERR)
	{
		PERRNO("signal");
		closeSocket(socket);
		return -1;
	}


	// start while forever
	end = 0;
	delay = 0;
	timer_sig = 0;
	timer_signal = 0;
	RUNNING_AGENTS = 1;


	// threading receiver data robot
	//   	/* receive thread */
	pthread_t IDrecvThread;
	pthread_attr_t thread_attr;

  	pthread_attr_init (&thread_attr);
	pthread_attr_setinheritsched (&thread_attr, PTHREAD_INHERIT_SCHED);
	if ((pthread_create(&IDrecvThread, &thread_attr, recvThread, (void *)&socket)) != 0)
	{
		PERRNO("pthread_create");
		closeSocket(socket);
		return -1;
	} 


	// siapkan buffer untuk dikirim
	char sendBuffer[BUFFER_SIZE];
	int indexBuffer = 0;


	// reset variable dynamic
	for(int i=0; i<MAX_AGENTS; i++)
	{
		lostPackets[i] = 0;
		totalLostPackets[i] = 0;
		agent[i].state = NOT_RUNNING;
		agent[i].lastFrameCounter = 0;

		totalDelay[i] = 0;
		averageDelay[i] = 0;
		cntDelay[i] = 0;
	}

	// reset counter header 
	header.counterFrame =0;

	struct timeval tempTimeStamp;
	int i,j;
	
	// Initial timer sesuai time update period
	struct itimerval timer, it;	
	timer.it_value.tv_usec = (__suseconds_t)(TTUP_US);
	timer.it_value.tv_sec = 0;
	timer.it_interval.tv_usec = (__suseconds_t)(TTUP_US);
	timer.it_interval.tv_sec = 0;
	setitimer(ITIMER_REAL, &timer, NULL);			

	// printf("while start ...\n");
	while (!end)
	{	

		// printf("datas....\n");

		// kondisional timer_sig berdasarkan alarm

		if (timer_signal == 0)
			continue;


#ifdef UNSYNC
		// dynamic agent 0
		if ((delay > (int)MIN_UPDATE_DELAY_US) && (agent[myNumber].dynamicID == 0) && timer_signal == 1)
		{
			it.it_value.tv_usec = (__suseconds_t)(delay - (int)MIN_UPDATE_DELAY_US/2);
			it.it_value.tv_sec = 0;
			it.it_interval.tv_usec = (__suseconds_t)(TTUP_US);
			it.it_interval.tv_sec = 0;
			setitimer (ITIMER_REAL, &it, NULL);
			delay = 0;
			continue;
		}
#endif

		// update isi stateTable
		update_stateTable();

		// update dynamicID
		j=0;
		for(i=0; i<MAX_AGENTS; i++)
		{
			if ((agent[i].state == RUNNING) || (agent[i].state == REMOVE))
			{
				agent[i].dynamicID = j;
				j++;
			}
			agent[myNumber].stateTable[i] = agent[i].state;
		}

		RUNNING_AGENTS = j;
		MAX_DELTA = (int)(TTUP_US/RUNNING_AGENTS * 2/3);


		// isi counterFrame dan size di header
		// set isi header
		header.indexAgent = myNumber;
		header.stateTable[myNumber] = RUNNING;
		for(int i=0; i<MAX_AGENTS; i++)
			header.stateTable[i] = agent[myNumber].stateTable[i];

		header.counterFrame ++;
		header.dataSize = sizeof(baseData);

		// printf("data = %d bytes\n", indexBuffer);	
		// printf("agent[myNumber].dynamicID = %d\n", agent[myNumber].dynamicID);
		// printf("header.indexAgent = %d\n", header.indexAgent);

		// kirim data dengan multicast reset isi buffer kirim
		bzero(sendBuffer, BUFFER_SIZE);

		// operasi memcpy untuk copy data dari object struct ke buffer
		memcpy(sendBuffer + indexBuffer, &header, sizeof(header));
		indexBuffer += sizeof(header);

		// debug
		// printf("indexBuffer header = %d bytes\n", indexBuffer);

		// memcpy copy isi data ke dataframe
		memcpy(sendBuffer + indexBuffer, &baseData, sizeof(baseData));		
		indexBuffer += sizeof(baseData);

		// kirim data dengan socket
		if (indexBuffer > BUFFER_SIZE) 
		{
			printf("ERROR = buffer size is out of size\n");
			closeSocket(socket);
			return -1;
		} 

		int datas;
		if ((datas=sendData(socket, sendBuffer, indexBuffer)) != indexBuffer) 
		{
			PERRNO("sendData");
			closeSocket(socket);
			return -1;
		} else  
			printf("sent %d bytes datas\n", datas);

#ifdef DEBUG_HENDRO
		//Debugging
		printf("baseData.state[ERSOW_HENDRO]:%d\n",baseData.state[ERSOW_HENDRO] );
		printf("baseData.target_hendro[POSX]:%d\n",baseData.target_hendro[POSX] );
		printf("baseData.target_hendro[POSY]:%d\n",baseData.target_hendro[POSY] );
		printf("baseData.mode[ERSOW_HENDRO] :%d\n",baseData.mode[ERSOW_HENDRO] );

#endif


		// publish data semua agents
		PubAgent1.publish(pubAgent1Data);
		PubAgent2.publish(pubAgent2Data);
		PubAgent3.publish(pubAgent3Data);

		gettimeofday(&tempTimeStamp, NULL);
		lastSendTimeStamp.tv_sec = tempTimeStamp.tv_sec;
		lastSendTimeStamp.tv_usec = tempTimeStamp.tv_usec;

		// reset variable received dan delta
		for (i=0; i<MAX_AGENTS; i++)
		{
			agent[i].received = NO;
			agent[i].delta = 0;
		}

		// sebelum looping berakhir
		timer_sig = 0;
		timer_signal = 0;
		indexBuffer = 0;
		
		ros::spinOnce();
	}


#ifdef AMBIL_DATA
	printf("\n------------------------- Communication: FINISHED ---------------------- \n");
	for(i=0; i<MAX_AGENTS; i++)
	{
		if (i == myNumber)
			continue;

		averageDelay[i] = ((totalDelay[i]/cntDelay[i])/1E3);

		printf("agent[%d] -> TUP: %0.2f ms, data received %d packets, %d lost, avrg.delay %0.2f ms\n", 
			i, 
			(TTUP_US/1E3),
			agent[i].lastFrameCounter,
			totalLostPackets[i],
			averageDelay[i]);
	}

#endif


	// closing socket 
	closeSocket(socket);

	printf("communication: FINISHED.\n");
	return 0;
}
