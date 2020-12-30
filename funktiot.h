#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int kysyNumero();
void aloitusValikko(void);
char* operaattoriValikko(void);
int latausSummaValikko(int saldo);
int ottoSummaValikko(int saldo);
void tulostaSaldo(int saldo);
void tulostaTapahtumat(void);
void tulostaViesti(int valinta, int saldo, int otto);
void tulostaSetelit(int summa);
void run(char tilinumero[], char salasana[], int saldo);
void kysyMjono(char mjono[]);
void lueTiliTiedot(char lista[][20], FILE* kahva);
void putsaaMerkkijono(char mjono[]);
int talletaSaldo(char saldo[], char tili[], char salasana[]);

FILE* outp;

int talletaSaldo(char saldo[], char tili[], char salasana[]) {
  outp = fopen(tili, "w");
  if (outp == NULL) {
    printf("Tapahtui virhe, tapahtuma keskeytetään, tiliä ei veloitettu\n");
    return 0;
  } else {
    fputs(salasana, outp);
    fputc('\n', outp);
    fputs(saldo, outp);
  }
  fclose(outp);

  return 1;
}
void putsaaMerkkijono(char mjono[]) {
  for (int i = 0; i < 20; i++) {
    if (mjono[i] == '\n') {
      mjono[i] = '\0';
      break;
    }
  }
}
void lueTiliTiedot(char lista[][20], FILE* kahva) {
  for (int i = 0; i < 2; i++) {
    if (feof(kahva)) {
      break;
    }
    fgets(lista[i], 20, kahva);
    putsaaMerkkijono(lista[i]);
  }
}

void run(char tilinumero[], char salasana[], int saldo) {
  // double saldo = 1000.30;
  int valinta = -1;
  int ottoSumma;

  while (valinta != 0) {
    aloitusValikko();
    valinta = kysyNumero();
    switch (valinta) {
      case 1:;  // puheliittymän lataus
        char* op = operaattoriValikko();
        if (!strcmp(op, "Takasalkuun")) {
          // käyttäjä haluaa palata alkuvalikkoon
          break;
        }
        if (!strcmp(op, "Lopeta")) {
          // käyttäjä haluaa lopettaa
          valinta = 0;
          tulostaViesti(valinta, saldo, 0);
          break;
        }
        int summa = latausSummaValikko(saldo);
        if (summa == -99) {
          // käyttäjä haluaa palata alkuvalikkoon
          break;
        }
        if (summa == -999) {
          // käyttäjä haluaa lopettaa
          valinta = 0;
          tulostaViesti(valinta, saldo, 0);
          break;
        }
        if (summa > 0) {
          // tarkistus mysteerisyötteiden varalle, joiden ei pitäisi olla
          // mahdollisia
          int tarkistus = saldo - summa;
          if (tarkistus < 0) {
            // käyttäjä onnistuu luomaan tilanteen missä saldo ylittyy,
            // keskeytys
            printf("Keskeytys, jokin meni vikaan, tiliä ei veloitettu\n");
            valinta = 0;
            tulostaViesti(valinta, saldo, 0);
            break;
          }

          char puhelinnumero[15];
          printf("Syota %s puhelinnumero\n", op);
          scanf("%s", puhelinnumero);
          saldo = saldo - summa;
          char mjonoSaldo[20];
          itoa(saldo, mjonoSaldo, 10);
          talletaSaldo(mjonoSaldo, tilinumero, salasana);
          tulostaViesti(valinta, saldo, 0);
          printf("%s:n latausvahvistus tekstiviestilla\n", op);
        }
        // exit mysteerisyötteistä
        valinta = 0;
        tulostaViesti(valinta, saldo, 0);
        break;
      case 2:  // otto
        ottoSumma = ottoSummaValikko(saldo);

        if (ottoSumma == -99) {
          // käyttäjä haluaa palata alkuvalikkoon
          break;
        }
        if (ottoSumma == -999) {
          // käyttäjä haluaa lopettaa, valinta=0 exit loop
          valinta = 0;
          tulostaViesti(valinta, saldo, 0);
          break;
        }
        if (ottoSumma > 0) {
          // tarkistus negatiivisten syötteiden varalle,ettei käyttäjä tule
          // keksineeksi rikastumiskeinoa. Tarkistus myös ettei nosto ylitä
          // saldoa
          int tarkistus = saldo - ottoSumma;
          if (tarkistus < 0) {
            // keskeytys jos käyttäjä jollakin konstilla sai saldon ylittävän
            // noston läpi
            printf("Keskeytys, jokin meni vikaan, tiliä ei veloitettu\n");
            valinta = 0;
            tulostaViesti(valinta, saldo, 0);
            break;
          }

          saldo = saldo - ottoSumma;
          char mjonoSaldo[20];
          itoa(saldo, mjonoSaldo, 10);
          talletaSaldo(mjonoSaldo, tilinumero, salasana);
          // poistuminen normaalisti sujuneesta nostosta
          tulostaViesti(valinta, saldo, ottoSumma);
          valinta = 0;
          break;
        }
        // poistuminen epänormaaleista nostoista, negatiiviset jne.
        tulostaViesti(valinta, saldo, ottoSumma);
        valinta = 0;
        break;
      case 3:  // saldokysely
        tulostaSaldo(saldo);
        int saldoValinta = kysyNumero();
        if (saldoValinta == 2) {
          // käyttäjä haluaa lopettaa
          tulostaViesti(valinta, saldo, 0);
          valinta = 0;
          break;
        }
        break;
      case 4:  // tapahtumakysely
        tulostaTapahtumat();
        int palaaAlkuun = kysyNumero();
        break;
      default:
        // stop- käyttäjä haluaa lopettaa, valinta=0 exit loop
        valinta = 0;
        tulostaViesti(valinta, saldo, 0);
        break;
    }
  }
}
void kysyMjono(char mjono[]) { scanf("%s", mjono); }

