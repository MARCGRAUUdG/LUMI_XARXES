/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer dnsC.c que "implementa" la capa d'aplicació DNS (part client),  */
/* o més ben dit, que encapsula les funcions de la interfície de sockets  */
/* de la part DNS, en unes altres funcions més simples i entenedores: la  */
/* "nova" interfície de la capa DNS (part client).                        */
/* Autors: Marc Grau i Xavier Roca                                                         */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions EXTERNES es cridessin entre elles, faria falta fer   */
/*   un #include "lumi.h")                                                */

#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include "MIp2-dnsC.h"

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int DNSc_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa DNS, la part del client                */

/* Donat el nom DNS "NomDNS" obté la corresponent @IP i l'escriu a "IP*"  */
/* "NomDNS" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud qualsevol, i "IP*" és un "string" de C (vector de */
/* chars imprimibles acabat en '\0') d'una longitud màxima de 16 chars    */
/* (incloent '\0').                                                       */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé     */
int DNSc_ResolDNSaIP(const char *NomDNS, char *IP)
{
	struct hostent *dadesHOST;
	struct in_addr **adrHOST;
	int i;

	if ((dadesHOST = gethostbyname(NomDNS)) == NULL) 
	{
		herror("gethostbyname");
		return -1;
	}

	adrHOST = (struct in_addr **) dadesHOST->h_addr_list;
	
	for(i = 0; adrHOST[i] != NULL; i++) 
	{
		strcpy(IP , inet_ntoa(*adrHOST[i]));
		return 0;
	}
	
	return -1;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
