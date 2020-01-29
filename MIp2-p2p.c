/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer p2p.c que implementa la interfície aplicació-usuari de          */
/* l'aplicació de MI amb l'agent de LUMI integrat, sobre les capes        */
/* d'aplicació de MI i LUMI (fent crides a les interfícies de les capes   */
/* MI -fitxers mi.c i mi.h- i LUMI -lumi.c i lumi.h- ).                   */
/* Autors: Marc Grau i Xavier Roca                                        */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "mi.h"
#include "lumiC.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>
#include "MIp2-t.h"
#include "MIp2-mi.h"

/* Definició de constants, p.e., #define XYZ       1500                   */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
/* int FuncioInterna(arg1, arg2...);                                      */

/*Funció per trobar una adreça IP global*/

void obtenirIPGlobal(char *ipGlobal){
	struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sockadress;
    char *address;

    getifaddrs (&ifap); //ens retorna una llista d'adreçes amb diferent informació en cada element de la llista
    ifa = ifap;

    for (ifa; ifa->ifa_next!=NULL; ifa = ifa->ifa_next) { //mentre no arribem al final de la llista
        if (ifa->ifa_addr->sa_family == AF_INET) { //si pertany a la familia AF_INET (ifa_addr és de tipus sockaddr)
            sockadress = (struct sockaddr_in *) ifa->ifa_addr; //fem un cast de ifa_addr
            address = inet_ntoa(sockadress->sin_addr); //funció inversa a inet_addr(), ens retorna l'adreça ip
            if(ifa->ifa_name[0] != 'l')
            {
				strcpy(ipGlobal, address); //si la interfície de l'adreça no és "loopback" l'agafem
			}
        }
    }
    freeifaddrs(ifap); //alliberem memòria
}

