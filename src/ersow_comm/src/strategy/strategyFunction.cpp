#include "strategyHeader.h" 

// enum pose robot 
// extern from strategyHeader.h
Posrobot stepposersow;
Posrobot stepposmusuh;

Steprobot stepersow;
Steprobot stepmusuh;
Steprobot stepstart;

void reset(void )
{	
	// reset variable enum 
	stepposersow = firstpos;
	stepposmusuh = firstpos;

	stepersow = first;
	stepmusuh = first;
	stepstart = first;

	strategyErsow.state_agent1 = 0;
	strategyErsow.state_agent2 = 0;
	strategyErsow.state_agent3 = 0;

	strategyErsow.mode_agent1 = 0;
	strategyErsow.mode_agent2 = 0;
	strategyErsow.mode_agent3 = 0;

	universal_data_reset = 0;

#ifdef DEBUG_DATA_STATE_MODE
	printf("strategyErsow.state_agent1:%d\n",strategyErsow.state_agent1 );
	printf("strategyErsow.state_agent2:%d\n",strategyErsow.state_agent2 );
	printf("strategyErsow.state_agent3:%d\n",strategyErsow.state_agent3 );

	printf("strategyErsow.mode_agent1:%d\n",strategyErsow.mode_agent1 );
	printf("strategyErsow.mode_agent2:%d\n",strategyErsow.mode_agent2 );
	printf("strategyErsow.mode_agent3:%d\n",strategyErsow.mode_agent3 );
#endif

}

void ready_pub()
{
	strategyErsow.state_agent1 = READY;
	strategyErsow.state_agent2 = READY;
	strategyErsow.state_agent3 = READY;

	strategyErsow.mode_agent1 = fsm_agent[1]; 
	strategyErsow.mode_agent2 = fsm_agent[2]; 
	strategyErsow.mode_agent3 = fsm_agent[3]; 

	strategyErsow.global_ballposx = global_ball[POSX];
	strategyErsow.global_ballposy = global_ball[POSY];
	strategyErsow.ball_detect = ball_detect;

}

void PointTo(char indexAgent, int targetx, int targety, int targett)
{
	int distance;
	int d_x;
	int d_y;

	switch(indexAgent) {
		case ERSOW_OKTO: 
			strategyErsow.state_agent1 	= READY; 
			strategyErsow.mode_agent1 	= POSITIONING;	
			
			strategyErsow.targetx_agent1 = targetx; 
			strategyErsow.targety_agent1 = targety; 
			strategyErsow.targett_agent1 = targett; 

			d_x = targetx - (agent[ERSOW_OKTO].position[POSX]);
			d_y = targety - (agent[ERSOW_OKTO].position[POSY]);
			distance = sqrt(pow(d_x, 2) + pow(d_y, 2));
			if(distance < 30)  
				agent[ERSOW_OKTO].steppositioning ++;
		break;

		case ERSOW_HENDRO: 
			strategyErsow.state_agent2 	= READY; 
			strategyErsow.mode_agent2 	= POSITIONING;	
			
			strategyErsow.targetx_agent2 = targetx; 
			strategyErsow.targety_agent2 = targety; 
			strategyErsow.targett_agent2 = targett; 

			d_x = targetx - (agent[ERSOW_HENDRO].position[POSX]);
			d_y = targety - (agent[ERSOW_HENDRO].position[POSY]);
			distance = sqrt(pow(d_x, 2) + pow(d_y, 2));
			if(distance < 30)  
				agent[ERSOW_HENDRO].steppositioning ++;
		break;

		case ERSOW_JAMIL: 
			strategyErsow.state_agent3 	= READY; 
			strategyErsow.mode_agent3 	= POSITIONING;	
			
			strategyErsow.targetx_agent3 = targetx; 
			strategyErsow.targety_agent3 = targety; 
			strategyErsow.targett_agent3 = targett; 

			d_x = targetx - (agent[ERSOW_JAMIL].position[POSX]);
			d_y = targety - (agent[ERSOW_JAMIL].position[POSY]);
			distance = sqrt(pow(d_x, 2) + pow(d_y, 2));
			if(distance < 30)  
				agent[ERSOW_JAMIL].steppositioning ++;
		break;
	}		
}


int validationStep(struct _agent& agent, int lastStep)
{
	// TODO 
	// nothing
	if ((agent.state == RUNNING || agent.state == REMOVE))
	{
		if ((agent.steppositioning != lastStep + 1))
			return (-1);
		else 
			return (0);		
	} else 
		return (1);

	return (0);
}

int getAngle(struct _agent& agent, int targetx, int targety)
{
	// menghitung sudut antara dua titik
    int dx = targetx - agent.position[POSX];
    int dy = targety - agent.position[POSY];
    
    return ((atan2((double)dx, (double)dy) * (180/3.14)) *(-1) );	
}


void checkRunningAgents(int &agents)
{
	// hitung jumlah agents yang running
	// menggunakan pass by reference
	for (int i=0; i<MAX_AGENTS; i++)
		if ((agent[i].state == RUNNING) || (agent[i].state == REMOVE)) 
			agents ++;
}


int Edistance(int startx, int starty, int endx, int endy)
{
	// hitung distance dua titik
	return (int)(sqrt(pow((startx - endx), 2) + pow((starty - endy), 2)));
}

//============KALIB=============//
void KALIBXBERSAMA()
{
	printf("Masuk Kalibrasi X bersama\n");
	KALIBX(ERSOW_OKTO);
	KALIBX(ERSOW_HENDRO);
	KALIBX(ERSOW_JAMIL);
}

void KALIBYBERSAMA()
{
	printf("Masuk Kalibrasi Y bersama\n");
	KALIBY(ERSOW_OKTO);
	KALIBY(ERSOW_HENDRO);
	KALIBY(ERSOW_JAMIL);
}

void KALIBXYBERSAMA()
{
	strategyErsow.state_agent1 = 501;
	strategyErsow.state_agent1 = 511;
	KALIBY(ERSOW_JAMIL);
}

void KALIBX(int robot)
{
	int selisihpos;
	switch(robot)
	{	
		case ERSOW_OKTO:
		{
			if(agent[ERSOW_OKTO].position[POSX] > agent[ERSOW_HENDRO].position[POSX]){
				strategyErsow.state_agent1 = 502;
				selisihpos = Edistance(agent[ERSOW_OKTO].position[POSX], agent[ERSOW_OKTO].position[POSY], 590, 225);
				if(selisihpos < 10){
					flagkalibdone_agent1 = 1;
				}
			}
			else if(agent[ERSOW_HENDRO].position[POSX] >= agent[ERSOW_OKTO].position[POSX]){
				strategyErsow.state_agent1 = 502;
				selisihpos = Edistance(agent[ERSOW_OKTO].position[POSX], agent[ERSOW_OKTO].position[POSY], 0, 225);
				if(selisihpos < 10){
					flagkalibdone_agent1 = 1;
				}
			}
			break;
		}

		case ERSOW_HENDRO:
		{
			if(agent[ERSOW_OKTO].position[POSX] > agent[ERSOW_HENDRO].position[POSX]){
				strategyErsow.state_agent2 = 512;
				selisihpos = Edistance(agent[ERSOW_HENDRO].position[POSX], agent[ERSOW_HENDRO].position[POSY], 0, 225);
				if(selisihpos < 10){
					flagkalibdone_agent2 = 1;
				}
			}
			else if(agent[ERSOW_HENDRO].position[POSX] >= agent[ERSOW_OKTO].position[POSX]){
				strategyErsow.state_agent1 = 512;
				selisihpos = Edistance(agent[ERSOW_HENDRO].position[POSX], agent[ERSOW_HENDRO].position[POSY], 590, 225);
				if(selisihpos < 10){
					flagkalibdone_agent2 = 1;
				}
			}
			break;
		}

		case ERSOW_JAMIL:
		{
			strategyErsow.state_agent3 = 522;
			break;
		}
	}
}

