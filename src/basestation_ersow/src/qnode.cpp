/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include <iostream>
#include <math.h>
#include "../include/basestation_ersow/qnode.hpp"
#include "basestation_ersow/data_agent0.h"
#include "basestation_ersow/dataAgentMsg.h"
#include "basestation_ersow/strategyMsg.h"
#include "basestation_ersow/data_gui.h"
#define STATE_SIMULASI
#define PI          3.14159265

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace basestation_ersow {

/*****************************************************************************
** Implementation
*****************************************************************************/
QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init() {
	ros::init(init_argc,init_argv,"basestation_ersow");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;

	subscribeFromNodeOkto = n.subscribe("data/agent1", 100, &QNode::getCbOkto, this);
	subscribeFromNodeHendro = n.subscribe("data/agent2", 100, &QNode::getCbHendro, this);
	subscribeFromNodeJamil = n.subscribe("data/agent3", 100, &QNode::getCbKeeper, this);
	subscribeFromStrategy = n.subscribe("data/Strategy", 100, &QNode::getCallbackStrategy, this);

	publish_data_gui = n.advertise<basestation_ersow::data_gui>("data/gui", 100);
	
	//subscribeFromNodeOkto = ainode.subscribe("data_okto", 200, &QNode::getCbOkto, this);

	start();
	return true;
}

void QNode::getCbKeeper(const basestation_ersow::dataAgentMsg& msg_handler)
{
	dataRobotJamil.agent0_posx_jamil = msg_handler.positionx;
	dataRobotJamil.agent0_posy_jamil = msg_handler.positiony;
	dataRobotJamil.agent0_pos0_jamil = msg_handler.positiont;
	dataRobotJamil.agent0_ballx_jamil = msg_handler.ballposx;
	dataRobotJamil.agent0_bally_jamil = msg_handler.ballposy;
	dataRobotJamil.agent0_lostPackets_jamil = msg_handler.lostPackets;
	dataRobotJamil.agent0_state_jamil = msg_handler.state;
}

void QNode::getCbOkto(const basestation_ersow::dataAgentMsg& msg_handler)
{
	dataRobotOkto.agent0_posx_okto = msg_handler.positionx;
	dataRobotOkto.agent0_posy_okto = msg_handler.positiony;
	dataRobotOkto.agent0_pos0_okto = msg_handler.positiont;
	dataRobotOkto.agent0_ballx_okto = msg_handler.ballposx;
	dataRobotOkto.agent0_bally_okto = msg_handler.ballposy;
	dataRobotOkto.agent0_lostPackets_okto = msg_handler.lostPackets;
	dataRobotOkto.agent0_state_okto = msg_handler.state;
}

void QNode::getCbHendro(const basestation_ersow::dataAgentMsg& msg_handler)
{

	dataRobotHendro.agent0_posx_hendro = msg_handler.positionx;
	dataRobotHendro.agent0_posy_hendro = msg_handler.positiony;
	dataRobotHendro.agent0_pos0_hendro = msg_handler.positiont;
	dataRobotHendro.agent0_ballx_hendro = msg_handler.ballposx;
	dataRobotHendro.agent0_bally_hendro = msg_handler.ballposy;
	dataRobotHendro.agent0_lostPackets_hendro = msg_handler.lostPackets;
	dataRobotHendro.agent0_state_hendro = msg_handler.state;
}


void QNode::getCallbackStrategy(const basestation_ersow::strategyMsg& msg_handler)
{
	dataStrategy.state_agent1 = msg_handler.state_agent1;
	dataStrategy.state_agent2 = msg_handler.state_agent2;
	dataStrategy.state_agent3 = msg_handler.state_agent3;
	//printf("dataStrategy.state_agent2:%d\n",dataStrategy.state_agent2 );

	dataStrategy.mode_agent1 = msg_handler.mode_agent1;
	dataStrategy.mode_agent2 = msg_handler.mode_agent2;
	dataStrategy.mode_agent3 = msg_handler.mode_agent3;

	dataStrategy.global_ballposx = msg_handler.global_ballposx;
	dataStrategy.global_ballposy = msg_handler.global_ballposy;

	dataStrategy.ball_detect = msg_handler.ball_detect;

	//printf("dataStrategy.targetx_agent1 = msg_handler.targetx_agent1:%d\n",msg_handler.targetx_agent1);
	dataStrategy.targetx_agent1 = msg_handler.targetx_agent1;
	dataStrategy.targety_agent1 = msg_handler.targety_agent1;
	dataStrategy.targett_agent1 = msg_handler.targety_agent1;

	dataStrategy.targetx_agent2 = msg_handler.targetx_agent2;
	dataStrategy.targety_agent2 = msg_handler.targety_agent2;
	dataStrategy.targett_agent2 = msg_handler.targett_agent2;

	dataStrategy.targetx_agent3 = msg_handler.targetx_agent3;
	dataStrategy.targety_agent3 = msg_handler.targety_agent3;
	dataStrategy.targett_agent3 = msg_handler.targett_agent3;
}

void QNode::run() {

	//ros::Rate loop_rate(5000);
	int counter = 0;
	double fps;
    double sec;
    time(&startb);

    int r = 115;

	ros::NodeHandle n;
	
	subscribeFromNodeOkto = n.subscribe("data/agent1", 100, &QNode::getCbOkto, this);
	subscribeFromNodeHendro = n.subscribe("data/agent2", 100, &QNode::getCbHendro, this);
	subscribeFromNodeJamil = n.subscribe("data/agent3", 100, &QNode::getCbKeeper, this);
	subscribeFromStrategy = n.subscribe("data/Strategy", 100, &QNode::getCallbackStrategy, this);

	ros::Rate loop_rate(r);
    

	while (ros::ok()) {
		
		publish_data_gui.publish(data_gui);

        //std::cout << "ipaddr " << data_gui.ipaddr << "\n";
        //std::cout << "port " << data_gui.port << "\n";
        //printf("qnode.data_gui.gui_key: %d\n",data_gui.gui_key );
        //printf("GUI.KEY: %d\n",data_gui.gui_key );
        //printf("REFBOX : %d\n",data_gui.refbox );

        // printf("data_gui.ipaddr:%s\n",data_gui.ipaddr);
        // printf("data_gui.port:%d\n",data_gui.port);


		// ros::spinOnce();
        ros::spinOnce();
        loop_rate.sleep();

        
		time(&endb); //
        ++counter; //
        sec = difftime (endb, startb); //	
        fps = counter / sec; //
        //printf("FPS nodeGui: %.1f \n",fps); //
      
        if(data_gui.simulation_lock==99){
        #ifdef STATE_SIMULASI 
            stepposersow=firstpos;
            flag=0;
            lock=0;
           
            dataStrategy.targetx_agent2=dataRobotHendro.agent0_posx_hendro;
            dataStrategy.targety_agent2=dataRobotHendro.agent0_posy_hendro;
            dataStrategy.targetx_agent1=dataRobotOkto.agent0_posx_okto;
            dataStrategy.targety_agent1=dataRobotOkto.agent0_posy_okto;
            dataStrategy.targetx_agent3=dataRobotJamil.agent0_posx_jamil;
            dataStrategy.targety_agent3=dataRobotJamil.agent0_posy_jamil;
        #endif
        }
        if(data_gui.simulation_lock==1){
            dataStrategy.global_ballposx=300;
            dataStrategy.global_ballposy=450;
        }
        if(data_gui.simulation_lock==2 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=450;
            dataStrategy.global_ballposy=150;
        }
        if(data_gui.simulation_lock==2 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=150;
            dataStrategy.global_ballposy=150;
        }
        if(data_gui.simulation_lock==4 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=450;
            dataStrategy.global_ballposy=150;
        }
        if(data_gui.simulation_lock==4 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=150;
            dataStrategy.global_ballposy=150;
        }
        if(data_gui.simulation_lock==3 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=600;
            dataStrategy.global_ballposy=900;
        }
        if(data_gui.simulation_lock==3 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=0;
            dataStrategy.global_ballposy=900;
        }
        if(data_gui.simulation_lock==11){
            dataStrategy.global_ballposx=300;
            dataStrategy.global_ballposy=450;
        }
        if(data_gui.simulation_lock==22 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=450;
            dataStrategy.global_ballposy=750;
        }
        if(data_gui.simulation_lock==22 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=150;
            dataStrategy.global_ballposy=750;
        }
        if(data_gui.simulation_lock==44 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=450;
            dataStrategy.global_ballposy=750;
        }
        if(data_gui.simulation_lock==44 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=150;
            dataStrategy.global_ballposy=750;
        }
        if(data_gui.simulation_lock==33 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=600;
            dataStrategy.global_ballposy=0;
        }
        if(data_gui.simulation_lock==33 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=0;
            dataStrategy.global_ballposy=0;
        }
        if(data_gui.simulation_lock==63 && data_gui.ball_set == 10){
            dataStrategy.global_ballposx=450;
            dataStrategy.global_ballposy=450;
        }
        if(data_gui.simulation_lock==63 && data_gui.ball_set == 20){
            dataStrategy.global_ballposx=150;
            dataStrategy.global_ballposy=450;
        }
        if(data_gui.simulation_lock==64){
            dataStrategy.global_ballposx=300;
            dataStrategy.global_ballposy=450;
        }

		//printf("simulation lock=%d\n",data_gui.simulation_lock );

        if(data_gui.simulation_lock==1){
            SIMULASI_OUR_KICKOFF();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==2){
            SIMULASI_OUR_GOALKICK();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==3){
            SIMULASI_OUR_CORNER();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==4){
            SIMULASI_OUR_FREEKICK();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==11){
            SIMULASI_THEIR_KICKOFF();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==22){
            SIMULASI_THEIR_GOALKICK();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==33){
            SIMULASI_THEIR_CORNER();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==44){
            SIMULASI_THEIR_FREEKICK();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==63){
            SIMULASI_DROPBALL();
            SIMULASI_DEFAULT();
        }
        if(data_gui.simulation_lock==64){
            SIMULASI_PARK();
            SIMULASI_DEFAULT();
        }
	}
	
	std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

void QNode::SIMULASI_DEFAULT()
{
    switch(stepposersow)
    {
    	case firstpos:
    	{
    		if(flag == 0)
    		{
                dataRobotOkto.agent0_pos0_okto=0;
                dataRobotHendro.agent0_pos0_hendro=0;
                dataRobotJamil.agent0_pos0_jamil=0;
        		if(dataRobotHendro.agent0_posx_hendro >= 0 && dataRobotHendro.agent0_posx_hendro <= postx_hendro && count==1){////////////
        			dataRobotHendro.agent0_posx_hendro++;
        			dataStrategy.targetx_agent2=postx_hendro;	
        		}
        		if(dataRobotHendro.agent0_posx_hendro >= 0 && dataRobotHendro.agent0_posx_hendro >= postx_hendro && count==1 && cout ==1){////////////
        			dataRobotHendro.agent0_posx_hendro--;
        			dataStrategy.targetx_agent2=postx_hendro;
        		}
        		if(dataRobotHendro.agent0_posx_hendro >= 0 && dataRobotHendro.agent0_posx_hendro >= postx_hendro && count==0){///////
        			dataRobotHendro.agent0_posx_hendro--;
        			dataStrategy.targetx_agent2=postx_hendro;	
        		}
                if(dataRobotHendro.agent0_posx_hendro >= 0 && dataRobotHendro.agent0_posx_hendro <= postx_hendro && count==0){///
                    dataRobotHendro.agent0_posx_hendro++;
                    dataStrategy.targetx_agent2=postx_hendro;   
                }
        		if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro <= posty_hendro && count==1){
        			dataRobotHendro.agent0_posy_hendro++;
        			dataStrategy.targety_agent2=posty_hendro;
        		}
                if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro >= posty_hendro && count==1){
                    dataRobotHendro.agent0_posy_hendro--;
                    dataStrategy.targety_agent2=posty_hendro;
                }
        		if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro >= posty_hendro && count==1 && cout ==1){////////////
        			dataRobotHendro.agent0_posy_hendro--;
        			dataStrategy.targety_agent2=posty_hendro;
        		}
        		if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro >= posty_hendro && count==0){
        			dataRobotHendro.agent0_posy_hendro--;
        			dataStrategy.targety_agent2=posty_hendro;
        		}
                if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro <= posty_hendro && count==0){
                    dataRobotHendro.agent0_posy_hendro++;
                    dataStrategy.targety_agent2=posty_hendro;
                }
        		if(dataRobotOkto.agent0_posx_okto >= 0 && dataRobotOkto.agent0_posx_okto <= postx_okto && count ==1){//////////////
        			dataRobotOkto.agent0_posx_okto++;
        			dataStrategy.targetx_agent1=postx_okto;	
        		}
        		if(dataRobotOkto.agent0_posx_okto >= 0 && dataRobotOkto.agent0_posx_okto >= postx_okto && count ==1 && cout ==1){//////////////
        			dataRobotOkto.agent0_posx_okto--;	
        			dataStrategy.targetx_agent1=postx_okto;
        		}
        		if(dataRobotOkto.agent0_posx_okto >= 0 && dataRobotOkto.agent0_posx_okto >= postx_okto && count ==0){//////////////
        			dataRobotOkto.agent0_posx_okto--;	
        			dataStrategy.targetx_agent1=postx_okto;
        		}
                if(dataRobotOkto.agent0_posx_okto >= 0 && dataRobotOkto.agent0_posx_okto <= postx_okto && count ==0){//////////////BERUBAH
                    dataRobotOkto.agent0_posx_okto++;   
                    dataStrategy.targetx_agent1=postx_okto;
                }
        		if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto <= posty_okto && count == 1){
        			dataRobotOkto.agent0_posy_okto++;
        			dataStrategy.targety_agent1=posty_okto;
        		}
                if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto >= posty_okto && count == 1){///berubah
                    dataRobotOkto.agent0_posy_okto--;
                    dataStrategy.targety_agent1=posty_okto;
                }
        		if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto >= posty_okto && count ==1 && cout ==1){//////////////
        			dataRobotOkto.agent0_posy_okto--;	
        			dataStrategy.targety_agent1=posty_okto;
        		}
        		if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto <= posty_okto && count == 0){
        			dataRobotOkto.agent0_posy_okto++;
        			dataStrategy.targety_agent1=posty_okto;
        		}
                if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto >= posty_okto && count == 0){////BERUBAH
                    dataRobotOkto.agent0_posy_okto--;
                    dataStrategy.targety_agent1=posty_okto;
                }
        		if(dataRobotJamil.agent0_posx_jamil >= 0 && dataRobotJamil.agent0_posx_jamil <= postx_jamil && count ==1){//////////////
        			dataRobotJamil.agent0_posx_jamil++;
        			dataStrategy.targetx_agent3=postx_jamil;	
        		}
        		if(dataRobotJamil.agent0_posx_jamil >= 0 && dataRobotJamil.agent0_posx_jamil >= postx_jamil && count ==1 && cout ==1){//////////////
        			dataRobotJamil.agent0_posx_jamil--;	
        			dataStrategy.targetx_agent3=postx_jamil;
        		}
        		if(dataRobotJamil.agent0_posx_jamil >= 0 && dataRobotJamil.agent0_posx_jamil >= postx_jamil && count ==0){//////////////
        			dataRobotJamil.agent0_posx_jamil--;	
        			dataStrategy.targetx_agent3=postx_jamil;
        		}
                if(dataRobotJamil.agent0_posx_jamil >= 0 && dataRobotJamil.agent0_posx_jamil <= postx_jamil && count ==0){//////////////BERUBAH
                    dataRobotJamil.agent0_posx_jamil++; 
                    dataStrategy.targetx_agent3=postx_jamil;
                }
        		if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil >= posty_jamil && count == 1){
        			dataRobotJamil.agent0_posy_jamil--;
        			dataStrategy.targety_agent3=posty_jamil;
        		}
        		if(dataRobotJamil.agent0_posx_jamil >= 0 && dataRobotJamil.agent0_posx_jamil <= posty_jamil && count ==1 && cout ==1){//////////////
        			dataRobotJamil.agent0_posy_jamil++;	
        			dataStrategy.targety_agent3=posty_jamil;
        		}
        		if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil <= posty_jamil && count == 0){
        			dataRobotJamil.agent0_posy_jamil++;
        			dataStrategy.targety_agent3=posty_jamil;
        		}
                if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil >= posty_jamil && count == 0){
                    dataRobotJamil.agent0_posy_jamil--;
                    dataStrategy.targety_agent3=posty_jamil;
                }
        		lock++;
        			if (lock>400){
        				flag=1;
        			}
    		}else{stepposersow=secondpos;}
    		
    		break;
    	}
    	case secondpos:
    	{	
    		if(dataRobotHendro.agent0_posx_hendro >= 0  && dataRobotHendro.agent0_posx_hendro <= postx_hendro && count == 0){
    			dataRobotHendro.agent0_posx_hendro++; 
    			dataStrategy.targetx_agent2=postx_hendro;
    			if(dataRobotHendro.agent0_posx_hendro == postx_hendro){
    				dataRobotHendro.agent0_posx_hendro=postx_hendro;
    			}
    		}
            if(dataRobotHendro.agent0_posx_hendro >= 0  && dataRobotHendro.agent0_posx_hendro >= postx_hendro && count == 0){
                dataRobotHendro.agent0_posx_hendro--; 
                dataStrategy.targetx_agent2=postx_hendro;
                if(dataRobotHendro.agent0_posx_hendro == postx_hendro){
                    dataRobotHendro.agent0_posx_hendro=postx_hendro;
                }
            }
    		if(dataRobotHendro.agent0_posx_hendro >= 0  && dataRobotHendro.agent0_posx_hendro >= postx_hendro && count == 1){
    			dataRobotHendro.agent0_posx_hendro--; 
    			dataStrategy.targetx_agent2=postx_hendro;
    			if(dataRobotHendro.agent0_posx_hendro == postx_hendro){
    				dataRobotHendro.agent0_posx_hendro=postx_hendro;
    			}
    		}
            if(dataRobotHendro.agent0_posx_hendro >= 0  && dataRobotHendro.agent0_posx_hendro <= postx_hendro && count == 1){
                dataRobotHendro.agent0_posx_hendro++; 
                dataStrategy.targetx_agent2=postx_hendro;
                if(dataRobotHendro.agent0_posx_hendro == postx_hendro){
                    dataRobotHendro.agent0_posx_hendro=postx_hendro;
                }
            }
    		if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro >= posty_hendro && count == 0){//////////////////
    			dataRobotHendro.agent0_posy_hendro--;
    			dataStrategy.targety_agent2=posty_hendro;
    			if(dataRobotHendro.agent0_posy_hendro == posty_hendro){
    				dataRobotHendro.agent0_posy_hendro=posty_hendro;
    			}
    		}
            if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro <= posty_hendro && count == 0){//////////////////
                dataRobotHendro.agent0_posy_hendro++;
                dataStrategy.targety_agent2=posty_hendro;
                if(dataRobotHendro.agent0_posy_hendro == posty_hendro){
                    dataRobotHendro.agent0_posy_hendro=posty_hendro;
                }
            }
    		if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro <= posty_hendro && count ==1){//////////////////
    			dataRobotHendro.agent0_posy_hendro++;
    			dataStrategy.targety_agent2=posty_hendro;
    			if(dataRobotHendro.agent0_posy_hendro == posty_hendro){
    				dataRobotHendro.agent0_posy_hendro=posty_hendro;
    			}
    		}
    		if(dataRobotHendro.agent0_posy_hendro >= 0 && dataRobotHendro.agent0_posy_hendro >= posty_hendro && count ==1){//////////////////
    			dataRobotHendro.agent0_posy_hendro--;
    			dataStrategy.targety_agent2=posty_hendro;
    			if(dataRobotHendro.agent0_posy_hendro == posty_hendro){
    				dataRobotHendro.agent0_posy_hendro=posty_hendro;
    			}
    		}
    		if(dataRobotOkto.agent0_posx_okto >= 0  && dataRobotOkto.agent0_posx_okto <= postx_okto && count == 0){
    			dataRobotOkto.agent0_posx_okto++;
    			dataStrategy.targetx_agent1=postx_okto;
    			if(dataRobotOkto.agent0_posx_okto == postx_okto){
    				dataRobotOkto.agent0_posx_okto=postx_okto;
    			}
    		}
            if(dataRobotOkto.agent0_posx_okto >= 0  && dataRobotOkto.agent0_posx_okto >= postx_okto && count == 0){
                dataRobotOkto.agent0_posx_okto--;
                dataStrategy.targetx_agent1=postx_okto;
                if(dataRobotOkto.agent0_posx_okto == postx_okto){
                    dataRobotOkto.agent0_posx_okto=postx_okto;
                }
            }
    		if(dataRobotOkto.agent0_posx_okto >= 0  && dataRobotOkto.agent0_posx_okto >= postx_okto && count == 1){
    			dataRobotOkto.agent0_posx_okto--; 
    			dataStrategy.targetx_agent1=postx_okto;
    			if(dataRobotOkto.agent0_posx_okto == postx_okto){
    				dataRobotOkto.agent0_posx_okto=postx_okto;
    			}
    		}
    		if(dataRobotOkto.agent0_posx_okto >= 0  && dataRobotOkto.agent0_posx_okto <= postx_okto && count == 1){
    			dataRobotOkto.agent0_posx_okto++;
    			dataStrategy.targetx_agent1=postx_okto;
    			if(dataRobotOkto.agent0_posx_okto == postx_okto){
    				dataRobotOkto.agent0_posx_okto=postx_okto;
    			}
    		}
    		if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto >= posty_okto && count == 0){/////////////////
    			dataRobotOkto.agent0_posy_okto--;
    			dataStrategy.targety_agent1=posty_okto;
    			if(dataRobotOkto.agent0_posy_okto == posty_okto){
    				dataRobotOkto.agent0_posy_okto=posty_okto;
    			}
    		}
            if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto <= posty_okto && count == 0){/////////////////
                dataRobotOkto.agent0_posy_okto++;
                dataStrategy.targety_agent1=posty_okto;
                if(dataRobotOkto.agent0_posy_okto == posty_okto){
                    dataRobotOkto.agent0_posy_okto=posty_okto;
                }
            }
    		if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto <= posty_okto && count ==1){/////////////////
    			dataRobotOkto.agent0_posy_okto++;
    			dataStrategy.targety_agent1=posty_okto;
    			if(dataRobotOkto.agent0_posy_okto == posty_okto){
    				dataRobotOkto.agent0_posy_okto=posty_okto;
    			}
    		}
    		if(dataRobotOkto.agent0_posy_okto >= 0 && dataRobotOkto.agent0_posy_okto >= posty_okto && count ==1){/////////////////
    			dataRobotOkto.agent0_posy_okto--;
    			dataStrategy.targety_agent1=posty_okto;
    			if(dataRobotOkto.agent0_posy_okto == posty_okto){
    				dataRobotOkto.agent0_posy_okto=posty_okto;
    			}
    		}
    		if(dataRobotJamil.agent0_posx_jamil >= 0  && dataRobotJamil.agent0_posx_jamil <= postx_jamil && count ==0){  //     		
    			dataRobotJamil.agent0_posx_jamil++;
    			dataStrategy.targetx_agent3=postx_jamil;
    			if(dataRobotJamil.agent0_posx_jamil == postx_jamil){
    				dataRobotJamil.agent0_posx_jamil=postx_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posx_jamil >= 0  && dataRobotJamil.agent0_posx_jamil >= postx_jamil && count ==0){  //     		
    			dataRobotJamil.agent0_posx_jamil--;
    			dataStrategy.targetx_agent3=postx_jamil;
    			if(dataRobotJamil.agent0_posx_jamil == postx_jamil){
    				dataRobotJamil.agent0_posx_jamil=postx_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posx_jamil >= 0  && dataRobotJamil.agent0_posx_jamil >= postx_jamil && count ==1){       		
    			dataRobotJamil.agent0_posx_jamil--;
    			dataStrategy.targetx_agent3=postx_jamil;
    			if(dataRobotJamil.agent0_posx_jamil == postx_jamil){
    				dataRobotJamil.agent0_posx_jamil=postx_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posx_jamil >= 0  && dataRobotJamil.agent0_posx_jamil <= postx_jamil && count ==1){       		
    			dataRobotJamil.agent0_posx_jamil++;
    			dataStrategy.targetx_agent3=postx_jamil;
    			if(dataRobotJamil.agent0_posx_jamil == postx_jamil){
    				dataRobotJamil.agent0_posx_jamil=postx_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil >= posty_jamil && count ==0){//
    			dataRobotJamil.agent0_posy_jamil--;
    			dataStrategy.targety_agent3=posty_jamil;
    			if(dataRobotJamil.agent0_posy_jamil == posty_jamil){
    				dataRobotJamil.agent0_posy_jamil=posty_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil <= posty_jamil && count ==0){//
    			dataRobotJamil.agent0_posy_jamil++;
    			dataStrategy.targety_agent3=posty_jamil;
    			if(dataRobotJamil.agent0_posy_jamil == posty_jamil){
    				dataRobotJamil.agent0_posy_jamil=posty_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil <= posty_jamil && count ==1){
    			dataRobotJamil.agent0_posy_jamil++;
    			dataStrategy.targety_agent3=posty_jamil;
    			if(dataRobotJamil.agent0_posy_jamil == posty_jamil){
    				dataRobotJamil.agent0_posy_jamil=posty_jamil;
    			}
    		}
    		if(dataRobotJamil.agent0_posy_jamil >= 0 && dataRobotJamil.agent0_posy_jamil >= posty_jamil && count ==1){
    			dataRobotJamil.agent0_posy_jamil--;
    			dataStrategy.targety_agent3=posty_jamil;
    			if(dataRobotJamil.agent0_posy_jamil == posty_jamil){
    				dataRobotJamil.agent0_posy_jamil=posty_jamil;
    			}
    		}
    		if(dataStrategy.global_ballposx>300){
    			theta_okto;
                theta_hendro;
    		}
    		else if(dataStrategy.global_ballposx<300){
    			theta_okto;
                theta_hendro;
    		}
            else if(dataStrategy.global_ballposx=300){
                theta_okto;
                theta_hendro;
            }
    		break;
    	}
    } 
}

void QNode::SIMULASI_OUR_KICKOFF()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx == 300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=180;
            postx_hendro=450;
            posty_hendro=225;
            postx_jamil=300;
            posty_jamil=100;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx == 300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=225;
            postx_hendro=150;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=100;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx == 300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=200;
            posty_okto=200;
            postx_hendro=300 +80;
            posty_hendro=450 -70;
            postx_jamil=300;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx == 300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=300 +80;
            posty_okto=450 -70;
            postx_hendro=200;
            posty_hendro=200;
            postx_jamil=300;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        
    }
}

void QNode::SIMULASI_OUR_GOALKICK()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=380;
            postx_hendro=450;
            posty_hendro=225;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=225;
            postx_hendro=150;
            posty_hendro=380;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=380;
            postx_hendro=450;
            posty_hendro=225;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=225;
            postx_hendro=150;
            posty_hendro=380;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=200;
            posty_okto=280;
            postx_hendro=450 -75;
            posty_hendro=150 +75;
            postx_jamil=300;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450 -75;
            posty_okto=150 +75;
            postx_hendro=200;
            posty_hendro=280;
            postx_jamil=300;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150 +75;
            posty_okto=150 +75;
            postx_hendro=400;
            posty_hendro=280;
            postx_jamil=300;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=400;
            posty_okto=280;
            postx_hendro=150 +75;
            posty_hendro=150 +75;
            postx_jamil=300;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
    }
} 

