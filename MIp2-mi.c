/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer mi.c que implementa la capa d'aplicació de MI, sobre la capa de */
/* transport TCP (fent crides a la "nova" interfície de la capa TCP o     */
/* "nova" interfície de sockets)                                          */
/* Autors: X, Y                                                           */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions externes es cridessin entre elles, faria falta fer   */
/*   un #include "mi.h")                                                  */

#include "t.h"

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int MI_FuncioExterna(arg1, arg2...) { }    */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa MI.                                    */

/* Inicia l’escolta de peticions remotes de conversa a través d’un nou    */
/* socket TCP, que té una @IP local qualsevol i el #port “portTCPloc”     */
/* (és a dir, crea un socket “servidor” o en estat d’escolta – listen –). */
/* Retorna -1 si hi ha error; l’identificador del socket d’escolta de MI  */
/* creat si tot va bé.                                                    */
int MI_IniciaEscPetiRemConv(int portTCPloc)
{
	
}

/* Escolta indefinidament fins que arriba una petició local de conversa   */
/* a través del teclat o bé una petició remota de conversa a través del   */
/* socket d’escolta de MI d’identificador “SckEscMI” (un socket           */
/* “servidor”).                                                           */
/* Retorna -1 si hi ha error; 0 si arriba una petició local; SckEscMI si  */
/* arriba una petició remota.                                             */
int MI_HaArribatPetiConv(int SckEscMI)
{
	
}

/* Crea una conversa iniciada per una petició local que arriba a través   */
/* del teclat: crea un socket TCP “client” (en un #port i @IP local       */
/* qualsevol), a través del qual fa una petició de conversa a un procés   */
/* remot, el qual les escolta a través del socket TCP ("servidor") d'@IP  */
/* “IPrem” i #port “portTCPrem” (és a dir, crea un socket “connectat” o   */
/* en estat establert – established –). Aquest socket serà el que es farà */
/* servir durant la conversa.                                             */
/* Omple “IPloc*” i “portTCPloc*” amb, respectivament, l’@IP i el #port   */
/* TCP del socket del procés local.                                       */
/* El nickname local “NicLoc” i el nickname remot són intercanviats amb   */
/* el procés remot, i s’omple “NickRem*” amb el nickname remot. El procés */
/* local és qui inicia aquest intercanvi (és a dir, primer s’envia el     */
/* nickname local i després es rep el nickname remot).                    */
/* "IPrem" i "IPloc*" són "strings" de C (vectors de chars imprimibles    */
/* acabats en '\0') d'una longitud màxima de 16 chars (incloent '\0').    */
/* "NicLoc" i "NicRem*" són "strings" de C (vectors de chars imprimibles  */
/* acabats en '\0') d'una longitud màxima de 300 chars (incloent '\0').   */
/* Retorna -1 si hi ha error; l’identificador del socket de conversa de   */
/* MI creat si tot va bé.                                                 */
int MI_DemanaConv(const char *IPrem, int portTCPrem, char *IPloc, int *portTCPloc, const char *NicLoc, char *NicRem)
{
	
}

/* Crea una conversa iniciada per una petició remota que arriba a través  */
/* del socket d’escolta de MI d’identificador “SckEscMI” (un socket       */
/* “servidor”): accepta la petició i crea un socket (un socket            */
/* “connectat” o en estat establert – established –), que serà el que es  */
/* farà servir durant la conversa.                                        */
/* Omple “IPrem*”, “portTCPrem*”, “IPloc*” i “portTCPloc*” amb,           */
/* respectivament, l’@IP i el #port TCP del socket del procés remot i del */
/* socket del procés local.                                               */
/* El nickname local “NicLoc” i el nickname remot són intercanviats amb   */
/* el procés remot, i s’omple “NickRem*” amb el nickname remot. El procés */
/* remot és qui inicia aquest intercanvi (és a dir, primer es rep el      */
/* nickname remot i després s’envia el nickname local).                   */
/* "IPrem*" i "IPloc*" són "strings" de C (vectors de chars imprimibles   */
/* acabats en '\0') d'una longitud màxima de 16 chars (incloent '\0').    */
/* "NicLoc" i "NicRem*" són "strings" de C (vectors de chars imprimibles  */
/* acabats en '\0') d'una longitud màxima de 300 chars (incloent '\0').   */
/* Retorna -1 si hi ha error; l’identificador del socket de conversa      */
/* de MI creat si tot va bé.                                              */
int MI_AcceptaConv(int SckEscMI, char *IPrem, int *portTCPrem, char *IPloc, int *portTCPloc, const char *NicLoc, char *NicRem)
{
	
}

/* Escolta indefinidament fins que arriba una línia local de conversa a   */
/* través del teclat o bé una línia remota de conversa a través del       */
/* socket de conversa de MI d’identificador “SckConvMI” (un socket        */
/* "connectat”).                                                          */
/* Retorna -1 si hi ha error; 0 si arriba una línia local; SckConvMI si   */
/* arriba una línia remota.                                               */
int MI_HaArribatLinia(int SckConvMI)
{
	
}

/* Envia a través del socket de conversa de MI d’identificador            */
/* “SckConvMI” (un socket “connectat”) la línia “Linia” escrita per       */
/* l’usuari local.                                                        */
/* "Linia" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0'), no conté el caràcter fi de línia ('\n') i té una longitud       */
/* màxima de 300 chars (incloent '\0').                                   */
/* Retorna -1 si hi ha error; el nombre de caràcters n de la línia        */
/* enviada (sense el ‘\0’) si tot va bé (0 <= n <= 299).                  */
int MI_EnviaLinia(int SckConvMI, const char *Linia)
{
	
}

/* Rep a través del socket de conversa de MI d’identificador “SckConvMI”  */
/* (un socket “connectat”) una línia escrita per l’usuari remot, amb la   */
/* qual omple “Linia”, o bé detecta l’acabament de la conversa per part   */
/* de l’usuari remot.                                                     */
/* "Linia*" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0'), no conté el caràcter fi de línia ('\n') i té una longitud       */
/* màxima de 300 chars (incloent '\0').                                   */
/* Retorna -1 si hi ha error; -2 si l’usuari remot acaba la conversa; el  */
/* nombre de caràcters n de la línia rebuda (sense el ‘\0’) si tot va bé  */
/* (0 <= n <= 299).                                                       */
int MI_RepLinia(int SckConvMI, char *Linia)
{
	
}

/* Acaba la conversa associada al socket de conversa de MI                */
/* d’identificador “SckConvMI” (un socket “connectat”).                   */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int MI_AcabaConv(int SckConvMI)
{
	
}

/* Acaba l’escolta de peticions remotes de conversa que arriben a través  */
/* del socket d’escolta de MI d’identificador “SckEscMI” (un socket       */
/* “servidor”).                                                           */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int MI_AcabaEscPetiRemConv(int SckEscMI)
{
	
}

/* Si ho creieu convenient, feu altres funcions EXTERNES                  */

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