int main(int argc,char *argv[])
{
    int midaMiss;
 	int sesc=0, scon=0;
 	int ipServidor;
 	int portServidor;
 	int portRemot;
 	int portLocal;
	int SckUDP;
 	char miss[200], missCod[200];
 	char IPRemot[20];
 	char IPLocal[20];
	char IPDom[20];
 	char nickLoc[200], nickRem[200], nickLocCod[200], nickRemCod[200];
 	char novaConversa = 'a';
	char MILocal[300];
	char MIRemot[300];
	
	int respostaRegistre;
	int respostaLocalitzacio;
	int bytesMI;
	int fitxerLog;
	int llistaSck[3];
	int ha_arribat;
	int hiHaConnexio=0;
	int usuariDesconectat = 0;
	int respostaDesregistre=0;
	int intentsDesregistre=0;
	
	obtenirIPGlobal(IPLocal); //Obtenim una IP que no sigui la de retorn
	
	if((SckUDP = LUMIc_CrearSocketUDP(0, IPLocal)) == -1) exit(-1);  //creem socket UDP
	do{
		printf("Entra la teva adreça MI:\n ");
		bytesMI = read(0,MILocal,sizeof(MILocal));
		MILocal[bytesMI-1] = '\0';
		fitxerLog = LUMIc_obrirOCrearFitxLogClient(MILocal);
		respostaRegistre = LUMIc_RegistrarUsuari(SckUDP, MILocal, IPDom, fitxerLog);
		if (respostaRegistre == -1) printf("Error amb el servidor o amb el format de la peticio\n");
		else if (respostaRegistre == 1) printf("L'usuari entrat no existeix, i per tant, no esta donat d'alta\n");
	} while (respostaRegistre != 0);	

 	while (novaConversa!='N'){

 		printf("Entra el teu nick:\n ");
 		int aux_nickLoc = read(0, nickLoc, sizeof(nickLoc));
 		nickLoc[aux_nickLoc-1] = '\0';

 		sprintf(nickLocCod, "%c%03d%s", 'N', strlen(nickLoc), nickLoc); //Codifiquem el nick seguint el protocol establert

 		printf("Escriu el port TCP del socket servidor:\n ");
 		scanf("%d", &portServidor);

 		sesc = MI_IniciaEscPetiRemConv(IPLocal,portServidor); //Creem socket servidor amb l'ip trobada anteriorment i un port que ens entra l'usuari;

 		printf("Socket servidor creat amb @IP: %s, i port TCP: %d\n",IPLocal,portServidor); //Mostrem informació del socket servidor

		llistaSck[0] = SckUDP; //socket UDP
		llistaSck[1] = sesc;   //socket TCP
		llistaSck[2] = 0;      //teclat

		//Localitzem un altre usuari
		while(hiHaConnexio!=1){
			printf("Escriu l'adreça MI de la persona amb la que et vulguis connectar:\n");
			ha_arribat = LUMIc_HaArribatAlgunaCosa(llistaSck, 3, -1); //temps=-1, espera sense restricció de temps
			if (ha_arribat == -1) exit(-1);
			else if (ha_arribat == 0){ //de teclat
				scanf("%s", MIRemot);
				respostaLocalitzacio = LUMIc_Localitzar(SckUDP, MIRemot, IPDom, MILocal, IPRemot, portRemot, fitxerLog);
				
				if (respostaLocalitzacio == 0){
					scon = MI_DemanaConv(IPRemot, portRemot, IPLocal, &portLocal, nickLocCod, nickRemCod)) == -1){
						printf("Error de connexio\n");
					}
					printf("Socket local amb @IP: %s, i port TCP: %d\n",IPLocal,portLocal);
					printf("Socket remot amb @IP: %s, i port TCP: %d\n",IPRemot,portRemot);
					hiHaConnexio = 1;
				}
				else if (respostaLocalitzacio == 1) printf("El domini es erroni\n");
				else if (respostaLocalitzacio == 2) printf("L'usuari esta offline\n");
				else if (respostaLocalitzacio == 3) printf("L'usuari no existeix\n");
				else if (respostaLocalitzacio == 4) printf("L'usuari està ocupat\n");
				else if (respostaLocalitzacio == -1) printf("No s'ha rebut cap resposta des del servidor\n");
			}
	
			else if (ha_arribat == sesc){ //arriba una petició de connexió per TCP
				if((scon = MI_AcceptaConv(sesc, IPRemot, &portRemot, IPLocal, &portLocal, nickLocCod, nickRemCod)) == -1){
					printf("Error de connexio\n");
				}
				
				printf("Socket local amb @IP: %s, i port TCP: %d\n",IPLocal,portLocal);
				printf("Socket remot amb @IP: %s, i port TCP: %d\n",IPRemot,portRemot);
				hiHaConnexio = 1;
			}
			else{
				LUMIc_codificarRespostaLocalitzacio(SckUDP, IPLocal, portLocal, 0, fitxerLog); //si arriba petició de connexió per UDP, li diu que està lliure amb codi 0
			}
		}
		
		//Hi ha hagut connexio entre 2 usuaris
		
		if(nickRemCod[0] != 'N') //Si el missatge no segueix el protocol establert
		{ 
			perror("El nick no segueix el protocol!");
			close(scon);
			exit(-1);
		}
		
		//Si segueix el protocol estipulat, descodifiquem el nickRemCod
		int midaNick = 100*(nickRemCod[1]-'0') + 10*(nickRemCod[2]-'0') + (nickRemCod[3]-'0');
		
		int byte;
		for (byte=0; byte<midaNick; byte++)
		{
			nickRem[byte] = nickRemCod[4+byte];
		}
		
		nickRem[byte] = '\0';
		/*Nick remot descodificat*/
		
		llistaSck[1] = scon; //deixem d'escoltar per sesc, i escoltem per scon
		
		printf("%s i %s us heu connectat correctament!\n", nickLoc, nickRem);
		printf("Ja podeu començar a xatejar!\n");
			

 		do{
 			if((ha_arribat = LUMIc_HaArribatAlgunaCosa(llistaSck, 3, -1))==-1) exit(-1); //ERROR! -- temps=-1, escolta indefinidament
 			if (ha_arribat == 0) //Envia missatge
 			{
 			  midaMiss = read(0, miss, sizeof(miss));
 			  miss[midaMiss-1] = '\0';

 			  if (miss[0] == '#') //Senyal acabament conversa
 			  {
				  usuariDesconectat=1;
 				  printf("T'has desconnectat\n");
 			  }
			  else{

				sprintf(missCod, "%c%03d%s", 'L', strlen(miss), miss); //Codifiquem el missatge seguint el protocol establert

				midaMiss = MI_EnviaLinia(scon, missCod); //Enviem el missatge
			  }
 			}
 			else if (ha_arribat == scon)//Rep missatge
 			{
 				midaMiss = MI_RepLinia(scon, missCod); //Rebem el missatge


 				if (midaMiss == -1) {exit(-1);} //Error
				else if (midaMiss == -2){
					usuariDesconectat = 1;
					printf("L'usuari %s s'ha desconnectat.\n", nickRem);
				}
 				else if (missCod[0] != 'L') //Comprovem que el missatge segueixi el protocol
 				{
 					perror("El missatge no segueix el protocol!");
 					close(scon);
 					exit(-1);
 				}
 				else
 				{
 					//Descodifiquem el missatge
 					int midaMiss = 100*(missCod[1]-'0') + 10*(missCod[2]-'0') + (missCod[3]-'0');

 					int byte;
 					for (byte=0; byte<midaMiss; byte++)
 					{
 						miss[byte] = missCod[4+byte];
 					}

 					miss[byte] = '\0';

 					//Missatge descodificat

					printf("%s: %s\n",nickRem,miss); //Mostrem el missatge
 				}
 			}
			else //si rep petició de connexio per UDP, li diu que està ocupat, amb el codi 4 (seguint el protocol)
			{
				LUMIc_codificarRespostaLocalitzacio(SckUDP, IPLocal, portLocal, 4, fitxerLog);
			}

 		} while (usuariDesconectat==0)	//Mentre un dels dos no acabi la conversa

 		MI_AcabaConv(scon); //Tanquem conexió i pleguem

 		//Preguntem si volen fer una nova conversa
 		printf("Vols iniciar una nova conversa? (S,N)\n");
 		scanf(" %c", &novaConversa); //posem espai en blanc al principi per alliberar el buffer
 	}
	
	while(respostaDesregistre!=0 && intentsDesregistre<3){
		respostaDesregistre = LUMIc_DesregistrarUsuari(SckUDP, MILocal, IPDom, fitxerLog);
		intentsDesregistre++;
	}
	
	if (respostaDesregistre==1) printf("No s'ha pogut desregistrar l'usuari\n");
	LUMIc_TancarSocketUDP(SckUDP);
	tancaLog(fitxLog);

 	printf("Gracies per utilitzar el nostre servei, fins la proxima!\n");

 	return(0);
  }
 }