void QNode::SIMULASI_OUR_FREEKICK()
{

	if(stepposersow==firstpos){
		if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=150;
			posty_okto=380;
			postx_hendro=450;
			posty_hendro=225;
			postx_jamil=300;
			posty_jamil=75;
			count=0;
			cout=0;
		}
		else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=450;
			posty_okto=225;
			postx_hendro=150;
			posty_hendro=380;
			postx_jamil=300;
			posty_jamil=75;
			count=1;
			cout=1;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=150;
			posty_okto=380;
			postx_hendro=450;
			posty_hendro=225;
			postx_jamil=300;
			posty_jamil=75;
			count=0;
			cout=0;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=450;
			posty_okto=225;
			postx_hendro=150;
			posty_hendro=380;
			postx_jamil=300;
			posty_jamil=75;
			count=1;
			cout=1;
		}
	}
	else if(stepposersow==secondpos){
		if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=200;
			posty_okto=280;
			postx_hendro=450 -75;
			posty_hendro=150 +75;
			postx_jamil=300;
			posty_jamil=1;
			count=0;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
		else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=450 -75;
			posty_okto=150 +75;
			postx_hendro=200;
			posty_hendro=280;
			postx_jamil=300;
			posty_jamil=1;
			count=1;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=150 +75;
			posty_okto=150 +75;
			postx_hendro=400;
			posty_hendro=280;
			postx_jamil=300;
			posty_jamil=1;
			count=0;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=400;
			posty_okto=280;
			postx_hendro=150 +75;
			posty_hendro=150 +75;
			postx_jamil=300;
			posty_jamil=1;
			count=1;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
	}
}