int kysyNumero() {
  int valinta;
  int onnistui = scanf("%d", &valinta);
  if (onnistui) {
    return valinta;
  }
  return 0;
}

void aloitusValikko(void) {
  printf("\n");
  printf("Valitse\n");
  printf("1 - Liittyman lataus puheaika\n");
  printf("2 - Otto\n");
  printf("3 - Saldo\n");
  printf("4 - Tapahtumat\n");
  printf("5 - Lopeta\n");
}
char* operaattoriValikko(void) {
  printf("\n");
  printf("Liittyman lataus - valitse operaattori\n");
  printf("1 - Saunalahti\n");
  printf("2 - Dna\n");
  printf("3 - Go mobile\n");
  printf("4 - Alkuun\n");
  printf("5 - Lopeta\n");

  int valinta = kysyNumero();
  if (valinta == 1) {
    return "saunalahti";
  }
  if (valinta == 2) {
    return "Dna";
  }
  if (valinta == 3) {
    return "Go mobile";
  }
  if (valinta == 4) {
    return "Takasalkuun";
  }
  if (valinta == 5) {
    return "Lopeta";
  }
  return "Lopeta";
}
int latausSummaValikko(int saldo) {
  printf("\n");
  printf("Liittyman lataus - valitse ladattava summa\n");
  if (saldo >= 10) {
    printf("1 - 10 euroa\n");
  }
  if (saldo >= 15) {
    printf("2 - 15 euroa\n");
  }
  if (saldo >= 20) {
    printf("3 - 20 euroa\n");
  }
  if (saldo >= 25) {
    printf("4 - 25 euroa\n");
  }
  if (saldo >= 30) {
    printf("5 - 30 euroa\n");
  }
  if (saldo >= 50) {
    printf("6 - 50 euroa\n");
  }
  if (saldo >= 100) {
    printf("7 - 100 euroa\n");
  }
  printf("8 - Alkuun\n");
  printf("9 - Lopeta\n");

  int valinta = kysyNumero();
  if (valinta == 1 && saldo >= 10) {
    return 10;
  }
  if (valinta == 2 && saldo >= 15) {
    return 15;
  }
  if (valinta == 3 && saldo >= 20) {
    return 20;
  }
  if (valinta == 4 && saldo >= 25) {
    return 25;
  }
  if (valinta == 5 && saldo >= 30) {
    return 30;
  }
  if (valinta == 6 && saldo >= 50) {
    return 50;
  }
  if (valinta == 7 && saldo >= 100) {
    return 100;
  }
  if (valinta == 8) {
    return -99;
  }
  if (valinta == 9) {
    return -999;
  }
  return -99;
}
int ottoSummaValikko(int saldo) {
  printf("\n");
  printf("OTTO\n");
  if (saldo >= 20) {
    printf("1 - 20 euroa\n");
  }
  if (saldo >= 40) {
    printf("2 - 40 euroa\n");
  }
  if (saldo >= 60) {
    printf("3 - 60 euroa\n");
  }
  if (saldo >= 90) {
    printf("4 - 90 euroa\n");
  }
  if (saldo >= 140) {
    printf("5 - 140 euroa\n");
  }
  if (saldo >= 240) {
    printf("6 - 240 euroa\n");
  }
  printf("7 - muu\n");
  printf("8 - alkuun\n");
  printf("9 - Lopeta\n");

  int valinta = kysyNumero();
  if (valinta == 1 && saldo >= 20) {
    return 20;
  }
  if (valinta == 2 && saldo >= 40) {
    return 40;
  }
  if (valinta == 3 && saldo >= 60) {
    return 60;
  }
  if (valinta == 4 && saldo >= 90) {
    return 90;
  }
  if (valinta == 5 && saldo >= 140) {
    return 140;
  }
  if (valinta == 6 && saldo >= 240) {
    return 240;
  }
  if (valinta == 7) {
    int nosto, tasaNosto;
    while (valinta == 7) {
      printf("Syota summa, maksimi nosto %d euroa\n",
             (saldo < 1000) ? saldo : 1000);
      printf("1 - alkuun\n");
      printf("2 - Lopeta\n");
      printf(
          "Voit nostaa vain tasasummia jotka koostuvat 50 euron ja 20 euron "
          "seteleistä\n");
      nosto = kysyNumero();
      if (nosto == 0) {
        continue;
      }

      if (nosto <= 0 || nosto > 1000 || nosto > saldo) {
        continue;
      }

      tasaNosto = nosto % 10 == 0;
      if (tasaNosto && nosto != 30 && nosto != 10) {
        return (nosto <= 1000) ? nosto : 1000;
      } else if (nosto == 1) {
        return -99;
      } else if (nosto == 2) {
        return -999;
      }
    }
  }
  if (valinta == 8) {
    return -99;
  }
  if (valinta == 9) {
    return -999;
  }
  // muu syöte kuin vaihtoehdoissa annettu, niin palataan alkuun
  return -99;
}
void tulostaSaldo(int saldo) {
  printf("Tilin tilanne\n");
  printf("Tilin saldo                    %d\n", saldo);
  printf("Tililta nostettavissa          %d\n", saldo);
  printf("Kortin kateisnostovara         %d\n", saldo);
  printf("1 - Palaa alkuun\n");
  printf("2 - Lopeta\n");
}
void tulostaTapahtumat(void) {
  printf("Tapahtumat\n");
  printf("Tapahtuma1.. diipa\n");
  printf("Tapahtuma2.. daapa\n");
  printf("1 - Lopeta tarkastelu\n");
}

