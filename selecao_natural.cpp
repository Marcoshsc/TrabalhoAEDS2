#include <cstdlib>
#include <fstream>
#include<sstream>
#include <iostream>
#include <string>
#define HIGH_VALUE 999

using namespace std;

extern int minimo(int*, int);

int selenat(string caminho_ent,int MAX_MEMO, string  nome_do_array[], string caminho_reservatorio,int max_reserv){
   fstream ent,reservatorio,temp;

   ent.open(caminho_ent.c_str(),ios::in);
   int i,memo[MAX_MEMO-1],tam_vet=0,no_reservatorio,ultima_chave;
   i=0;
   bool feito;
   string auxiliar,auxiliar2;
   char buffer[3];
   do{//repetição para copiar os registros do arquivo de entrada (não-ordenado)
      //para um array de registros em memória ("memo")
      getline(ent,auxiliar2);
      memo[i]=atoi(auxiliar2.c_str());
      i+=1;
   }
   while ((ent>0) && (i != MAX_MEMO));
   int max_memo_real = i;
   int j = -1;
   int menor;
   bool sem_espaco;
   do{
      j+=1;
      no_reservatorio = 0;
      sem_espaco = false;
      temp.open(nome_do_array[j].c_str(),ios::out|ios::app);
      reservatorio.open(caminho_reservatorio.c_str(),ios::out);
     do{
                     menor=minimo(memo,max_memo_real);
         ultima_chave=memo[menor];
         temp << itoa(memo[menor],buffer,10)<<endl;
         if(ent>0){
            feito=false;
            do{
               getline(ent,auxiliar);
               if (auxiliar!=""){
			//isso quer dizer que o registro atual do arquivo
			// "ent" é maior ou igual ao menor elemento de
			//memo,não necessita ir pro reservatório
                  if (atoi(auxiliar.c_str())>= ultima_chave){
                    memo[menor]=atoi(auxiliar.c_str());
                    feito=true;
                  }
			     // quer dizer que deu merda, ou seja, o cara lido
			     // da linha atual do arq "ent" é menor do que
			     // o menor cara de "memo", não permitindo a
			     //inserção no arq de saída (que já está classificado)
			     //, por isso, vai para o "reservatório"
                  else if (no_reservatorio < (max_reserv-1)){
                         reservatorio << auxiliar <<endl;
                         no_reservatorio+=1;
  			      }
                       else {//quer dizer que o reservatório está cheio
                             sem_espaco=true;
                             reservatorio << auxiliar <<endl;
                             no_reservatorio+=1;
                             }
               }
               else {feito=true; memo[menor]=HIGH_VALUE;}
            }
            while( (!(sem_espaco))&& (!(feito)) );
         }
      }
      while((ent>0) && (!(sem_espaco)));
      menor=minimo(memo,max_memo_real);
      while(menor!=-1){ //ainda existem elementos que estão no array 	//"memo" e que devem ser inseridos na partição de saída
         if(memo[menor]!=ultima_chave){
            temp << itoa(memo[menor],buffer,10)<<endl;}
         else {ultima_chave=HIGH_VALUE;};
     		memo[menor]=HIGH_VALUE;
         menor=minimo(memo,max_memo_real);
      }
      temp<<HIGH_VALUE<<endl;
      int registros_a_mover;
      if(no_reservatorio > 0){
         if(no_reservatorio < MAX_MEMO){
            registros_a_mover = no_reservatorio;
         }
         else {registros_a_mover=MAX_MEMO;}
      reservatorio.close();
	   //abrindo o reservatório para inserir seus elementos no array "memo"
      reservatorio.open(caminho_reservatorio.c_str(),ios::in);
      for(i=0;i<registros_a_mover;i++){
         getline(reservatorio,auxiliar2);
         memo[i]=atoi(auxiliar2.c_str());
         --no_reservatorio;
         }

         		 reservatorio.close();
         if (registros_a_mover < MAX_MEMO){
            i=registros_a_mover + 1;
            while((ent>0) && (i<MAX_MEMO)){
               getline(ent,auxiliar2);
               memo[i]=atoi(auxiliar2.c_str());
               i++;
            }
         }
      }
      menor=minimo(memo,max_memo_real);
      temp.close();
   }
   while (menor!=-1);
   ent.close();
   reservatorio.close();
   return j;
};
