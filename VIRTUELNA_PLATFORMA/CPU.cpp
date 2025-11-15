#include "CPU.hpp"




int heuristic(int x1,int y1,int x2,int y2){
           return abs(x1-x2)+abs(y1-y2);
}

bool isValid(int x,int y,int mazeRows,int mazeCols,unsigned char mazeData[MAX_ROWS][MAX_COL]){
       return x>=0 && x<mazeRows && y>=0 && y<mazeCols && mazeData[x][y]==0;

}
int read_ddr_cnt=0;
int write_ddr_cnt=0;




SC_HAS_PROCESS(CPU);
                             
CPU::CPU(sc_module_name name,int rows,int cols):sc_module(name),offset(SC_ZERO_TIME),mazeRows(rows),mazeCols(cols){
     
     SC_THREAD(processMaze);
     SC_REPORT_INFO("CPU","CONSTRUCTED");
     for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COL; ++j) {
            mazeData[i][j] = 0; // Inicijalizuj sve vrednosti na 0 (ili neku drugu default vrednost)
        }
    }

}
CPU::~CPU(){
      SC_REPORT_INFO("CPU","DESTROYED");

}

void CPU::processMaze(){
     SC_REPORT_INFO("CPU", "Writing rows to HARD");
     write_hard(ADDR_ROWS,mazeRows);
     
     SC_REPORT_INFO("CPU", "Writing cols to HARD");
     write_hard(ADDR_COLS,mazeCols);
     
      SC_REPORT_INFO("CPU", "Starting HARD processing");
     write_hard(ADDR_START,1);
     
     //cekaj da HARD komponenta zavrsi sa obradom
     bool done=false;
     while(!done){
         int ready=read_hard(ADDR_READY);
          SC_REPORT_INFO("CPU", ("Ready status: " + std::to_string(ready)).c_str());
     if(ready){
           write_hard(ADDR_START,0);
           done=true;
     }else{
        wait(5*DELAY,SC_NS);
     }
     }
        SC_REPORT_INFO("CPU", "HARD processing done");
     //procitaj lavirint iz BRAM-a
      // unsigned char mazeData[MAX_ROWS][MAX_COL];//cuvaj podatke o lavirntu generisane od starne HARADA
       //unsigned char mazeRowsBRAM,mazeColsBRAM;
        
        
        //read_bram(ADDR_ROWS,&mazeRowsBRAM,1);
        //read_bram(ADDR_COLS,&mazeColsBRAM,1);
        
        
        
        
    
          //mazeRows=mazeRowsBRAM;
          cout<<"MAZE_ROWS_CPU="<<mazeRows<<endl;
          //mazeCols=mazeColsBRAM;
           cout<<"MAZE_COLS_CPU="<<mazeCols<<endl;
           
        sc_uint<64>addr_maze=0x00;//pocetna adresa citanja podatak iz bram-a(laviritna)
         //sc_uint<64>a=0;
        for(int i=0;i<mazeRows;i++){
        for(int j=0;j<mazeCols;j++){
               read_bram(addr_maze++,&mazeData[i][j],1);
              
        }
        }
        
      
       
       cout<<"LAVIRINT IZ BRAM-a"<<endl;
       displayMaze();
       SC_REPORT_INFO("CPU","SOLVING MAZE");
       solveMaze();//resi lavirint
       SC_REPORT_INFO("CPU","MAZE SOLVED");
       
       SC_REPORT_INFO("CPU","DISPLAYING MAZE");
       displayMaze();//nacrtaj lavirint
       SC_REPORT_INFO("CPU","MAZE DISPLAYED");
       offset = SC_ZERO_TIME;
}

