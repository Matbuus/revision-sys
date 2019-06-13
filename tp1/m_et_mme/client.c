#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "client-serveur.h"

int main(int argc, char *argv[]) {
  request_t request;
  response_t response;
  int i;
  int len;
  int fdW, fdR;
  int nbRead, nbWrite;

  if (argc != 2) {
    printf("Usage = client nomDeFamilleCherche\n");
    exit(EXIT_FAILURE);
  }

  /* Préparation du champ pointAccesClient de la requête */
  sprintf(request.pointAccesClient, "./tube_serveur-client.%d", getpid());

  /* Préparation du champ contenu : On y recopie argv[1] en mettant en */
  /* majuscules                                                        */
  len = strlen(argv[1]);
  for (i=0 ; i < len ; i++) {
    request.contenu[i] = toupper(argv[1][i]);
  }
  request.contenu[len] = '\0';

  /* Création du tube nommé du client */
  if (mkfifo(request.pointAccesClient, S_IRUSR|S_IWUSR) < 0) {
    if (errno != EEXIST) {
      perror("mkfifo(tube nommé client");
      exit(EXIT_FAILURE);
    }
    else {
      printf("%s existe deja : on suppose que c'est un pipe nomme\n",
	     request.pointAccesClient );
      printf("et qu'on peut continuer le programme sans probleme\n");
      puts("");
    }
  }

  /* Envoi de la requête vers le serveur */
  fdW = open(NOMPOINTACCESSERV, O_WRONLY);
  if (fdW == -1) {
    perror("open(NOMPOINTACCESSERV)");
    exit(EXIT_FAILURE);
  }
  nbWrite = write(fdW, &request, sizeof(request));
  if (nbWrite < sizeof(request)) {
    perror("pb ecriture sur pipe nomme");
    exit(EXIT_FAILURE);
  }

  /* On n'aura pas d'autres messages à envoyer au serveur. On peut donc    */
  /* fermer le tube                                                        */
  close(fdW);

  /* On ouvre seulement maintenant le tube pour écouter la réponse, sinon  */
  /* on serait bloqué jusqu'à ce que le serveur ouvre le pipe en écriture  */
  /* pour envoyer sa réponse. Si on ouvrait le tube avant d'envoyer la     */
  /* requête, le serveur ne recevrait jamais la requête et donc n'ouvrirait*/
  /* jamais le tube ==> On serait bloqué indéfiniment.                     */
  fdR = open(request.pointAccesClient, O_RDONLY);
  if (fdR == -1) {
    perror("open(tube nommé)");
    exit(EXIT_FAILURE);
  }

  /* On lit la réponse */
  nbRead = read(fdR, &response, sizeof(response));
  if (nbRead != sizeof(response)) {
    printf("Communication avec le serveur probablement rompue\n");
    exit(EXIT_FAILURE);
  }

  /* On affiche la réponse */
  printf("Response du serveur = \"%s\"\n", response.contenu);

  /* NB : Dans cette application, on utilise pas le point d'accès privé */
  /* vers le serveur                                                    */

  /* On ferme le tube côté réception */
  close(fdR);

  /* On détruit le tube nommé du client (puisque ce tube nommé ne servira */
  /* plus jamais.                                                         */
  if (unlink(request.pointAccesClient) < 0) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}


   
