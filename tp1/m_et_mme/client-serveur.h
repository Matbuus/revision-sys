/*************************************************/
/* D�clarations communes au client et au serveur */
/*************************************************/

#define NOMPOINTACCESSERV "tube_client-serveur"

/* Structure des messages utilis�s dans le sens client-->serveur */
/* NB : les 2 cha�nes de caract�res se terminent par '\0'        */
typedef struct{
  char pointAccesClient[128];
  char contenu[256];
} request_t;

/* Structure des messages utilis�s dans le sens serveur-->client */
/* NB : les 2 cha�nes de caract�res se terminent par '\0'        */
typedef struct{
  char pointAccesPrive[128];
  char contenu[256];
} response_t;
