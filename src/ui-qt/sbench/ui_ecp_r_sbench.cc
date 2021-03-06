/*!
 * @file
 * @brief File contains ui EcpRobot class definition for SwarmItFix Bench
 * @author twiniars <twiniars@ia.pw.edu.pl>, Warsaw University of Technology
 *
 * @ingroup sbench
 */

#include <boost/shared_ptr.hpp>

#include "ui_ecp_r_sbench.h"
#include "base/ecp/ecp_robot.h"

#include "../base/ui_robot.h"

namespace mrrocpp {
namespace ui {
namespace sbench {

// ---------------------------------------------------------------
EcpRobot::EcpRobot(common::UiRobot& _ui_robot) :
		EcpRobotDataPort(_ui_robot)
{
	the_robot = (boost::shared_ptr <robot_t>) new ecp::sbench::robot(*(ui_robot.interface.config), *(ui_robot.msg));
	common::EcpRobot::ecp = (ecp::common::robot::common_buffers_ecp_robot*) (the_robot.get());
}

}
} //namespace ui
} //namespace mrrocpp
