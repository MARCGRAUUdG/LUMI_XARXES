/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer capçalera de mi.c                                               */
/*                                                                        */
/* Autors: X, Y                                                           */
/*                                                                        */
/**************************************************************************/

/* Declaració de funcions EXTERNES de mi.c, és a dir, d'aquelles          */
/* funcions que es faran servir en un altre fitxer extern a mi.c,         */
/* p.e., int MI_FuncioExterna(arg1, arg2...) { }                          */
/* El fitxer extern farà un #include del fitxer mi.h a l'inici, i         */
/* així les funcions seran conegudes en ell.                              */
/* En termes de capes de l'aplicació, aquest conjunt de funcions          */
/* EXTERNES formen la interfície de la capa MI.                           */
 
int MI_IniciaEscPetiRemConv(int portTCPloc);
int MI_HaArribatPetiConv(int SckEscMI);
int MI_DemanaConv(const char *IPrem, int portTCPrem, char *IPloc, int *portTCPloc, const char *NicLoc, char *NicRem);
int MI_AcceptaConv(int SckEscMI, char *IPrem, int *portTCPrem, char *IPloc, int *portTCPloc, const char *NicLoc, char *NicRem);
int MI_HaArribatLinia(int SckConvMI);
int MI_EnviaLinia(int SckConvMI, const char *Linia);
int MI_RepLinia(int SckConvMI, char *Linia);
int MI_AcabaConv(int SckConvMI);
int MI_AcabaEscPetiRemConv(int SckEscMI);