void QNode::SIMULASI_OUR_CORNER()
{

	if(stepposersow==firstpos){
		if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=200;
			posty_okto=700;
			postx_hendro=400;
			posty_hendro=700;
			postx_jamil=300;
			posty_jamil=75;
			count=0;
			cout=0;
		}
		else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=400;
			posty_okto=700;
			postx_hendro=200;
			posty_hendro=700;
			postx_jamil=300;
			posty_jamil=75;
			count=1;
			cout=1;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=200;
			posty_okto=700;
			postx_hendro=400;
			posty_hendro=700;
			postx_jamil=300;
			posty_jamil=75;
			count=0;
			cout=0;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=400;
			posty_okto=700;
			postx_hendro=200;
			posty_hendro=700;
			postx_jamil=300;
			posty_jamil=75;
			count=1;
			cout=1;
		}
	}
	else if(stepposersow==secondpos){
		if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=300;
			posty_okto=790;
			postx_hendro=600 -75;
			posty_hendro=900 -75;
			postx_jamil=350;
			posty_jamil=1;
			count=0;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
		else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=600 -75;
			posty_okto=900 -75;
			postx_hendro=300;
			posty_hendro=790;
			postx_jamil=350;
			posty_jamil=1;
			count=1;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
			postx_okto=0 +75;
			posty_okto=900 -75;
			postx_hendro=300;
			posty_hendro=790;
			postx_jamil=250;
			posty_jamil=1;
			count=0;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
		else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
			postx_okto=300;
			posty_okto=790;
			postx_hendro=0 +75;
			posty_hendro=900 -75;
			postx_jamil=250;
			posty_jamil=1;
			count=1;

			dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
		}
	}
}


