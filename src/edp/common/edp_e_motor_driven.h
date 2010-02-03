/*!
 * \file edp_e_motor_driven.h
 * \brief File containing the declaration of edp::common::motor_driven_effector class.
 *
 * \author yoyek
 * \date 2009
 *
 */

#ifndef __EDP_E_MOTOR_DRIVEN_H
#define __EDP_E_MOTOR_DRIVEN_H

#include <stdint.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "lib/typedefs.h"
#include "lib/impconst.h"
#include "lib/com_buf.h"
#include "lib/srlib.h"
#include "lib/mis_fun.h"

#if defined(USE_MESSIP_SRR)
#include <messip.h>
#endif

#include "edp/common/edp_effector.h"

// Konfigurator
#include "lib/configurator.h"

#ifdef DOCENT_SENSOR
#include <boost/function.hpp>
#endif

namespace mrrocpp {
namespace edp {
namespace sensor {
class force;
}
namespace common {

// TODO: remove forward declarations
class manip_trans_t;
class in_out_buffer;
class vis_server;
class servo_buffer;
class edp_vsp;
class reader_buffer;

/*!
 * \class motor_driven_effector
 * \brief Base class of all EDP effectors using motors (e.g. robots)
 *
 * The class can be treated as multi variant shield. The derrived classes can optionally use servo_buffer (dedicated servo thread)
 * reader_buffer - dedicated reader thread, mt_tt_obj - dedicated thread to interpolate in task coordinates, e.g. force control in manipulators
 * vis_server - dedicated thread to sent joint position e.g. to visualisation processes,
 * sensor::force -- dedicated thread to measure force for the purpose of position force control of robotics manipulator
 * edp_vsp_obj - thread to sent data to VSP process (when the force sensor is used both as the prioceptor and exteroceptor)
 * *
 * \author yoyek
 */
class motor_driven_effector: public effector, public kinematics::common::kinematics_manager
{
protected:

#ifdef DOCENT_SENSOR
	void onReaderStarted();
	void onReaderStopped();
#endif

	/*!
	 * \brief The number of steps in the macrostep.
	 *
	 * Each step takes typically 1 to 2ms.
	 */
	uint16_t motion_steps;

	/*!
	 * \brief The number of steps between the SET and QUERY command.
	 *
	 * This number should be lower then motion_steps while the manipulator is moving to receive new command before the execution of the previous is finished.
	 */
	uint16_t value_in_step_no;

#ifdef DOCENT_SENSOR
	boost::function<void()> startedCallback_;
	bool startedCallbackRegistered_;
	boost::function<void()> stoppedCallback_;
	bool stoppedCallbackRegistered_;
#endif

	/*!
	 * \brief friend class of servo thread to handle the motion controllers
	 *
	 * It is used when the controllers loop is implemented in the EDP.
	 */
	friend class servo_buffer;

	/*!
	 * \brief method to set the outputs in the hardware commanded by the ECP
	 *
	 * It is done with usage of in_out_object, processed in interrupt handler.
	 */
	void set_outputs(const lib::c_buffer &instruction); // ustawienie wyjsc binarnych

	/*!
	 * \brief method to get the inputs from the hardware commanded by the ECP and then reply it to the ECP.
	 *
	 * It is done with usage of in_out_object, processed in interrupt handler.
	 */
	void get_inputs(lib::r_buffer *local_reply); // odczytanie wejsc binarnych

	/*!
	 * \brief method reset to zero the vectors of motor and joint position.
	 *
	 * number_of_servos should be previously set
	 */
	void reset_variables();

	/*!
	 * \brief method to extract desired_motor_position basing on the ECP instruction with motor desired motor position in absolute or relative variant.
	 *
	 * It also checks kinematic constrains of motor and equivalent joint position.
	 */
	void compute_motors(const lib::c_buffer &instruction); // obliczenia dla ruchu ramienia (silnikami)

	/*!
	 * \brief method to compute desired_motor_position basing on the ECP instruction with motor desired joint position in absolute or relative variant.
	 *
	 * It also checks kinematic constrains of motor and equivalent joint position.
	 */
	void compute_joints(const lib::c_buffer &instruction); // obliczenia dla ruchu ramienia (stawami)

	/*!
	 * \brief method to prepare command for servos (typically servo_buffer thread)
	 *
	 * It bases on desired_motor_position.
	 */
	void move_servos();

	/*!
	 * \brief
	 *
	 *
	 */
	lib::MotorArray servo_current_motor_pos; // Polozenia walow silnikow -// dla watku edp_servo    XXXX