void KALIBY(int robot)
{
	switch(robot)
	{
		case ERSOW_OKTO:
		{
			strategyErsow.state_agent1 = 503; break;
		}

		case ERSOW_HENDRO:
		{
			strategyErsow.state_agent2 = 513; break;
		}

		case ERSOW_JAMIL:
		{
			strategyErsow.state_agent3 = 523; break;
		}
	}
}

void getCorner(int agent, int targetx, int targety, int targett)
{
	switch(agent){
		case ERSOW_OKTO:
			strategyErsow.state_agent1 	= READY; 
			strategyErsow.mode_agent1 	= CORNER;	
			
			strategyErsow.targetx_agent1 = targetx; 
			strategyErsow.targety_agent1 = targety; 
			strategyErsow.targett_agent1 = targett;
			break;
		case ERSOW_HENDRO:
			strategyErsow.state_agent2 	= READY; 
			strategyErsow.mode_agent2 	= CORNER;	
			
			strategyErsow.targetx_agent2 = targetx; 
			strategyErsow.targety_agent2 = targety; 
			strategyErsow.targett_agent2 = targett;
			break;
		case ERSOW_JAMIL:
			strategyErsow.state_agent3 	= READY; 
			strategyErsow.mode_agent3 	= CORNER;	
			
			strategyErsow.targetx_agent3 = targetx; 
			strategyErsow.targety_agent3 = targety; 
			strategyErsow.targett_agent3 = targett;
			break;
	}
}

void getDropball(int agent, int targetx, int targety, int targett) 
{
	switch(agent){
		case ERSOW_OKTO:
			strategyErsow.state_agent1 	= READY; 
			strategyErsow.mode_agent1 	= DRPBALL;	
			
			strategyErsow.targetx_agent1 = targetx; 
			strategyErsow.targety_agent1 = targety; 
			strategyErsow.targett_agent1 = targett;
			break;
		case ERSOW_HENDRO:
			strategyErsow.state_agent2 	= READY; 
			strategyErsow.mode_agent2 	= DRPBALL;	
			
			strategyErsow.targetx_agent2 = targetx; 
			strategyErsow.targety_agent2 = targety; 
			strategyErsow.targett_agent2 = targett;
			break;
		case ERSOW_JAMIL:
			strategyErsow.state_agent3 	= READY; 
			strategyErsow.mode_agent3 	= DRPBALL;	
			
			strategyErsow.targetx_agent3 = targetx; 
			strategyErsow.targety_agent3 = targety; 
			strategyErsow.targett_agent3 = targett;
			break;
	}	
}


void getGoalKick(int agent, int targetx, int targety, int targett)
{
	switch(agent) {
		case ERSOW_OKTO:
			strategyErsow.state_agent1 	= READY; 
			strategyErsow.mode_agent1 	= PUTARBOLA;	
			
			strategyErsow.targetx_agent1 = targetx; 
			strategyErsow.targety_agent1 = targety; 
			strategyErsow.targett_agent1 = targett; 		
		break;

		case ERSOW_HENDRO:
			strategyErsow.state_agent2 	= READY; 
			strategyErsow.mode_agent2 	= PUTARBOLA;	
			
			strategyErsow.targetx_agent2 = targetx; 
			strategyErsow.targety_agent2 = targety; 
			strategyErsow.targett_agent2 = targett; 		
		break;

		case ERSOW_JAMIL:
			strategyErsow.state_agent3 	= READY; 
			strategyErsow.mode_agent3 	= PUTARBOLA;	
			
			strategyErsow.targetx_agent3 = targetx; 
			strategyErsow.targety_agent3 = targety; 
			strategyErsow.targett_agent3 = targett; 		
		break;

	}
}

void getPassing(int agent, int targetx, int targety)
{
	switch(agent) {
		case ERSOW_OKTO:
			strategyErsow.state_agent1 	= READY; 
			strategyErsow.mode_agent1 	= MAKEPASS;	
			
			strategyErsow.targetx_agent1 = targetx; 
			strategyErsow.targety_agent1 = targety; 		
		break;

		case ERSOW_HENDRO:
			strategyErsow.state_agent2 	= READY; 
			strategyErsow.mode_agent2 	= MAKEPASS;	
			
			strategyErsow.targetx_agent2 = targetx; 
			strategyErsow.targety_agent2 = targety; 		
		break;

		case ERSOW_JAMIL:
			strategyErsow.state_agent3 	= READY; 
			strategyErsow.mode_agent3 	= MAKEPASS;	
			
			strategyErsow.targetx_agent3 = targetx; 
			strategyErsow.targety_agent3 = targety; 		
		break;

	}
}

void receivePassing(int agent, int targetx, int targety)
{
	switch(agent) {
		case ERSOW_OKTO:
			strategyErsow.state_agent1 	= READY; 
			strategyErsow.mode_agent1 	= RECEIVEPASS;	
			
			strategyErsow.targetx_agent1 = targetx; 
			strategyErsow.targety_agent1 = targety; 		
		break;

		case ERSOW_HENDRO:
			strategyErsow.state_agent2 	= READY; 
			strategyErsow.mode_agent2 	= RECEIVEPASS;	
			
			strategyErsow.targetx_agent2 = targetx; 
			strategyErsow.targety_agent2 = targety; 		
		break;

		case ERSOW_JAMIL:
			strategyErsow.state_agent3 	= READY; 
			strategyErsow.mode_agent3 	= RECEIVEPASS;	
			
			strategyErsow.targetx_agent3 = targetx; 
			strategyErsow.targety_agent3 = targety; 		
		break;

	}
}

