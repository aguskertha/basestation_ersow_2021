/**
 * @file /include/basestation_ersow/main_window.hpp
 *
 * @brief Qt based gui for basestation_ersow.
 *
 * @date November 2010
 **/
#ifndef basestation_ersow_MAIN_WINDOW_H
#define basestation_ersow_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace basestation_ersow {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();
	//void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

	/*---------- Variable cadangan ------------- */
	QPixmap myPixmap;
	QPixmap pix_player_1;
	QPixmap pix_player_2;
	QPixmap pix_player_3;
	QPixmap pix_stts_jamil;
	QPixmap pix_stts_hendro;
	QPixmap pix_stts_okto;
	QPixmap ball_;
	QPixmap pix;	

	int h_player_1, h_player_2, h_player_3, m_hBall, h_stts_jamil, h_stts_hendro, h_stts_okto, h;
	int w_player_1, w_player_2, w_player_3, m_wBall, w_stts_jamil, w_stts_hendro, w_stts_okto, w;

	
	int keyCtrl, keyRobot1, keyRobot2, keyRobot3;
	int pxw1, pxh1, pxw2, pxh2, pxw3, pxh3;

	int locposx, locposy;
	int m_dist, lock;
	double m_angle, m_valSend;

	int simulation;
	int robovar1_flagLoc;
	int robovar2_flagLoc;
	int robovar3_flagLoc;

	int flag_sudut;
	
	int lock_maju, lock_mundur, lock_kanan, lock_kiri;

	//int w,h;


public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void updateGui();
	void on_pB_INITIAL_clicked(bool check);
	void on_pB_Connect_to_refbox_clicked();
	void on_pB_DISCONNECT_clicked();
	void on_pB_close_clicked();

	void updateDataRobot();
	void updateTimerMinute();
	void updateTimerSecond();

	void on_pB_Jamil_Left_clicked();
	void on_pB_Jamil_Right_clicked();
	void on_pB_Jamil_Up_clicked();
	void on_pB_Jamil_Down_clicked();

	void on_pB_Hendro_Left_clicked();
	void on_pB_Hendro_Right_clicked();
	void on_pB_Hendro_Up_clicked();
	void on_pB_Hendro_Down_clicked();

	void on_pB_Okto_Left_clicked();
	void on_pB_Okto_Right_clicked();
	void on_pB_Okto_Up_clicked();
	void on_pB_Okto_Down_clicked();

	void on_pB_CYAN_Goal_clicked();
	void on_pB_CYAN_Kick_off_clicked();
	void on_pB_CYAN_Goal_kick_clicked();
	void on_pB_CYAN_Throw_in_clicked();
	void on_pB_CYAN_Corner_clicked();
	void on_pB_CYAN_Free_kick_clicked();
	void on_pB_CYAN_Penalty_clicked();
	void on_pB_CYAN_Repair_clicked();

	void on_pB_MAGENTA_Goal_clicked();
	void on_pB_MAGENTA_Kick_off_clicked();
	void on_pB_MAGENTA_Goal_kick_clicked();
	void on_pB_MAGENTA_Throw_in_clicked();
	void on_pB_MAGENTA_Corner_clicked();
	void on_pB_MAGENTA_Free_kick_clicked();
	void on_pB_MAGENTA_Penalty_clicked();
	void on_pB_MAGENTA_Repair_clicked();

	void on_pB_RC_Start_clicked();
	void on_pB_RC_Stop_clicked();
	void on_pB_RC_Drop_Ball_clicked();
	void on_pB_RC_Park_clicked();
	void on_pB_RC_End_Part_clicked();
	void on_pB_RC_Reset_clicked();
	void on_pB_RC_End_Game_clicked();
	void on_pB_RC_Reset_Localref_clicked();

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *ev);
	

	int mapx_player(int); 
	int mapy_player(int);
	int mapt(int);

	int mapx_line_player(int);
	int mapy_line_player(int);
	int mapx_line_target(int);
	int mapy_line_target(int);

	int mapxToReal(int);
	int mapyToReal(int);

	int mapx_ball(int); 
	int mapy_ball(int);

	void on_pB_Jamil_Reset_clicked();
	void on_pB_Jamil_Kalib_Full_clicked();
	void on_pB_Jamil_Kalib_X_clicked();
	void on_pB_Jamil_Kalib_Y_clicked();
	void on_pB_Jamil_Kick_clicked();
	void on_pB_Jamil_Dribbler_clicked();

	void on_pB_Hendro_Reset_clicked();
	void on_pB_Hendro_Kalib_Full_clicked();
	void on_pB_Hendro_Kalib_X_clicked();
	void on_pB_Hendro_Kalib_Y_clicked();
	void on_pB_Hendro_Kick_clicked();
	void on_pB_Hendro_Dribbler_clicked();
	
	void on_pB_Okto_Reset_clicked();
	void on_pB_Okto_Kalib_Full_clicked();
	void on_pB_Okto_Kalib_X_clicked();
	void on_pB_Okto_Kalib_Y_clicked();
	void on_pB_Okto_Kick_clicked();
	void on_pB_Okto_Dribbler_clicked();

	void on_pB_Mode_Direct_clicked();
	void on_pB_Mode_Pass_clicked();
	void on_pB_Ori_Strategy_clicked();
	void on_pB_Def_Strategy_clicked();
	void on_pB_JUST_SHOOT_clicked();
	void on_pB_SKILL_OBS_clicked();
	//void paintEvent(QPaintEvent *event);



    /******************************************
    ** Manual connections
    *******************************************/
//protected:
	

private:
	Ui::MainWindowDesign ui;
	QNode qnode;

};

}  // namespace basestation_ersow

#endif // basestation_ersow_MAIN_WINDOW_H