	/*!
	 * \brief
	 *
	 *
	 */
	lib::MotorArray global_current_motor_pos; // Polozenia walow silnikow -// globalne dla procesu EDP  XXXX

	/*!
	 * \brief
	 *
	 *
	 */
	lib::JointArray servo_current_joints; // Wspolrzedne wewnetrzne -// dla watku EDP_SERVO   XXXXXX

	/*!
	 * \brief
	 *
	 *
	 */
	lib::JointArray global_current_joints; // Wspolrzedne wewnetrzne -// globalne dla procesu EDP   XXXXX

	/*!
	 * \brief
	 *
	 *
	 */
	boost::mutex edp_irp6s_effector_mutex; // mutex    XXXXXX

	/*!
	 * \brief
	 *
	 *
	 */
	lib::JointArray desired_joints; // Wspolrzedne wewnetrzne -
	// ostatnio obliczone (zadane) (w radianach)

	/*!
	 * \brief
	 *
	 *
	 */
	lib::JointArray current_joints;
	// ostatnio odczytane (w radianach) // by Y dla watku EDP_MASTER

	/*!
	 * \brief
	 *
	 *
	 */
	lib::MotorArray desired_motor_pos_old;
	// Polozenia walow silnikow -
	// poprzednio obliczone (zadane) (w radianach)

	/*!
	 * \brief
	 *
	 *
	 */
	lib::MotorArray desired_motor_pos_new;
	// Polozenia walow silnikow -
	// aktualnie obliczone (zadane) (w radianach)

	/*!
	 * \brief
	 *
	 *
	 */
	lib::MotorArray current_motor_pos; // Polozenia walow silnikow -
	// ostatnio odczytane (w radianach)


public:

	/*!
	 * \brief
	 *
	 *
	 */
	void master_joints_read(double*);
#ifdef DOCENT_SENSOR
	void registerReaderStartedCallback(boost::function<void()> startedCallback);
	void registerReaderStoppedCallback(boost::function<void()> stoppedCallback);
#endif

	/*!
	 * \brief
	 *
	 *
	 */
	in_out_buffer *in_out_obj; // bufor wejsc wyjsc

	/*!
	 * \brief
	 *
	 *
	 */
	reader_buffer *rb_obj;

	/*!
	 * \brief
	 *
	 *
	 */
	manip_trans_t *mt_tt_obj;

	/*!
	 * \brief
	 *
	 *
	 */
	servo_buffer* sb;

	/*!
	 * \brief
	 *
	 *
	 */
	vis_server* vis_obj;

	/*!
	 * \brief
	 *
	 *
	 */
	sensor::force *vs;

	/*!
	 * \brief
	 *
	 *
	 */
	edp_vsp* edp_vsp_obj;

	/*!
	 * \brief
	 *
	 *
	 */
	motor_driven_effector(lib::configurator &_config, lib::robot_name_t l_robot_name); // konstruktor

	/*!
	 * \brief
	 *
	 *
	 */
	virtual ~motor_driven_effector();

	/*!
	 * \brief
	 *
	 *
	 */
	virtual void set_rmodel(lib::c_buffer &instruction); // zmiana narzedzia

	/*!
	 * \brief
	 *
	 *
	 */
	virtual void get_rmodel(lib::c_buffer &instruction); // odczytanie narzedzia

	/*!
	 * \brief
	 *
	 *
	 */
	lib::controller_state_t controller_state_edp_buf; // do okreslenia stanu robota

	/*!
	 * \brief
	 *
	 *
	 */
	unsigned long step_counter;

	/*!
	 * \brief
	 *
	 *
	 */
	short number_of_servos; // by Y ilosc serwomechanizmow  XXX
	// w zaleznosci od tego czy chwytak ma byc aktywny czy nie

	/*!
	 * \brief
	 *
	 *
	 */
	virtual void move_arm(lib::c_buffer &instruction) = 0; // przemieszczenie ramienia

	/*!
	 * \brief
	 *
	 *
	 */
	void multi_thread_move_arm(lib::c_buffer &instruction);

	/*!
	 * \brief
	 *
	 *
	 */
	void single_thread_move_arm(lib::c_buffer &instruction);

	/*!
	 * \brief
	 *
	 *
	 */
	virtual void get_arm_position(bool read_hardware, lib::c_buffer &instruction) = 0; // odczytanie pozycji ramienia

	/*!
	 * \brief
	 *
	 *
	 */
	void get_arm_position_read_hardware_sb(); // odczytanie pozycji ramienia sprzetowo z sb

	/*!
	 * \brief
	 *
	 *
	 */
	void get_arm_position_set_reply_step(); // odczytanie pozycji ramienia sprzetowo z sb

