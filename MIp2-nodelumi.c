/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer nodelumi.c que implementa la interfície aplicació-administrador */
/* d'un node de LUMI, sobre la capa d'aplicació de LUMI (fent crides a la */
/* interfície de la capa LUMI -fitxers lumi.c i lumi.h-).                 */
/* Autors: X, Y                                                           */
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
   int nombreUsuaris, llistaSockets[2], nBytes, canal, log;
   int final = 0;
   /* Expressions, estructures de control, crides a funcions, etc.          */

	printf("hola\n");
   cfg = fopen("MIp2-nodelumi.cfg", "r"); //r per poder llegir i escriure.
   fscanf(cfg, "%s", domini);
   fscanf(cfg, "%d", &nombreUsuaris);
   struct usuaris taulaUsuaris[nombreUsuaris];
   log = LUMIs_obrirOCrearFitxLogServidor(domini);

   llistaSockets[0] = LUMIs_Inicialitzar(domini, nombreUsuaris, cfg, taulaUsuaris); //mirar com es crida
   llistaSockets[1] = 0;

   printf("# per acabar\n");

   while (final==0)
   {
     canal = LUMIs_HaArribatAlgunaCosa(llistaSockets, 2, -1); //mirar com es crida
     
     if (canal == 0)
     {
       nBytes = read(0,entrada_t,sizeof(entrada_t));
  		 if(entrada_t[0] == '#') final = 1;
     } else
     {
       LUMIs_ServeixPeticio(canal, domini, taulaUsuaris, nombreUsuaris, log); //mirar com es crida
     }
   }
   LUMIs_tancaLog(log);
   return 0;
 }

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