void ourKickOff(void )
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];

	static int i,j, cntValid=0;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	if(strategyErsow.ball_detect == UNDETECTED){
		referenceBall[POSX] = 300;
		referenceBall[POSY] = 450;
	}
	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	// step menggunakan enum
	switch(stepposersow)
	{
		case firstpos: 
		{			
			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			

			// pindah step dua secondpos
			stepposersow = secondpos;
			break;
		}

		case secondpos: 
		{
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					PointTo(ERSOW_OKTO, referenceBall[POSX]+80, referenceBall[POSY]-70, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, referenceBall[POSX] - 100, referenceBall[POSY] - 175, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					PointTo(ERSOW_HENDRO, referenceBall[POSX]+80, referenceBall[POSY]-70, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, referenceBall[POSX] - 100, referenceBall[POSY] - 175, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				PointTo(ERSOW_OKTO, referenceBall[POSX]+80, referenceBall[POSY]-70, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				PointTo(ERSOW_HENDRO, referenceBall[POSX]+80, referenceBall[POSY]-70, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
		break;
		}
	}
}


void ourGoalKick(void)
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posxgo, posygo, posxdef, posydef;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	if(referenceBall[POSX] > 300){
		if((agent[ERSOW_OKTO].position[POSY] >= referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] >= referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] + 100; 
		}
		else if((agent[ERSOW_OKTO].position[POSY] < referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] < referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] - 100; 
		}
		posxdef = referenceBall[POSX] - 150;
		posydef = referenceBall[POSY] + 150;
	}
	else if(referenceBall[POSX] < 300){
		if((agent[ERSOW_OKTO].position[POSY] >= referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] >= referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] + 100; 
		}
		else if((agent[ERSOW_OKTO].position[POSY] < referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] < referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] - 100; 
		}
		posxdef = referenceBall[POSX] + 150;
		posydef = referenceBall[POSY] + 150;
	}	

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					getGoalKick(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posxdef, posydef, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getGoalKick(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posxdef, posydef, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				getGoalKick(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				getGoalKick(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}

}	



void ourPenalty(void )
{ }


void ourFreeKick(void)
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posxgo, posygo, posxdef, posydef;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	if(referenceBall[POSX] > 300){
		if((agent[ERSOW_OKTO].position[POSY] >= referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] >= referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] + 100; 
		}
		else if((agent[ERSOW_OKTO].position[POSY] < referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] < referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] - 100; 
		}
		posxdef = referenceBall[POSX] - 150;
		posydef = referenceBall[POSY] + 150;
	}
	else if(referenceBall[POSX] < 300){
		if((agent[ERSOW_OKTO].position[POSY] >= referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] >= referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] + 100; 
		}
		else if((agent[ERSOW_OKTO].position[POSY] < referenceBall[POSY]) || (agent[ERSOW_HENDRO].position[POSY] < referenceBall[POSY])){
			posxgo = referenceBall[POSX];
			posygo = referenceBall[POSY] - 100; 
		}
		posxdef = referenceBall[POSX] + 150;
		posydef = referenceBall[POSY] + 150;
	}	

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					getGoalKick(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posxdef, posydef, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getGoalKick(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posxdef, posydef, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				getGoalKick(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				getGoalKick(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}
}


void ourCornerKick(void )
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posxgo, posygo, posxdef, posydef;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;	

	if(referenceBall[POSX] > 300){
		posxgo = referenceBall[POSX] - 100;
		posygo = referenceBall[POSY] - 100;

		posxdef = referenceBall[POSX] - 300;
		posydef = referenceBall[POSY] - 125;
	}else{
		posxgo = referenceBall[POSX] + 100;
		posygo = referenceBall[POSY] - 100;

		posxdef = referenceBall[POSX] + 300;
		posydef = referenceBall[POSY] - 125;
	}

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					getCorner(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posxdef, posydef, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getCorner(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posxdef, posydef, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				getCorner(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				getCorner(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}
}




void theirKickOff(void )
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posx, posy;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	if(strategyErsow.global_ballposx == UNDETECTED){
		referenceBall[POSX] = 300;
		referenceBall[POSY] = 450;
	}else{
		referenceBall[POSX] = strategyErsow.global_ballposx;
		referenceBall[POSY] = strategyErsow.global_ballposy;	
	}
	
	posx = referenceBall[POSX] + 60;
	posy = referenceBall[POSY] -225;

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					PointTo (ERSOW_OKTO, posx, posy, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, referenceBall[POSX]-120, referenceBall[POSY]-225, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					PointTo(ERSOW_HENDRO, posx, posy, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, referenceBall[POSX]-120, referenceBall[POSY]-225, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				PointTo(ERSOW_OKTO, posx, posy, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				PointTo(ERSOW_HENDRO, posx, posy, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
		break;
		}		
	} 
}


void theirGoalKick(void )
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posx,posy;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	if(referenceBall[POSX] > 300){
		posx = referenceBall[POSX] - 150;
		posy = referenceBall[POSY] - 420;
	}else{
		posx = referenceBall[POSX] + 150;
		posy = referenceBall[POSY] - 420;
	}	

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					PointTo(ERSOW_OKTO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posx, posy, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getGoalKick(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posx, posy, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				PointTo(ERSOW_OKTO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				PointTo(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}
}


void theirPenalty(void ){ }


void theirFreeKick(void )
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posx,posy;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	if(referenceBall[POSX] > 300){
		posx = referenceBall[POSX] - 150;
		posy = referenceBall[POSY] - 420;
	}else{
		posx = referenceBall[POSX] + 150;
		posy = referenceBall[POSY] - 420;
	}	

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					PointTo(ERSOW_OKTO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posx, posy, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getGoalKick(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posx, posy, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				PointTo(ERSOW_OKTO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				PointTo(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}
}


void theirCornerKick(void)
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posxgo,posygo, posxdef, posydef;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	if(referenceBall[POSX] > 300){
		posxgo = referenceBall[POSX] - 215;
		posygo = referenceBall[POSY] + 215;
		
		posxdef = referenceBall[POSX] - 250;
		posydef = referenceBall[POSY] + 100;
	}else{
		posxgo = referenceBall[POSX] + 215;
		posygo = referenceBall[POSY] + 215;

		posxdef = referenceBall[POSX] + 350;
		posydef = referenceBall[POSY] + 100;
	}	

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					PointTo(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posxdef, posydef, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getGoalKick(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 315, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				PointTo(ERSOW_OKTO, posxgo, posygo, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				PointTo(ERSOW_HENDRO, posxgo, posygo, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}
}

void startGoalKick(int team) ////--------------------------->NEED INPUT VALUE >>>>>FIX
{
	static int databolax;
	static int databolay;
	static int geser_bola, timeout;
	static int counter;
	static int jarak_robot;

	switch(team)
	{
		case TEAM_ERSOW:
		{
			switch(stepstart)
			{
				case first:
				{
					//ambil data bola saat ini
					databolax = strategyErsow.global_ballposx;
					databolay = strategyErsow.global_ballposy;

					strategyErsow.mode_agent3 = FSM_KEEPER;
					strategyErsow.state_agent3 = READY;

					//set timer to 0
					timer_sig=0;

					//next step
					stepstart = second;
					break;
				}

				case second:
				{
					geser_bola = Edistance(databolax, databolay, strategyErsow.global_ballposx, strategyErsow.global_ballposy);
					jarak_robot = Edistance(agent[ERSOW_OKTO].position[POSX], agent[ERSOW_OKTO].position[POSY], agent[ERSOW_HENDRO].position[POSX], agent[ERSOW_HENDRO].position[POSY]);
					FSM();
					break;
				}
				break;
			}
			break;
		}
		break;

		case TEAM_MUSUH:
		{
			//deklarasi variable static
			static int lastballposx;
			static int lastballposy;
			static int timeout;
			static int jarak;

			switch(stepmusuh)
			{
				case first:
				{
					//ambil data bola
					lastballposx = strategyErsow.global_ballposx;
					lastballposy = strategyErsow.global_ballposy;

					//set timer to 0, serta next step 3
					timer_sig = 0;
					//next step 2
					stepmusuh = second;
					break;
				}
				case second:
				{
					//hitung jarak pergeseran bola
					jarak = Edistance(lastballposx, lastballposy, strategyErsow.global_ballposx, strategyErsow.global_ballposy);
					
					//keeper dalam keadaan ready dan mode FSM_KEEPER
					strategyErsow.state_agent3 = READY;
					strategyErsow.mode_agent3 = FSM_KEEPER;

					strategyErsow.state_agent2 = SLEEP;
					strategyErsow.mode_agent2 = 0;

					strategyErsow.state_agent1 = SLEEP;
					strategyErsow.mode_agent1 = 0;

					//kondisi jika timer lebih dari 7sec atau jarak lebih dari 50cm
					if((timer_sig >= 6) || (jarak >=50)){
						if(jarak >= 150){
							stepmusuh = second;
						}
						else
						stepmusuh = third;
					}
					break;
				}
				case third:
				{
					FSM();
				}
				break;
			}
			break;
		}
	}
}

void startKickOff(int team) ////--------------------------->NEED INPUT RECIEVE PASS IF NEED >>>>>FIX
{
	//deklarasi variable static
	static int bola_tengahx; 
	static int bola_tengahy; 
	static int geserbola;
	static int counter=0;
	static int jarak_robot;

	switch(team)
	{
		case TEAM_ERSOW:
		{
			switch(stepstart)
			{
				case first:
				{
					//ambil data bola
					bola_tengahx = strategyErsow.global_ballposx;
					bola_tengahy = strategyErsow.global_ballposy;
					//set state dan mode agent3[ERSOW_JAMIL]
					strategyErsow.mode_agent3 = FSM_KEEPER;
					strategyErsow.state_agent3 = READY;

					//set timer to 0
					timer_sig=0;
					//next step 2
					stepstart = second;
					break;
				}

				case second:
				{
					geserbola = Edistance(bola_tengahx, bola_tengahy, strategyErsow.global_ballposx, strategyErsow.global_ballposy);
					jarak_robot = Edistance(agent[ERSOW_OKTO].position[POSX], agent[ERSOW_OKTO].position[POSY], agent[ERSOW_HENDRO].position[POSX], agent[ERSOW_HENDRO].position[POSY]);
					
					stepstart = third;
					break;
				}

				case third:
				{
					FSM();
					printf("MASUK FSM KICKOFF\n");
					break;
				}
				break;
			}
			break;
		}
		break;

		case TEAM_MUSUH:
		{
			//deklarasi variable static
			static int lastballposx;
			static int lastballposy;
			static int timeout;
			static int jarak;

			switch(stepmusuh)
			{
				case first:
				{
					//ambil data bola
					lastballposx = strategyErsow.global_ballposx;
					lastballposy = strategyErsow.global_ballposy;

					//set timer to 0, serta next step 3
					timer_sig = 0;
					//next step 2
					stepmusuh = second;
					break;
				}
				case second:
				{
					//hitung jarak pergeseran bola
					jarak = Edistance(lastballposx, lastballposy, strategyErsow.global_ballposx, strategyErsow.global_ballposy);

					//keeper dalam keadaan ready dan mode FSM_KEEPER
					strategyErsow.state_agent3 = READY;
					strategyErsow.mode_agent3 = FSM_KEEPER;

					strategyErsow.state_agent2 = SLEEP;
					strategyErsow.mode_agent2 = 0;

					strategyErsow.state_agent1 = SLEEP;
					strategyErsow.mode_agent1 = 0;

					//kondisi jika timer lebih dari 7sec atau jarak lebih dari 50cm
					if((timer_sig >= 6) || (jarak >=50)){
						if(jarak >= 150){
							stepmusuh = second;
						}
						else
						stepmusuh = third;
					}
					break;
				}
				case third:
				{
					FSM();
				}
				break;
			}
			break;
		}
	}
}

void startCorner(int team) ////--------------------------->NEED INPUT VALUE >>>>>NOT FIX
{
	// deklarasi variable static
	static int databolax, databolay;
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int theta_okto,theta_hendro;
	static int geser_bola;
	static int jarak_robot;


	switch(team)
	{
		case TEAM_ERSOW:
		{
			switch(stepstart)
			{
				case first:
				{
					//ambil data robot dan bola
					AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
					AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];
					AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
					AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];
					databolax = strategyErsow.global_ballposx;
					databolay = strategyErsow.global_ballposy;
					//set keeper to mode FSM_KEEPER
					strategyErsow.mode_agent3 = FSM_KEEPER;

					// seti timer to 0, next step 2
					stepstart=second;
					timer_sig = 0;
					break;
				}

				case second:
				{
					//hiung jarak antar robot
					jarak_robot = Edistance(AgentOkto[POSX], AgentOkto[POSY], AgentHendro[POSX], AgentHendro[POSY]);
					//hitung jarak pergeseran bola
					geser_bola = Edistance(databolax, databolay, strategyErsow.global_ballposx, strategyErsow.global_ballposy);
					
					FSM();
					break;
				}
				break;
			}
			break;
		}
		break;

		case TEAM_MUSUH:
		{
			//deklarasi variable static
			static int lastballposx;
			static int lastballposy;
			static int timeout;
			int shiftballx;
			int shiftbally;

			switch(stepmusuh)
			{
				case first:
				{
					//ambil data bola
					lastballposx = strategyErsow.global_ballposx;
					lastballposy = strategyErsow.global_ballposy;

					//set timer to 0 dan next step 3
					timer_sig = 0;

					//next step 2
					stepmusuh = second;
					break;
				}

				case second:
				{	
					//hitung jarak pergeseran bola
					geser_bola = Edistance(lastballposx, lastballposy, strategyErsow.global_ballposx, strategyErsow.global_ballposy);

					//keeper dalam keadaan ready dan mode FSM_KEEPER
					strategyErsow.state_agent3 = READY;
					strategyErsow.mode_agent3 = FSM_KEEPER;

					strategyErsow.state_agent2 = SLEEP;
					strategyErsow.mode_agent2 = 0;

					strategyErsow.state_agent1 = SLEEP;
					strategyErsow.mode_agent1 = 0;

					//jika geser bola lebih dari 30cm atau timer lebih dari 7sec -> FSM
					if(geser_bola > 30 || timer_sig >=6){
						if(geser_bola > 150){
							stepmusuh = second;
						}
						else
							stepmusuh = third;
					}
					break;
				}

				case third:
				{
					FSM();	
					break;
				}
				break;
			}
			break;
		} 
		break;
	}
}

void startFreeKick(){}

void startDropball(int team)
{
	//set keeper mode FSM_KEEPER and state READY
	strategyErsow.state_agent3 = READY;
	strategyErsow.mode_agent3 = FSM_KEEPER;

	switch(team)
	{
		case TEAM_ERSOW:
		{
			switch(stepstart)
			{ 
				case first:
				{
					FSM();
				}
				break;
			}
			
		}
		break;
	}
}

void PosDropball()
{
	// deklarasi variable static
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];
	static int angle[3];
	static int referenceBall[2];
	static int posx, posy;

	angle[ERSOW_OKTO] = getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	angle[ERSOW_HENDRO] = getAngle(agent[ERSOW_HENDRO], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	referenceBall[POSX] = strategyErsow.global_ballposx;
	referenceBall[POSY] = strategyErsow.global_ballposy;

	if(referenceBall[POSX] > 300){
		posx = referenceBall[POSX] - 150;
		posy = referenceBall[POSY] - 150;
	}
	else{
		posx = referenceBall[POSX] + 150;
		posy = referenceBall[POSY] - 150;
	}	

	// step positioning ersow
	switch(stepposersow)
	{
		case(firstpos):
		{

			// define data odometry okto
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];

			// define data odometry hendro
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];

			// define data odometry jamil
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			agent[ERSOW_OKTO].dist2ball = Edistance(AgentOkto[POSX], AgentOkto[POSY], referenceBall[POSX], referenceBall[POSY] );
			agent[ERSOW_HENDRO].dist2ball = Edistance(AgentHendro[POSX], AgentHendro[POSY], referenceBall[POSX], referenceBall[POSY] );
			
			// next step
			stepposersow = secondpos;
		break;
		}

		case(secondpos):
		{	
			if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				if(agent[ERSOW_OKTO].dist2ball <= agent[ERSOW_HENDRO].dist2ball){
					getDropball(ERSOW_OKTO, referenceBall[POSX], referenceBall[POSY] - 115, angle[ERSOW_OKTO]);
					PointTo(ERSOW_HENDRO, posx, posy, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}else{
					getDropball(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 115, angle[ERSOW_HENDRO]);
					PointTo(ERSOW_OKTO, posx, posy, angle[ERSOW_OKTO]);
					PointTo(ERSOW_JAMIL, 300, 50, 0);
				}	
			}
			else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))) {
				getDropball(ERSOW_OKTO, referenceBall[POSX], referenceBall[POSY] - 115, angle[ERSOW_OKTO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}
			else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))) {
				getDropball(ERSOW_HENDRO, referenceBall[POSX], referenceBall[POSY] - 115, angle[ERSOW_HENDRO]);
				PointTo(ERSOW_JAMIL, 300, 50, 0);
			}


		break;
		}		
	}
}

void PosPark()
{
	// deklarasi variable static
	static int referenceBall[2];
	static short int AgentOkto[2]; 
	static short int AgentHendro[2];
	static short int AgentJamil[2];

	switch(stepposersow)
	{
		case firstpos:
		{
			//ambil data robot
			AgentOkto[POSX] = agent[ERSOW_OKTO].position[POSX];
			AgentOkto[POSY] = agent[ERSOW_OKTO].position[POSY];
			AgentHendro[POSX] = agent[ERSOW_HENDRO].position[POSX];
			AgentHendro[POSY] = agent[ERSOW_HENDRO].position[POSY];
			AgentJamil[POSX] = agent[ERSOW_JAMIL].position[POSX];
			AgentJamil[POSY] = agent[ERSOW_JAMIL].position[POSY];

			//set timer to 0
			timer_sig = 0;
			// pindah step dua secondpos
			stepposersow = secondpos;
			break;
		}

		case secondpos:
		{

			//kondisi jika posisi hendro x lebih besar dari okto
			if(AgentHendro[POSY] >= AgentOkto[POSY]){
				PointTo(ERSOW_OKTO, 300, 180, 0);
				PointTo(ERSOW_HENDRO, 300, 380, 0);
				PointTo(ERSOW_JAMIL, 200, 75, 0);
			}
			//kondisi jika posisi okto x lebih besar dari hendro
			else if(AgentOkto[POSY] > AgentHendro[POSY]){
				PointTo(ERSOW_HENDRO, 300, 180, 0);
				PointTo(ERSOW_OKTO, 300, 380, 0);
				PointTo(ERSOW_JAMIL, 200, 75, 0);
			}
			if(timer_sig >=4){
				stepposersow = thirdpos;
			}
			break;
		}

		case thirdpos:
		{
			//last step park jika x hendro lebih dari okto
			if(AgentHendro[POSY] >= AgentOkto[POSY]){
				PointTo(ERSOW_OKTO, 0, 0, 0);
				PointTo(ERSOW_HENDRO, 600, 0, 0);
				PointTo(ERSOW_JAMIL, 87, 0, 0);
			}
			//last step park jika x okto lebih dari hendro
			else if(AgentOkto[POSY] > AgentHendro[POSY]){
				PointTo(ERSOW_HENDRO, 600, 0, 0);
				PointTo(ERSOW_OKTO, 0, 0, 0);
				PointTo(ERSOW_JAMIL, 87, 0, 0);
			}
		}
	}
}

void FSM()
{
	//set variable static
	static int jaraklari=0, jaraklariokto=0, jaraklarihendro=0;
	static int acclari=0;
	static int lastdataoktoposx=0,lastdataoktoposy=0;
	static int lastdatahendroposx=0,lastdatahendroposy=0;
	static int bolaOkto, bolaHendro;


	//hitung jarak tempuh okto
	jaraklariokto = Edistance(agent[ERSOW_OKTO].position[POSX], agent[ERSOW_OKTO].position[POSY], lastdataoktoposx, lastdataoktoposy);
	//hitung jarak tempuh hendro
	jaraklarihendro = Edistance(agent[ERSOW_HENDRO].position[POSX], agent[ERSOW_HENDRO].position[POSY], lastdatahendroposx, lastdatahendroposy);
	//hitung jarak tempuh dari kedua agent
	jaraklari = jaraklariokto+jaraklarihendro;
	//akumulasi jarak tempuh dari kedua agent
	acclari += jaraklari;

	printf("FSM IN\n");
	
	// if ((agent[ERSOW_OKTO].state == RUNNING))
	bolaOkto = Edistance(agent[ERSOW_OKTO].position[POSX], agent[ERSOW_OKTO].position[POSY], strategyErsow.global_ballposx, strategyErsow.global_ballposy);
	bolaHendro = Edistance(agent[ERSOW_HENDRO].position[POSX], agent[ERSOW_HENDRO].position[POSY], strategyErsow.global_ballposx, strategyErsow.global_ballposy);

	//kondisi jika okto dan hendro running
	if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE	))){
		if (bolaOkto < bolaHendro) {
			strategyErsow.state_agent1 = READY; strategyErsow.mode_agent1 = FSM_STRIKER_FULLSKILL; 
			strategyErsow.state_agent2 = READY;	strategyErsow.mode_agent2 = FSM_MIDFIELDER; 
			strategyErsow.state_agent3 = READY; strategyErsow.mode_agent3 = FSM_KEEPER; 
		} else {
			strategyErsow.state_agent1 = READY; strategyErsow.mode_agent1 = FSM_MIDFIELDER; 
			strategyErsow.state_agent2 = READY;	strategyErsow.mode_agent2 = FSM_STRIKER_FULLSKILL; 
			strategyErsow.state_agent3 = READY; strategyErsow.mode_agent3 = FSM_KEEPER; 
		}
	}
	//kondisi jika okto running dan hendro tidak running
	else if(((agent[ERSOW_OKTO].state == RUNNING) || (agent[ERSOW_OKTO].state == REMOVE)) && ((agent[ERSOW_HENDRO].state == NOT_RUNNING) || (agent[ERSOW_HENDRO].state == INSERT))){	
		strategyErsow.state_agent1 = READY; strategyErsow.mode_agent1 = FSM_STRIKER_FULLSKILL; 
		strategyErsow.state_agent3 = READY; strategyErsow.mode_agent3 = FSM_KEEPER; 
	}
	else if(((agent[ERSOW_OKTO].state == NOT_RUNNING) || (agent[ERSOW_OKTO].state == INSERT)) && ((agent[ERSOW_HENDRO].state == RUNNING) || (agent[ERSOW_HENDRO].state == REMOVE))){
		strategyErsow.state_agent2 = READY; strategyErsow.mode_agent2 = FSM_STRIKER_FULLSKILL; 
		strategyErsow.state_agent3 = READY; strategyErsow.mode_agent3 = FSM_KEEPER; 
	}
	
	//at last, update posisi akhir agent
	lastdatahendroposx = agent[ERSOW_HENDRO].position[POSX];
	lastdatahendroposy = agent[ERSOW_HENDRO].position[POSY];
	lastdataoktoposx = agent[ERSOW_OKTO].position[POSX];
	lastdataoktoposy = agent[ERSOW_OKTO].position[POSY];
}