void QNode::SIMULASI_THEIR_GOALKICK()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=350;
            postx_hendro=450;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=350;
            postx_hendro=150;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=350;
            postx_hendro=450;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=350;
            postx_hendro=150;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=300;
            posty_okto=275;
            postx_hendro=450 -0;
            posty_hendro=750 -325;
            postx_jamil=350;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){////////////
            postx_okto=450 -0;
            posty_okto=750 -325;
            postx_hendro=300;
            posty_hendro=275;
            postx_jamil=350;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150 +0;
            posty_okto=750 -325;
            postx_hendro=300;
            posty_hendro=275;
            postx_jamil=250;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=300;
            posty_okto=275;
            postx_hendro=150 +0;
            posty_hendro=750 -325;
            postx_jamil=250;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
    }   
}

void QNode::SIMULASI_THEIR_FREEKICK()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=350;
            postx_hendro=450;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=350;
            postx_hendro=150;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=350;
            postx_hendro=450;
            posty_hendro=350;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=275;
            postx_hendro=150;
            posty_hendro=275;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=300;
            posty_okto=275;
            postx_hendro=450 -0;
            posty_hendro=750 -325;
            postx_jamil=350;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){////////////
            postx_okto=450 -0;
            posty_okto=750 -325;
            postx_hendro=300;
            posty_hendro=275;
            postx_jamil=350;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150 +0;
            posty_okto=750 -325;
            postx_hendro=300;
            posty_hendro=275;
            postx_jamil=250;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=300;
            posty_okto=275;
            postx_hendro=150 +0;
            posty_hendro=750 -325;
            postx_jamil=250;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
    }
}

