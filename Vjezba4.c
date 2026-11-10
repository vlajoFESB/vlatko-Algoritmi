
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct polinom;
typedef struct polinom* Pozicija;

struct polinom {
    double koef;
    int eksponent;
    Pozicija next;
};

Pozicija stvoriClan(double koef, int eksponent)
{
    Pozicija novi = (Pozicija)malloc(sizeof(struct polinom));

    if (novi == NULL)
        return NULL;

    novi->koef = koef;
    novi->eksponent = eksponent;
    novi->next = NULL;

    return novi;
}

void dodajClan(Pozicija head, double koef, int eksponent)
{
    Pozicija p = head;

    while (p->next != NULL && p->next->eksponent > eksponent)
        p = p->next;

    if (p->next != NULL && p->next->eksponent == eksponent) {
        p->next->koef += koef;

        if (p->next->koef == 0) {
            Pozicija temp = p->next;
            p->next = temp->next;
            free(temp);
        }
        return;
    }

    if (koef == 0)
        return;

    Pozicija novi = stvoriClan(koef, eksponent);

    if (novi == NULL)
        return;

    novi->next = p->next;
    p->next = novi;
}

Pozicija procitajDatoteku(const char* naziv)
{
    FILE* dat = fopen(naziv, "r");
    Pozicija head = stvoriClan(0, 0);
    double koef;
    int eksponent;

    if (dat == NULL || head == NULL)
        return NULL;

    while (fscanf(dat, "%lf %d", &koef, &eksponent) == 2)
        dodajClan(head, koef, eksponent);

    fclose(dat);
    return head;
}

Pozicija zbroji(Pozicija p1, Pozicija p2)
{
    Pozicija rezultat = stvoriClan(0, 0);
    Pozicija p;

    if (rezultat == NULL)
        return NULL;

    for (p = p1->next; p != NULL; p = p->next)
        dodajClan(rezultat, p->koef, p->eksponent);

    for (p = p2->next; p != NULL; p = p->next)
        dodajClan(rezultat, p->koef, p->eksponent);

    return rezultat;
}

Pozicija pomnozi(Pozicija p1, Pozicija p2)
{
    Pozicija rezultat = stvoriClan(0, 0);
    Pozicija p, q;

    if (rezultat == NULL)
        return NULL;

    for (p = p1->next; p != NULL; p = p->next) {
        for (q = p2->next; q != NULL; q = q->next) {
            dodajClan(rezultat, p->koef * q->koef,
                p->eksponent + q->eksponent);
        }
    }

    return rezultat;
}

void ispisi(Pozicija head)
{
    Pozicija p = head->next;

    if (p == NULL) {
        printf("0\n");
        return;
    }

    while (p != NULL) {
        printf("(%.2lf)x^%d", p->koef, p->eksponent);

        if (p->next != NULL)
            printf(" + ");

        p = p->next;
    }

    printf("\n");
}

void oslobodi(Pozicija head)
{
    Pozicija temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    Pozicija p1 = procitajDatoteku("polinom1.txt");
    Pozicija p2 = procitajDatoteku("polinom2.txt");
    Pozicija zbroj, umnozak;

    if (p1 == NULL || p2 == NULL) {
        printf("Greska pri citanju datoteke.\n");
        return 1;
    }

    zbroj = zbroji(p1, p2);
    umnozak = pomnozi(p1, p2);

    printf("Prvi polinom:\n");
    ispisi(p1);

    printf("Drugi polinom:\n");
    ispisi(p2);

    printf("Zbroj polinoma:\n");
    ispisi(zbroj);

    printf("Umnozak polinoma:\n");
    ispisi(umnozak);

    oslobodi(p1);
    oslobodi(p2);
    oslobodi(zbroj);
    oslobodi(umnozak);

    return 0;
}
