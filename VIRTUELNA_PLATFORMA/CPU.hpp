#ifndef CPU_H
#define CPU_H

#include"HARD.hpp"
#include"BRAM.hpp"
#include"DEFINES.hpp"

class CPU:public sc_module{
   public:
   SC_HAS_PROCESS(CPU);
   CPU(sc_module_name name,int rows,int cols);
   ~CPU();
   tlm_utils::simple_initiator_socket<CPU>INTERCONNECT_socket;
   protected:
     void processMaze();
     sc_time offset;
     
     int mazeRows,mazeCols;
     unsigned char mazeData[MAX_ROWS][MAX_COL];
    unsigned char mazeRowsBRAM,mazeColsBRAM;
     void displayMaze();
     void solveMaze();
    
    
     void read_bram(sc_uint<64>addr,unsigned char *all_data,int length);
     void write_bram(sc_uint<64>addr,unsigned char val);
     int read_hard(sc_uint<64>addr);
     void write_hard(sc_uint<64>addr,int val);









};

  



















#endif
