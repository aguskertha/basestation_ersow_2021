#include <stdio.h>
#include <ros/ros.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <cstdlib>
#include <ctime>

// Include message library  
#include "ersow_comm/dataAgentMsg.h"
#include "ersow_comm/strategyMsg.h"
#include "ersow_comm/ifaceMsg.h"
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

#define POSX 	0
#define POSY 	1
#define POST 	2

#define MAX_AGENTS 		5
#define ERSOW_JAMIL		3	
#define ERSOW_HENDRO	2
#define ERSOW_OKTO 		1
#define ERSOW_BASE 		0

#define STRIKER			0
#define DEFENDER		1
#define KEEPER 			2

#define NOT_RUNNING			0
#define RUNNING				1
#define INSERT				2
#define REMOVE				3

#define READY 			99
#define SLEEP 			0	

#define CONNECTED 		1
#define DISCONNECTED 	0

#define DETECTED	1
#define UNDETECTED	0

#define ENABLE 	1
#define DISABLE 0


// REFEREE BOX MANUAL COMMAND
#define STARTMATCH		115
#define STOPMATCH 		83
#define DROPBALL		78
#define ENDPART 		104
#define PARK 			76
#define RESET 			122

#define CYAN_GK 		71
#define CYAN_KO			75
#define CYAN_THROW		84
#define CYAN_GOAL		65
#define CYAN_CORNER		67
#define CYAN_FK			70
#define CYAN_PEN		80
#define CYAN_REPAIR		79

#define MAGENTA_GOAL	97
#define MAGENTA_KO 		107
#define MAGENTA_GK 		103
#define MAGENTA_THROW	116
#define MAGENTA_CORNER	99
#define MAGENTA_FK		102
#define MAGENTA_PEN		112
#define MAGENTA_REPAIR	111

#define TEAM_ERSOW      100
#define TEAM_MUSUH		200
#define	TEAM_CYAN		500
#define TEAM_MAGENTA	600

// ========================
#define OKTO_RIGHT		911
#define OKTO_LEFT		912
#define OKTO_FORWARD	913
#define OKTO_BACK 		914
#define OKTO_RLEFT		915
#define OKTO_RRIGHT		916

#define OKTO_CALIBFULL	512
#define OKTO_CALIBX 	513
#define OKTO_CALIBY 	514
#define OKTO_RESET 		511
#define OKTO_DRIBBLE   	516
#define OKTO_KICK   	515
#define OKTO_STOP		801

#define HENDRO_CALIBFULL 522
#define HENDRO_CALIBX 	 523
#define HENDRO_CALIBY 	 524
#define HENDRO_RESET 	 521
#define HENDRO_DRIBBLE   526
#define HENDRO_KICK   	 525
#define HENDRO_STOP		 601

#define JAMIL_CALIBFULL	532
#define JAMIL_CALIBX 	533
#define JAMIL_CALIBY 	534
#define JAMIL_RESET 	531
#define JAMIL_DRIBBLE   536
#define JAMIL_KICK   	535
#define JAMIL_STOP		701
// ------------------------

#define HENDRO_RIGHT	921
#define HENDRO_LEFT		922
#define HENDRO_FORWARD	923
#define HENDRO_BACK		924
#define HENDRO_RLEFT	925
#define HENDRO_RRIGHT	926
// ------------------------

#define JAMIL_RIGHT 	931
#define JAMIL_LEFT 		932
#define JAMIL_FORWARD	933
#define JAMIL_BACK 		934
#define JAMIL_RLEFT		935
#define JAMIL_RRIGHT	936
// ------------------------




// ========================
#define POSITIONING 	11
#define PUTARBOLA		20
#define CORNER          21
#define DRPBALL 		18
#define RECEIVEPASS 	13
#define	MAKEPASS		14

//========================
#define	MODEDIRECT		656
#define MODEPASS		657

//========================
#define FSM_KEEPER 				158;
#define FSM_MIDFIELDER			156;
#define FSM_STRIKER_FULLSKILL	155;
#define SET_SHOOTING			397;
#define FSM_STRIKER				157;  
#define SLOWKICK 				22;





// FOR DEBUGGING ONLY
// #define DEBUG_RECEIVER
// #define DEBUG_BALL
// #define DEBUG_DATA_AGENT2
// #define DEBUG_STATE_AGENT
// #define DEBUG_GUI
// #define DEBUG_SETSTRATEGY
// #define DEBUG_DATA_AGENT2
// #define DEBUG_DATA_STATE_MODE
 #define DEBUG_ALL_DATA	

