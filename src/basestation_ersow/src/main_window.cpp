/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include <QPixmap>
#include <QTimer>
#include <QShortcut>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QPainter>
#include "../include/basestation_ersow/main_window.hpp"
#include "basestation_ersow/data_agent0.h"
#include "basestation_ersow/dataAgentMsg.h"
#include "basestation_ersow/data_gui.h"
#include "basestation_ersow/strategyMsg.h"
#include <math.h>
#define PI          3.14159265
#define DRAW_LINE
#define BALL_DETECT

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace basestation_ersow {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent)
    , qnode(argc,argv)

{
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

    //--------------SET ICON TO COMBOBOX TEAM--------
    ui.comboBox->addItem(QIcon(":images/black_cyan.png"),"Cyan");
    ui.comboBox->addItem(QIcon(":images/black_magenta.png"),"Magenta");

    /* ---------------- Set field picture --------------------*/ 
	pix.load("://images/Lapangan_ORSOW_Regional.png");
	w = ui.label_pict->width();
	h = ui.label_pict->height();
	
	// set a scaled pixmap to a w x h window keeping its aspect ratio 
	ui.label_pict->setPixmap(pix.scaled(w,h,Qt::IgnoreAspectRatio));

    /*-------- set ball pic-------- */
	ball_.load(":images/ball_.png");
	m_wBall = ui.label_ball->width();
	m_hBall = ui.label_ball->height();
	ui.label_ball->setPixmap(ball_.scaled(m_wBall, m_hBall, Qt::KeepAspectRatio));

    /*-------set timer to 0--------*/
	ui.lineEdit_minute->setText(QString::number(0));
	ui.lineEdit_second->setText(QString::number(0));

	/*----set pic Indicator----*/
	pix_stts_jamil.load(":images/led_RED.png");
	w_stts_jamil = ui.label_stts_jamil->width();
    h_stts_jamil = ui.label_stts_jamil->height();
	ui.label_stts_jamil->setPixmap(pix_stts_jamil.scaled(w_stts_jamil, h_stts_jamil, Qt::KeepAspectRatio));

	pix_stts_hendro.load(":images/led_RED.png");
	w_stts_hendro = ui.label_stts_hendro->width();
    h_stts_hendro = ui.label_stts_hendro->height();
	ui.label_stts_hendro->setPixmap(pix_stts_hendro.scaled(w_stts_hendro, h_stts_hendro, Qt::KeepAspectRatio));

	pix_stts_okto.load(":images/led_RED.png");
	w_stts_okto = ui.label_stts_okto->width();
    h_stts_okto = ui.label_stts_okto->height();
	ui.label_stts_okto->setPixmap(pix_stts_okto.scaled(w_stts_okto, h_stts_okto, Qt::KeepAspectRatio));
    
    //set data Jamil to 0
	ui.lineEdit_jamil_state->setText("0");
	ui.lineEdit_jamil_mode->setText("0");
	ui.lineEdit_jamil_posx->setText("0");
	ui.lineEdit_jamil_posy->setText("0");
	ui.lineEdit_jamil_pos0->setText("0");
	ui.lineEdit_jamil_ballx->setText("0");
	ui.lineEdit_jamil_bally->setText("0");
	ui.lineEdit_lost_jamil->setText("0");

	//set data Okto to 0
	ui.lineEdit_okto_state->setText("0");
	ui.lineEdit_okto_mode->setText("0");
	ui.lineEdit_okto_posx->setText("0");
	ui.lineEdit_okto_posy->setText("0");
	ui.lineEdit_okto_pos0->setText("0");
	ui.lineEdit_okto_ballx->setText("0");
	ui.lineEdit_okto_bally->setText("0");
	ui.lineEdit_lost_okto->setText("0");

	//set data Hendro to 0
	ui.lineEdit_hendro_state->setText("0");
	ui.lineEdit_hendro_mode->setText("0");
	ui.lineEdit_hendro_posx->setText("0");
	ui.lineEdit_hendro_posy->setText("0");
	ui.lineEdit_hendro_pos0->setText("0");
	ui.lineEdit_hendro_ballx->setText("0");
	ui.lineEdit_hendro_bally->setText("0");
	ui.lineEdit_lost_hendro->setText("0");

	//set data Posisi Bola to 0
	ui.lineEdit_PosBallX->setText("0");
	ui.lineEdit_PosBallY->setText("0");
	ui.lineEdit_BallDetect->setText("0");

	//set on expandfield
	//ui.checkBox_Expand_Field->setChecked(1);

	//-------set on checkbox referee	
	ui.checkBox_Referee->setChecked(1);

	//status refbox
	ui.lineEdit_Status_Combo_Box->setText("NOT CONNECTED");
	ui.lineEdit_Port_Refbox->setText(QString::number(28097));
	ui.lineEdit_Addr_Refbox->setText("192.168.10.10");

	//------add image magenta to label robot---------
	pix_player_1.load(":/images/black_magenta_1.png");
	pix_player_2.load(":images/black_magenta_2.png");
	pix_player_3.load(":images/black_magenta_3.png");
	//------add image cyan to label robot----------
	pix_player_1.load(":images/black_cyan_1.png");
    pix_player_2.load(":images/black_cyan_2.png");
    pix_player_3.load(":images/black_cyan_3.png");              

    /*-------set pixel robot----------*/
	QTransform bodyTrans1;
    bodyTrans1.rotate(-90);
	QPixmap inrobot1(pix_player_1.transformed(bodyTrans1));
	pxw1 = inrobot1.width(); pxh1 = inrobot1.height();

	w_player_1 = ui.label_pic_player1->width();
	h_player_1 = ui.label_pic_player1->height();
	ui.label_pic_player1->setPixmap(inrobot1.scaled(w_player_1,h_player_1,Qt::KeepAspectRatio));

	QTransform bodyTrans2;
    bodyTrans2.rotate(-90);
	QPixmap inrobot2(pix_player_2.transformed(bodyTrans2));
	pxw2 = inrobot2.width(); pxh2 = inrobot2.height();

	w_player_2 = ui.label_pic_player2->width();
	h_player_2 = ui.label_pic_player2->height();
	ui.label_pic_player2->setPixmap(inrobot2.scaled(w_player_2,h_player_2,Qt::KeepAspectRatio));


	QTransform bodyTrans3;
    bodyTrans3.rotate(-90);
	QPixmap inrobot3(pix_player_3.transformed(bodyTrans3));
	pxw3 = inrobot3.width(); pxh3 = inrobot3.height();

	w_player_3 = ui.label_pic_player3->width();
	h_player_3 = ui.label_pic_player3->height();
	ui.label_pic_player3->setPixmap(inrobot3.scaled(w_player_3,h_player_3,Qt::KeepAspectRatio));

	//------add shortcut key CTRL+C for fast exit------
	new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this, SLOT(close()));


	

}

MainWindow::~MainWindow() {}


void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

void MainWindow::on_pB_close_clicked(){
	close();
}

