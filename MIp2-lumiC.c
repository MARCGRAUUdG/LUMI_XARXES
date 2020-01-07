/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer lumiC.c que implementa la capa d'aplicació de MI, sobre la capa */
/* de transport UDP (fent crides a la "nova" interfície de la capa UDP o  */
/* "nova" interfície de sockets), però només la part client               */
/* Autors: X, Y                                                           */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions EXTERNES es cridessin entre elles, faria falta fer   */
/*   un #include "lumi.h")                                                */

#include "t.h"
#include "dnsC.h"

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
/* des d'altres fitxers, p.e., int LUMIc_FuncioExterna(arg1, arg2...) { } */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa LUMI, la part del client               */

/* Codifica el misstge de registre/desregistre segons el protocol acordat */
/* Retorna els bytes del missatge codificat o -1 si hi ha error			  */
int codificarMissatgeRegistre(char *text, char tipus, char *miss){
    if(tipus == 'R' || tipus == 'D'){ //Registre o Desregistre
		return sprintf(miss, "%c%s", tipus, text);
	}
	else return -1;
}

/* Codifica el missatge de localització en format L@Mi(trucat)#@Mi(trucador) */
/* Retorna els bytes del missatge codificat					 				 */
int codificarMissatgeLocalitzacio(char *trucat, char *trucador, char *res)
{
	return sprintf(res, "L%s#%s", trucat, trucador);
}


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

/* Mètodes previs per al fitxer de Logs 								  */

/* Obre el fitxer de log, o en crea un si no existeix					  */
/* Retorna -1 si hi ha error, 1 si no. 									  */
int obrirCrearFitxLogClient(char *MI)
{
	char nomfitx[50];
	sprintf(nomfitx, "p2p-%s.log", MI);
	
	return Log_CreaFitx(nomfitx);
}

/* Escriu una linia de text al fitxer de log amb nom "nomfitx"			  */
int escriureLiniaFitxLog(int nomfitx, char codi, char *IP, int port, char *miss, int nBytes)
{
	char liniaLog[100];
	sprintf(liniaLog, "%c:  %s/UDP/%d,  %s, %d", codi, IP, port, miss, nBytes);
	return Log_Escriu(nomfitx, liniLog);
}

/* Mètodes del client 													  */

/* Crea el missatge per registrar segons protocol i s'envia al servidor per efectuar el registre
 * Té un timeout de 10ms i s'intenta fins 3 vegades 					  */
int LUMIc_Registrar(int Sck, char *adrMI, char *IPdom, int log)
{
	int i = 0, j = 0, nBytes, tSck[1], portAux;
	char nom[100], aux, domini[100], miss[150], IPaux[16];
	
	do{
		aux = adrMI[i];
		nom[i] = aux;
		i++;
	}while(aux != '@');
	
	nom[i-1] = '\0';
	while(aux != '\0'){
		aux = adrMI[i];
		domini[j] = aux;
		j++; i++;
	}
	domini[j] = '\0';
	
	nBytes = codificarMissatgeRegistre(nom, 'R', miss);
	
	ResolDNSaIP(domini, IPdom);
		
	nBytes = UDP_EnviaA(Sck, IPdom, PORT_UDP, miss, nBytes);
	escriureLiniaLog(log, 'E', IPdom, PORT_UDP, miss, nBytes);  //E d'enviat
	
	tSck[0] = Sck;
	i = 0;
	while(i < 3){		//s'intenta fins a 3 vegades
		int canal = HaArribatAlgunaCosaEnTemps(tSck, 1, 10);
		if(canal < 0) i++;
		else{
			nBytes = UDP_RepDe(Sck, IPaux, &portAux, miss, sizeof(miss));
			escriureLiniaLog(log, 'R', IPaux, portAux, miss, nBytes);
			
			if(miss[0] != 'C') return -1;
			else return miss[1] - '0';
		}
	}
	return -1;
}

/* Crea el missatge per desregistrar segons protocol i l'envia al servidor per desregistrar-se.
 * Té un timeout de 10ms i s'intenta fins a 3 vegades */
int LUMIc_Desregistrar(int Sck, char *adrMI, char *IPdom, int log)
{
	int i = 0, nBytes, portAux, tSck[1];
	char nom[100], aux, miss[150], IPaux[16];
	
	do{
		aux = adrMI[i];
		nom[i] = aux;
		i++;
	}while(aux != '@');
	
	nom[i-1] = '\0';
		
	nBytes = codificarMissatgeRegistre(nom, 'D', miss);
		
	nBytes = UDP_EnviaA(Sck, IPdom, PORT_UDP, miss, nBytes);
	escriureLiniaLog(log, 'E', IPdom, PORT_UDP, miss, nBytes);  //E d'enviat
	
	tSck[0] = Sck;
	i = 0;
	while(i < 3){		//s'intenta fins a 3 vegades
		int canal = HaArribatAlgunaCosaEnTemps(tSck, 1, 10);
		if(canal < 0) i++;
		else{
			nBytes = UDP_RepDe(Sck, IPaux, &portAux, miss, sizeof(miss));
			escriureLiniaLog(log, 'R', IPaux, portAux, miss, nBytes);
			
			if(miss[0] != 'C') return -1;
			else return miss[1] - '0';
		}
	}
	return -1;
}

/* Crea el format PLUMI i l'envia al servidor. Espera resposta 3 cops amb un timeout de 50ms).
 * Retorna -1 si hi ha error (no rep res), 1 si tot va bé. */
int LUMIc_Localitzar(const int Sck, char *adrMI, char *IPdom, char *MIloc, char *ipTCP, int *portTCP, int log)
{
	char res[300], IPrem[16], miss[300];
	int compt = 0, portRem, nBytes;
		
	codificarMissatgeLocalitzacio(MIloc, adrMI, res);
	
	while(compt < 3){	//s'intenta 3 vegades
		nBytes = UDP_EnviaA(Sck, IPdom, PORT_UDP, res, strlen(res));
		escriureLiniaLog(log, 'E', IPdom, PORT_UDP, res, nBytes);  //E d'enviat
		
		int llistaSck[1];
		llistaSck[0] = Sck;
		
		if(HaArribatAlgunaCosaEnTemps(llistaSck, 1, 50) == Sck){
			nBytes = UDP_RepDe(Sck, IPrem, &portRem, miss, 300);
			escriureLiniaLog(log, 'R', IPrem, portRem, miss, nBytes);
						
			if(miss[1] == '1'){
				extreureIPport(ipTCP, portTCP, miss);
				return 1;
			}
			else return miss[1] - '0';
		}
		compt++;
	}
	return -1; //No rep res;
}

/* Crea un socket UDP 													  */
/* Retorna el socket UDP creat 											  */
int LUMIc_CrearSocketUDP(int port, char *IP){
	int sck;
	if((sck = UDP_CreaSock(IP, port)) == -1){
		return -1;
	}
	return sck;
}

/* Tanca Sck															  */
/* Retorna 1 si va bé, -1 si no. 									  */
int LUMIc_TancarSocketUDP(int Sck)
{
	return close(Sck);
}

/* Si ho creieu convenient, feu altres funcions INTERNES                  */

