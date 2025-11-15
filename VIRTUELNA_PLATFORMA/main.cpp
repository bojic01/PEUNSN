#include "VP.hpp"
#include "DEFINES.hpp"

int sc_main(int argc,char* argv[]){
    

   int row,col;
   cout<<"UNESITE ROW(MAX_ROW=50,DIMENIZJA NEKA BUDE NEPARAN BROJ)=";
   cin>>row;
   cout<<"UNESITE COL(MAX_ROW=50,DIMENZIAJ NEKA BUDE NEPARAN BROJ)=";
   cin>>col;

  
  VP vp("PLATFORMA",row,col);
  sc_start(1,SC_SEC);








return 0;

} 
