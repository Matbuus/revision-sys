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

  /* Pr�paration du champ pointAccesClient de la requ�te */
  sprintf(request.pointAccesClient, "./tube_serveur-client.%d", getpid());

  /* Pr�paration du champ contenu : On y recopie argv[1] en mettant en */
  /* majuscules                                                        */
  len = strlen(argv[1]);
  for (i=0 ; i < len ; i++) {
    request.contenu[i] = toupper(argv[1][i]);
  }
  request.contenu[len] = '\0';

  /* Cr�ation du tube nomm� du client */
  if (mkfifo(request.pointAccesClient, S_IRUSR|S_IWUSR) < 0) {
    if (errno != EEXIST) {
      perror("mkfifo(tube nomm� client");
      exit(EXIT_FAILURE);
    }
    else {
      printf("%s existe deja : on suppose que c'est un pipe nomme\n",
	     request.pointAccesClient );
      printf("et qu'on peut continuer le programme sans probleme\n");
      puts("");
    }
  }

  /* Envoi de la requ�te vers le serveur */
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

  /* On n'aura pas d'autres messages � envoyer au serveur. On peut donc    */
  /* fermer le tube                                                        */
  close(fdW);

  /* On ouvre seulement maintenant le tube pour �couter la r�ponse, sinon  */
  /* on serait bloqu� jusqu'� ce que le serveur ouvre le pipe en �criture  */
  /* pour envoyer sa r�ponse. Si on ouvrait le tube avant d'envoyer la     */
  /* requ�te, le serveur ne recevrait jamais la requ�te et donc n'ouvrirait*/
  /* jamais le tube ==> On serait bloqu� ind�finiment.                     */
  fdR = open(request.pointAccesClient, O_RDONLY);
  if (fdR == -1) {
    perror("open(tube nomm�)");
    exit(EXIT_FAILURE);
  }

  /* On lit la r�ponse */
  nbRead = read(fdR, &response, sizeof(response));
  if (nbRead != sizeof(response)) {
    printf("Communication avec le serveur probablement rompue\n");
    exit(EXIT_FAILURE);
  }

  /* On affiche la r�ponse */
  printf("Response du serveur = \"%s\"\n", response.contenu);

  /* NB : Dans cette application, on utilise pas le point d'acc�s priv� */
  /* vers le serveur                                                    */

  /* On ferme le tube c�t� r�ception */
  close(fdR);

  /* On d�truit le tube nomm� du client (puisque ce tube nomm� ne servira */
  /* plus jamais.                                                         */
  if (unlink(request.pointAccesClient) < 0) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}


   
