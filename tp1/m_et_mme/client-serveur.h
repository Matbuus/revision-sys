/*************************************************/
/* Déclarations communes au client et au serveur */
/*************************************************/

#define NOMPOINTACCESSERV "tube_client-serveur"

/* Structure des messages utilisés dans le sens client-->serveur */
/* NB : les 2 chaînes de caractères se terminent par '\0'        */
typedef struct{
  char pointAccesClient[128];
  char contenu[256];
} request_t;

/* Structure des messages utilisés dans le sens serveur-->client */
/* NB : les 2 chaînes de caractères se terminent par '\0'        */
typedef struct{
  char pointAccesPrive[128];
  char contenu[256];
} response_t;
