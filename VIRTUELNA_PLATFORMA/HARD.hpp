#ifndef HARD_HPP
#define HARD_HPP
#include"DEFINES.hpp"
#include"UTILS.hpp"
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

using namespace sc_core;
using namespace tlm;
using namespace std;
using namespace sc_dt;

class HARD: public sc_module {
public:
    // TLM socket za komunikaciju sa interkonektom
    tlm_utils::simple_target_socket<HARD> INTERCONNECT_socket;

    // TLM socket za komunikaciju sa BRAM
    tlm_utils::simple_initiator_socket<HARD> BRAM_socket;
    // sc_time offset;
    // Konstruktor
    SC_HAS_PROCESS(HARD);
    HARD(sc_module_name name);
    
    // Destruktor
    ~HARD();

    // Transport funkcija za TLM
    void b_transport(pl_t &pl, sc_time &offset);

protected:
    unsigned char maze[MAX_ROWS][MAX_COL];
   
    sc_int<8> rows;        // Broj redova lavirinta
    sc_int<8> cols;        // Broj kolona lavirinta
    sc_int<2> start;       //flag
    sc_int<2> ready;       // flag
     sc_time offset;
    // Pomoćne funkcije
    void generateMaze();
    void write_bram(sc_uint<64> addr, unsigned char val);
    unsigned char read_bram(sc_uint<64> addr);
    void nacrtajLavirint();

    
    
    

   
};

#endif // HARD_HPP