//A*
void CPU::solveMaze() {
    cout << "USAO U F-JU SOLVE" << endl;
    cout << "MAZE BEFORE SOLVING:" << endl;
    displayMaze();

    // Definišemo potrebne strukture za A* algoritam
    int gCost[MAX_ROWS][MAX_COL];
    int fCost[MAX_ROWS][MAX_COL];
    int parent[MAX_ROWS][MAX_COL];
    bool closedList[MAX_ROWS][MAX_COL] = {false};
    bool openList[MAX_ROWS][MAX_COL] = {false};
    
    // Inicijalizacija
    for (int i = 0; i < mazeRows; i++) {
        for (int j = 0; j < mazeCols; j++) {
            gCost[i][j] = MAX_COST;
            fCost[i][j] = MAX_COST;
            parent[i][j] = -1;
        }
    }

    int startRow = 0;
    int startCol = 0;
    int goalRow = mazeRows - 1;
    int goalCol = mazeCols - 1;
    
    gCost[startRow][startCol] = 0;
    fCost[startRow][startCol] = heuristic(startRow, startCol, goalRow, goalCol);
    
    openList[startRow][startCol] = true;

    // Kreni sa traženjem puta
    while (true) {
        int currentRow = -1;
        int currentCol = -1;
        int currentF = MAX_COST;

        for (int i = 0; i < mazeRows; i++) {
            for (int j = 0; j < mazeCols; j++) {
                if (openList[i][j] && fCost[i][j] < currentF) {
                    currentRow = i;
                    currentCol = j;
                    currentF = fCost[i][j];
                }
            }
        }

        if (currentRow == -1 || currentCol == -1) {
            cout << "No path found!" << endl;
            break;
        }

        if (currentRow == goalRow && currentCol == goalCol) {
            break;
        }

        openList[currentRow][currentCol] = false;
        closedList[currentRow][currentCol] = true;

        int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        
        for (int d = 0; d < 4; d++) {
            int nx = currentRow + directions[d][0];
            int ny = currentCol + directions[d][1];

            if (isValid(nx, ny, mazeRows, mazeCols, mazeData) && !closedList[nx][ny]) {
                int newG = gCost[currentRow][currentCol] + 1;
                if (newG < gCost[nx][ny]) {
                    gCost[nx][ny] = newG;
                    fCost[nx][ny] = newG + heuristic(nx, ny, goalRow, goalCol);
                    parent[nx][ny] = currentRow * mazeCols + currentCol;
                    openList[nx][ny] = true;
                }
            }
        }
    }

    // Rekonstrukcija putanje
    int pathRow = goalRow;
    int pathCol = goalCol;
    while (pathRow != startRow || pathCol != startCol) {
        mazeData[pathRow][pathCol] = 2;
        int parentIndex = parent[pathRow][pathCol];
        if (parentIndex == -1) break;  // Ako nema roditelja, prekini
        pathRow = parentIndex / mazeCols;
        pathCol = parentIndex % mazeCols;
    }

    mazeData[startRow][startCol] = 2;
    
    cout << "MAZE AFTER SOLVING:" << endl;
    displayMaze();
}//kraj A*



void CPU::displayMaze() {
    cout << "DISPLATING MAZE" << endl;
    
    

    
    for (int i = 0; i < mazeRows; i++) {
        for (int j = 0; j < mazeCols; j++) {
        
      
            unsigned char cell = mazeData[i][j];

            if (cell == 1) {
                cout << "#"; // Zid
            }else if(cell==2){
               cout<<"*";//resenje
            }else{
              cout<<".";//putanja
            }
        }
        cout << endl; // Nova linija za sledeći red lavirinta
    }
    cout << "Maze displayed successfully!" << endl;
}




void CPU::write_bram(sc_uint<64> addr, unsigned char val)
{
	pl_t pl;		
	unsigned char buf;
	buf = val;
	
	pl.set_address(ADDR_BRAM_LOW + addr);
	pl.set_data_length(1); 
	pl.set_data_ptr(&buf);
	pl.set_command( tlm::TLM_WRITE_COMMAND );
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	sc_core::sc_time delay = SC_ZERO_TIME;
	INTERCONNECT_socket->b_transport(pl,delay);
	
	
	cout<<"Written to BRAM at address"<<addr<<"="<<(int)val<<endl;
}

void CPU::read_bram(sc_uint<64> addr, unsigned char *all_data, int length)
{
    
    pl_t pl;
    unsigned char buf;//buffer za citanje jedog bajtA
    int n=0;//brojac za popunjavanje niza all_data;
    
    for(int i=0;i<length;i++){
       write_ddr_cnt+=4;
       
       pl.set_address(ADDR_BRAM_LOW+addr+i);
       pl.set_data_length(1);
       pl.set_data_ptr(&buf);
       pl.set_command(tlm::TLM_READ_COMMAND);
       pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
       sc_core::sc_time delay = SC_ZERO_TIME;
       INTERCONNECT_socket->b_transport(pl,delay);
       
       if(pl.get_response_status()!=tlm::TLM_OK_RESPONSE){
          cerr<<"ERROR:TLM RESPONSE STATUS NOT OK"<<endl;
          break;
       }
            all_data[n]=buf;
            n++;
    
    
    }
    
    
}


int CPU::read_hard(sc_uint<64> addr)
{
    	pl_t pl;
    	unsigned char buf[8];
    	pl.set_address(ADDR_HARD_LOW + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_READ_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	
    	sc_core::sc_time delay = SC_ZERO_TIME;
    	INTERCONNECT_socket->b_transport(pl,delay);
   
    	
    	return toInt(buf);
}

void CPU::write_hard(sc_uint<64> addr,int val)
{
    	pl_t pl;
    	unsigned char buf[4];
    	toUchar(buf,val); 	
    	pl.set_address(ADDR_HARD_LOW + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_WRITE_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	sc_core::sc_time delay = SC_ZERO_TIME;
    	INTERCONNECT_socket->b_transport(pl,delay);
    
}




