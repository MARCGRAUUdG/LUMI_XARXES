/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer capçalera de lumiC.c                                            */
/*                                                                        */
/* Autors: X, Y                                                           */
/*                                                                        */
/**************************************************************************/

/* Declaració de funcions EXTERNES de lumiC.c, és a dir, d'aquelles       */
/* funcions que es faran servir en un altre fitxer extern a lumiC.c,      */
/* p.e., int LUMIc_FuncioExterna(arg1, arg2...) { }                       */
/* El fitxer extern farà un #include del fitxer lumiC.h a l'inici, i      */
/* així les funcions seran conegudes en ell.                              */
/* En termes de capes de l'aplicació, aquest conjunt de funcions          */
/* EXTERNES formen la interfície de la capa LUMI, la part del client      */

int LUMIc_RegistrarUsuari(int Sck, char *adrMI, char *IPdom, int log);
int LUMIc_obrirOCrearFitxLogClient(char *adrMI);
int LUMIc_escriureLiniaFitxLog(int nomfitx, char codi, char *IP, int port, char *missatge, int bytes);
int LUMIc_DesregistrarUsuari(int Sck, char *adrMI, char *IPDom, int fitxLog);
int LUMIc_Localitzar(const int Sck, char *adrMI, char *IPDom, char *MIloc, char *ipTCP, int *portTCP, int fitxLog);
int LUMIc_CrearSocketUDP(int port, char *IP);
int LUMIc_TancarSocketUDP(int Sck);
int LUMIc_RespostaLocalitzacio(int Sck, char *ipTCP, int portTCP, int codi, int fitxLog);
