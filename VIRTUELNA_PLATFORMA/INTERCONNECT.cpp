#include"INTERCONNECT.hpp"

INTERCONNECT::INTERCONNECT(sc_module_name name):sc_module(name){
     CPU_socket.register_b_transport(this,&INTERCONNECT::b_transport);
     SC_REPORT_INFO("INTERCONNECT","CONSTRUCTED");
}
  INTERCONNECT::~INTERCONNECT(){
      SC_REPORT_INFO("INTERCONNECT","DESTROYED");
  
  }

void INTERCONNECT::b_transport(pl_t&pl,sc_time&){
      sc_uint<64>addr=pl.get_address();
       SC_REPORT_INFO("INTERCONNECT", ("Address: " + std::to_string(addr)).c_str());
       sc_uint<64>taddr=addr&0x00FFFFFF;//maskiranje
 
    if(addr>=ADDR_BRAM_LOW && addr<=ADDR_BRAM_HIGH){
                pl.set_address(taddr);
                wait(10*DELAY,SC_NS);
		BRAM_socket->b_transport(pl, offset);
		pl.set_address(addr);
		
         
    }else if(addr>=ADDR_HARD_LOW && addr<=ADDR_HARD_HIGH){
             pl.set_address(taddr);
             wait(10*DELAY,SC_NS);
		HARD_socket->b_transport(pl, offset);
		pl.set_address(addr);
		
		
        
    
    }else{
           SC_REPORT_ERROR("INTERCONNECT","WRONG ADDRESS");
           pl.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            wait(10*DELAY,SC_NS);
    
    }
          


}

