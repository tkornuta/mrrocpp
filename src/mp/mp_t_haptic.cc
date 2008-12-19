// -------------------------------------------------------------------------
//                              mp_t_haptic.cc
// 
// MP task for two robot haptic device
// 
// -------------------------------------------------------------------------


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <map>

#include "common/typedefs.h"
#include "common/impconst.h"
#include "common/com_buf.h"

#include "lib/srlib.h"
#include "mp/mp.h"
#include "mp/mp_g_force.h"
#include "mp/mp_t_haptic.h"
#include "ecp_mp/ecp_mp_s_schunk.h"

mp_task* return_created_mp_task (configurator &_config)
{
	return new mp_task_haptic(_config);
}

mp_task_haptic::mp_task_haptic(configurator &_config) : mp_task(_config)
{
}

// methods fo mp template to redefine in concete class
void mp_task_haptic::task_initialization(void) 
{
	sr_ecp_msg->message("MP haptic device loaded");
};



bool mp_task_haptic::configure_edp_force_sensor(bool configure_track, bool configure_postument)
{
    if (configure_track)
    {
        set_next_ecps_state ((int) ECP_GEN_BIAS_EDP_FORCE, 0, "", 1, ROBOT_IRP6_ON_TRACK);
      
    }

    if (configure_postument)
    {
        set_next_ecps_state ((int) ECP_GEN_BIAS_EDP_FORCE, 0, "", 1, ROBOT_IRP6_POSTUMENT);
    }

    if ((configure_track)&&(!configure_postument))
    {
        run_extended_empty_generator_for_set_of_robots_and_wait_for_task_termination_message_of_another_set_of_robots
                (1, 1, ROBOT_IRP6_ON_TRACK, ROBOT_IRP6_ON_TRACK);
    }
    else if ((!configure_track)&&(configure_postument))
    {
        run_extended_empty_generator_for_set_of_robots_and_wait_for_task_termination_message_of_another_set_of_robots
                (1, 1, ROBOT_IRP6_POSTUMENT, ROBOT_IRP6_POSTUMENT);
    }
    else if ((configure_track)&&(configure_postument))
    {
        run_extended_empty_generator_for_set_of_robots_and_wait_for_task_termination_message_of_another_set_of_robots
                (2, 2, ROBOT_IRP6_ON_TRACK, ROBOT_IRP6_POSTUMENT,
                 ROBOT_IRP6_ON_TRACK, ROBOT_IRP6_POSTUMENT );
    }
   
}

 
void mp_task_haptic::main_task_algorithm(void)
{

  	mp_haptic_generator mp_h_gen(*this, 10); 
   	mp_h_gen.robot_m = robot_m;


   	sr_ecp_msg->message("New series");
   	//pierwsza konfiguracja czujnikow
   	// wlaczenie generatora do konfiguracji czujnika w EDP w obydwu robotach
   	configure_edp_force_sensor(true, true);


   	// wlaczenie generatora transparentnego w obu robotach
   	set_next_ecps_state ((int) ECP_GEN_TRANSPARENT, (int) 0, "", 1, ROBOT_IRP6_ON_TRACK);
   	set_next_ecps_state ((int) ECP_GEN_TRANSPARENT, (int) 0, "", 1, ROBOT_IRP6_POSTUMENT);

   	// mp_h_gen.sensor_m = sensor_m;
   	mp_h_gen.configure(1, 0);
   	sr_ecp_msg->message("Track podatny do czasu wcisniecia mp_trigger");
   	mp_h_gen.Move();


   	send_end_motion_to_ecps (2, ROBOT_IRP6_ON_TRACK, ROBOT_IRP6_POSTUMENT);


}
