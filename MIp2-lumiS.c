/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer lumiC.c que implementa la capa d'aplicació de MI, sobre la capa */
/* de transport UDP (fent crides a la "nova" interfície de la capa UDP o  */
/* "nova" interfície de sockets), però només la part servidora            */
/* Autors: Marc Grau i Xavier Roca                                                      */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions EXTERNES es cridessin entre elles, faria falta fer   */
/*   un #include "lumi.h")                                                */

#include "MIp2-t.h"
#include "MIp2-dnsC.h"
#include "MIp2-lumiS.h"
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

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */
/* Com a mínim heu de fer les següents funcions INTERNES:                 */

int Log_CreaFitx(const char *NomFitxLog);
int Log_Escriu(int FitxLog, const char *MissLog);
int Log_TancaFitx(int FitxLog);
int codificarRespostaLocalitzarS(char *missLoc, int codi, char *res);
int codificarRespostaRegiste(int codi, char tipus, char *res);
int RegistrarUsuari(struct usuaris *taulaUsuaris, int nUsuaris, char *missatgeCodificat, char *IP, int port);
int DesregistrarUsuari(struct usuaris *taulaUsuaris, int nUsuaris, char *missatgeCodificat);
int TractarPeticioLoc(char *miss, char *dominiPeticio, int nClients, char *IPEntrada, int portEntrada, int Sck, int log, struct usuaris *taulaClients);
int buscarUsuariRegistrat(struct usuaris *taulaUsuaris, char *usernamePeticio, int numClients, char * IPPeticio, int *portPeticio);


/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int LUMIs_FuncioExterna(arg1, arg2...) { } */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa LUMI, la part del servidor             */

/* Crea el nom del fitxer de log, i seguidament el crea, o l'obre si ja existeix	*/
/* Retorna -1 si hi ha error, o l'identificador del fitxer si tot ha anat bé        */
int LUMIs_obrirOCrearFitxLogServidor(char *nomDomini)
{
	char nomfitx[40];
	sprintf(nomfitx, "nodelumi-%s.log", nomDomini);

	return Log_CreaFitx(nomfitx);
}

/* Tanca el fitxer de log. Retorna 1 si tot va bé, -1 sinó. */
int LUMIs_tancaLog(int log){
	return Log_TancaFitx(log);
}

/* Retorna la funció d'T_HaArribatAlgunaCosaEnTemps */
int LUMIs_HaArribatAlgunaCosa(int *llistaSck, int midaLlista, int temps)
{
	return T_HaArribatAlgunaCosaEnTemps(llistaSck, midaLlista, temps);
}

/* Escriu una linia de text al fitxer de log amb nom "nomfitx"			  */
/* Retorna -1 si hi ha error, o el nombre de caràcters de la línia altrament  */
int escriureLiniaFitxLog(int nomfitx, char codi, char *IP, int port, char *missatge, int bytes){
	char liniaLog[300];
	sprintf(liniaLog, "%c:  %s/UDP/%d,  %s, %d", codi, IP, port, missatge, bytes);
	return Log_Escriu(nomfitx, liniaLog);
}

int LUMIs_Inicialitzar(char *nomDomini, int nUsuaris, FILE *cfg, struct usuaris *taulaUsuaris){
	int socketServidor = UDP_CreaSock("0.0.0.0", 6000);

	int i;

	for(i = 0; i < nUsuaris; i++){
		fscanf(cfg, "%s\n", taulaUsuaris[i].usuari);
		strcpy(taulaUsuaris[i].sckLUMI, "0");
	}

	return socketServidor;
}

