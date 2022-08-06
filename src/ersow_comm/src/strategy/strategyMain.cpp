// include all from strategy
#include "strategyHeader.h" 

// message object
ersow_comm::strategyMsg strategyErsow;

// extracting extern Global variable
int end;
int timer;
int timer_sig;
int universal_data_reset;

int global_ball[2];
int fsm_agent[3];
char ball_detect;
int flagkalibdone_agent1;
int flagkalibdone_agent2;

// struct agents variable
// _agent agent1;
// _agent agent2;
// _agent agent3;

_agent agent[MAX_AGENTS];

// struct gui variable
_guiVariable GUI;

// struct referee variable
_refereeVar referee;

//	*************************
//  Random value
//	
int random_value()
{
	int upper = 200; 
	int lower = 10;

	return (int)((rand() % (upper - lower + 1)) + lower);
}

void timer_catch(int sig)
{
	if (sig == SIGALRM) {
		timer_sig ++;
		// printf("sig=SIGALRM\n");
		// printf("timer_sig:%d\n",timer_sig);
	}
}


int main(int argc, char *argv[])
{
    // Init ROS Node
    ros::init(argc, argv, "strategy_node");
    ros::NodeHandle strategy_node;

	// publish data 
	ros::Publisher PubDataStrategy = strategy_node.advertise<ersow_comm::strategyMsg>("data/Strategy", 100);

	// subscribe data dari GUI
	ros::Subscriber SubDataGUI = strategy_node.subscribe("data/gui", 100, callbackGUI);

	// subscribe data robot dari intelligent_comm
	ros::Subscriber SubAgent1 = strategy_node.subscribe("data/agent1", 100, callbackAgent1);
	ros::Subscriber SubAgent2 = strategy_node.subscribe("data/agent2", 100, callbackAgent2);
	ros::Subscriber SubAgent3 = strategy_node.subscribe("data/agent3", 100, callbackAgent3);

	// subscribe data referee
	ros::Subscriber SubReferee = strategy_node.subscribe("data/referee", 100, callbackReferee);

    // init global var
    end = 0;
    timer = 0;
    timer_sig=0;
    universal_data_reset=0;
    flagkalibdone_agent1 = 0;
	flagkalibdone_agent2 = 0;

    // loop tiap 20 ms
    ros::Rate loop_rate(50);

    if ((signal(SIGALRM, timer_catch)) == SIG_ERR)
	{
	  PERRNO("signal");
	  return -1;
	}

    struct itimerval timer;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0 ;
    setitimer(ITIMER_REAL, &timer, NULL);

    while(ros::ok()) 
    {
    	
    	// TODO
    	// Put data to be disseminated to multicast group
    	
    	// proses decision
		// decision_fsm();	
		FSMClosestBall();

		//
		//
		SetStrategyErsow();
		
		// 
		//
#ifdef DEBUG_ALL_DATA
		printf("Laststate: %d || REFBOX: %d || GUI_KEY: %d\n", referee.lastState, GUI.refbox, GUI.gui_key );
    	std::cout <<"stepstart   : "<<stepstart << "     stepmusuh: "<< stepmusuh<<"\n";
    	std::cout <<"stepposersow: "<<stepposersow << "     timer_sig: "<< timer_sig<< "\n";
		printf("STATE    :  %d ||  %d || %d\n", strategyErsow.state_agent1, strategyErsow.state_agent2, strategyErsow.state_agent3);
		printf("TARGET   : %d|%d <> %d|%d <> %d|%d\n", strategyErsow.targetx_agent1, strategyErsow.targety_agent1, strategyErsow.targetx_agent2, strategyErsow.targety_agent2, strategyErsow.targetx_agent3, strategyErsow.targety_agent3);
		printf("MODE     : %d <> %d <> %d\n",strategyErsow.mode_agent1, strategyErsow.mode_agent2, strategyErsow.mode_agent3);
		printf("VALUE    : %d <> %d <> %d\n\n",agent[ERSOW_OKTO].value, agent[ERSOW_HENDRO].value, agent[ERSOW_JAMIL].value );
#endif


#ifdef DEBUG_BALL
		printf("global ball posx = %d\tposy = %d\n", 
			strategyErsow.global_ballposx, 
			strategyErsow.global_ballposy);
#endif	
		// 
    	// Publish data strategy
    	PubDataStrategy.publish(strategyErsow);
		
    	//
    	//

    	ros::spinOnce();
    	loop_rate.sleep();
    }




	return 0;
}