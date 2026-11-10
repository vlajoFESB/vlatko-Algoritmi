
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
    if (nova == NULL)
        return NULL;

    strcpy(nova->ime, ime);
    strcpy(nova->prezime, prezime);
    nova->godinaRodenja = godina;
    nova->next = NULL;
    return nova;
}

int dodajNaKraj(Pozicija head, const char* ime, const char* prezime, int godina)
{
    Pozicija nova = stvoriOsobu(ime, prezime, godina);
    Pozicija p = head;

    if (nova == NULL)
        return -1;

    while (p->next != NULL)
        p = p->next;

    p->next = nova;
    return 0;
}

int dodajIza(Pozicija head, const char* trazeno, const char* ime, const char* prezime, int godina)
{
    Pozicija p = head->next;

    while (p != NULL && strcmp(p->prezime, trazeno) != 0)
        p = p->next;

    if (p == NULL)
        return -1;

    Pozicija nova = stvoriOsobu(ime, prezime, godina);
    if (nova == NULL)
        return -1;

    nova->next = p->next;
    p->next = nova;

    return 0;
}

int dodajIspred(Pozicija head, const char* trazeno, const char* ime, const char* prezime, int godina)
{
    Pozicija p = head;

    while (p->next != NULL && strcmp(p->next->prezime, trazeno) != 0)
        p = p->next;

    if (p->next == NULL)
        return -1;

    Pozicija nova = stvoriOsobu(ime, prezime, godina);
    if (nova == NULL)
        return -1;

    nova->next = p->next;
    p->next = nova;

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

void sortirajListu(Pozicija head)
{
    Pozicija p, q;
    char ime[MAX], prezime[MAX];
    int godina;

    for (p = head->next; p != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if (strcmp(p->prezime, q->prezime) > 0) {
                strcpy(ime, p->ime);
                strcpy(prezime, p->prezime);
                godina = p->godinaRodenja;

                strcpy(p->ime, q->ime);
                strcpy(p->prezime, q->prezime);
                p->godinaRodenja = q->godinaRodenja;

                strcpy(q->ime, ime);
                strcpy(q->prezime, prezime);
                q->godinaRodenja = godina;
            }
        }
    }
}

void upisiUDatoteku(Pozicija head)
{
    FILE* dat = fopen("osobe.txt", "w");
    Pozicija p = head->next;

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    while (p != NULL) {
        fprintf(dat, "%s %s %d\n", p->ime, p->prezime, p->godinaRodenja);
        p = p->next;
    }

    fclose(dat);
}

void procitajIzDatoteke(Pozicija head)
{
    FILE* dat = fopen("osobe.txt", "r");
    char ime[MAX], prezime[MAX];
    int godina;

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    while (fscanf(dat, "%49s %49s %d", ime, prezime, &godina) == 3) {
        dodajNaKraj(head, ime, prezime, godina);
    }

    fclose(dat);
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
    char ime[MAX], prezime[MAX], trazeno[MAX];
    int godina, izbor;

    do {
        printf("\n===== IZBORNIK =====\n");
        printf("1 - Dodaj na kraj\n");
        printf("2 - Ispisi listu\n");
        printf("3 - Dodaj iza osobe\n");
        printf("4 - Dodaj ispred osobe\n");
        printf("5 - Sortiraj listu\n");
        printf("6 - Upisi u datoteku\n");
        printf("7 - Procitaj iz datoteke\n");
        printf("0 - Izlaz\n");
        printf("Izbor: ");
        if (scanf("%d", &izbor) != 1)
            break;

        switch (izbor) {
        case 1:
            unesiPodatke(ime, prezime, &godina);
            dodajNaKraj(&head, ime, prezime, godina);
            break;

        case 2:
            ispisiListu(&head);
            break;

        case 3:
            printf("Iza kojeg prezimena dodati osobu: ");
            scanf("%49s", trazeno);
            unesiPodatke(ime, prezime, &godina);
            if (dodajIza(&head, trazeno, ime, prezime, godina) == 0)
                printf("Osoba dodana.\n");
            else
                printf("Osoba nije pronadena.\n");
            break;

        case 4:
            printf("Ispred kojeg prezimena dodati osobu: ");
            scanf("%49s", trazeno);
            unesiPodatke(ime, prezime, &godina);
            if (dodajIspred(&head, trazeno, ime, prezime, godina) == 0)
                printf("Osoba dodana.\n");
            else
                printf("Osoba nije pronadena.\n");
            break;

        case 5:
            sortirajListu(&head);
            printf("Lista je sortirana.\n");
            break;

        case 6:
            upisiUDatoteku(&head);
            printf("Lista je upisana u datoteku.\n");
            break;

        case 7:
            procitajIzDatoteke(&head);
            printf("Lista je procitana iz datoteke.\n");
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
