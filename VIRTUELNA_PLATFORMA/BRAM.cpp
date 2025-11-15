#include"BRAM.hpp"


BRAM::BRAM(sc_module_name name):sc_module(name),BRAM_socket_1("BRAM_socket_1"),BRAM_socket_2("BRAM_socket_2"){
   
   BRAM_socket_1.register_b_transport(this,&BRAM::b_transport);
   BRAM_socket_2.register_b_transport(this,&BRAM::b_transport);
   
   for(int i=0;i<BRAM_SIZE;i++){
        mem[i]=0;//inicijlaizacija svih lokacija na 0
   }
    SC_REPORT_INFO("BRAM", "Constructed.");
   }
   
   BRAM::~BRAM()
{
	SC_REPORT_INFO("BRAM", "Destroyed.");
}

void BRAM::b_transport(pl_t &pl,sc_time &){

     tlm::tlm_command cmd = pl.get_command();//komadna da li da memorija radi upis ili citanje
     int addr=pl.get_address();//dobijam adresu na koju je potrebno nesto upisati/procitati
     unsigned int len = pl.get_data_length();//koliko bajtova,tj memorijskih lokacija ce podatak zauzeti.Npr ako saljemo 64bajtni podatak,f-ja vraca broj 64
     unsigned char *buf = pl.get_data_ptr(); //pokazivac na podatak koji treab da se upise/procita
     
     
     if (addr + len > BRAM_SIZE) {
        // Provera da li su adresa i dužina podataka u opsegu memorije
        pl.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        cout<<"PREKORACEN OPSEG MEMORIJE"<<endl;
        return;
    }
     cout << "Memory content before operation: ";
    for (unsigned int i = 0; i < len; ++i)
        cout << (int)mem[addr+i] << " ";
    cout << endl;
     
     switch(cmd)
	{
		case tlm::TLM_WRITE_COMMAND:
			for (unsigned int i = 0; i < len; ++i)
			{
		  		mem[addr+i] =buf[i];
		  	
			}
			pl.set_response_status( tlm::TLM_OK_RESPONSE );
			
			wait(DELAY-3,SC_NS);
			break;
	
		case tlm::TLM_READ_COMMAND:
			for (unsigned int i = 0; i < len; ++i)
			{
		  		buf[i] = mem[addr+i];
				
			}
			pl.set_response_status( tlm::TLM_OK_RESPONSE );
			
			wait(DELAY-3,SC_NS);
			break;
	
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			wait(DELAY-3,SC_NS);
	}
	}