void FSMClosestBall()
{
	// butuh data
	// posisi semua robot
	// persepsi masing - masing robot terhadap posisi bola
	
	// hitung jarak robot terhadap bola
	// Euclidian distance
	agent[ERSOW_OKTO].dist2ball = (int)(sqrt(pow((agent[ERSOW_OKTO].position[POSX] - agent[ERSOW_OKTO].ball_position[POSX]), 2) 
			+ pow((agent[ERSOW_OKTO].position[POSY] - agent[ERSOW_OKTO].ball_position[POSY]), 2)));

	agent[ERSOW_HENDRO].dist2ball = (int)(sqrt(pow((agent[ERSOW_HENDRO].position[POSX] - agent[ERSOW_HENDRO].ball_position[POSX]), 2) 
			+ pow((agent[ERSOW_HENDRO].position[POSY] - agent[ERSOW_HENDRO].ball_position[POSY]), 2)));

	agent[ERSOW_JAMIL].dist2ball = (int)(sqrt(pow((agent[ERSOW_JAMIL].position[POSX] - agent[ERSOW_JAMIL].ball_position[POSX]), 2) 
			+ pow((agent[ERSOW_JAMIL].position[POSY] - agent[ERSOW_JAMIL].ball_position[POSY]), 2)));


#ifdef DEBUG_STATE_AGENT
	printf("___________DEBUG STATE AGENT____________\n");
	printf("State Agent %d\t%d\t%d\n", 
		agent[ERSOW_OKTO].state,
		agent[ERSOW_HENDRO].state,
		agent[ERSOW_JAMIL].state);
#endif


	// menentukan deteksi bola
	ball_detect = UNDETECTED;
	int i, j;
	for(i=1; i<=3; i++)
	{
		if (agent[i].ball_detect == (1))
			ball_detect = DETECTED;
	}

	if((agent[ERSOW_OKTO].state == NOT_RUNNING) 
		&& (agent[ERSOW_HENDRO].state == NOT_RUNNING) && (agent[ERSOW_JAMIL].state == NOT_RUNNING))
	{
			// set global perception of ball
		global_ball[POSX] = 0;	
		global_ball[POSY] = 0;
		ball_detect = UNDETECTED;
	}



	// menentukan persepsi global ball 
	// Kondisi A. Semua agent RUNNING
	else if ((agent[ERSOW_OKTO].state == RUNNING) && (agent[ERSOW_HENDRO].state == RUNNING) && (agent[ERSOW_JAMIL].state == RUNNING))
	{	
		// Kondisi 1: jarak agent 1 paling dekat dengan bola
		if ((agent[ERSOW_OKTO].dist2ball < agent[ERSOW_HENDRO].dist2ball) 
			&& (agent[ERSOW_OKTO].dist2ball < agent[ERSOW_JAMIL].dist2ball)) {

			// set global perception of ball
			global_ball[POSX] = agent[ERSOW_OKTO].ball_position[POSX];	
			global_ball[POSY] = agent[ERSOW_OKTO].ball_position[POSY];

			// set fsm agents
			fsm_agent[ERSOW_OKTO] = STRIKER;
			fsm_agent[ERSOW_HENDRO] = DEFENDER;
			fsm_agent[ERSOW_JAMIL] = KEEPER;
		} 

		// Kondisi 2: jarak agent 2 paling dekat dengan bola
		else if ((agent[ERSOW_HENDRO].dist2ball < agent[ERSOW_JAMIL].dist2ball) 
			&& (agent[ERSOW_HENDRO].dist2ball < agent[ERSOW_OKTO].dist2ball)) {

			// set global perception of ball
			global_ball[POSX] = agent[ERSOW_HENDRO].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_HENDRO].ball_position[POSY];

			// set fsm agents
			fsm_agent[ERSOW_OKTO] = DEFENDER;	
			fsm_agent[ERSOW_HENDRO] = STRIKER;
			fsm_agent[ERSOW_JAMIL] = KEEPER;
		} 

		// Kondisi 3: jarak agent 3 paling dekat dengan bola
		else if ((agent[ERSOW_JAMIL].dist2ball < agent[ERSOW_OKTO].dist2ball) 
			&& (agent[ERSOW_JAMIL].dist2ball < agent[ERSOW_HENDRO].dist2ball)) {

			// set global perception of ball
			global_ball[POSX] = agent[ERSOW_JAMIL].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_JAMIL].ball_position[POSY];

			// set fsm agents
			fsm_agent[ERSOW_OKTO] = DEFENDER;	
			fsm_agent[ERSOW_HENDRO] = STRIKER;
			fsm_agent[ERSOW_JAMIL] = KEEPER;
		} 		
	}

	// Kondisi B. Agent 1 dan Agent 2 only RUNNING
	else if ((agent[ERSOW_OKTO].state == RUNNING) && (agent[ERSOW_HENDRO].state == RUNNING))
	{
		// Kondisi 1: jarak agent 1 paling dekat dengan bola
		if ((agent[ERSOW_OKTO].dist2ball < agent[ERSOW_HENDRO].dist2ball)) {

			// set global perception of ball
			global_ball[POSX] = agent[ERSOW_OKTO].ball_position[POSX];	
			global_ball[POSY] = agent[ERSOW_OKTO].ball_position[POSY];

			// set fsm agents
			fsm_agent[ERSOW_OKTO] = STRIKER;
			fsm_agent[ERSOW_HENDRO] = DEFENDER;
		} 

		// Kondisi 2: jarak agent 2 paling dekat dengan bola
		else if ((agent[ERSOW_OKTO].dist2ball > agent[ERSOW_HENDRO].dist2ball)) {

			// set global perception of ball
			global_ball[POSX] = agent[ERSOW_HENDRO].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_HENDRO].ball_position[POSY];

			// set fsm agents
			fsm_agent[ERSOW_OKTO] = DEFENDER;	
			fsm_agent[ERSOW_HENDRO] = STRIKER;
		}		
	} 

	// Kondisi C. agent1 NOT_RUNNING
	else if ((agent[ERSOW_OKTO].state == NOT_RUNNING) && (agent[ERSOW_HENDRO].state == RUNNING)) 
	{
		if ((agent[ERSOW_JAMIL].state == RUNNING) && agent[ERSOW_JAMIL].dist2ball < agent[ERSOW_HENDRO].dist2ball) 
		{
			// robot 3 terdekat dengan bola
			global_ball[POSX] = agent[ERSOW_JAMIL].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_JAMIL].ball_position[POSY];
						
		} else {

			// robot 2 terdekat dengan bola
			global_ball[POSX] = agent[ERSOW_HENDRO].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_HENDRO].ball_position[POSY];

			fsm_agent[ERSOW_HENDRO] = STRIKER;
			fsm_agent[ERSOW_JAMIL] = KEEPER;
		}	
	}

	// Kondisi D. agent2 NOT_RUNNING
	else if ((agent[ERSOW_OKTO].state == RUNNING) && (agent[ERSOW_HENDRO].state == NOT_RUNNING)) 
	{
		if ((agent[ERSOW_JAMIL].state == RUNNING) && (agent[ERSOW_JAMIL].dist2ball < agent[ERSOW_OKTO].dist2ball)) 
		{
			// robot 3 terdekat dengan bola
			global_ball[POSX] = agent[ERSOW_JAMIL].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_JAMIL].ball_position[POSY];

		} else {

			// robot 1 terdekat dengan bola
			global_ball[POSX] = agent[ERSOW_OKTO].ball_position[POSX];
			global_ball[POSY] = agent[ERSOW_OKTO].ball_position[POSY];
			
			fsm_agent[ERSOW_OKTO] = STRIKER;
			fsm_agent[ERSOW_JAMIL] = KEEPER;
		}	
	}

	else if((agent[ERSOW_OKTO].state == NOT_RUNNING) && (agent[ERSOW_HENDRO].state == NOT_RUNNING) && (agent[ERSOW_JAMIL].state == RUNNING))
	{
		// robot 3 terdekat dengan bola
		global_ball[POSX] = agent[ERSOW_JAMIL].ball_position[POSX];
		global_ball[POSY] = agent[ERSOW_JAMIL].ball_position[POSY];
	}


	// set data to publish
	ready_pub();
}

