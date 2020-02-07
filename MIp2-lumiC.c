/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer lumiC.c que implementa la capa d'aplicació de MI, sobre la capa */
/* de transport UDP (fent crides a la "nova" interfície de la capa UDP o  */
/* "nova" interfície de sockets), però només la part client               */
/* Autors: Marc Grau i Xavier Roca                                        */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions EXTERNES es cridessin entre elles, faria falta fer   */
/*   un #include "lumi.h")                                                */

#include "MIp2-t.h"
#include "MIp2-dnsC.h"
#include "MIp2-lumiC.h"

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */
/* Com a mínim heu de fer les següents funcions INTERNES:                 */

int codificarMissatgeRegistre(char *usuari, char tipus, char *missatgeCodificat);
int codificarMissatgeLocalitzacio(char *trucat, char *trucador, char *missatgeCodificat);
int codificarRespostaLocalitzacioC(int codi, char *res, char *adrMI, char *IP, int port);
int Log_CreaFitxC(const char *NomFitxLog);
int Log_EscriuC(int FitxLog, const char *MissLog);
int Log_TancaFitxC(int FitxLog);
int escriureLiniaLog(int nomfitx, char codi, char *IP, int port, char *missatge, int bytes);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int LUMIc_FuncioExterna(arg1, arg2...) { } */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa LUMI, la part del client               */

/* Mètodes previs per al fitxer de Logs 								  */

/* Crea el nom del fitxer de log, i seguidament el crea, o l'obre si ja existeix	*/
/* Retorna -1 si hi ha error, o l'identificador del fitxer si tot ha anat bé        */
int LUMIc_obrirOCrearFitxLogClient(char *adrMI)
{
	char nomfitx[40];
	sprintf(nomfitx, "p2p-%s.log", adrMI);

	return Log_CreaFitxC(nomfitx);
}

/* Tanca el fitxer de log. Retorna 1 si tot va bé, -1 sinó. */
int LUMIc_tancaLog(int log)
{
	return Log_TancaFitxC(log);
}


/* Crea el missatge per registrar segons protocol i s'envia al servidor per efectuar el registre */
/* Té un timeout de 10ms i s'intenta fins 3 vegades 					  */
int LUMIc_RegistrarUsuari(int Sck, char *adrMI, char *IPDom, int fitxLog)
{
	char usuari[299], domini[20], missatgeCodificat[2];
	int nBytes, tSck[1], portAux, canal;
	char IPaux[16];
	int i=0;

	separaUsuariDomini(adrMI,usuari,domini);  //obtenim l'usuari i el domini de l'adreça MI

	int numBytes;
	
	numBytes = codificarMissatgeRegistre(usuari, 'R', missatgeCodificat);
	

	DNSc_ResolDNSaIP(domini, IPDom);

	nBytes = UDP_EnviaA(Sck, IPDom, 6000, missatgeCodificat, strlen(missatgeCodificat));

	escriureLiniaLog(fitxLog, 'E', IPDom, 6000, missatgeCodificat, nBytes);  //E d'enviat

	tSck[0] = Sck;
	
	while(i < 3){		//s'intenta fins a 3 vegades
		canal = T_HaArribatAlgunaCosaEnTemps(tSck, 1, 200);
		if(canal < 0) i++;
		else{
			nBytes = UDP_RepDe(Sck, IPaux, &portAux, missatgeCodificat, sizeof(missatgeCodificat));
			escriureLiniaLog(fitxLog, 'R', IPaux, portAux, missatgeCodificat, nBytes);

			if(missatgeCodificat[0] != 'C') return -1;
			else return missatgeCodificat[1] - '0';
		}
	}
	return -1;
}

/* Crea el missatge per desregistrar segons protocol i l'envia al servidor per desregistrar-se.
 * Té un timeout de 10ms i s'intenta fins a 3 vegades */
