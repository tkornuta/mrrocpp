add_library(mp_generators
	../../generator/mp/mp_g_delta_tight_coop.cc
)

target_link_libraries(mp_generators ecp_mp_subtasks)

install(TARGETS mp_generators DESTINATION lib)
