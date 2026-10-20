#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

struct osoba;
typedef struct osoba* Pozicija;

struct osoba {
    char ime[MAX];
    char prezime[MAX];
    int godinaRodenja;
    Pozicija next;
};

Pozicija stvoriOsobu(const char* ime, const char* prezime, int godina)
{
    Pozicija nova = (Pozicija)malloc(sizeof(struct osoba));
    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    strcpy(nova->ime, ime);
    strcpy(nova->prezime, prezime);
    nova->godinaRodenja = godina;
    nova->next = NULL;
    return nova;
}

int dodajNaPocetak(Pozicija head, const char* ime, const char* prezime, int godina)
{
    Pozicija nova = stvoriOsobu(ime, prezime, godina);
    if (nova == NULL)
        return -1;

    nova->next = head->next;
    head->next = nova;
    return 0;
}

void ispisiListu(Pozicija head)
{
    Pozicija p = head->next;

    if (p == NULL) {
        printf("Lista je prazna.\n");
        return;
    }
    printf("\n%-15s %-15s %s\n", "Ime", "Prezime", "Godina rodenja");
    printf("--------------------------------------------\n");
    while (p != NULL) {
        printf("%-15s %-15s %d\n", p->ime, p->prezime, p->godinaRodenja);
        p = p->next;
    }
}

int dodajNaKraj(Pozicija head, const char* ime, const char* prezime, int godina)
{
    Pozicija nova = stvoriOsobu(ime, prezime, godina);
    Pozicija p = head;

    if (nova == NULL)
        return -1;

    while (p->next != NULL)
        p = p->next;

    nova->next = p->next;   
    p->next = nova;
    return 0;
}

Pozicija pronadiPoPrezimenu(Pozicija head, const char* prezime)
{
    Pozicija p = head->next;

    while (p != NULL && strcmp(p->prezime, prezime) != 0)
        p = p->next;

    return p;
}

Pozicija pronadiPrethodnika(Pozicija head, const char* prezime)
{
    Pozicija prev = head;

    while (prev->next != NULL && strcmp(prev->next->prezime, prezime) != 0)
        prev = prev->next;

    if (prev->next == NULL)
        return NULL;

    return prev;
}

int obrisiElement(Pozicija head, const char* prezime)
{
    Pozicija prev = pronadiPrethodnika(head, prezime);
    Pozicija zaBrisanje;

    if (prev == NULL)
        return -1;

    zaBrisanje = prev->next;
    prev->next = zaBrisanje->next;
    free(zaBrisanje);
    return 0;
}

void oslobodiListu(Pozicija head)
{
    Pozicija temp;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
}

void unesiPodatke(char* ime, char* prezime, int* godina)
{
    printf("Ime: ");
    scanf("%49s", ime);
    printf("Prezime: ");
    scanf("%49s", prezime);
    printf("Godina rodenja: ");
    scanf("%d", godina);
}

int main(void)
{
    struct osoba head = { "", "", 0, NULL };  
    char ime[MAX], prezime[MAX];
    int godina, izbor;
    Pozicija nadena;

    do {
        printf("\n===== IZBORNIK =====\n");
        printf("1 - Dodaj na pocetak\n");
        printf("2 - Ispisi listu\n");
        printf("3 - Dodaj na kraj\n");
        printf("4 - Pronadi po prezimenu\n");
        printf("5 - Obrisi po prezimenu\n");
        printf("0 - Izlaz\n");
        printf("Izbor: ");
        if (scanf("%d", &izbor) != 1)
            break;

        switch (izbor) {
        case 1:
            unesiPodatke(ime, prezime, &godina);
            dodajNaPocetak(&head, ime, prezime, godina);
            break;
        case 2:
            ispisiListu(&head);
            break;
        case 3:
            unesiPodatke(ime, prezime, &godina);
            dodajNaKraj(&head, ime, prezime, godina);
            break;
        case 4:
            printf("Prezime: ");
            scanf("%49s", prezime);
            nadena = pronadiPoPrezimenu(&head, prezime);
            if (nadena != NULL)
                printf("Pronadeno: %s %s, %d\n",
                    nadena->ime, nadena->prezime, nadena->godinaRodenja);
            else
                printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
            break;
        case 5:
            printf("Prezime: ");
            scanf("%49s", prezime);
            if (obrisiElement(&head, prezime) == 0)
                printf("Element obrisan.\n");
            else
                printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
            break;
        case 0:
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);

    oslobodiListu(&head);
    return 0;
}