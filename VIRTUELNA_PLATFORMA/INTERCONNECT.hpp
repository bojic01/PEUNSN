#ifndef INTERCONNECT_HPP_
#define INTERCONNECT_HPP_

#include"DEFINES.hpp"

class INTERCONNECT:public sc_module{
    public:
       INTERCONNECT(sc_module_name name);
       ~INTERCONNECT();
       tlm_utils::simple_initiator_socket<INTERCONNECT>BRAM_socket;
       tlm_utils::simple_initiator_socket<INTERCONNECT>HARD_socket;
       tlm_utils::simple_target_socket<INTERCONNECT>CPU_socket;
       protected:
            pl_t pl;
            sc_time offset;
            void b_transport(pl_t &pl,sc_time&offset);

};













#endif 
