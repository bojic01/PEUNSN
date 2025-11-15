#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

  #include<systemc.h>
  #include<iostream>
  #include<tlm>
  #include <ctime>
  #include <cstdlib>
  #include <tlm_utils/simple_initiator_socket.h>
  #include<tlm_utils/simple_target_socket.h>
  #include<cmath>
  #include<cstring>
  using namespace std;
  using namespace sc_core;
  using namespace sc_dt;
  
typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;
  
  #define MAX_ROWS 50//max redova lavirinta
  #define MAX_COL 50//masx kolona lavirinta
  
  #define MAX_STACK_SIZE 10000
  #define  CELL_SIZE 4
  
  #define MAX_COST 1000000
  
  #define DELAY 10//delay 10NS
  
  //BRAM SIEZE 3KB
  #define BRAM_SIZE 0x0C00
 
  
 //REGISTERS IN HARD,tj indikatorski registri
#define ADDR_ROWS 0x00
#define ADDR_COLS 0x01
#define ADDR_START 0x02
#define ADDR_READY 0x03
  
  
//BRAM
  #define ADDR_BRAM_LOW 0x00000000
  #define ADDR_BRAM_HIGH 0x00000000 + BRAM_SIZE
  //HARD
  #define ADDR_HARD_LOW 0x40000000
  #define ADDR_HARD_HIGH 0x4000000F

#endif
