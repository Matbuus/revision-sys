/*
 * serveur.c
 *
 * Rq: open RDWR au lieu de RD_ONLY du tube serveur 
 * pour que le read n'echoue pas a la terminaison du client
 * en supposant la connexion avec le client terminee.
 *
 */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "client-serveur.h"

FILE* f= NULL;

response_t search_key(request_t *request) {
  char ligne[256];
  response_t response;

  /* Calcul response.pointAccesPrive */
  strcpy(response.pointAccesPrive, "");
  /* Le point d'acc�s priv� n'est pas utilis� dans cette appli */
  rewind(f);
  while (feof(f) == 0) {
    if (fgets(ligne, sizeof(ligne), f) != NULL) {
      if (strncmp(ligne, request->contenu, strlen(request->contenu)) == 0){
	break;
      }
    }
  }
  if (feof(f) == 0) {
    strcpy(response.contenu, ligne);
    response.contenu[strlen(ligne)-1] = '\0'; /* Supprime le '\n' final */
  }
  else {
    strcpy(response.contenu,"Desole, je ne le connais pas");
  }
  return response;
}

void process_request(request_t *request){
  response_t response;
  int fdW;
  int nbWrite;

  response = search_key(request);

  /* Affichage */
  printf("Serveur a recu \"%s\" et repond \"%s\"\n",
	 request->contenu,
	 response.contenu );

  /* Connexion au point d'acc�s client et r�ponse */
  fdW = open(request->pointAccesClient, O_WRONLY);
  if (fdW == -1) {
    perror("open(pointAccessClient)");
    exit(EXIT_FAILURE);
  }
  nbWrite = write(fdW, &response, sizeof(response));
  if (nbWrite < sizeof(response)) {
    perror("pb ecriture sur pipe nomme");
    exit(EXIT_FAILURE);
  }

  /* Dans cette application, le client ne renvoie pas de requ�te ult�rieure*/
  /* n�cessitant une r�ponse ==> On peut fermer ce tube                    */
  close(fdW);
}

int main() {
  int fdR;
  int nbRead;

  /* Cr�ation du tube nomm� du serveur */
  if (mkfifo(NOMPOINTACCESSERV, S_IRUSR|S_IWUSR) < 0) {
    if (errno != EEXIST) {
      perror("mkfifo(tube nomm� client");
      exit(EXIT_FAILURE);
    }
    else {
      printf("%s existe deja : on suppose que c'est un pipe nomme\n",
	     NOMPOINTACCESSERV );
      printf("et qu'on peut continuer le programme sans probleme\n");
      puts("");
    }
  }

  /* Ouverture du fichier mEtMme.txt */
  f = fopen("mEtMme.txt", "r");
  if (f == NULL) {
    perror("open(mEtMme.txt)");
    exit(EXIT_FAILURE);
  }
  
  /* Ouverture de ce tube nomm� (�quivalent � une attente de connexion) */
  fdR = open(NOMPOINTACCESSERV, O_RDWR);
  if (fdR == -1) 
    {
      perror("open(tube nomm�)");
      exit(EXIT_FAILURE);
    }


  /* Boucle principale */
  while (1) {
    /* Attente d'une requ�te */
    request_t request;

    nbRead = read(fdR, &request, sizeof(request_t));
    if (nbRead != sizeof(request_t)) {
      printf("Communication avec le client probablement rompue\n");
      exit(EXIT_FAILURE);
    }
    /* On traite la requ�te */
    process_request(&request);
  }

  close(fdR);
  fclose(f);
  return EXIT_SUCCESS;
}


   
