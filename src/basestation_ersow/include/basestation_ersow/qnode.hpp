/**
 * @file /include/basestation_ersow/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef basestation_ersow_QNODE_HPP_
#define basestation_ersow_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include "basestation_ersow/data_agent0.h"
#include "basestation_ersow/dataAgentMsg.h"
#include "basestation_ersow/strategyMsg.h"
#include "basestation_ersow/data_gui.h"



/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace basestation_ersow {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	void run();
	void setPoint();
	void SIMULASI_DEFAULT();
	void SIMULASI_OUR_FREEKICK();
	void SIMULASI_OUR_GOALKICK();
	void SIMULASI_OUR_CORNER();
	void SIMULASI_OUR_KICKOFF();
	void SIMULASI_THEIR_GOALKICK();
	void SIMULASI_THEIR_FREEKICK();
	void SIMULASI_THEIR_CORNER();
	void SIMULASI_THEIR_KICKOFF();
	void SIMULASI_DROPBALL();
	void SIMULASI_PARK();
	void getCbKeeper(const basestation_ersow::dataAgentMsg& msg_handler);
	void getCbOkto(const basestation_ersow::dataAgentMsg& msg_handler);
	void getCbHendro(const basestation_ersow::dataAgentMsg& msg_handler);
	void getCallbackStrategy(const basestation_ersow::strategyMsg& msg_handler);
	int getAngle(int, int, int, int);

	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

	enum Posrobot { firstpos, secondpos, thirdpos, fourthpos, fifthpos, sixthpos };
	//extern Posrobot stepposersow;
	Posrobot stepposersow;

	 basestation_ersow::data_agent0 dataRobotJamil;
	 basestation_ersow::data_agent0 dataRobotOkto;
	 basestation_ersow::data_agent0 dataRobotHendro;
	 basestation_ersow::data_agent0 dataBallBase;
	 basestation_ersow::strategyMsg dataStrategy;
	 basestation_ersow::data_gui data_gui;
	 
	 //basestation_ersow::dataAgentMsg referee;

	struct Point {
    int x; 
    int y;
	};
	int flag;
	int postx_okto;
	int posty_okto;
	int postx_hendro;
	int posty_hendro;
	int postx_jamil;
	int posty_jamil;
	int theta_okto;
	int theta_hendro;
	int theta;
	int count, increment, decrement, cout;
	int lock;
	

	struct Point expandfield;

	time_t startb, endb;

Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();


private:
	int init_argc;
	char** init_argv;
	//ros::Publisher chatter_publisher_jamil;
	//ros::Publisher chatter_publisher_okto;
	//ros::Publisher chatter_publisher_hendro;
	ros::Publisher referee_publisher;

	ros::Publisher publish_data_gui;

	ros::Subscriber subscribeFromNodeHendro;
	ros::Subscriber subscribeFromNodeJamil;
	ros::Subscriber subscribeFromNodeOkto;
	ros::Subscriber subscribeFromStrategy;
};

}  // namespace basestation_ersow

#endif /* basestation_ersow_QNODE_HPP_ */