void MainWindow::on_pB_INITIAL_clicked(bool check) {
	//----check ros master-----
	if ( !qnode.init()) showNoMasterMessage();
	else ui.pB_INITIAL->setEnabled(false);


	/*------------ check expand field 12 x 8 ----------------*/
	if(ui.checkBox_Expand_Field->checkState() == Qt::Unchecked) {
		qnode.data_gui.expandfield = 0;
		qnode.expandfield.x = 0;
		qnode.expandfield.y = 0;
	} else {
		qnode.data_gui.expandfield = 1;
		qnode.expandfield.x = 200;
		qnode.expandfield.y = 300;


	}/*------------ check simulation mode ----------------*/
	if(ui.checkBox_Simulation->checkState() == Qt::Unchecked) {
		simulation=0;
		lock=0;
	} else {
		simulation=1;
		qnode.data_gui.simulation_lock=99;
		qnode.data_gui.ball_set=10;
		qnode.dataRobotOkto.agent0_state_okto = 1;
		qnode.dataRobotJamil.agent0_state_jamil = 1;
		qnode.dataRobotHendro.agent0_state_hendro = 1;
		lock=0;
	}


	/* ------------------ team select ------------------ */
	if(ui.comboBox->currentText() == "Cyan"){
		qnode.data_gui.team = 500;
		pix_player_1.load(":images/black_cyan_1.png");
    	pix_player_2.load(":images/black_cyan_2.png");
    	pix_player_3.load(":images/black_cyan_3.png");
    }
	else {
		qnode.data_gui.team = 600;	
		pix_player_1.load(":images/black_magenta_1.png");
		pix_player_2.load(":images/black_magenta_2.png");
		pix_player_3.load(":images/black_magenta_3.png");
	}

	
	/* -------------- MANUAL / AUTO CALIB ------------------ */
	if(ui.checkBox_Auto_calibration->checkState() == Qt::Unchecked) {
		ui.checkBox_Manual_calibration->setChecked(1);
		qnode.data_gui.setCalibMode = 101; //Manual calib
	} else {

		qnode.data_gui.setCalibMode = 102; //Auto calib
		qnode.data_gui.CalibAutoIn  = ui.lineEdit_distance_CM->text().toInt();
	}

	//-----------set timer--------------
	QTimer *timer = new QTimer(this);
    //timer->setInterval(50);
	
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGui()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDataRobot()));
	
    timer->start(1);

    QTimer *timerSecond = new QTimer(this);
    connect(timerSecond, SIGNAL(timeout()), this, SLOT(updateTimerSecond()));
    timerSecond->start(1000);
}

void MainWindow::on_pB_Connect_to_refbox_clicked() {

	/*---data refbox----*/
	qnode.data_gui.ipaddr 		= ui.lineEdit_Addr_Refbox->text().toStdString();
	qnode.data_gui.port 		= ui.lineEdit_Port_Refbox->text().toInt();

	// printf("qnode.data_gui.ipaddr:%s\n",qnode.data_gui.ipaddr );
	// printf("qnode.data_gui.port:%d\n",qnode.data_gui.port);

	//------------set text to refbox if connect to refbox---------
	ui.lineEdit_Status_Combo_Box->setText("CONNECTED to REFBOX");
	ui.lineEdit_Status_Combo_Box->setStyleSheet("QLineEdit#lineEdit_Status_Combo_Box {color:green}");
}


void MainWindow::updateTimerMinute(){
	static int dataTime = 1;
	ui.lineEdit_minute->setText(QString::number(dataTime));

	dataTime++;
}
void MainWindow::updateTimerSecond(){
	static int data = 0;
	if(data > 58){ 
		updateTimerMinute(); 
		data = 0; 
	}
	else 
		data++;

	ui.lineEdit_second->setText(QString::number(data));
}

/*void MainWindow::paintEvent(QPaintEvent *e){
	QPainter painter(this);

	painter.drawLine(mapy_player(qnode.dataRobotOkto.agent0_posy_okto),mapx_player(qnode.dataRobotOkto.agent0_posx_okto),200,300);
}*/

void MainWindow::updateDataRobot()
{
#ifdef DRAW_LINE
	QPixmap tmp = pix;
	QPainter paint(&tmp);
	
	/*----size map drawLine adjust to GUI*/
	//x awal=1050    x akhir=7300
	//y awal=1050	 y akhir=10500


	/*-----drawLine ROBOT----*/
	/*-----------------------------------*/
	if(qnode.dataRobotOkto.agent0_state_okto==1 || qnode.dataRobotOkto.agent0_state_okto==3){
		QPen linePen1(Qt::red);
		linePen1.setWidth(5);
		linePen1.setStyle(Qt::DotLine);
		paint.setPen(linePen1);
		//drawLine Okto
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto),
					   mapy_line_target(qnode.dataStrategy.targety_agent1), mapx_line_target(qnode.dataStrategy.targetx_agent1));
	}

	/*-----------------------------------*/
	if(qnode.dataRobotHendro.agent0_state_hendro==1 || qnode.dataRobotHendro.agent0_state_hendro==3){
		QPen linePen2(Qt::darkGreen);
		linePen2.setWidth(5);
		linePen2.setStyle(Qt::DotLine);
		paint.setPen(linePen2);
		//drawLine Hendro
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro),
					   mapy_line_target(qnode.dataStrategy.targety_agent2), mapx_line_target(qnode.dataStrategy.targetx_agent2));
	}

	/*-----------------------------------*/
	if(qnode.dataRobotJamil.agent0_state_jamil==1 || qnode.dataRobotJamil.agent0_state_jamil==3){
		QPen linePen3(Qt::blue);
		linePen3.setWidth(5);
		linePen3.setStyle(Qt::DotLine);
		paint.setPen(linePen3);
		//drawLine Jamil
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil),
					   mapy_line_target(qnode.dataStrategy.targety_agent3), mapx_line_target(qnode.dataStrategy.targetx_agent3));
	}

	
