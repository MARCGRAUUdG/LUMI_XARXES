/**************************************************************************/
/*                                                                        */
/* P2 - MI amb sockets TCP/IP - Part II                                   */
/* Fitxer capçalera de lumiC.c                                            */
/*                                                                        */
/* Autors: Marc Grau i Xavier Roca                                        */
/*                                                                        */
/**************************************************************************/

#include <stdlib.h> 
#include <stdio.h>

/* Declaració de funcions EXTERNES de lumiS.c, és a dir, d'aquelles       */
/* funcions que es faran servir en un altre fitxer extern a lumiS.c,      */
/* p.e., int LUMIs_FuncioExterna(arg1, arg2...) { }                       */
/* El fitxer extern farà un #include del fitxer lumiS.h a l'inici, i      */
/* així les funcions seran conegudes en ell.                              */
/* En termes de capes de l'aplicació, aquest conjunt de funcions          */
/* EXTERNES formen la interfície de la capa LUMI, la part del servidor    */


struct usuaris{
    char usuari[299];
    char sckLUMI[25];
};

int LUMIs_Inicialitzar(char *nomDomini, int nUsuaris, FILE *cfg, struct usuaris *taulaUsuaris);
int LUMIs_ServeixPeticio(int Sck, char *nomDomini, struct usuaris *taulaClients, int nClients, int log);
int LUMIs_obrirOCrearFitxLogServidor(char *nomDomini);
int LUMIs_tancaLog(int log);
int LUMIs_HaArribatAlgunaCosa(int *llistaSck, int midaLlista, int temps);


