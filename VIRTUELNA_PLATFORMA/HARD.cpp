#include "HARD.hpp"

SC_HAS_PROCESS(HARD);

HARD::HARD(sc_module_name name) : sc_module(name),rows(0),cols(0),start(0),ready(1) {
    INTERCONNECT_socket.register_b_transport(this, &HARD::b_transport);
    SC_REPORT_INFO("HARD", "Constructed.");
}

HARD::~HARD() {
    SC_REPORT_INFO("HARD", "Destroyed");
}

void HARD::b_transport(pl_t &pl, sc_time &) {
    tlm_command cmd = pl.get_command();
    sc_dt::uint64 addr = pl.get_address();
    unsigned int len = pl.get_data_length();
    unsigned char *buf = pl.get_data_ptr();
    pl.set_response_status(TLM_OK_RESPONSE);

    switch (cmd) {
        case TLM_WRITE_COMMAND:
            switch (addr) {
                case ADDR_ROWS:
                    rows = toInt(buf);
                    
                    cout << "rows = " << rows << endl;
                    break;
                case ADDR_COLS:
                    cols = toInt(buf);
                    
                    cout << "cols = " << cols << endl;
                    break;
                case ADDR_START:
                    start = toInt(buf);
                    cout << "start = " << start << endl;
                    if(start==1 && ready==1){
                    ready=0;
                  
                    generateMaze();   
                    wait(DELAY*23,SC_NS);
                    ready=1;
                   
                     
                    }
                    break;
                default:
                    pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
                    cout << "Wrong address" << endl;
            }
            break;

        case TLM_READ_COMMAND:
            switch (addr) {
                case ADDR_READY:
                    toUchar(buf, ready);
                    break;
                default:
                    pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
            }
            break;

        default:
            pl.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            cout << "Wrong command" << endl;
    }
              
}

void HARD::generateMaze() {
             
           //int maze[MAX_ROWS][MAX_COL];//lavirint
           cout<<"HARD GENERISE LAVIRITN"<<endl;
         
    int stack[rows*cols][2];//stek za cuvanje kordinata dfs 
     int stackTop=-1;//"pokazivac" na vrh steka.Posto nas stek radi na principu FIFO stackTop ce mi pokazivati na prvi upisani/obrisani elemnt sa steka
     
     int pravac[4][2]={//pravci kretanja kroz matricu,tj lavirint
     {0,1},//desno
     {0,-1},//levo
     {1,0},//dole
     {-1,0}//gore
     };
   
   int trenutniPravac,slucajniPravac,zamenaPravca[2];
   int novoX,novoY,trenutnoX,trenutnoY;//trenutne i nove pozicije koordinara u lavirintu

   //inicijlazicacija lavirnta da su sve zidovi
      for(unsigned int i=0;i<rows;i++){
      for(unsigned int j=0;j<cols;j++){
         maze[i][j]=1;//matrica zidova
      }
      }
         stackTop++;//ubaci na stek
         stack[stackTop][0]=0;//X kordinata  //}
                                             //na stek stmo stavili pocetnu poziciju (0,0) kao pocetak lavirint(startna tacka)
         stack[stackTop][1]=0;//Y kordianra  //}
       
       maze[0][0]=0;//pocetna pozicija,gde 0 predstvalja put
       
       srand(time(0));

    while(stackTop>=0){

       trenutnoX=stack[stackTop][0]; //sa steka smo uzeli vrednosti,i sada to postaju kordinate,koje predstavljaju pravac kretanaj dfs-a X osa
       trenutnoY=stack[stackTop][1]; // -||- Y osa
       stackTop--;//skini sa steka
       
       //mesanje pravaca,da bi dobili razlicite lavirinte
         for(trenutniPravac=0;trenutniPravac<4;trenutniPravac++){
              slucajniPravac=rand()%4;//ovde smo smestili neki od pravaca u kome ce krenuti laviritn
              
              zamenaPravca[0]=pravac[trenutniPravac][0];//cuva privremene vrednosti trenutnog tavca po X osi(slucajna pozicija X)
              zamenaPravca[1]=pravac[trenutniPravac][1];//po Y osi(slucajna pozicija Y)
              
             pravac[trenutniPravac][0]=pravac[slucajniPravac][0]; 
             pravac[trenutniPravac][1]=pravac[slucajniPravac][1];    
         
            pravac[slucajniPravac][0]=zamenaPravca[0];
            pravac[slucajniPravac][1]=zamenaPravca[1];
         }
         
         //trazenje novih koordinta,odakle ce dfs nastaviti sa radom
         for(trenutniPravac=0;trenutniPravac<4;trenutniPravac++){
            novoX=trenutnoX+2*pravac[trenutniPravac][0];//ovde smo mnozenjem sa 2 omogucili da imamo zid izmedju dve susedne celij
            novoY=trenutnoY+2*pravac[trenutniPravac][1];
          //proveravamo da li su nove kordiante u ospsegu,i da li je celija sa novim koridantama jos uvek zid
           if(novoX>=0 && novoX<rows && novoY>=0 && novoY<cols && maze[novoX][novoY]==1){
           
           maze[novoX-pravac[trenutniPravac][0]][novoY-pravac[trenutniPravac][1]]=0;//kordinate celije koja se nalazi tacno izmejdu trenutne i sledece celije
           maze[novoX][novoY]=0;//prolaz
           
           
             stackTop++;//dodajemo novu poziciju na stek
             stack[stackTop][0]=novoX;//na stek su dodate nove pozicije po X osi
             stack[stackTop][1]=novoY;//-||- po Y osi
           
           }
            
            
         }
}             
           cout<<"CRTANJE"<<endl;
                nacrtajLavirint();
           
           
           /*write_bram(ADDR_ROWS,rows);
           cout<<"ADDR_ROW_HARD="<<rows<<endl;
           write_bram(ADDR_COLS,cols);
           cout<<"ADDR_COLS_HARD="<<cols<<endl;*/
           
          
            //UPIS U BRAM
            for(unsigned char i=0;i<rows;i++){
            for(unsigned char j=0;j<cols;j++){
               write_bram(i*cols+j,maze[i][j]);
               cout << "Written to BRAM at address " << i*cols+j << " = " << (int)maze[i][j] << endl;
  
            }
            
            }//KRAJ UPISA U BRAM
            
             
            cout<<"IZGENERSIAN LAVIRITN"<<endl;
           
       
 }
          
void HARD::nacrtajLavirint(){
     for(unsigned int i=0;i<rows;i++){
    for(unsigned int j=0;j<cols;j++){
     
     if(maze[i][j]==1){
       cout<<"#";//zid
     }else{
          cout<<".";//putanja
     }
    }
     cout<<"\n";

}


}
void HARD::write_bram(sc_uint<64> addr, unsigned char val) {
    
  
    
    pl_t pl;
    unsigned char buf;
    buf = val;
    pl.set_address(addr);
    pl.set_data_length(1);
    pl.set_data_ptr(&buf);
    pl.set_command(tlm::TLM_WRITE_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    sc_core::sc_time bram_delay = SC_ZERO_TIME;
    
    BRAM_socket->b_transport(pl,bram_delay);
    

}

unsigned char HARD::read_bram(sc_uint<64> addr) {
    pl_t pl;
    unsigned char buf;
    pl.set_address(addr);
    pl.set_data_length(1);
    pl.set_data_ptr(&buf);
    pl.set_command(tlm::TLM_READ_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    sc_core::sc_time bram_delay = SC_ZERO_TIME;
    BRAM_socket->b_transport(pl,bram_delay);
    
    return buf;
}