#endif

	/*=========DRAW RADIUS SUDUT=============*/
	if(lock == 1 && flag_sudut == 1){
		QPen linePen4(Qt::red);
		linePen4.setWidth(3);
		linePen4.setStyle(Qt::SolidLine);
		paint.setPen(linePen4);
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto-210), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto+210), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto));
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto-210),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto+210));
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto-150), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto+150),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto+150), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto-150));
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto-150), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto-150),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto+150), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto+150));

		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto-80), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto-200),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto+80), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto+200));
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto+80), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto-200),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto-80), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto+200));
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto-200), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto-80),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto+200), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto+80));
		paint.drawLine(mapy_line_player(qnode.dataRobotOkto.agent0_posy_okto+200), mapx_line_player(qnode.dataRobotOkto.agent0_posx_okto-80),
					   mapy_line_target(qnode.dataRobotOkto.agent0_posy_okto-200), mapx_line_target(qnode.dataRobotOkto.agent0_posx_okto+80));
	}
	if(lock == 1 && flag_sudut == 2){
		QPen linePen5(Qt::darkGreen);
		linePen5.setWidth(3);
		linePen5.setStyle(Qt::SolidLine);
		paint.setPen(linePen5);
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro-210), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro+210), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro));
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro-210),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro+210));
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro-150), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro+150),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro+150), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro-150));
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro-150), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro-150),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro+150), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro+150));

		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro-80), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro-200),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro+80), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro+200));
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro+80), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro-200),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro-80), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro+200));
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro-200), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro-80),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro+200), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro+80));
		paint.drawLine(mapy_line_player(qnode.dataRobotHendro.agent0_posy_hendro+200), mapx_line_player(qnode.dataRobotHendro.agent0_posx_hendro-80),
					   mapy_line_target(qnode.dataRobotHendro.agent0_posy_hendro-200), mapx_line_target(qnode.dataRobotHendro.agent0_posx_hendro+80));
	}
	if(lock == 1 && flag_sudut == 3){
		QPen linePen5(Qt::blue);
		linePen5.setWidth(3);
		linePen5.setStyle(Qt::SolidLine);
		paint.setPen(linePen5);
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil-210), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil+210), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil));
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil-210),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil+210));
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil-150), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil+150),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil+150), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil-150));
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil-150), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil-150),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil+150), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil+150));

		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil-80), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil-200),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil+80), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil+200));
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil+80), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil-200),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil-80), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil+200));
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil-200), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil-80),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil+200), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil+80));
		paint.drawLine(mapy_line_player(qnode.dataRobotJamil.agent0_posy_jamil+200), mapx_line_player(qnode.dataRobotJamil.agent0_posx_jamil-80),
					   mapy_line_target(qnode.dataRobotJamil.agent0_posy_jamil-200), mapx_line_target(qnode.dataRobotJamil.agent0_posx_jamil+80));
	}
	//-----------------SET DRAW LINE AND SUDUT TO FIELD----------------------
	ui.label_pict->setPixmap(tmp.scaled(w,h,Qt::IgnoreAspectRatio));

	

	/* ------------ Update data theta robot ----------- */
	QTransform bodyTrans1;
    bodyTrans1.rotate(mapt(qnode.dataRobotJamil.agent0_pos0_jamil+180));
	QPixmap inrobot1(pix_player_1.transformed(bodyTrans1));
	inrobot1 = inrobot1.copy((inrobot1.width() - pxw1)/2, (inrobot1.height() - pxh1)/2, pxw1, pxh1);

	ui.label_pic_player1->setPixmap(inrobot1.scaled(w_player_1,h_player_1, Qt::KeepAspectRatio));

	QTransform bodyTrans2;
    bodyTrans2.rotate(mapt(qnode.dataRobotHendro.agent0_pos0_hendro+180));
	QPixmap inrobot2(pix_player_2.transformed(bodyTrans2));
	inrobot2 = inrobot2.copy((inrobot2.width() - pxw2)/2, (inrobot2.height() - pxh2)/2, pxw2, pxh2);

	ui.label_pic_player2->setPixmap(inrobot2.scaled(w_player_2,h_player_2, Qt::KeepAspectRatio));

	QTransform bodyTrans3;
    bodyTrans3.rotate(mapt(qnode.dataRobotOkto.agent0_pos0_okto+180));
	QPixmap inrobot3(pix_player_3.transformed(bodyTrans3));
	inrobot3 = inrobot3.copy((inrobot3.width() - pxw3)/2, (inrobot3.height() - pxh3)/2, pxw3, pxh3);

	ui.label_pic_player3->setPixmap(inrobot3.scaled(w_player_3,h_player_3, Qt::KeepAspectRatio));

	//---------RESET PIC ROBOT JIKA OFF---------------
	if(qnode.dataRobotOkto.agent0_state_okto==2 || qnode.dataRobotOkto.agent0_state_okto==4 || qnode.dataRobotOkto.agent0_state_okto==0){
		ui.label_pic_player3->setPixmap(inrobot3.scaled(0,0,Qt::KeepAspectRatio));
	}
	if(qnode.dataRobotHendro.agent0_state_hendro==2 || qnode.dataRobotHendro.agent0_state_hendro==4 || qnode.dataRobotHendro.agent0_state_hendro==0){
		ui.label_pic_player2->setPixmap(inrobot2.scaled(0,0,Qt::KeepAspectRatio));
	}
	if(qnode.dataRobotJamil.agent0_state_jamil==2 || qnode.dataRobotJamil.agent0_state_jamil==4 || qnode.dataRobotJamil.agent0_state_jamil==0){
		ui.label_pic_player1->setPixmap(inrobot1.scaled(0,0,Qt::KeepAspectRatio));
	}


	/* ----------------- robot position x and y ----------------------- */
	ui.label_pic_player1->setGeometry(mapy_player(qnode.dataRobotJamil.agent0_posy_jamil),mapx_player(qnode.dataRobotJamil.agent0_posx_jamil), 70, 70);
	ui.label_pic_player2->setGeometry(mapy_player(qnode.dataRobotHendro.agent0_posy_hendro),mapx_player(qnode.dataRobotHendro.agent0_posx_hendro), 70, 70);
	ui.label_pic_player3->setGeometry(mapy_player(qnode.dataRobotOkto.agent0_posy_okto),mapx_player(qnode.dataRobotOkto.agent0_posx_okto), 70, 70);	

	/* ------------------------- Ball position ----------------------- */
	ui.label_ball->setGeometry(mapy_ball(qnode.dataStrategy.global_ballposy), mapx_ball(qnode.dataStrategy.global_ballposx), 30, 30);

	/*------set gui key to 0------*/
	if(qnode.data_gui.interupt != 1){
		qnode.data_gui.gui_key=0;
	}
	
	//-----------RESET CASE KALIBRASI ROBOT-----------------
	if(qnode.dataStrategy.state_agent1 == 11){
		qnode.data_gui.refbox = 801;
	}
	if(qnode.dataStrategy.state_agent2 == 11){
		qnode.data_gui.refbox = 601;
	}
	if(qnode.dataStrategy.state_agent3 == 11){
		qnode.data_gui.refbox = 701;
	}

	//DRWAW LINE
	


}

