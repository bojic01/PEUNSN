#ifndef VP_HPP_
#define VP_HPP_

#include"DEFINES.hpp"
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include"CPU.hpp"
#include"INTERCONNECT.hpp"
#include"HARD.hpp"
#include"BRAM.hpp"

class VP:public sc_module{
   public:                  
     VP(sc_module_name name,int row,int col);
     ~VP();
     
     protected:
        CPU cpu;
        INTERCONNECT interconnect;
        HARD hard;
        BRAM bram;


};



#endif
