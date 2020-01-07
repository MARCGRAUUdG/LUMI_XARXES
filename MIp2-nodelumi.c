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

#include "lumiS.h"
#include <stdbool.h>

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
   int nombreClients, llistaSockets[2], nBytes, canal, log;
   bool final = false;
   /* Expressions, estructures de control, crides a funcions, etc.          */

   cfg = fopen("MIp2-nodelumi.cfg", "r"); //r per poder llegir i escriure.
   fscanf(cfg, "%s", domini);
   fscanf(cfg, "%d", domini);
   struct adrUDP taulaClients[nClients];
   log = crearLogServidor(nomDomini);

   llistaSockets[0] = 0;
   llistaSockets[1] = LUMIs_Inicialitzar(domini, nombreClients, cfg, taulaClients); //mirar com es crida

   printf("# per acabar");

   while (!final)
   {
     canal = LUMIc_HaArribatAlgunaCosa(llistaSockets, -1, 2); //mirar com es crida
     if (canal == 0)
     {
       nBytes = read(0,teclat,sizeof(teclat));
  		 if(teclat[0] == '#') fi = true;
     } else
     {
       LUMIs_ServeixPeticio(Sck, domini, taulaClients, nombreClients, log); //mirar com es crida
     }
   }
   tancaLog(log);
   return 0;
 }

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