void MainWindow::updateGui()
{
	//Update Data Jamil ke GUI
	ui.lineEdit_jamil_state->setText(QString::number(qnode.dataRobotJamil.agent0_state_jamil));
	ui.lineEdit_jamil_mode->setText(QString::number(qnode.dataStrategy.mode_agent3));
	ui.lineEdit_jamil_posx->setText(QString::number(qnode.dataRobotJamil.agent0_posx_jamil));
	ui.lineEdit_jamil_posy->setText(QString::number(qnode.dataRobotJamil.agent0_posy_jamil));
	ui.lineEdit_jamil_pos0->setText(QString::number(qnode.dataRobotJamil.agent0_pos0_jamil));
	ui.lineEdit_jamil_ballx->setText(QString::number(qnode.dataRobotJamil.agent0_ballx_jamil));
	ui.lineEdit_jamil_bally->setText(QString::number(qnode.dataRobotJamil.agent0_bally_jamil));
	ui.lineEdit_lost_jamil->setText(QString::number(qnode.dataRobotJamil.agent0_lostPackets_jamil));

	//Update Data Okto ke GUI
	ui.lineEdit_okto_state->setText(QString::number(qnode.dataRobotOkto.agent0_state_okto));
	ui.lineEdit_okto_mode->setText(QString::number(qnode.dataStrategy.mode_agent1));
	ui.lineEdit_okto_posx->setText(QString::number(qnode.dataRobotOkto.agent0_posx_okto));
	ui.lineEdit_okto_posy->setText(QString::number(qnode.dataRobotOkto.agent0_posy_okto));
	ui.lineEdit_okto_pos0->setText(QString::number(qnode.dataRobotOkto.agent0_pos0_okto));
	ui.lineEdit_okto_ballx->setText(QString::number(qnode.dataRobotOkto.agent0_ballx_okto));
	ui.lineEdit_okto_bally->setText(QString::number(qnode.dataRobotOkto.agent0_bally_okto));
	ui.lineEdit_lost_okto->setText(QString::number(qnode.dataRobotOkto.agent0_lostPackets_okto));

	//Update Data Hendro ke GUI
	ui.lineEdit_hendro_state->setText(QString::number(qnode.dataRobotHendro.agent0_state_hendro));
	ui.lineEdit_hendro_mode->setText(QString::number(qnode.dataStrategy.mode_agent2));
	ui.lineEdit_hendro_posx->setText(QString::number(qnode.dataRobotHendro.agent0_posx_hendro));
	ui.lineEdit_hendro_posy->setText(QString::number(qnode.dataRobotHendro.agent0_posy_hendro));
	ui.lineEdit_hendro_pos0->setText(QString::number(qnode.dataRobotHendro.agent0_pos0_hendro));
	ui.lineEdit_hendro_ballx->setText(QString::number(qnode.dataRobotHendro.agent0_ballx_hendro));
	ui.lineEdit_hendro_bally->setText(QString::number(qnode.dataRobotHendro.agent0_bally_hendro));
	ui.lineEdit_lost_hendro->setText(QString::number(qnode.dataRobotHendro.agent0_lostPackets_hendro));

	//Update Data Posisi Bola
	ui.lineEdit_PosBallX->setText(QString::number(qnode.dataStrategy.global_ballposx));
	ui.lineEdit_PosBallY->setText(QString::number(qnode.dataStrategy.global_ballposy));
	ui.lineEdit_BallDetect->setText(QString::number(qnode.dataStrategy.ball_detect));

	#ifdef BALL_DETECT
	//-----------set ball if ball detected-------------
	if(simulation != 1){
		if(qnode.dataStrategy.ball_detect == 0){
			ball_.load(":images/ball_.png");
			ui.label_ball->setPixmap(ball_.scaled(0, 0, Qt::KeepAspectRatio));	
		}
		else{
			ball_.load(":images/ball_.png");
			m_wBall = ui.label_ball->width();
			m_hBall = ui.label_ball->height();
			ui.label_ball->setPixmap(ball_.scaled(m_wBall, m_hBall, Qt::KeepAspectRatio));
		}
	}
	#endif

	
	//---------check state referee-----------
	if(ui.checkBox_Referee->checkState() == Qt::Unchecked){
		qnode.data_gui.state_referee=0;
		//printf("qnode.data_gui.state_referee:%d\n",qnode.data_gui.state_referee );
	}
	else{
		qnode.data_gui.state_referee=1;
		//printf("qnode.data_gui.state_referee:%d\n",qnode.data_gui.state_referee );
	}

	/*---------check status robot with indicator-------*/
	if(qnode.dataRobotJamil.agent0_state_jamil==1 || qnode.dataRobotJamil.agent0_state_jamil==3){
		pix_stts_jamil.load(":images/led_GREEN.png");
    	w_stts_jamil = ui.label_stts_jamil->width();
    	h_stts_jamil = ui.label_stts_jamil->height();
    	ui.label_stts_jamil->setPixmap(pix_stts_jamil.scaled(w_stts_jamil, h_stts_jamil, Qt::KeepAspectRatio));
	}else{
		pix_stts_jamil.load(":images/led_RED.png");
		w_stts_jamil = ui.label_stts_jamil->width();
    	h_stts_jamil = ui.label_stts_jamil->height();
    	ui.label_stts_jamil->setPixmap(pix_stts_jamil.scaled(w_stts_jamil, h_stts_jamil, Qt::KeepAspectRatio));
	}

	if(qnode.dataRobotHendro.agent0_state_hendro==1 || qnode.dataRobotHendro.agent0_state_hendro==3){
		pix_stts_hendro.load(":images/led_GREEN.png");
    	w_stts_hendro = ui.label_stts_hendro->width();
    	h_stts_hendro = ui.label_stts_hendro->height();
    	ui.label_stts_hendro->setPixmap(pix_stts_hendro.scaled(w_stts_hendro, h_stts_hendro, Qt::KeepAspectRatio));
	}else{
		pix_stts_hendro.load(":images/led_RED.png");
		w_stts_hendro = ui.label_stts_hendro->width();
    	h_stts_hendro = ui.label_stts_hendro->height();
    	ui.label_stts_hendro->setPixmap(pix_stts_hendro.scaled(w_stts_hendro, h_stts_hendro, Qt::KeepAspectRatio));
	}

	if(qnode.dataRobotOkto.agent0_state_okto==1 || qnode.dataRobotOkto.agent0_state_okto==3){
		pix_stts_okto.load(":images/led_GREEN.png");
    	w_stts_okto = ui.label_stts_okto->width();
    	h_stts_okto = ui.label_stts_okto->height();
    	ui.label_stts_okto->setPixmap(pix_stts_okto.scaled(w_stts_okto, h_stts_okto, Qt::KeepAspectRatio));
	}else{
		pix_stts_okto.load(":images/led_RED.png");
		w_stts_okto = ui.label_stts_okto->width();
    	h_stts_okto = ui.label_stts_okto->height();
    	ui.label_stts_okto->setPixmap(pix_stts_okto.scaled(w_stts_okto, h_stts_okto, Qt::KeepAspectRatio));
	}
}


/*----size map drawLine adjust to GUI*/
	//x awal=115    x akhir=775
	//y awal=115	 y akhir=1115
int MainWindow::mapy_line_player(int val){
	return ((val - 0) * (1115 - 115) / ((900+qnode.expandfield.y) - 0) + 115);
}
int MainWindow::mapx_line_player(int val){
	return ((val - 0) * (775 - 115) / ((600+qnode.expandfield.x) - 0) + 115);
}

int MainWindow::mapy_line_target(int val){
	return ((val - 0) * (1115 - 115) / ((900+qnode.expandfield.y) - 0) + 115);
}
int MainWindow::mapx_line_target(int val){
	return ((val - 0) * (775 - 115) / ((600+qnode.expandfield.x) - 0) + 115);
}