// function definition
void SetStrategyErsow(void )
{
	// TODO
	// nothing
	// exit

	// state dari Referee Box
	int state;	
	if (GUI.enableRef == ENABLE){
		if (GUI.gui_key == 0)
			state = referee.state;	// please edit later, it must receive from referee
		else if(GUI.gui_key == 69) {
			state = GUI.refbox;
			printf("Masuk Interupt\n");
		}
	}
	else 
    	state = GUI.refbox;

	int lastState = referee.lastState;
	// printf("referee.lastState:%d\n",referee.lastState);
	// set Team Cyan atau Magenta
	int ourteam = GUI.team;		

#ifdef DEBUG_SETSTRATEGY
	printf("__________DEBUG SETSTRATEGY________\n");
	printf("state %d\t%d\t%d\t\n", 
		state, 
		lastState,
		ourteam);
#endif

	// kondisional state dari referee
	switch (state) 
	{
		case STARTMATCH: 
			
			// apabila startmatch, maka kondisional team 
			switch(ourteam)
			{	
				case TEAM_CYAN: 
				{
					switch(lastState) {

						// kondisi saat ERSOW_CYAN saat CYAN_COMMAND
						case CYAN_KO: startKickOff(TEAM_ERSOW); break;
						case CYAN_GK: startGoalKick(TEAM_ERSOW); break;
						case CYAN_FK: startGoalKick(TEAM_ERSOW); break;
						case CYAN_PEN: 	break;
						case CYAN_THROW: break;
						case CYAN_CORNER: startCorner(TEAM_ERSOW); break;
						case CYAN_REPAIR: break;

						// kondisi saat ERSOW_CYAN saat MAGENTA_COMMAND
						case MAGENTA_KO: startKickOff(TEAM_MUSUH); break;
						case MAGENTA_GK: startGoalKick(TEAM_MUSUH); break;
						case MAGENTA_FK: startGoalKick(TEAM_MUSUH); break;
						case MAGENTA_PEN: break;
						case MAGENTA_THROW: break;
						case MAGENTA_CORNER: startCorner(TEAM_MUSUH); break;
						case MAGENTA_REPAIR: break;

						// kondisi saat ERSOW_CYAN saat DROPBALL
						case DROPBALL: startDropball(TEAM_ERSOW); break;
					}	
				}
				 break; // end case ERSOW_CYAN

				case TEAM_MAGENTA: 
				{
					switch(lastState) {

						// kondisi saat ERSOW_MAGENTA saat CYAN_COMMAND
						case CYAN_KO: startKickOff(TEAM_MUSUH); break;
						case CYAN_GK: startGoalKick(TEAM_MUSUH); break;
						case CYAN_FK: startGoalKick(TEAM_MUSUH); break;
						case CYAN_PEN: 	break;
						case CYAN_THROW: break;
						case CYAN_CORNER: startCorner(TEAM_MUSUH); break;
						case CYAN_REPAIR: break;

						// kondisi saat ERSOW_MAGENTA saat MAGENTA_COMMAND
						case MAGENTA_KO: startKickOff(TEAM_ERSOW); break;
						case MAGENTA_GK: startGoalKick(TEAM_ERSOW); break;
						case MAGENTA_FK: startGoalKick(TEAM_ERSOW); break;
						case MAGENTA_PEN: break;
						case MAGENTA_THROW: break;
						case MAGENTA_CORNER: startCorner(TEAM_ERSOW); break;
						case MAGENTA_REPAIR: break;

						// kondisi saat ERSOW_MAGENTA saat DROPBALL
						case DROPBALL: startDropball(TEAM_ERSOW); break;
					}	

				}
				 break; // end case ERSOW_MAGENTA

			}
		 break;	// end case STARTMATCH

		case PARK: PosPark(); break;
		case RESET:	reset(); break;
		case STOPMATCH:	break;
		case ENDPART: 	break;
		case CYAN_GOAL:	break;
		case MAGENTA_GOAL:	break;
		case DROPBALL : PosDropball(); referee.lastState = state; break;

		// ==================== MANUAL ROBOT ============================
		case OKTO_FORWARD: 		strategyErsow.state_agent1 = 111; break;
		case OKTO_BACK:			strategyErsow.state_agent1 = 115; break;
		case OKTO_RIGHT:		strategyErsow.state_agent1 = 113; break;
		case OKTO_LEFT:			strategyErsow.state_agent1 = 117; break;
		case OKTO_RRIGHT:		strategyErsow.state_agent1 = 118; break;
		case OKTO_RLEFT:		strategyErsow.state_agent1 = 119; break; 


		// -------------------------------------------------------------
		case HENDRO_FORWARD: 	strategyErsow.state_agent2 = 111; break;
		case HENDRO_BACK:		strategyErsow.state_agent2 = 115; break;
		case HENDRO_RIGHT:		strategyErsow.state_agent2 = 113; break;
		case HENDRO_LEFT:		strategyErsow.state_agent2 = 117; break;
		case HENDRO_RRIGHT:		strategyErsow.state_agent2 = 118; break;
		case HENDRO_RLEFT:		strategyErsow.state_agent2 = 119; break; 

		// -------------------------------------------------------------
		case JAMIL_FORWARD: 	strategyErsow.state_agent3 = 111; break;
		case JAMIL_BACK:		strategyErsow.state_agent3 = 115; break;
		case JAMIL_RIGHT:		strategyErsow.state_agent3 = 113; break;
		case JAMIL_LEFT:		strategyErsow.state_agent3 = 117; break;
		case JAMIL_RRIGHT:		strategyErsow.state_agent3 = 118; break;
		case JAMIL_RLEFT:		strategyErsow.state_agent3 = 119; break; 


		// ===================== CONTROL ROBOT =========================
		case OKTO_CALIBFULL:	printf("KALIB FULL OKTO\n"); 	if(agent[ERSOW_OKTO].value == 5){strategyErsow.state_agent1 = 11;}
																else strategyErsow.state_agent1 = 51;  			break;	
		case OKTO_CALIBX: 		printf("KALIB X OKTO\n"); 		if(agent[ERSOW_OKTO].value == 5){strategyErsow.state_agent1 = 11;}
																else strategyErsow.state_agent1 = 52;  			break;
		case OKTO_CALIBY: 		printf("KALIB Y OKTO\n"); 		if(agent[ERSOW_OKTO].value == 5){strategyErsow.state_agent1 = 11;}
																else strategyErsow.state_agent1 = 53;  			break;
		case OKTO_RESET: 		printf("KALIB RESET OKTO\n"); 	if(agent[ERSOW_OKTO].value == 5){strategyErsow.state_agent1 = 11;}
																else strategyErsow.state_agent1 = 50;  			break;
		case OKTO_DRIBBLE:   	printf("KALIB DRIBBLE OKTO\n"); if(agent[ERSOW_OKTO].value == 5){strategyErsow.state_agent1 = 11;}
																else strategyErsow.state_agent1 = 55;  			break;
		case OKTO_KICK:   		printf("KALIB KICK OKTO\n"); 	if(agent[ERSOW_OKTO].value == 5){strategyErsow.state_agent1 = 11;}
																else strategyErsow.state_agent1 = 54;  			break;
		case OKTO_STOP:			printf("KALIB STOP OKTO\n"); 	strategyErsow.state_agent1 = 0;  			break;

		case HENDRO_CALIBFULL:	printf("KALIB FULL HENDRO\n"); 	if(agent[ERSOW_HENDRO].value == 5){strategyErsow.state_agent2 = 11;}
																else strategyErsow.state_agent2 = 51;  			break;	
		case HENDRO_CALIBX: 	printf("KALIB X HENDRO\n"); 	if(agent[ERSOW_HENDRO].value == 5){strategyErsow.state_agent2 = 11;}
																else strategyErsow.state_agent2 = 52;  			break;
		case HENDRO_CALIBY: 	printf("KALIB Y HENDRO\n"); 	if(agent[ERSOW_HENDRO].value == 5){strategyErsow.state_agent2 = 11;}
																else strategyErsow.state_agent2 = 53;  			break;
		case HENDRO_RESET: 		printf("KALIB RESET HENDRO\n"); if(agent[ERSOW_HENDRO].value == 5){strategyErsow.state_agent2 = 11;}
																else strategyErsow.state_agent2 = 50;  			break;
		case HENDRO_DRIBBLE:   	printf("KALIB DRIBBLE HENDRO\n"); if(agent[ERSOW_HENDRO].value == 5){strategyErsow.state_agent2 = 11;}
																else strategyErsow.state_agent2 = 55;  			break;
		case HENDRO_KICK:   	printf("KALIB KICK HENDRO\n"); 	if(agent[ERSOW_HENDRO].value == 5){strategyErsow.state_agent2 = 11;}
																else strategyErsow.state_agent2 = 54;  			break;
		case HENDRO_STOP:		printf("KALIB STOP HENDRO\n"); 	strategyErsow.state_agent2 = 0;  			break;

		case JAMIL_CALIBFULL:	printf("KALIB FULL JAMIL\n"); 	if(agent[ERSOW_JAMIL].value == 5){strategyErsow.state_agent3 = 11;}
																else strategyErsow.state_agent3 = 51;  			break;	
		case JAMIL_CALIBX: 		printf("KALIB X JAMIL\n"); 		if(agent[ERSOW_JAMIL].value == 5){strategyErsow.state_agent3 = 11;}
																else strategyErsow.state_agent3 = 52;  			break;
		case JAMIL_CALIBY: 		printf("KALIB Y JAMIL\n"); 		if(agent[ERSOW_JAMIL].value == 5){strategyErsow.state_agent3 = 11;}
																else strategyErsow.state_agent3 = 53;  			break;
		case JAMIL_RESET: 		printf("KALIB RESET JAMIL\n"); 	if(agent[ERSOW_JAMIL].value == 5){strategyErsow.state_agent3 = 11;}
																else strategyErsow.state_agent3 = 50;  			break;
		case JAMIL_DRIBBLE:   	printf("KALIB DRIBBLE JAMIL\n");if(agent[ERSOW_JAMIL].value == 5){strategyErsow.state_agent3 = 11;}
																else strategyErsow.state_agent3 = 55;  			break;
		case JAMIL_KICK:   		printf("KALIB KICK JAMIL\n"); 	if(agent[ERSOW_JAMIL].value == 5){strategyErsow.state_agent3 = 11;}
																else strategyErsow.state_agent3 = 54;  			break;
		case JAMIL_STOP:		printf("KALIB STOP JAMIL\n"); 	strategyErsow.state_agent3 = 0;  			break;


		// =================== INITIAL CONDITION =======================
		default:
		{
			switch(ourteam) 
			{
				/* ----------------------- ERSOW CYAN ------------------ */
				case TEAM_CYAN		:	
					switch(state) 
					{
						/* -------------- CYAN REFEREE STATE ----------- */  				
						case CYAN_KO		: ourKickOff();  printf("masuk cyan KO\n");	referee.lastState = state; break; 
						case CYAN_GK		: ourGoalKick(); 	referee.lastState = state; break; 
						case CYAN_PEN		: referee.lastState = state; break; 
						case CYAN_FK		: ourFreeKick();    referee.lastState = state; break; 
						case CYAN_CORNER	: ourCornerKick();  referee.lastState = state; break; 	
						case CYAN_THROW		: referee.lastState = state; break;
						case CYAN_REPAIR	: referee.lastState = state; break;

						/* ------------ MAGENTA REFEREE STATE ---------- */
						case MAGENTA_KO		: theirKickOff();   referee.lastState = state; break; 
						case MAGENTA_GK		: theirGoalKick();  referee.lastState = state; break; 
						case MAGENTA_PEN	: getGoalKick(ERSOW_OKTO, strategyErsow.global_ballposx, strategyErsow.global_ballposy, getAngle(agent[ERSOW_OKTO], strategyErsow.global_ballposx, strategyErsow.global_ballposy)); referee.lastState = state; break; 
						case MAGENTA_FK		: theirFreeKick();  referee.lastState = state; break; 
						case MAGENTA_CORNER	: theirCornerKick();referee.lastState = state; break; 	
						case MAGENTA_THROW	: referee.lastState = state; break;
						case MAGENTA_REPAIR	: referee.lastState = state; break;
					}
				 break; // end case TEAM_CYAN

				/* --------------------- ERSOW MAGENTA ----------------- */
				case TEAM_MAGENTA	:
					switch(state) 
					{
						/* -------------- CYAN REFEREE STATE ----------- */  				
						case CYAN_KO		: theirKickOff();   referee.lastState = state; break; 
						case CYAN_GK		: theirGoalKick();  referee.lastState = state; break; 
						case CYAN_PEN		: referee.lastState = state; break; 
						case CYAN_FK		: theirFreeKick();  referee.lastState = state; break; 
						case CYAN_CORNER	: theirCornerKick();referee.lastState = state; break; 	
						case CYAN_THROW		: referee.lastState = state; break;
						case CYAN_REPAIR	: referee.lastState = state; break;

						/* ------------ MAGENTA REFEREE STATE ---------- */
						case MAGENTA_KO		: ourKickOff();		referee.lastState = state; break; 
						case MAGENTA_GK		: ourGoalKick(); 	referee.lastState = state; break; 
						case MAGENTA_PEN	: referee.lastState = state; break; 
						case MAGENTA_FK		: ourFreeKick();    referee.lastState = state; break; 
						case MAGENTA_CORNER	: ourCornerKick();  referee.lastState = state; break; 	
						case MAGENTA_THROW	: referee.lastState = state; break;
						case MAGENTA_REPAIR	: referee.lastState = state; break;
					}

				 break; // end case TEAM_MAGENTA
			} // end switch default

		 break;		
		}	// end default

	}	// end switch utama
}



