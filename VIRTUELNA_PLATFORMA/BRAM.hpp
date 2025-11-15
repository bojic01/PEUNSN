#ifndef _BRAM_HPP_
#define _BRAM_HPP_

#include"DEFINES.hpp"
      
      
class BRAM:public sc_module{

     public:
          BRAM(sc_module_name name);//konstruktor
          ~BRAM();//destruktor
          
          tlm_utils::simple_target_socket<BRAM> BRAM_socket_1;
	  tlm_utils::simple_target_socket<BRAM> BRAM_socket_2;      
     protected:
          void b_transport(pl_t &, sc_core::sc_time &);
          unsigned char mem[BRAM_SIZE];//memorija



};





#endif