/*------------- Localization, assumption: realx 900Cm, realy 600 Cm ------------- */
int MainWindow::mapx_player(int val) 
{ 
	if (val<(-100)){
		return ((-100 - 0) * (700 - 100) / ((600+qnode.expandfield.x) - 0) + 100)-35;
	}
	else if (val>700+qnode.expandfield.x){
		return ((700+qnode.expandfield.x - 0) * (700 - 100) / ((600+qnode.expandfield.x) - 0) + 100)-35; 
	}else
		return ((val - 0) * (700 - 100) / ((600+qnode.expandfield.x) - 0) + 100)-35; 
}

int MainWindow::mapy_player(int val) 
{ 	
	if (val<(-50)){
		return ((-50 - 0) * (1390 - 530) / ((900+qnode.expandfield.y) - 0) + 530)-35; 
	}
	else if (val>950+qnode.expandfield.y){
		return ((950+qnode.expandfield.y - 0) * (1390 - 530) / ((900+qnode.expandfield.y) - 0) + 530)-35; 
	}else
		return ((val - 0) * (1390 - 530) / ((900+qnode.expandfield.y) - 0) + 530)-35; 	 
}

int MainWindow::mapt(int val) 
{
		if(val == 0) return 90;
	    else if(val < 0) return (90 - val);
    	else if(val > 0) return (90 + (-1*val));	
}

/*------------- Localization, ball ------------- */
int MainWindow::mapx_ball(int val)
{
	return ((val - 0) * (700 - 100) / ((600+qnode.expandfield.x) - 0) + 100)-15; 
}

int MainWindow::mapy_ball(int val)
{
	return ((val - 0) * (1390 - 530) / ((900+qnode.expandfield.y) - 0) + 530)-15; 	 
}

/*---------change pixel to size real----------*/
int MainWindow::mapxToReal(int val){ 
	return (val - 530) * ((900 + qnode.expandfield.y) - 0) / (1390 - 530) + 0; 
}
int MainWindow::mapyToReal(int val){ 
	return (val - 100) * ((600 + qnode.expandfield.x) - 0) / (700 - 100) + 0; 
}


void MainWindow::on_pB_DISCONNECT_clicked(){
	qnode.data_gui.refbox_status=0;
}

/*---------Manual Control Jamil--------*/
void MainWindow::on_pB_Jamil_Left_clicked(){
	qnode.data_gui.refbox=912;  
	if(simulation == 1){qnode.dataRobotJamil.agent0_posy_jamil--;}
}
void MainWindow::on_pB_Jamil_Right_clicked(){
	qnode.data_gui.refbox=911;
	if(simulation == 1){qnode.dataRobotJamil.agent0_posy_jamil++;}
}
void MainWindow::on_pB_Jamil_Up_clicked(){
	qnode.data_gui.refbox=913;
	if(simulation == 1){qnode.dataRobotJamil.agent0_posx_jamil--;}
}
void MainWindow::on_pB_Jamil_Down_clicked(){
	qnode.data_gui.refbox=914;
	if(simulation == 1){qnode.dataRobotJamil.agent0_posx_jamil++;}
}

/*---------Manual Control Hendro--------*/
void MainWindow::on_pB_Hendro_Left_clicked(){
	qnode.data_gui.refbox=922;
	if(simulation == 1){qnode.dataRobotHendro.agent0_posy_hendro--;}
}
void MainWindow::on_pB_Hendro_Right_clicked(){
	qnode.data_gui.refbox=921;
	if(simulation == 1){qnode.dataRobotHendro.agent0_posy_hendro++;}
}
void MainWindow::on_pB_Hendro_Up_clicked(){
	qnode.data_gui.refbox=923;
	if(simulation == 1){qnode.dataRobotHendro.agent0_posx_hendro--;}
}
void MainWindow::on_pB_Hendro_Down_clicked(){
	qnode.data_gui.refbox=924;
	if(simulation == 1){qnode.dataRobotHendro.agent0_posx_hendro++;}
}

/*---------Manual Control Okto--------*/
void MainWindow::on_pB_Okto_Left_clicked(){
	qnode.data_gui.refbox=932;
	if(simulation == 1){qnode.dataRobotOkto.agent0_posy_okto--;}
}
void MainWindow::on_pB_Okto_Right_clicked(){
	qnode.data_gui.refbox=921;
	if(simulation == 1){qnode.dataRobotOkto.agent0_posy_okto++;}
}
void MainWindow::on_pB_Okto_Up_clicked(){
	qnode.data_gui.refbox=923;
	if(simulation == 1){qnode.dataRobotOkto.agent0_posx_okto--;}
}
void MainWindow::on_pB_Okto_Down_clicked(){
	qnode.data_gui.refbox=924;
	if(simulation == 1){qnode.dataRobotOkto.agent0_posx_okto++;}
}
	
