/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer capçalera de lumiC.c                                            */
/*                                                                        */
/* Autors: X, Y                                                           */
/*                                                                        */
/**************************************************************************/
int LUMIs_Registrar(char *miss, struct adrUDP *taulaClients, int nClients, char *IP, int port);
int LUMIs_Desregistrar(char *miss, struct adrUDP *taulaClients, int nClients);
int LUMIs_PeticioLoc(char *miss, char *domini, char *username);
int LUMIs_RespostaLoc(char *miss, char *domini, char *username);
int LUMIs_Inicialitzar(char *nomDomini, int nClients, FILE *cfg, struct adrUDP *taulaClients);
int LUMIs_ServeixPeticio(int Sck, char *nomDomini, struct adrUDP *taulaClients, int nClients, int log);

/* Declaració de funcions EXTERNES de lumiS.c, és a dir, d'aquelles       */
/* funcions que es faran servir en un altre fitxer extern a lumiS.c,      */
/* p.e., int LUMIs_FuncioExterna(arg1, arg2...) { }                       */
/* El fitxer extern farà un #include del fitxer lumiS.h a l'inici, i      */
/* així les funcions seran conegudes en ell.                              */
/* En termes de capes de l'aplicació, aquest conjunt de funcions          */
/* EXTERNES formen la interfície de la capa LUMI, la part del servidor    */
