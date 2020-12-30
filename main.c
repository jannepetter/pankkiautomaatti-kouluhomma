#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funktiot.h"

FILE* inp;

int main(void) {
  char tilinumero[20];           // käyttäjän syöttämä
  char tunnusluku[20];           // käyttäjän syöttämä
  char tilitiedot[1][20];        // tiedostosta haetut, tilitiedot[0]=salasana,
  printf("Anna tilinumero:\n");  // 12345.tili
  kysyMjono(tilinumero);
  inp = fopen(tilinumero, "r");

  if (inp == NULL) {
    printf("Sellaista tilia ei ole olemassa\n");
  } else {
    printf("Anna tunnusluku:\n");  // salasana
    kysyMjono(tunnusluku);

    // tilitiedot taulukkoon luetaan tiedosto sisältä
    lueTiliTiedot(tilitiedot, inp);
    fclose(inp);
    if (!strcmp(tunnusluku, tilitiedot[0])) {
      int saldo = atoi(tilitiedot[1]);
      run(tilinumero, tilitiedot[0], saldo);
    } else {
      printf("vaara tunnusluku\n");
    }
  }

  return 0;
}