int LUMIc_DesregistrarUsuari(int Sck, char *adrMI, char *IPDom, int fitxLog)
{
	int i = 0, numBytes, portAux, tSck[1], canal;
	char usuari[299], aux, missatgeCodificat[300], IPaux[16];

	//S'HA DE PROVAR SI FUNCIONA
	strcpy(usuari, strtok(adrMI, "@"));
	
	usuari[strlen(usuari)] = '\0';

	/*do{
		aux = adrMI[i];
		nom[i] = aux;
		i++;
	}while(aux != '@');

	usuari[i-1] = '\0';*/

	numBytes = codificarMissatgeRegistre(usuari, 'D', missatgeCodificat);

	numBytes = UDP_EnviaA(Sck, IPDom, 6000, missatgeCodificat, numBytes);
	escriureLiniaLog(fitxLog, 'E', IPDom, 6000, missatgeCodificat, numBytes);  //E d'enviat

	tSck[0] = Sck;
	i = 0;
	while(i < 3){		//s'intenta fins a 3 vegades
		canal = T_HaArribatAlgunaCosaEnTemps(tSck, 1, 200);
		if(canal < 0) i++;
		else{
			numBytes = UDP_RepDe(Sck, IPaux, &portAux, missatgeCodificat, sizeof(missatgeCodificat));
			escriureLiniaLog(fitxLog, 'R', IPaux, portAux, missatgeCodificat, numBytes);

			if(missatgeCodificat[0] != 'C') return -1;
			else return missatgeCodificat[1] - '0';
		}
	}
	return -1;
}

/* Crea el format PLUMI i l'envia al servidor. Espera resposta 3 cops amb un timeout de 50ms).
/* Crea el format PLUMI i l'envia al servidor. Espera resposta 3 cops amb un timeout de 50ms).
 * Retorna -1 si hi ha error (no rep res), o el codi segons el protocol. */
int LUMIc_Localitzar(int Sck, char *adrMI, char *IPDom, char *MIloc, char *ipTCP, int *portTCP, int fitxLog)
{
	char res[300], IPrem[16], missatgeCodificat[300];
	int compt = 0, portRem, numBytes;

	codificarMissatgeLocalitzacio(adrMI, MIloc, res);

	while(compt < 3){	//s'intenta 3 vegades
		numBytes = UDP_EnviaA(Sck, IPDom, 6000, res, strlen(res));
		escriureLiniaLog(fitxLog, 'E', IPDom, 6000, res, numBytes);  //E d'enviat

		int llistaSck[1];
		llistaSck[0] = Sck;

		if(T_HaArribatAlgunaCosaEnTemps(llistaSck, 1, 200) == Sck){
			numBytes = UDP_RepDe(Sck, IPrem, &portRem, missatgeCodificat, 300);
			escriureLiniaLog(fitxLog, 'R', IPrem, portRem, missatgeCodificat, numBytes);

			if(missatgeCodificat[1] == '0'){
				extreureIPport(ipTCP, portTCP, missatgeCodificat);
				return 0;
			}
			else return missatgeCodificat[1] - '0';
		}
		compt++;
	}
	return -1; //No rep res;
}

/* Crea un socket UDP 													  */
/* Retorna el socket UDP creat 											  */
int LUMIc_CrearSocketUDP(int port, char *IP)
{
	int Sck;
	if((Sck = UDP_CreaSock(IP, port)) == -1){
		return -1;
	}
	return Sck;
}

/* Tanca Sck															  */
/* Retorna 1 si va bé, -1 si no. 									  */
int LUMIc_TancarSocketUDP(int Sck)
{
	return close(Sck);
}

/* Retorna la funció d'T_HaArribatAlgunaCosaEnTemps */
int LUMIc_HaArribatAlgunaCosa(int *llistaSck, int midaLlista, int temps)
{
	return T_HaArribatAlgunaCosaEnTemps(llistaSck, midaLlista, temps);
}

int LUMIc_RespostaLocalitzacio(int Sck, char *ipTCP, int portTCP, int codi, int fitxLog)
{
	char IPServidor[16], missatge[300], missatgeCodificat[300];
	int portServidor, numBytes;

	numBytes = UDP_RepDe(Sck, IPServidor, &portServidor, missatge, 300);
	escriureLiniaLog(fitxLog, 'R', IPServidor, portServidor, missatge, numBytes);
	
	char *adrecaMI, *res;
	res = strtok(missatge+1, "#");
	adrecaMI = strtok(NULL, "#");
	
	codificarRespostaLocalitzacioC(codi, missatgeCodificat, adrecaMI, ipTCP, portTCP);
	
	
	numBytes = UDP_EnviaA(Sck, IPServidor, portServidor, missatgeCodificat, strlen(missatgeCodificat));
	escriureLiniaLog(fitxLog, 'E', IPServidor, portServidor, missatgeCodificat, numBytes);
}