void callbackAgent1(const ersow_comm::dataAgentMsg& dataAgent1)
{
	agent[ERSOW_OKTO].position[POSX] = dataAgent1.positionx;
	agent[ERSOW_OKTO].position[POSY] = dataAgent1.positiony;
	agent[ERSOW_OKTO].position[POST] = dataAgent1.positiont;

	agent[ERSOW_OKTO].ball_position[POSX] =  dataAgent1.ballposx;
	agent[ERSOW_OKTO].ball_position[POSY] =  dataAgent1.ballposy;
	agent[ERSOW_OKTO].ball_detect = dataAgent1.balldetect;
	agent[ERSOW_OKTO].state = dataAgent1.state;

	agent[ERSOW_OKTO].value = dataAgent1.value;

#ifdef DEBUG_DATA_AGENT1
	
	printf("Agent1 ball posx | ballposy = %d | %d\n", 
		dataAgent1.ballposx, dataAgent1.ballposy);
#endif

}

void callbackAgent2(const ersow_comm::dataAgentMsg& dataAgent2)
{
	// printf("ball %d | %d\n", dataAgent2.ballposx, dataAgent2.ballposy);

	agent[ERSOW_HENDRO].position[POSX] = dataAgent2.positionx;
	agent[ERSOW_HENDRO].position[POSY] = dataAgent2.positiony;
	agent[ERSOW_HENDRO].position[POST] = dataAgent2.positiont;

	agent[ERSOW_HENDRO].ball_position[POSX] = dataAgent2.ballposx;
	agent[ERSOW_HENDRO].ball_position[POSY] = dataAgent2.ballposy;
	agent[ERSOW_HENDRO].ball_detect = dataAgent2.balldetect;
	agent[ERSOW_HENDRO].state = dataAgent2.state;

	agent[ERSOW_HENDRO].value = dataAgent2.value;

#ifdef DEBUG_DATA_AGENT2
	printf("Agent2 state = %d\n", 
		dataAgent2.state);

	printf("pos x | y | t = %d | %d | %d\n",
		agent[ERSOW_HENDRO].position[POSX],
		agent[ERSOW_HENDRO].position[POSY],
		agent[ERSOW_HENDRO].position[POST]);

	printf("Agent2 ball posx | ballposy | detect = %d | %d | %d\n", 
		dataAgent2.ballposx, 
		dataAgent2.ballposy,
		dataAgent2.balldetect);
#endif

}

