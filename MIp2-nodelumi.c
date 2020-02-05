/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer nodelumi.c que implementa la interfície aplicació-administrador */
/* d'un node de LUMI, sobre la capa d'aplicació de LUMI (fent crides a la */
/* interfície de la capa LUMI -fitxers lumi.c i lumi.h-).                 */
/* Autors: Marc Grau i Xavier Roca                                        */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "MIp2-lumiS.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
   /* Declaració de variables, p.e., int n;                                 */
   FILE *cfg;
   char domini[100/*assignat pel protocol?*/], entrada_t[300];
   int nombreUsuaris, llistaSockets[2], numBytes, canal, log;
   int acabar = 0;
   /* Expressions, estructures de control, crides a funcions, etc.          */

   cfg = fopen("MIp2-nodelumi.cfg", "r"); //r per poder llegir i escriure.
   fscanf(cfg, "%s", domini);
   fscanf(cfg, "%d", &nombreUsuaris);
   struct usuaris taulaUsuaris[nombreUsuaris];
   log = LUMIs_obrirOCrearFitxLogServidor(domini);

   llistaSockets[0] = LUMIs_Inicialitzar(domini, nombreUsuaris, cfg, taulaUsuaris);
   llistaSockets[1] = 0;

   printf("Entra un #, per acabar l'execució del servidor\n");

   while (acabar==0)
   {
     canal = LUMIs_HaArribatAlgunaCosa(llistaSockets, 2, -1);
     
     if (canal == 0)
     {
       numBytes = read(0,entrada_t,sizeof(entrada_t));
  		 if(entrada_t[0] == '#') acabar = 1;
     } else
     {
       LUMIs_ServeixPeticio(canal, domini, taulaUsuaris, nombreUsuaris, log);
     }
   }
   LUMIs_tancaLog(log);
   return 0;
 }