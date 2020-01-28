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
	char MIlocal[300];
	char MIremot[300];

 	while (novaConversa!='N'){

 		printf("Entra el teu nick:\n ");
 		int aux_nickLoc = read(0, nickLoc, sizeof(nickLoc));
 		nickLoc[aux_nickLoc-1] = '\0';

 		sprintf(nickLocCod, "%c%03d%s", 'N', strlen(nickLoc), nickLoc); //Codifiquem el nick seguint el protocol establert


 		obtenirIPGlobal(IPLocal); //Obtenim una IP que no sigui la de retorn

 		printf("Escriu el port TCP del socket servidor:\n ");
 		scanf("%d", &portServidor);

 		sesc = MI_IniciaEscPetiRemConv(IPLocal,portServidor); //Creem socket servidor amb l'ip trobada anteriorment i un port que ens entra l'usuari;

 		printf("Socket servidor creat amb @IP: %s, i port TCP: %d\n",IPLocal,portServidor); //Mostrem informació del socket servidor

		SckUDP = LUMIc_CrearSocketUDP(0/*port 0*/, IPLocal); //Creem socket udp;

		do {
			printf("Escriu adreça MI:"\n);
			int lonMI = read(0, MIlocal, 300);
			MIlocal[lonMi-1] = '\0';
			int fitxLog = LUMIc_obrirOCrearFitxLogClient(MIlocal);
			int missRegistrar = LUMIc_RegistrarUsuari(SckUDP, MIlocal, IPDom, fitxLog);
			if (missRegistrar == -1)	printf("Aquest usuari no està donoat d'alta, prova amb un altre usuari.\n", );
		} while(missRegistrar == -1);

 		/*printf("Introdueix la @IP a on et vols connectar:\n ");

 		int ha_arribat = MI_HaArribatPetiConv(sesc);

 		if(ha_arribat == -1) //Error
 		{
 			perror("Error, a l'arribar alguna cosa");
 			return -1;
 		}
 		else if (ha_arribat == 0) //Entrem informació per teclat
 		{
 			scanf("%s", IPRemot);

 			printf("Introdueix el port TCP al que et vols connectar:\n");
 			scanf("%d", &portRemot);

 			scon = MI_DemanaConv(IPRemot, portRemot, IPLocal, &portLocal, nickLocCod, nickRemCod);

 			//Mostrem informació dels sockets connectats
 			printf("Socket local amb @IP: %s, i port TCP: %d\n",IPLocal,portLocal);
 			printf("Socket remot amb @IP: %s, i port TCP: %d\n",IPRemot,portRemot);

 		}
 		else { //Ens arriba una petició de connexió
 			scon = MI_AcceptaConv(sesc, IPRemot, &portRemot, IPLocal, &portLocal, nickLocCod, nickRemCod);

 			//Mostrem informació dels sockets connectats
 			printf("Socket local amb @IP: %s, i port TCP: %d\n",IPLocal,portLocal);
 			printf("Socket remot amb @IP: %s, i port TCP: %d\n",IPRemot,portRemot);
 		}*/

		while (/*no acabem connexió UDP*/)
		{

			int ha_arribat = LUMIc_HaArribatAlgunaCosa(SckUDP, sesc, -1/*temps*/);//ffalta fer (crida ha arribat alguna cosa en temps de la capa t)

			if(ha_arribat == -1) //Error
	 		{
	 			perror("Error, a l'arribar alguna cosa");
	 			return -1;
	 		}
	 		else if (ha_arribat == 0) //Entrem informació per teclat
	 		{
				printf("Escriu el nick amb qui et vols connectar:\n");
	 			scanf("%s\n", MIremot);

				int loc = LUMIc_Localitzar(SckUDP, MIremot, IPDom, MIlocal, IPRemot, portRemot, fitxLog);

				if (loc == -1)
				{
					perror("Error, no hi ha resposta del servidor");
		 			return -1;
				} else
				{
					scon = MI_DemanaConv(IPRemot, portRemot, IPLocal, &portLocal, nickLocCod, nickRemCod);
					printf("Socket local amb @IP: %s, i port TCP: %d\n",IPLocal,portLocal);
					printf("Socket remot amb @IP: %s, i port TCP: %d\n",IPRemot,portRemot);
				}
	 		}
			else if (ha_arribat == sesc)
			{
				scon = MI_DemanaConv(sesc, IPRemot, &portRemot, IPLocal, &portLocal, nickLocCod, nickRemCod);
				printf("Socket local amb @IP: %s, i port TCP: %d\n",IPLocal,portLocal);
				printf("Socket remot amb @IP: %s, i port TCP: %d\n",IPRemot,portRemot);
			}
	 		else
			{
				LUMIc_RespostaLocalitzacio(SckUDP, IPLocal, portLocal, 1/*codi?*/, fitxLog);
	 		}
		}

 		//Ja ens hem connectat!

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

 		printf("%s i %s us heu connectat correctament!\n", nickLoc, nickRem);
 		printf("Ja podeu començar a xatejar!\n");

 		do{
 			if((ha_arribat = LUMIc_HaArribatAlgunaCosa(SckUDP, sesc, -1/*temps*/))==-1) exit(-1); //ERROR!
 			if (ha_arribat == 0) //Envia missatge
 			{
 			  midaMiss = read(0, miss, sizeof(miss));
 			  miss[midaMiss-1] = '\0';

 			  if (miss[0] == '#') //Senyal acabament conversa
 			  {
 				  printf("T'has desconnectat\n");
 			  }

 			  sprintf(missCod, "%c%03d%s", 'L', strlen(miss), miss); //Codifiquem el missatge seguint el protocol establert

 			  midaMiss = MI_EnviaLinia(scon, missCod); //Enviem el missatge

 			}
 			else if (ha_arribat == scon)//Rep missatge
 			{
 				midaMiss = MI_RepLinia(scon, missCod); //Rebem el missatge


 				if (midaMiss == -1) {exit(-1);} //Error
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

 					if (miss[0] == '#') printf("%s s'ha desconectat\n", nickRem); //Comprovem si el missatge és el senyal d'acabament de la conversa
 					else
 					{
 						printf("%s: %s\n",nickRem,miss); //Mostrem el missatge
 					}
 				}
 			}
			else
			{
				LUMIc_RespostaLocalitzacio(SckUDP, IPLocal, portLocal, 1/*codi?*/, fitxLog);
			}

 		} while (miss[0]!='#');	//Mentre el missatge no sigui la marca de fi

 		MI_AcabaConv(scon); //Tanquem conexió i pleguem

 		//Preguntem si volen fer una nova conversa
 		printf("Vols iniciar una nova conversa? (S,N)\n");
 		scanf(" %c", &novaConversa); //posem espai en blanc al principi per alliberar el buffer
 	}

	desr = LUMIc_DesregistrarUsuari(SckUDP, MIlocal, IPDom, fitxLog);
	LUMIc_TancarSocketUDP(SckUDP);
	tancaLog(fitxLog);

 	printf("Gracies per utilitzar el nostre servei, fins la proxima!\n");

 	return(0);
  }
 }

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