void callbackAgent3(const ersow_comm::dataAgentMsg& dataAgent3)
{ 
	agent[ERSOW_JAMIL].position[POSX] = dataAgent3.positionx;
	agent[ERSOW_JAMIL].position[POSY] = dataAgent3.positiony;
	agent[ERSOW_JAMIL].position[POST] = dataAgent3.positiont;

	agent[ERSOW_JAMIL].ball_position[POSX] = dataAgent3.ballposx;
	agent[ERSOW_JAMIL].ball_position[POSY] = dataAgent3.ballposy;
	agent[ERSOW_JAMIL].ball_detect = dataAgent3.balldetect;
	agent[ERSOW_JAMIL].state = dataAgent3.state;

	agent[ERSOW_JAMIL].value = dataAgent3.value;
}

void callbackReferee(const ersow_comm::refereeMsg& dataReferee) 
{ 
	referee.state = dataReferee.state; 
	referee.status = dataReferee.connected;
	//printf("referee.state:%d\n",referee.state );
	//printf("referee.status:%d\n",referee.status );
}

void callbackGUI(const ersow_comm::data_gui& msgHandler)
{ 

// int16 gui_key
// int16 expandfield
// int16 refbox
// int16 team

	GUI.gui_key 	= msgHandler.gui_key;
	GUI.expandfield = msgHandler.expandfield;
	GUI.refbox 		= msgHandler.refbox;
	GUI.team 		= msgHandler.team;
	GUI.enableRef	= msgHandler.state_referee;
	GUI.modeskill = msgHandler.modeskill;
	GUI.modestart = msgHandler.modestart;
	GUI.interupt = msgHandler.interupt;
	GUI.dataAll_x = msgHandler.dataAll_x;
	GUI.dataAll_y = msgHandler.dataAll_y;
	GUI.dataAll_0 = msgHandler.dataAll_0;
	GUI.robot_mode = msgHandler.robot_mode;

	strategyErsow.localization_posx = msgHandler.dataAll_x;
	strategyErsow.localization_posy = msgHandler.dataAll_y;
	strategyErsow.localization_post = msgHandler.dataAll_0;	

	strategyErsow.flaglocalization = msgHandler.flagLoc;



	//printf("GUI.dataAll_x:%d GUI.dataAll_y:%d GUI.dataAll_0:%d flagLoc:%d\n", msgHandler.dataAll_x, msgHandler.dataAll_y, msgHandler.dataAll_0, msgHandler.flagLoc );
	

#ifdef DEBUG_GUI
	printf("___________ DEBUG GUI ___________\n");
	printf("GUI key %d\texpandfield %d\trefbox %d\tteam %d\t enableRef %d\n",
		GUI.gui_key,
		GUI.expandfield,
		GUI.refbox,
		GUI.team,
		GUI.enableRef);
#endif

}