void QNode::SIMULASI_THEIR_CORNER()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=225;
            posty_okto=180;
            postx_hendro=375;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=375;
            posty_okto=180;
            postx_hendro=225;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=225;
            posty_okto=180;
            postx_hendro=375;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=375;
            posty_okto=180;
            postx_hendro=225;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=250;
            posty_okto=100;
            postx_hendro=600 - 225;
            posty_hendro=0 + 150;
            postx_jamil=350;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=600 - 225;
            posty_okto=0 + 150;
            postx_hendro=250;
            posty_hendro=100;
            postx_jamil=350;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=0 + 255;
            posty_okto=0 + 150;
            postx_hendro=350;
            posty_hendro=100;
            postx_jamil=250;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=350;
            posty_okto=100;
            postx_hendro=0+225;
            posty_hendro=0+150;
            postx_jamil=250;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
    }
}

void QNode::SIMULASI_THEIR_KICKOFF()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx == 300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=180;
            postx_hendro=450;
            posty_hendro=225;
            postx_jamil=300;
            posty_jamil=100;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx == 300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=225;
            postx_hendro=150;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=100;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx == 300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=240;
            posty_okto=225;
            postx_hendro=300 +60;
            posty_hendro=450 -225;
            postx_jamil=300;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx == 300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=300 +60;
            posty_okto=450 -225;
            postx_hendro=240;
            posty_hendro=225;
            postx_jamil=300;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        
    }    
}