	/*!
	 * \brief
	 *
	 *
	 */
	virtual void get_arm_position_get_arm_type_switch(lib::c_buffer &instruction); // odczytanie pozycji ramienia sprzetowo z sb

	/*!
	 * \brief
	 *
	 *
	 */
	virtual void synchronise(); // synchronizacja robota

	/*!
	 * \brief
	 *
	 *
	 */
	virtual bool servo_joints_and_frame_actualization_and_upload(void); // by Y

	/*!
	 * \brief main loop of the EDP master thread.
	 *
	 * It is a sewuence of the three following small loops.
	 */
	void main_loop(); // main loop

	/*!
	 * \brief loop of the system before EDP is beeing synchronised.
	 *
	 * It waits for the command to gover initial system state.
	 */
	void pre_synchro_loop(STATE& next_state);

	/*!
	 * \brief loop of the system after the initial state is sent to ECP.
	 *
	 * It waits for the synchronisation command. Sometimes robot is initially synchronised just after it is run. Then this loop is inactive.
	 */
	void synchro_loop(STATE& next_state);

	/*!
	 * \brief loop after system synchronisation.
	 *
	 * This is common loop of the system, used durring task execution.
	 */
	void post_synchro_loop(STATE& next_state);

	/*!
	 * \brief method to create threads other then EDP master thread.
	 *
	 * It implemented for the puropose of the specific EDP effector, choosing the suitable components (e.g. servo_buffer, transformation etc.)
	 */
	void hi_create_threads();

	/*!
	 * \brief method interpreting ECP command.
	 *
	 * It decides what kind of command specific methods will be  run using master_order method.
	 */
	void interpret_instruction(lib::c_buffer &instruction);

	/*!
	 * \brief Method checking the algorithms of the motor controllers
	 *
	 * It typically communicates with servo_buffer or other hardware libraries.
	 */
	void get_algorithms();

	/*!
	 * \brief The method checks the initial state of the controller.
	 *
	 * This method typically communicates with hardware to check if the robot is synchronised etc.
	 */
	virtual void get_controller_state(lib::c_buffer &instruction); // by Y

	/*!
	 * \brief The method checks if the hardware is on.
	 *
	 * It is suitable to detect some kind of system initialization failure.
	 */
	bool is_power_on() const;

	/*!
	 * \brief method to increments servo_current_motor_pos by its argument
	 *
	 * It is used by the servo_buffer
	 */
	void update_servo_current_motor_pos(double motor_position_increment, int i);

	/*!
	 * \brief method to set servo_current_motor_pos as its argument
	 *
	 * It is used by the servo_buffer
	 */
	void update_servo_current_motor_pos_abs(double abs_motor_position, int i);

	/*!
	 * \brief method that generates type of reply commanded by the ECP.
	 *
	 * It takes into account instruction commanded by the ECP. The rep_type is also used durning ECP command interpretation and execution to gother valid information to be replied to ECP.
	 */
	lib::REPLY_TYPE rep_type(const lib::c_buffer &instruction);

	/*!
	 * \brief method that check if the ECP command is valid for the robot before it is synchronied.
	 *
	 * The valid methods are specific motion instruction (in motor coordinates) and synchronisation command.
	 */
	bool pre_synchro_motion(lib::c_buffer &instruction) const;


	/*!
	 * \brief Method informing if the robot is synchronised or not.
	 *
	 * Some of the robots are initially synchronised after trurn on, some needs extra synchronisation procedure.
	 */
	bool is_synchronised(void) const;

	/*!
	 * \brief method that returns pointer to object of servo_buffer thread. It is used in some robots.
	 *
	 * when the servo_buffer thread is used it have to be implemented in spoecific robot. Otherwise it is not needed.
	 */
	virtual servo_buffer* return_created_servo_buffer();

	/*!
	 * \brief pure virtual method to be implemented in specific effector.
	 *
	 * It decides which variant of master_ordewr is used (single or multi thread)
	 */
	virtual void master_order(MT_ORDER nm_task, int nm_tryb) = 0;

	/*!
	 * \brief method running ECP command specific methods in two thread version
	 *
	 * It uses extra, dedicated transformation thread
	 */
	void multi_thread_master_order(common::MT_ORDER nm_task, int nm_tryb);

	/*!
	 * \brief method running ECP command specific methods in single thread version
	 *
	 * It does not use extra transformation thread
	 */
	void single_thread_master_order(common::MT_ORDER nm_task, int nm_tryb);
};


} // namespace common
} // namespace edp
} // namespace mrrocpp

#endif