void MainWindow::keyPressEvent(QKeyEvent *event){

	switch(event->key()) {
		//ManualKey=9

		/*-----robot----*/
		//okto=1
		//hendro=2
		//jamil=3

		/*-----intruksi----*/
		//kanan=1
		//kiri=2
		//atas=3
		//bawah=4
		//putarkiri=5
		//putarkanan=6

		/*------FORMAT:  ManualKey-robot-intruksi*/
		case Qt::Key_D: qnode.data_gui.refbox=911; qnode.data_gui.interupt = 1; //Okto kanan
			if(simulation == 1){qnode.dataRobotOkto.agent0_posy_okto++;}
			break;
		case Qt::Key_A: qnode.data_gui.refbox=912; qnode.data_gui.interupt = 1; //Okto kiri
			if(simulation == 1){qnode.dataRobotOkto.agent0_posy_okto--;}
			break;
		case Qt::Key_W: qnode.data_gui.refbox=913; qnode.data_gui.interupt = 1; //Okto atas
			if(simulation == 1){qnode.dataRobotOkto.agent0_posx_okto--;}
			break;
		case Qt::Key_S: qnode.data_gui.refbox=914; qnode.data_gui.interupt = 1; //Okto bawah
			if(simulation == 1){qnode.dataRobotOkto.agent0_posx_okto++;}
			break;
		case Qt::Key_Q: qnode.data_gui.refbox=915; qnode.data_gui.interupt = 1; //Okto putar kiri
			if(simulation == 1){qnode.dataRobotOkto.agent0_pos0_okto++;}
			break;
		case Qt::Key_E: qnode.data_gui.refbox=916; qnode.data_gui.interupt = 1; //Okto putar kanan
			if(simulation == 1){qnode.dataRobotOkto.agent0_pos0_okto--;}
			break;

		//Manual Control Hendro+Key
		case Qt::Key_L: qnode.data_gui.refbox=921; qnode.data_gui.interupt = 1; //Hendro kanan
			if(simulation == 1){qnode.dataRobotHendro.agent0_posy_hendro++;}
			break;
		case Qt::Key_J: qnode.data_gui.refbox=922; qnode.data_gui.interupt = 1;//Hendro kiri
			if(simulation == 1){qnode.dataRobotHendro.agent0_posy_hendro--;}
			break;
		case Qt::Key_I: qnode.data_gui.refbox=923; qnode.data_gui.interupt = 1; //Hendro atas
			if(simulation == 1){qnode.dataRobotHendro.agent0_posx_hendro--;}
			break;
		case Qt::Key_K: qnode.data_gui.refbox=924; qnode.data_gui.interupt = 1; //Hendro bawah
			if(simulation == 1){qnode.dataRobotHendro.agent0_posx_hendro++;}
			break;
		case Qt::Key_U: qnode.data_gui.refbox=925; qnode.data_gui.interupt = 1; //Hendro putar kiri
			if(simulation == 1){qnode.dataRobotHendro.agent0_pos0_hendro++;}
			break;
		case Qt::Key_O: qnode.data_gui.refbox=926; qnode.data_gui.interupt = 1; //Hendro putar kanan
			if(simulation == 1){qnode.dataRobotHendro.agent0_pos0_hendro--;}
			break;

		//Manual Control Jamil+Key
		case Qt::Key_H: qnode.data_gui.refbox=931; qnode.data_gui.interupt = 1; //Jamil kanan
			if(simulation == 1){qnode.dataRobotJamil.agent0_posy_jamil++;}
			break;
		case Qt::Key_F: qnode.data_gui.refbox=932; qnode.data_gui.interupt = 1; //Jamil kiri
			if(simulation == 1){qnode.dataRobotJamil.agent0_posy_jamil--;}
			break;
		case Qt::Key_T: qnode.data_gui.refbox=933; qnode.data_gui.interupt = 1; //Jamil atas
			if(simulation == 1){qnode.dataRobotJamil.agent0_posx_jamil--;}
			break;
		case Qt::Key_G: qnode.data_gui.refbox=934; qnode.data_gui.interupt = 1; //Jamil bawah
			if(simulation == 1){qnode.dataRobotJamil.agent0_posx_jamil++;}
			break;
		case Qt::Key_R: qnode.data_gui.refbox=935; qnode.data_gui.interupt = 1; //Jamil putar kiri
			if(simulation == 1){qnode.dataRobotJamil.agent0_pos0_jamil++;}
			break;
		case Qt::Key_Y: qnode.data_gui.refbox=936; qnode.data_gui.interupt = 1; //Jamil putar kanan
			if(simulation == 1){qnode.dataRobotJamil.agent0_pos0_jamil--;}
			break;

		//--------key localization x y okto--------
		case Qt::Key_Z: flag_sudut = 1;
			if(lock==1){
				robovar1_flagLoc=4; robovar2_flagLoc=0; robovar3_flagLoc=0;
			}else{
				robovar1_flagLoc=1; robovar2_flagLoc=0; robovar3_flagLoc=0;
			}			 
			break;

		//--------key localization x y hendro------
		case Qt::Key_X: flag_sudut = 2;
			if(lock==1){
				robovar1_flagLoc=0; robovar2_flagLoc=5; robovar3_flagLoc=0;
			}else{
				robovar1_flagLoc=0; robovar2_flagLoc=2; robovar3_flagLoc=0;
			}
			break;

		//--------key localization x y jamil------
		case Qt::Key_C:  flag_sudut = 3;
			if(lock==1){
				robovar1_flagLoc=0; robovar2_flagLoc=0; robovar3_flagLoc=6;
			}else{
				robovar1_flagLoc=0; robovar2_flagLoc=0; robovar3_flagLoc=3;
			}
			break;
		//--------key localization theta------	
		case Qt::Key_Shift: lock=1;
			break;
		//--------key reset----------
		case Qt::Key_Escape: qnode.data_gui.refbox=122;  qnode.data_gui.flagLoc = 0; 
			if(simulation==1){
				qnode.data_gui.simulation_lock=99;
			}
			break;
		case Qt::Key_PageDown: 
			if(simulation==1){
				qnode.data_gui.ball_set=10;
			}
			break;
		case Qt::Key_PageUp: 
			if(simulation==1){
				qnode.data_gui.ball_set=20;
			} 
			break;
		//-------get interupt from referee--------
		case Qt::Key_Control: qnode.data_gui.gui_key = 69; qnode.data_gui.interupt = 1;
			break;
		//-------close interupt from referee--------
		case Qt::Key_M: qnode.data_gui.interupt = 0; qnode.data_gui.refbox = 0;
			break;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
	switch(event->key()){

		case Qt::Key_Z: robovar1_flagLoc=0; qnode.data_gui.flagLoc=0; flag_sudut = 0; break;
		case Qt::Key_X: robovar2_flagLoc=0; qnode.data_gui.flagLoc=0; flag_sudut = 0; break;
		case Qt::Key_C: robovar3_flagLoc=0; qnode.data_gui.flagLoc=0; flag_sudut = 0; break;

		case Qt::Key_Shift: lock=0; flag_sudut = 0; break;

		case Qt::Key_D: qnode.data_gui.refbox=801; break;
		case Qt::Key_A: qnode.data_gui.refbox=801; break;
		case Qt::Key_W: qnode.data_gui.refbox=801; break;
		case Qt::Key_S: qnode.data_gui.refbox=801; break;
		case Qt::Key_Q: qnode.data_gui.refbox=801; break;
		case Qt::Key_E: qnode.data_gui.refbox=801; break;

		case Qt::Key_L: qnode.data_gui.refbox=601; break;
		case Qt::Key_J: qnode.data_gui.refbox=601; break;
		case Qt::Key_I: qnode.data_gui.refbox=601; break;
		case Qt::Key_K: qnode.data_gui.refbox=601; break;
		case Qt::Key_U: qnode.data_gui.refbox=601; break;
		case Qt::Key_O: qnode.data_gui.refbox=601; break;

		case Qt::Key_H: qnode.data_gui.refbox=701; break;
		case Qt::Key_F: qnode.data_gui.refbox=701; break;
		case Qt::Key_T: qnode.data_gui.refbox=701; break;
		case Qt::Key_G: qnode.data_gui.refbox=701; break;
		case Qt::Key_R: qnode.data_gui.refbox=701; break;
		case Qt::Key_Y: qnode.data_gui.refbox=701; break;
		break;
	}
}
/*----------Mode--------*/
//CYAN=4
//MAGENTA=5

/*--------Intruksi------*/
//Goal=1
//Kickoff=2
//Goal_kick=3
//Throw_in=4
//Corner=5
//Free_kick=6
//Penalty=7
//Repair=8

/*---------Format----------*/
//Mode-Intruksi

/*---------------RefBox CYAN---------------*/
void MainWindow::on_pB_CYAN_Goal_clicked(){
	qnode.data_gui.refbox=65;
}
void MainWindow::on_pB_CYAN_Kick_off_clicked(){
	qnode.data_gui.refbox=75;
	if(simulation==1){
		qnode.data_gui.simulation_lock=1;
	}
}
void MainWindow::on_pB_CYAN_Goal_kick_clicked(){
	qnode.data_gui.refbox=71;
	if(simulation==1){
		qnode.data_gui.simulation_lock=2;
	}
}
void MainWindow::on_pB_CYAN_Throw_in_clicked(){
	qnode.data_gui.refbox=84;
}
void MainWindow::on_pB_CYAN_Corner_clicked(){
	qnode.data_gui.refbox=67;
	if(simulation==1){
		qnode.data_gui.simulation_lock=3;
	}
}
void MainWindow::on_pB_CYAN_Free_kick_clicked(){
	qnode.data_gui.refbox=70;
	if(simulation==1){
		qnode.data_gui.simulation_lock=4;
	}
}
void MainWindow::on_pB_CYAN_Penalty_clicked(){
	qnode.data_gui.refbox=80;
}
void MainWindow::on_pB_CYAN_Repair_clicked(){
	qnode.data_gui.refbox=79;
}

/*---------------RefBox MAGENTA---------------*/
void MainWindow::on_pB_MAGENTA_Goal_clicked(){
	qnode.data_gui.refbox=97;
}
void MainWindow::on_pB_MAGENTA_Kick_off_clicked(){
	qnode.data_gui.refbox=107;
	if(simulation==1){
		qnode.data_gui.simulation_lock=11;
	}
}
void MainWindow::on_pB_MAGENTA_Goal_kick_clicked(){
	qnode.data_gui.refbox=103;
	if(simulation==1){
		qnode.data_gui.simulation_lock=22;
	}
}
void MainWindow::on_pB_MAGENTA_Throw_in_clicked(){
	qnode.data_gui.refbox=116;
}
void MainWindow::on_pB_MAGENTA_Corner_clicked(){
	qnode.data_gui.refbox=99;
	if(simulation==1){
		qnode.data_gui.simulation_lock=33;
	}
}
void MainWindow::on_pB_MAGENTA_Free_kick_clicked(){
	qnode.data_gui.refbox=102;
	if(simulation==1){
		qnode.data_gui.simulation_lock=44;
	}
}
void MainWindow::on_pB_MAGENTA_Penalty_clicked(){
	qnode.data_gui.refbox=112;
}
void MainWindow::on_pB_MAGENTA_Repair_clicked(){
	qnode.data_gui.refbox=111;
}

/*----------Ref Command---------*/
void MainWindow::on_pB_RC_Start_clicked(){
	qnode.data_gui.refbox=115;
}
void MainWindow::on_pB_RC_Stop_clicked(){
	qnode.data_gui.refbox=83;
}
void MainWindow::on_pB_RC_Drop_Ball_clicked(){
	qnode.data_gui.refbox=78;
	if(simulation==1){
		qnode.data_gui.simulation_lock=63;
	}
}
void MainWindow::on_pB_RC_Park_clicked(){
	qnode.data_gui.refbox=76;
	if(simulation==1){
		qnode.data_gui.simulation_lock=64;
	}
}
void MainWindow::on_pB_RC_End_Part_clicked(){
	qnode.data_gui.refbox=104;
}
void MainWindow::on_pB_RC_Reset_clicked(){
	qnode.data_gui.refbox=102;
}
void MainWindow::on_pB_RC_End_Game_clicked(){
	qnode.data_gui.refbox=67;
}
void MainWindow::on_pB_RC_Reset_Localref_clicked(){
	qnode.data_gui.refbox=68;
}

/*-----------Jamil Command----------*/
void MainWindow::on_pB_Jamil_Reset_clicked(){
	qnode.data_gui.refbox=531;
}
void MainWindow::on_pB_Jamil_Kalib_Full_clicked(){
	qnode.data_gui.refbox=532;
}
void MainWindow::on_pB_Jamil_Kalib_X_clicked(){
	qnode.data_gui.refbox=533;
}
void MainWindow::on_pB_Jamil_Kalib_Y_clicked(){
	qnode.data_gui.refbox=534;
}
void MainWindow::on_pB_Jamil_Kick_clicked(){
	qnode.data_gui.refbox=535;
}
void MainWindow::on_pB_Jamil_Dribbler_clicked(){
	qnode.data_gui.refbox=536;
}

/*-----------Hendro Command----------*/
void MainWindow::on_pB_Hendro_Reset_clicked(){
	qnode.data_gui.refbox=521;
}
void MainWindow::on_pB_Hendro_Kalib_Full_clicked(){
	qnode.data_gui.refbox=522;
}
void MainWindow::on_pB_Hendro_Kalib_X_clicked(){
	qnode.data_gui.refbox=523;
}
void MainWindow::on_pB_Hendro_Kalib_Y_clicked(){
	qnode.data_gui.refbox=524;
}
void MainWindow::on_pB_Hendro_Kick_clicked(){
	qnode.data_gui.refbox=525;
}
void MainWindow::on_pB_Hendro_Dribbler_clicked(){
	qnode.data_gui.refbox=526;
}

/*-----------Okto Command----------*/
void MainWindow::on_pB_Okto_Reset_clicked(){
	qnode.data_gui.refbox=511;
}
void MainWindow::on_pB_Okto_Kalib_Full_clicked(){
	qnode.data_gui.refbox=512;
}
void MainWindow::on_pB_Okto_Kalib_X_clicked(){
	qnode.data_gui.refbox=513;
}
void MainWindow::on_pB_Okto_Kalib_Y_clicked(){
	qnode.data_gui.refbox=514;
}
void MainWindow::on_pB_Okto_Kick_clicked(){
	qnode.data_gui.refbox=515;
}
void MainWindow::on_pB_Okto_Dribbler_clicked(){
	qnode.data_gui.refbox=516;
}

/*-------Mode Robot------*/
void MainWindow::on_pB_Mode_Direct_clicked(){
	qnode.data_gui.robot_mode=11;
}
void MainWindow::on_pB_Mode_Pass_clicked(){
	qnode.data_gui.robot_mode=12;
}
void MainWindow::on_pB_Ori_Strategy_clicked(){
	qnode.data_gui.robot_mode=13;
}
void MainWindow::on_pB_Def_Strategy_clicked(){
	qnode.data_gui.robot_mode=14;
}
void MainWindow::on_pB_JUST_SHOOT_clicked(){
	qnode.data_gui.robot_mode=15;
}
void MainWindow::on_pB_SKILL_OBS_clicked(){
	qnode.data_gui.robot_mode=16;
}

void MainWindow::mousePressEvent(QMouseEvent *ev){
	//-------mouse click for x y okto---------
	if(robovar1_flagLoc==1 && lock==0 ){
		if(simulation==1){
			qnode.dataRobotOkto.agent0_posx_okto = mapyToReal(ev->y());
			qnode.dataRobotOkto.agent0_posy_okto = mapxToReal(ev->x());
		}
		else{
			qnode.data_gui.dataAll_x = mapyToReal(ev->y());
			qnode.data_gui.dataAll_y = mapxToReal(ev->x());
			qnode.data_gui.dataAll_0 = qnode.dataRobotOkto.agent0_pos0_okto;
			qnode.data_gui.flagLoc = 1;
		}
		
	}
	//-------mouse click for x y hendro---------
	else if(robovar2_flagLoc==2 && lock==0 ){
		if(simulation==1){
			qnode.dataRobotHendro.agent0_posx_hendro = mapyToReal(ev->y());
			qnode.dataRobotHendro.agent0_posy_hendro = mapxToReal(ev->x());
		}
		else{
			qnode.data_gui.dataAll_x = mapyToReal(ev->y());
			qnode.data_gui.dataAll_y = mapxToReal(ev->x());
			qnode.data_gui.dataAll_0 = qnode.dataRobotHendro.agent0_pos0_hendro;
			qnode.data_gui.flagLoc = 2;
			printf("Hendro dataAll_x:%d\n",qnode.data_gui.dataAll_x);
			printf("Hendro dataAll_y:%d\n",qnode.data_gui.dataAll_y);
			printf("data_gui.flagLoc:%d\n",qnode.data_gui.flagLoc );
		}

	}
	//-------mouse click for x y jamil---------
	else if(robovar3_flagLoc==3 && lock==0){
		if(simulation==1){
			qnode.dataRobotJamil.agent0_posx_jamil = mapyToReal(ev->y());
			qnode.dataRobotJamil.agent0_posy_jamil = mapxToReal(ev->x());
		}
		else{
			qnode.data_gui.dataAll_x = mapyToReal(ev->y());
			qnode.data_gui.dataAll_y = mapxToReal(ev->x());
			qnode.data_gui.dataAll_0 = qnode.dataRobotJamil.agent0_pos0_jamil;
			qnode.data_gui.flagLoc = 3;
		}
		
	}	
	//-------mouse click for theta okto---------
	else if(robovar1_flagLoc==4 && lock==1){
		locposx = ev->x() - 35; 
		locposy = ev->y() - 40;
		// distance between 2 nodes
		m_dist = sqrt(pow(locposx - mapy_player(qnode.dataRobotOkto.agent0_posy_okto), 2) + pow(locposy - mapx_player(qnode.dataRobotOkto.agent0_posx_okto), 2));
						
		// angle 
		float deltaX = locposx - mapy_player(qnode.dataRobotOkto.agent0_posy_okto);
		float deltaY = locposy - mapx_player(qnode.dataRobotOkto.agent0_posx_okto);

		double x = atan(deltaX/deltaY);
		double val = 180.0 / PI;

		if(deltaX > 0) {
			if(deltaY > 0 )  	m_angle 	= x * val;  
			else if(deltaY < 0) m_angle 	= 180 + (x * val);
		} 	
		else if(deltaX < 0) {
			if(deltaY < 0)  	m_angle 	= 180 + (x * val);
			else if(deltaY > 0) m_angle 	= 360 + (x * val);
					
		}

		// data heading yang akan dikirim adalah variable valSend
		m_valSend = m_angle - 90;
		if(m_valSend > 180) m_valSend -= 360; 
		else m_valSend = m_valSend;

		if(simulation==1){
			qnode.dataRobotOkto.agent0_pos0_okto=m_valSend;
		}
		else{
			qnode.data_gui.dataAll_x = qnode.dataRobotOkto.agent0_posx_okto;
			qnode.data_gui.dataAll_y = qnode.dataRobotOkto.agent0_posy_okto;
			qnode.data_gui.dataAll_0 = m_valSend;
			qnode.data_gui.flagLoc = 4;
			printf("dataAll_0: %d\n",qnode.data_gui.dataAll_0 );
			printf("data_gui.flagLoc:%d\n",qnode.data_gui.flagLoc );
		}
							
	} 
	//-------mouse click for theta hendro---------
	else if(robovar2_flagLoc == 5 && lock==1){
		locposx = ev->x() - 35; 
		locposy = ev->y() - 40;
		// distance between 2 nodes
		m_dist = sqrt(pow(locposx - mapy_player(qnode.dataRobotHendro.agent0_posy_hendro), 2) + pow(locposy - mapx_player(qnode.dataRobotHendro.agent0_posx_hendro), 2));
						
		// angle 
		float deltaX = locposx - mapy_player(qnode.dataRobotHendro.agent0_posy_hendro);
		float deltaY = locposy - mapx_player(qnode.dataRobotHendro.agent0_posx_hendro);

		double x = atan(deltaX/deltaY);
		double val = 180.0 / PI;

		if(deltaX > 0) {
			if(deltaY > 0 )  	m_angle 	= x * val;  
			else if(deltaY < 0) m_angle 	= 180 + (x * val);
		} 	
		else if(deltaX < 0) {
			if(deltaY < 0)  	m_angle 	= 180 + (x * val);
			else if(deltaY > 0) m_angle 	= 360 + (x * val);			
		}

		// data heading yang akan dikirim adalah variable valSend
		m_valSend = m_angle - 90;
		if(m_valSend > 180) m_valSend -= 360; 
		else m_valSend = m_valSend;

		if(simulation==1){
			qnode.dataRobotHendro.agent0_pos0_hendro=m_valSend;
		}
		else{
			qnode.data_gui.dataAll_x = qnode.dataRobotHendro.agent0_posx_hendro;
			qnode.data_gui.dataAll_y = qnode.dataRobotHendro.agent0_posy_hendro;
			qnode.data_gui.dataAll_0 = m_valSend;
			qnode.data_gui.flagLoc = 5;
		}
	}
	//-------mouse click for theta jamil---------
	else if(robovar3_flagLoc == 6 && lock==1){
		locposx = ev->x() - 35; 
		locposy = ev->y() - 40;
		// distance between 2 nodes
		m_dist = sqrt(pow(locposx - mapy_player(qnode.dataRobotJamil.agent0_posy_jamil), 2) + pow(locposy - mapx_player(qnode.dataRobotJamil.agent0_posx_jamil), 2));
						
		// angle 
		float deltaX = locposx - mapy_player(qnode.dataRobotJamil.agent0_posy_jamil);
		float deltaY = locposy - mapx_player(qnode.dataRobotJamil.agent0_posx_jamil);

		double x = atan(deltaX/deltaY);
		double val = 180.0 / PI;

		if(deltaX > 0) {
			if(deltaY > 0 )  	m_angle 	= x * val;  
			else if(deltaY < 0) m_angle 	= 180 + (x * val);
		} 	
		else if(deltaX < 0) {
			if(deltaY < 0)  	m_angle 	= 180 + (x * val);
			else if(deltaY > 0) m_angle 	= 360 + (x * val);
					
		}

		// data heading yang akan dikirim adalah variable valSend
		m_valSend = m_angle - 90;
		if(m_valSend > 180) m_valSend -= 360; 
		else m_valSend = m_valSend;

		if(simulation==1){
			qnode.dataRobotJamil.agent0_pos0_jamil=m_valSend;
		}
		else{
			qnode.data_gui.dataAll_x = qnode.dataRobotJamil.agent0_posx_jamil;
			qnode.data_gui.dataAll_y = qnode.dataRobotJamil.agent0_posy_jamil;
			qnode.data_gui.dataAll_0 = m_valSend;
			qnode.data_gui.flagLoc = 6;
		}
	
	}
}
}  // namespace basestation_ersow

