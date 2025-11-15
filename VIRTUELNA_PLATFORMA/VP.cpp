 #include"VP.hpp"

                            
VP::VP(sc_module_name name,int row,int col):sc_module(name),cpu("CPU",row,col),interconnect("INTERCONNECT"),hard("HARD"),bram("BRAM")
{

      cpu.INTERCONNECT_socket.bind(interconnect.CPU_socket);
      interconnect.BRAM_socket.bind(bram.BRAM_socket_1);
      interconnect.HARD_socket.bind(hard.INTERCONNECT_socket);
      hard.BRAM_socket.bind(bram.BRAM_socket_2);
      
      SC_REPORT_INFO("VIRTUAL PLATFORM","CONSTRUCTED");


}

VP::~VP(){
   
     SC_REPORT_INFO("VIRTUAL PLATFORM","DESTROYED");

}