void tulostaViesti(int valinta, int saldo, int otto) {
  if (otto < 0) {
    // koska setelien tulostus hoidetaan tätä kautta, varmistetaan ettei
    // negatiivisia ottoja
    otto = 0;
  }

  printf("Ota kortti ole hyva\n");

  if (valinta == 1) {
    printf("Puheaika maksettu\n");
    printf("Tilin saldo %d\n", saldo);
  } else if (valinta == 2) {
    printf("Rahat tulossa\n");
    tulostaSetelit(otto);
    printf("Tilin saldo %d\n", saldo);
  } else {
    printf("Lopetit - Hyvaa paivanjatkoa\n");
  }

  printf("Kiitos kaynnista ja tervetuloa uudelleen\n");
}
void tulostaSetelit(int summa) {
  int viiskymppiset = summa / 50;
  int kakskymppiset = (summa - viiskymppiset * 50) / 20;
  int summaTasan = (summa - viiskymppiset * 50) % 20 == 0;
  if (summaTasan) {
    printf(
        "Sait %d kpl viidenkymmenen euron ja %d kpl kahdenkymmenen euron "
        "setelia\n",
        viiskymppiset, kakskymppiset);
  } else {
    viiskymppiset--;
    kakskymppiset = (summa - viiskymppiset * 50) / 20;
    printf(
        "Sait %d kpl viidenkymmenen euron ja %d kpl kahdenkymmenen euron "
        "setelia\n",
        viiskymppiset, kakskymppiset);
  }
}