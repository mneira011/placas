#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define L 5
#define lp 2
#define d 1
#define nfilas 256
#define V0 100
#define N 131072
#define w 1.97
#define p1i 26445
#define p1f 26547
#define p2i 39501
#define p2f 39603

int main(int argc, char **argv){
  int world_rank, world_size;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

/*
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
DECLARACION - DECLARACION - DECLARACION
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
*/
  int con;
  int con1;
  //procesadores jodidos porque les tocaron las placas
  int pj1;
  int pj2;
  int fpp;
  fpp = nfilas/world_size;
  int pi;
  pi=fpp*nfilas*world_rank;
  int pf;
  pf = (fpp*nfilas*(world_rank+1))-1;
  float *vps;
  vps = malloc(nfilas*sizeof(float));
  float *vp;
  vp = malloc(nfilas*(fpp-2)*sizeof(float));
  float *vpi;
  vpi = malloc(nfilas*sizeof(float));


  if(world_size==4){
    //a pj1 le cayo en la mitad
    pj1 =1;
    //a pj2 le cayo en la mitad
    pj2 = 2 ;
  }
  else if(world_size==8){
    //a pj1 le cayo en la mitad
    pj1 = 3;
    //a pj2 le cayo en la mitad
    pj2 = 4;
  }
  else if(world_size==16){
    //a pj1 le cayo en la mitad
    pj1 = 6;
    //a pj2 le cayo en la mitad
    pj2 = 9;
  }
  else if(world_size==32){
    //a pj1 le cayo en la mitad
    pj1 = 12;
    //a pj2 le cayo en la mitad
    pj2 = 19;
  }
  else if(world_size==64){
    //a pj1 le cayo en la mitad
    pj1 = 25;
    //a pj2 le cayo en la mitad
    pj2 = 38;
  }

  if(world_rank==0){
    //arreglo total
    float *vt;
    vt = malloc(nfilas*nfilas*sizeof(float));
    float *vei;

    vei = malloc(nfilas*sizeof(float));
  }
  else if(world_rank == (world_size-1)){
    float *ves;

    ves = malloc(nfilas*sizeof(float));
  }
  else{
    float *ves;
    ves = malloc(nfilas*sizeof(float));
    float *vei;
    vei = malloc(nfilas*sizeof(float));
  }

/*
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
INIZIALIZACION - INICIALIZACION - INICIALIZACION
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
*/
if(world_rank==0){
  //llenamos el primer procesador
  for(con=0;con<nfilas;con++){
    vps[con] = 0.0;
    vpi[con] = 0.0;
  }
  //el unico caso donde le puede tocar la placa
  if(world_size==2){

    for(con=0; con<(p1i - (nfilas));con++){
      vp[con] = 0.0;
    }

    for(con=(p1i - (nfilas));con<(p1f-nfilas);con++){
      vp[con] = V0/2.0;
    }

    for(con = (p1f-nfilas);con<nfilas*(fpp-2);con++){
      vp[con] = 0.0;
    }

  }
  //en caso de que no le toquen las placas
  else{
    for(con=0;con<nfilas*(fpp-2);con++){
      vp[con] = 0.0;
    }
  }

}
//Para el ultimo procesador
else if(world_rank == (world_size-1)){
  for(con = 0; con<nfilas;con++){
    vps[con] = 0.0;
    vpi[con] = 0.0;
  }
  //en caso que le toque la placa
  if(world_size==2){
    for(con=0; con<(p2i - (nfilas+pi));con++){
      vp[con] = 0.0;
    }

    for(con=(p2i - (nfilas+pi));con<(p2f-(nfilas+pi));con++){
      vp[con] = -V0/2.0;
    }

    for(con = (p2f-(nfilas+pi));con<nfilas*(fpp-2);con++){
      vp[con] = 0.0;
    }
  }
  //en caso que no le toque el procesador
  else{
    for(con=0;con<nfilas*(fpp-2);con++){
      vp[con] = 0.0;
    }
  }

}
//los procesadores ensanduchados entre el primero y el ultimo
else{
  //el primer procesador jodido
  if(world_rank==pj1 ){
    //lenamos los extremos
    for(con = 0; con<nfilas;con++){
      vps[con]= 0.0;
      vpi[con]= 0.0;
    }
    //llenamos el sanduche de los sanduches
    for(con = 0 ; con<(p1i-(pi+nfilas)) ; con++){
      vp[con] = 0.0;
    }
    for(con =  (p1i-(pi+nfilas)); con<(p1f-(pi+nfilas)) ; con++){
      vp[con] = V0/2.0;
    }
    for(con = (p1f-(pi+nfilas)); con<nfilas*(fpp-2) ; con++){
      vp[con] = 0.0;
    }
  }
  //el segundo procesador jodido
  else if(world_rank==pj2 ){
    //llenamos los extremos
    for(con = 0; con<nfilas;con++){
      vps[con]= 0.0;
      vpi[con]= 0.0;
    }
    //llenamos el sanduche de los sanduches
    for(con = 0 ; con<(p2i-(pi+nfilas)) ; con++){
      vp[con] = 0.0;
    }
    for(con =  (p2i-(pi+nfilas)); con<(p2f-(pi+nfilas)) ; con++){
      vp[con] = -V0/2.0;
    }
    for(con = (p2f-(pi+nfilas)); con<nfilas*(fpp-2) ; con++){
      vp[con] = 0.0;
    }

  }
  // en caso que no le toque ninguna placa
  else{
    for(con = 0; con<nfilas;con++){
      vps[con]= 0.0;
      vpi[con]= 0.0;
    }
    for(con=0;con<(nfilas*(fpp-2));con++){
      vp[con] = 0.0;
    }

  }
}



/*
--------------------------------------------------------------
--------------------------------------------------------------
FIN DE LA INIZIALIZACION BASICA, FALTA COMUNICAR FRONTERAS PARA CALCULAR
--------------------------------------------------------------
--------------------------------------------------------------

*/







/*
--------------------------------------------------------------
--------------------------------------------------------------
COMUNICAR E IMPRIMIR EL ESTADO ACTUAL DEL GRID
--------------------------------------------------------------
--------------------------------------------------------------
*/

//iteramos sobre los procesadores
for(con=0;con<world_size;con++){
    if(world_rank==con){
      //cada proc imprime los suyo

      //la primera linea
      for(con1 = 0 ; con1<nfilas;con1++){
        printf("%f ",vps[con1] );
      }
      printf("\n");
      //las lineas del sanduche
      for(con1 = 0 ; con1<nfilas*(fpp-2);con1++){
        if(con1%(nfilas)==(nfilas-1) && con1!=0){
          printf("%f \n",vp[con1] );
        }else{
          printf("%f ",vp[con1] );
        }
      }
      //la ultima linea
      for(con1 = 0 ; con1<nfilas;con1++){
        printf("%f ",vpi[con1] );
      }
      printf("\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

 printf("%d",world_size);



  MPI_Finalize();
  return 0;
}
