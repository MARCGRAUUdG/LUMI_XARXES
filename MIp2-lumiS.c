/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer lumiC.c que implementa la capa d'aplicació de MI, sobre la capa */
/* de transport UDP (fent crides a la "nova" interfície de la capa UDP o  */
/* "nova" interfície de sockets), però només la part servidora            */
/* Autors: X, Y                                                           */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions EXTERNES es cridessin entre elles, faria falta fer   */
/*   un #include "lumi.h")                                                */

#include "t.h"
#include "dnsC.h"
#include <stdio.h>
#include <string.h>

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */
/* Com a mínim heu de fer les següents funcions INTERNES:                 */

int Log_CreaFitx(const char *NomFitxLog);
int Log_Escriu(int FitxLog, const char *MissLog);
int Log_TancaFitx(int FitxLog);


/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int LUMIs_FuncioExterna(arg1, arg2...) { } */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa LUMI, la part del servidor             */

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Crea un fitxer de "log" de nom "NomFitxLog".                           */
/* "NomFitxLog" és un "string" de C (vector de chars imprimibles acabat   */
/* en '\0') d'una longitud qualsevol.                                     */
/* Retorna -1 si hi ha error; l'identificador del fitxer creat si tot va  */
/* bé.                                                                    */
int Log_CreaFitx(const char *NomFitxLog)
{
	char diaHora[50];
	DiaHora(diaHora);
	int fitxerIden = FitxLog = open(NomFitxLog, O_WRONLY/*obrir mode escriptura*/ | O_APPEND /*escriptura des del final*/| O_CREAT/*si no existeix el crea*/);
	Log_Escriu(fitxerIden, diaHora);
	return fitxerIden;//retorna l'identificador del fitxer?
}

/* Escriu al fitxer de "log" d'identificador "FitxLog" el missatge de     */
/* "log" "MissLog".                                                       */
/* "MissLog" és un "string" de C (vector de chars imprimibles acabat      */
/* en '\0') d'una longitud qualsevol.                                     */
/* Retorna -1 si hi ha error; el nombre de caràcters del missatge de      */
/* "log" (sense el '\0') si tot va bé                                     */
int Log_Escriu(int FitxLog, const char *MissLog)
{
	strcat(MissLog, "\n"); //inserim salt de línia
	int n = write(FitxLog, MissLog, strlen(MissLog));
	if (n < 0)
	{
		return -1;
	}
	return n-1;
}

/* Tanca el fitxer de "log" d'identificador "FitxLog".                    */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int Log_TancaFitx(int FitxLog)
{
	return close(FitxLog);
}

int LUMI_S_Registrar(char *miss, struct adrUDP *taulaClients, int nClients, char *IP, int port)
{
	char usuari[299];
	int tipus, client = 0;
	bool trobat = false;

	while (!trobat && client != nClients)
	{
		if (strcpy(taulaClients[i].nomClient, usuari) == 0))
		{
			trobat = true;
			strcpy(taulaClients[i].ipUDP, IP);
			taulaClients[i].portUDP = port;
			return 0; //tot ok, ha trobat el  client
		}
	}
	return 1; //no ha trobat l'usuari
}

int LUMI_S_Desregistrar(char *miss, struct adrUDP *taulaClients, int nClients)
{
	char usuari[299];
	int tipus, client = 0;
	bool trobat = false;

	while (!trobat && client != nClients)
	{
		if (strcpy(taulaClients[i].nomClient, usuari) == 0))
		{
			trobat = true;
			strcpy(taulaClients[i].ipUDP, "-");
			taulaClients[i].portUDP = 0;
			taulaClients[i].fallades = 0;
			return 0; //tot ok, ha trobat el  client
		}
	}
	return 1; //no ha trobat l'usuari
}

int LUMI_S_PeticioLoc(char *miss, char *domini, char *username)
{
	username = strtok(miss, "@#");
	username[username.sizeof()] = '\0';
	domini = strtok(miss, "@#");
	domini[domini.sizeof()] = '\0';

	return 0;
}

int LUMI_S_RespostaLoc(char *miss, char *domini, char *username)
{
	char *res = strtok(miss, "@#");
	username = strtok(miss, "@#");
	username[username.sizeof()] = '\0';
	domini = strtok(miss, "@#");
	domini[domini.sizeof()] = '\0';

	return 0;
}

int LUMI_S_Inicialitzar(char *nomDomini, int nClients, FILE *cfg, struct adrUDP *taulaClients)
{

}

int LUMI_S_ServeixPeticio(int Sck, char *nomDomini, struct adrUDP *taulaClients, int nClients, int log)
{

}

/* Si ho creieu convenient, feu altres funcions INTERNES                  */
