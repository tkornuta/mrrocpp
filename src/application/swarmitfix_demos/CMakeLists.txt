## WUT demo - experiment performed int the end of december in Warsaw. 
add_executable(mp_swarmitfix_demo_agent1_warsaw 
	mp_t_demo_base.cc
	mp_t_demo_agent1_warsaw.cpp
)
target_link_libraries(mp_swarmitfix_demo_agent1_warsaw mp)


## SMB test program. 
add_executable(mp_smb_test 
	mp_t_demo_base.cc
	mp_t_smb_test.cpp
)
target_link_libraries(mp_smb_test mp)

## SBENCH test program. 
add_executable(mp_sbench_test 
	mp_t_demo_base.cc
	mp_t_sbench_test.cpp
)
target_link_libraries(mp_sbench_test mp)

## Powered SMB test program. 
add_executable(mp_smb_powered_from_bench_test 
	mp_t_demo_base.cc
	mp_t_smb_powered_from_bench_test.cpp
)
target_link_libraries(mp_smb_powered_from_bench_test mp)


## Agent1 test program. 
add_executable(mp_agent1_demo 
	mp_t_demo_base.cc
	mp_t_agent1_demo.cpp
)
target_link_libraries(mp_agent1_demo mp)







## SMB *transparent* process. 
add_executable(ecp_smb_transparent
	ecp_g_smb.cc
	ecp_t_smb.cc
)
target_link_libraries(ecp_smb_transparent ecp_robot)


## SPKM *transparent* process. 
add_executable(ecp_spkm_transparent
	ecp_g_spkm.cc
	ecp_t_spkm.cc
)
target_link_libraries(ecp_spkm_transparent ecp_robot)


## SHEAD *transparent* process. 
add_executable(ecp_shead_transparent
	ecp_g_shead.cc
	ecp_t_shead.cc
)
target_link_libraries(ecp_shead_transparent ecp_robot)


## SBENCH *transparent* process. 
add_executable(ecp_sbench_transparent
	ecp_g_sbench.cc
	ecp_t_sbench.cc
)
target_link_libraries(ecp_sbench_transparent ecp_robot)


install(TARGETS 
ecp_spkm_transparent 
ecp_smb_transparent 
ecp_shead_transparent
ecp_sbench_transparent
mp_swarmitfix_demo_agent1_warsaw
mp_smb_test
mp_sbench_test
mp_smb_powered_from_bench_test
mp_agent1_demo
DESTINATION bin)