/* Si ho creieu convenient, feu altres funcions INTERNES                  */


/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Codifica el misstge de registre/desregistre segons el protocol acordat */
/* Retorna el nombre de bytes del missatge ja codificat, -1 altrament     */
int codificarMissatgeRegistre(char *usuari, char tipus, char *missatgeCodificat)
{
    if(tipus == 'R' || tipus == 'D'){ //Registre o Desregistre
		return sprintf(missatgeCodificat, "%c%s", tipus, usuari);
	}
	else return -1;
}

/* Codifica el missatge de localització en format L@Mi(trucat)#@Mi(trucador) */
/* Retorna el nombre bytes del missatge ja codificat					 				 */
int codificarMissatgeLocalitzacio(char *trucat, char *trucador, char *missatgeCodificat)
{
	return sprintf(missatgeCodificat, "L%s#%s", trucat, trucador);
}

int codificarRespostaLocalitzacioC(int codi, char *res, char *adrMI, char *IP, int port)
{
  return sprintf(res, "S%d%s#%s#%d", codi,adrMI,IP,port);
}

/*Separa l'Usuari i el Domini d'una adreça MI en format usuari@domini*/
int separaUsuariDomini(const char *adrMI, char *usuari, char *domini)
{
	int i=0, j=0;
	char aux;

	while (adrMI[i]!= '@'){
		aux = adrMI[i];
		usuari[i] = aux;
		i++;
	}

	usuari[i] = '\0';
	i++;

	while(adrMI[i] != '\0'){
		aux = adrMI[i];
		domini[j] = aux;
		j++; i++;
	}
	domini[j] = '\0';
	
	return 1;
}

/* Posa la IP i port TCP del final d'un missatge de resposta de localitzacio PLUMI a 
 * ipTCP i portTCP. */
int extreureIPport(char *ipTCP, int *portTCP, char *miss)
{
	int i = 0, j = 0;
	
	while(miss[i] != '#'){
		i++;
	}
	i++;
	while(miss[i] != '#'){
		ipTCP[j] = miss[i];
		i++; j++;
	}
	ipTCP[j] = '\0';
	i++;
	
	int aux = 0;
	while(miss[i] != '\0'){
		aux = aux*10 + (miss[i] - '0');
		*portTCP = aux;
		i++;
	}
	return 1;
}

/* Escriu una linia de text al fitxer de log amb nom "nomfitx"			  */
/* Retorna -1 si hi ha error, o el nombre de caràcters de la línia altrament  */
int escriureLiniaLog(int nomfitx, char codi, char *IP, int port, char *missatge, int bytes)
{
	char liniaLog[300];
	sprintf(liniaLog, "%c:  %s/UDP/%d,  %s, %d", codi, IP, port, missatge, bytes);
	return Log_EscriuC(nomfitx, liniaLog);
}

/* Crea un fitxer de "log" de nom "NomFitxLog".                           */
/* "NomFitxLog" és un "string" de C (vector de chars imprimibles acabat   */
/* en '\0') d'una longitud qualsevol.                                     */
/* Retorna -1 si hi ha error; l'identificador del fitxer creat si tot va  */
/* bé.                                                                    */
int Log_CreaFitxC(const char *NomFitxLog)
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
int Log_EscriuC(int FitxLog, const char *MissLog)
{
	int numChars;
    numChars = write(FitxLog, MissLog, strlen(MissLog));
    if(numChars < 0) return -1;
    numChars = write(FitxLog, "\n", 1);
    if(numChars < 0) return -1;
    return numChars-1;
}

/* Tanca el fitxer de "log" d'identificador "FitxLog".                    */
/* Retorna -1 si hi ha error; un valor positiu qualsevol si tot va bé.    */
int Log_TancaFitxC(int FitxLog)
{
	return close(FitxLog);
}