void QNode::SIMULASI_DROPBALL()
{
    if(stepposersow==firstpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=280;
            postx_hendro=450;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=180;
            postx_hendro=150;
            posty_hendro=280;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150;
            posty_okto=280;
            postx_hendro=450;
            posty_hendro=180;
            postx_jamil=300;
            posty_jamil=75;
            count=0;
            cout=0;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450;
            posty_okto=180;
            postx_hendro=150;
            posty_hendro=280;
            postx_jamil=300;
            posty_jamil=75;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataStrategy.global_ballposx>300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=225;
            posty_okto=335;
            postx_hendro=450 - 50;
            posty_hendro=450 - 125;
            postx_jamil=300;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx>300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=450 - 50;
            posty_okto=450 - 125;
            postx_hendro=225;
            posty_hendro=335;
            postx_jamil=300;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotHendro.agent0_posx_hendro >= dataRobotOkto.agent0_posx_okto){
            postx_okto=150 + 50;
            posty_okto=450 - 125;
            postx_hendro=375;
            posty_hendro=335;
            postx_jamil=300;
            posty_jamil=1;
            count=0;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
        else if(dataStrategy.global_ballposx<300 && dataRobotOkto.agent0_posx_okto > dataRobotHendro.agent0_posx_hendro){
            postx_okto=375;
            posty_okto=335;
            postx_hendro=150+50;
            posty_hendro=450-125;
            postx_jamil=300;
            posty_jamil=1;
            count=1;

            dataRobotHendro.agent0_pos0_hendro =getAngle(dataRobotHendro.agent0_posx_hendro, dataRobotHendro.agent0_posy_hendro, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_hendro=dataRobotHendro.agent0_pos0_hendro;

            dataRobotOkto.agent0_pos0_okto =getAngle(dataRobotOkto.agent0_posx_okto, dataRobotOkto.agent0_posy_okto, dataStrategy.global_ballposx, dataStrategy.global_ballposy);
            theta_okto=dataRobotOkto.agent0_pos0_okto;
        }
    }
}

void QNode::SIMULASI_PARK()
{
    if(stepposersow==firstpos){
        if(dataRobotHendro.agent0_posy_hendro >= dataRobotOkto.agent0_posy_okto){
            postx_okto=300;
            posty_okto=180;
            postx_hendro=300;
            posty_hendro=380;
            postx_jamil=200;
            posty_jamil=50;
            count=0;
            cout=0;
        }
        else if(dataRobotOkto.agent0_posy_okto > dataRobotHendro.agent0_posy_hendro){
            postx_okto=300;
            posty_okto=380;
            postx_hendro=300;
            posty_hendro=180;
            postx_jamil=200;
            posty_jamil=50;
            count=1;
            cout=1;
        }
    }
    else if(stepposersow==secondpos){
        if(dataRobotHendro.agent0_posy_hendro >= dataRobotOkto.agent0_posy_okto){
            postx_okto=0;
            posty_okto=1;
            postx_hendro=600;
            posty_hendro=1;
            postx_jamil=87;
            posty_jamil=1;
            count=0;
            cout=0;
            theta_okto=0;
            theta_hendro=0;
        }
        else if(dataRobotOkto.agent0_posy_okto > dataRobotHendro.agent0_posy_hendro){
            postx_okto=0;
            posty_okto=1;
            postx_hendro=600;
            posty_hendro=1;
            postx_jamil=87;
            posty_jamil=1;
            count=1;
            cout=1;
            theta_okto=0;
            theta_hendro=0;
        }
    }
}

int QNode::getAngle(int roboty, int robotx, int targety, int targetx)
{
	// menghitung sudut antara dua titik
// menghitung sudut antara dua titik
    int dx = targetx - robotx;
    int dy = targety - roboty;
    
    return (atan2((double)dx, (double)dy) * (180/3.14))-90;
}


}  // namespace basestation_ersow
