// -------------------------------------------------------------------------
//                            task/mp_t_haptic.h
//
// MP task for two robot haptic device
// Ostatnia modyfikacja: 2007
// -------------------------------------------------------------------------

#if !defined(__MP_T_SK_MR_H)
#define __MP_T_SK_MR_H

namespace mrrocpp {
namespace mp {
namespace task {

class sk_mr : public task
{
protected:

public:

	sk_mr(lib::configurator &_config);

	// methods for mp template
	void main_task_algorithm(void);

};

/** @} */// end of edge_following

} // namespace task
} // namespace mp
} // namespace mrrocpp

#endif