int LUMIs_ServeixPeticio(int Sck, char *domini, struct usuaris *taulaUsuaris, int nUsuaris, int fitxLog){
	char missatge[300], IP[16], resRegDes[2], resLoc[300];
	int port, bytes;
	bytes = UDP_RepDe(Sck, IP, &port, missatge, sizeof(missatge));

	escriureLiniaFitxLog(fitxLog, 'R', IP, port, missatge, bytes);
	
	printf("MISSATGE REGISTRE: %s\n",missatge);

	if (missatge[0] == 'R')
	{
		int codiReg = RegistrarUsuari(taulaUsuaris, nUsuaris, missatge, IP, port);
		codificarRespostaRegiste(codiReg, 'C', resRegDes);
		int bytes = UDP_EnviaA(Sck, IP, port, resRegDes, sizeof(resRegDes));
		escriureLiniaFitxLog(fitxLog, 'E', IP, port, resRegDes, bytes);
	}
	else if (missatge[0] == 'D')
	{
		int codDes = DesregistrarUsuari(taulaUsuaris, nUsuaris, missatge);
		codificarRespostaRegiste(codDes, 'C', resRegDes);
		int bytes = UDP_EnviaA(Sck, IP, port, resRegDes, sizeof(resRegDes));
		escriureLiniaFitxLog(fitxLog, 'E', IP, port, resRegDes, bytes);
	}
	else if (missatge[0] == 'L')
	{
		TractarPeticioLoc(missatge, domini, nUsuaris, IP, port, Sck, fitxLog, taulaUsuaris);
	}
	else if (missatge[0] == 'S')
	{
		printf("Missatge original abans mètode %s\n", missatge);
		TractarPeticioRespLoc(missatge, domini, nUsuaris, IP, port, Sck, fitxLog, taulaUsuaris);
	}

	return 1;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

int RegistrarUsuari(struct usuaris *taulaUsuaris, int nUsuaris, char *missatgeCodificat, char *IP, int port){
	char usuari[299], IPPort[25];
	int i = 0;
  //agafem  el nom d'usuari del missatge de registre del client
	strcpy(usuari,missatgeCodificat+1); //missatge+1    RUsername

	sprintf(IPPort, "%s-%d", IP, port);  //guardem en una string el format desitjat per guardar la IP + el port

	while (i != nUsuaris)
	{
		if (strcmp(taulaUsuaris[i].usuari, usuari) == 0)  //si té el mateix nom d'usuari
		{
			//printf("HE DONAT D'ALTA L'USUARI %s\n", taulaUsuaris[i].usuari);
			strcpy(taulaUsuaris[i].sckLUMI, IPPort);  //hi posem la ip i el port en el format desitjat, de l'usuari que s'ha connectat
			//printf("LA IP ÉS %s\n", IPPort);
			return 0; //tot ok, ha trobat el  client
		}
		i++;
	}
	return 1; //no ha trobat l'usuari

}

int DesregistrarUsuari(struct usuaris *taulaUsuaris, int nUsuaris, char *missatgeCodificat){
	char usuari[299];
	int i = 0;
	//agafem  el nom d'usuari del missatge de desregistre del client
	strcpy(usuari,missatgeCodificat+1); //missatge+1    RUsername

	while (i != nUsuaris)
	{
		if (strcmp(taulaUsuaris[i].usuari, usuari) == 0)
		{
			strcpy(taulaUsuaris[i].sckLUMI, "0");
			return 0; //tot ok, ha trobat el  client
		}
		i++;
	}
	return 1; //no ha trobat l'usuari
}

/* Crea un fitxer de "log" de nom "NomFitxLog".                           */
/* "NomFitxLog" és un "string" de C (vector de chars imprimibles acabat   */
/* en '\0') d'una longitud qualsevol.                                     */
/* Retorna -1 si hi ha error; l'identificador del fitxer creat si tot va  */
/* bé.                                                                    */
int Log_CreaFitx(const char *NomFitxLog)
{
	int fitxerIden;
	fitxerIden = open(NomFitxLog, O_WRONLY/*obrir mode escriptura*/ | O_APPEND /*escriptura des del final*/| O_CREAT/*si no existeix el crea*/);
	return fitxerIden;//retorna l'identificador del fitxer
}



/* Escriu al fitxer de "log" d'identificador "FitxLog" el missatge de     */
/* "log" "MissLog".                                                       */
/* "MissLog" és un "string" de C (vector de chars imprimibles acabat      */
/* en '\0') d'una longitud qualsevol.                                     */
/* Retorna -1 si hi ha error; el nombre de caràcters del missatge de      */
/* "log" (sense el '\0') si tot va bé                                     */
int Log_Escriu(int FitxLog, const char *MissLog)
{
	int nombreChars;
    nombreChars = write(FitxLog, MissLog, strlen(MissLog));
    if(nombreChars < 0) return -1;
    nombreChars = write(FitxLog, "\n", 1);
    if(nombreChars < 0) return -1;
    return nombreChars-1;
}

/* Tanca el fitxer de "log" d'identificador "FitxLog".                    */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int Log_TancaFitx(int FitxLog)
{
	return close(FitxLog);
}

/* Si ho creieu convenient, feu altres funcions INTERNES                  */

int codificarRespostaRegiste(int codi, char tipus, char *res)
{
	if(tipus != 'C'){
		return -1;
	}
	return sprintf(res, "%c%d", tipus, codi);
}

int codificarRespostaLocalitzacio(int codi, char *res, char *adrMI, char *IP, int port)
{
  return sprintf(res, "S%d%s#%s#%d", codi,adrMI,IP,port);
}

//Busca un usuari a la taula de Usuaris registrats, retorna la posició a la taula d'Usuaris si el troba, -1 si no existeix a la taula.
int buscarUsuariRegistrat(struct usuaris *taulaUsuaris, char *usernamePeticio, int numClients, char * IPPeticio, int *portPeticio)
{
	int i=0;
    int trobat = 0;
    //printf(" Hola hilota\n");

	while (i<numClients && !trobat)
	{
		if (strcmp(taulaUsuaris[i].usuari,usernamePeticio)==0) trobat = 1;
		else i++;
	}

	if (trobat==1)
	{
		if (strcmp(taulaUsuaris[i].sckLUMI,"0")==0){
			strcpy(IPPeticio, "0");
			*portPeticio=0;
			return i;
		}
		else{
			char AdrMIUsuari[25];
			strcpy(AdrMIUsuari,taulaUsuaris[i].sckLUMI);
			char IPUsuari[16];
			strcpy(IPUsuari,strtok(AdrMIUsuari, "-"));
			int portUsuari;
			portUsuari = atoi(strtok(NULL, "-"));
			//printf("%d", portUsuari);

			strcpy(IPPeticio, IPUsuari);
			*portPeticio=portUsuari;
			return i;
		}
	}
	else return -1;
}

int TractarPeticioLoc(char *miss, char *nostreDomini, int numUsuaris, char *IPEntrada, int portEntrada, int Sck, int log, struct usuaris *taulaUsuaris)
{
	char missatgeOriginal[300];
	strcpy(missatgeOriginal,miss);
	
	char usernamePeticio[74];
	strcpy(usernamePeticio, strtok(miss+1, "@#"));
	usernamePeticio[sizeof(usernamePeticio)] = '\0';
	char dominiPeticio[74];
	strcpy(dominiPeticio, strtok(NULL, "#"));
	dominiPeticio[sizeof(dominiPeticio)] = '\0';
		
	//printf("Username %s\n", usernamePeticio);
	
	//printf("Domini %s\n", dominiPeticio);

	char IPPeticio[16];
	char missRespLoc[300];
    int numBytes;

	if((strcmp(dominiPeticio,nostreDomini))==0) //si es demana connectar amb un usuari del nostre domini...
	{
		//printf("EL NOSTRE DOMINI ES %s\n",nostreDomini);

		int portPeticio, posUsuari;

		posUsuari = buscarUsuariRegistrat(taulaUsuaris, usernamePeticio, numUsuaris, IPPeticio, &portPeticio); //busquem un usuari
		//printf(" POSICIO DE LUSER	%d\n", posUsuari);

		if (posUsuari==-1) //no s'ha trobat l'usuari
		{
			codificarRespostaLocalitzacio(3,missRespLoc, "0", "0", 0);
			numBytes = UDP_EnviaA(Sck, IPEntrada, portEntrada, missRespLoc, strlen(missRespLoc));
			escriureLiniaFitxLog(log,'E', IPEntrada, portEntrada, missRespLoc, numBytes);
		}
		else if (strcmp(taulaUsuaris[posUsuari].sckLUMI,"0")==0) //usuari offline
		{
			codificarRespostaLocalitzacio(2,missRespLoc, "0", "0" , 0);
			numBytes = UDP_EnviaA(Sck, IPEntrada, portEntrada, missRespLoc, strlen(missRespLoc));
			escriureLiniaFitxLog(log,'E', IPEntrada, portEntrada, missRespLoc, numBytes);
		}
		else //ha trobat usuari, enviem peticio a l'usuari
		{
			
			numBytes = UDP_EnviaA(Sck, IPPeticio, portPeticio, missatgeOriginal, strlen(missatgeOriginal));
			escriureLiniaFitxLog(log,'E', IPPeticio, portPeticio, missatgeOriginal, numBytes);
		}
			
	}
	else //si no es el domini que correspon, busca el domini corresponent...
	{
			if (DNSc_ResolDNSaIP(dominiPeticio, IPPeticio)==-1) //domini erroni, no existeix cap domini amb el nom dominiPeticio
			{
				codificarRespostaLocalitzacio(1,missRespLoc, "0", "0", 0);
				numBytes = UDP_EnviaA(Sck, IPEntrada, portEntrada, missRespLoc, strlen(missRespLoc));
				escriureLiniaFitxLog(log,'E', IPEntrada, portEntrada, missRespLoc, numBytes);
			}
			else{
				numBytes = UDP_EnviaA(Sck, IPPeticio, 6000, missatgeOriginal, strlen(missatgeOriginal));
				//printf("NOMBRE DE BYTES %d\n", numBytes);
				escriureLiniaFitxLog(log, 'E', IPPeticio, 6000, missatgeOriginal, numBytes);
			}
	 }

	return 0;
}

int TractarPeticioRespLoc(char *miss, char *nostreDomini, int numUsuaris, char *IPEntrada, int portEntrada, int Sck, int log, struct usuaris *taulaUsuaris)
{
	char missatgeOriginal[300];
	strcpy(missatgeOriginal,miss);
	
	printf("Missatge original %s\n", miss);
	
	char usernamePeticio[74];
	strcpy(usernamePeticio, strtok(miss+2, "@#"));
	usernamePeticio[sizeof(usernamePeticio)] = '\0';
	char dominiPeticio[74];
	strcpy(dominiPeticio, strtok(NULL, "#"));
	dominiPeticio[sizeof(dominiPeticio)] = '\0';
	
	printf("Username %s\n", usernamePeticio);
	
	printf("Domini %s\n", dominiPeticio);
	
		
	char IPPeticio[16];
	char missRespLoc[300];
	int nBytes, portPeticio;
	
	if((strcmp(dominiPeticio,nostreDomini))==0){
		buscarUsuariRegistrat(taulaUsuaris, usernamePeticio, numUsuaris, IPPeticio, &portPeticio);
		printf("Missatge de resposata de LOC %s\n", missatgeOriginal);
		nBytes = UDP_EnviaA(Sck, IPPeticio, portPeticio, missatgeOriginal, strlen(missatgeOriginal));
		printf("Num de bytes de envia a del mateix domini %d\n", nBytes);
		escriureLiniaFitxLog(log, 'E', IPPeticio, portPeticio, missatgeOriginal, nBytes);
	}
	else{
		DNSc_ResolDNSaIP(dominiPeticio, IPPeticio);
		printf("Missatge de resposata de LOC %s\n", missatgeOriginal);
		nBytes = UDP_EnviaA(Sck, IPPeticio, 6000, missatgeOriginal, strlen(missatgeOriginal));
		printf("Num de bytes de envia a %d\n", nBytes);
		escriureLiniaFitxLog(log, 'E', IPPeticio, 6000, missatgeOriginal, nBytes);
	}

	return 0;
}