// Variable ROS message
extern ersow_comm::strategyMsg strategyErsow;
extern ersow_comm::data_gui data_gui;

// Global variable
extern int end;
extern int timer;
extern int timer_sig;

extern int global_ball[2];
extern int fsm_agent[3];
extern char ball_detect;

// flag re-kickoff match
//int flag_kickoff_match = 0;
extern int universal_data_reset;
//int robotkonek;
extern int flagkalibdone_agent1;
extern int flagkalibdone_agent2; 
//static int datatrigger=0;

/* ----------- Step variables ---------- */
enum Steprobot { first, second, third, fourth, fifth, sixth, seventh, eighth, ninth, tenth };
extern Steprobot stepersow;
extern Steprobot stepmusuh;
extern Steprobot stepstart;

enum Posrobot { firstpos, secondpos, thirdpos, fourthpos, fifthpos, sixthpos };
extern Posrobot stepposersow;
extern Posrobot stepposmusuh;

// Global variable terima data
struct _agent
{
	short int position[3];	// 2*3 -> 6 bytes data posisi robot 
	short int value;	// 2 bytes data robot value robot (value opsional) 

	unsigned char state;	// state dari robot (RUNNING, REMOVE, etc)
	unsigned char ball_detect; // 1 byte data informasi deteksi bola
	unsigned short int ball_position[2];	// 2*2 -> 4 bytes data posisi koordinat bola
	unsigned short int dist2ball;  	// 2 bytes data distance from robot to ball

	unsigned int steppositioning;
};

extern _agent agent[MAX_AGENTS];

// simpan variable dari gui
struct _guiVariable
{
	int gui_key;
	int expandfield;
	int refbox;
	int team;
	int enableRef;
	int modeskill;
	int modestart;
	int interupt;
	int dataAll_x;
	int dataAll_y;
	int dataAll_0;
	int robot_mode;

};
extern _guiVariable GUI;


struct _refereeVar
{
	int state;
	char status;
	int lastState;
};
extern _refereeVar referee;


// function declaration
extern void SetStrategyErsow(void );
extern void callbackAgent1(const ersow_comm::dataAgentMsg& dataAgent1);
extern void callbackAgent2(const ersow_comm::dataAgentMsg& dataAgent2);
extern void callbackAgent3(const ersow_comm::dataAgentMsg& dataAgent3);
extern void callbackReferee(const ersow_comm::refereeMsg& dataReferee);
extern void callbackGUI(const ersow_comm::data_gui& msgHandler);
extern void FSMClosestBall(void );
extern void PosDropball(void);
extern void PosPark(void);

// function for set positioning
extern void ourKickOff(void );
extern void ourGoalKick(void );
extern void ourPenalty(void );
extern void ourFreeKick(void );
extern void ourCornerKick(void );

extern void theirKickOff(void );
extern void theirGoalKick(void );
extern void theirPenalty(void );
extern void theirFreeKick(void );
extern void theirCornerKick(void);

// Function for set start from positioning
extern void startGoalKick(int );
extern void startKickOff(int );
extern void startCorner(int );
extern void startFreeKick(int );
extern void startDropball(int );

//============================
extern void MODEFULLBASE();
extern void FULLBASE();
extern void DYNAMICBASE(int, int);
extern void FIGHTINGBASE(int);
extern void ZEROBASE();

//============================
extern void FSM(void);
/*extern void FSM_KICKOFF(int robot);
extern void FSM_SHOOTING(int robot);
extern void FSM_SLOWKICK(int robot, int targetx, int targety);*/

//============================
extern void KALIBXBERSAMA();
extern void KALIBYBERSAMA();
extern void KALIBXYBERSAMA();

extern void KALIBX(int robot);
extern void KALIBY(int robot);

////////////// Special Function ////////////////
extern void PointTo(char, int, int, int);
extern int validationStep(struct _agent& agent, int lastStep);
extern int getAngle(struct _agent& agent, int targetx, int targety);
extern void checkRunningAgents(int &agents);
extern void reset(void);
extern int Edistance(int startx, int starty, int endx, int endy);
extern void timer_catch(int sig );

//============================
extern void getGoalKick(int robot, int targetx, int targety, int targett);
extern void getCorner(int robot, int targetx, int targety, int targett);
extern void getDropball(int robot, int targetx, int targety, int targett);
extern void getPassing(int robot, int targetx, int targety);
extern void recivePassing(int robot, int targetx, int targety);

