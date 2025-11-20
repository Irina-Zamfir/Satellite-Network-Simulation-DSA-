/*Zamfir Irina-Maria 313CC*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct satelite
{
    char *nume;
    int frecv;
    struct satelite *mother;

    int nr_copii;
    struct satelite **children;
};
typedef struct satelite satelite;

struct nod
{
    satelite *sat;
    struct nod *next;
};
typedef struct nod nod;

struct coada
{
    nod *front, *rear;
};
typedef struct coada coada;

/*functie de comparare, care returneaza 1 daca s1 este mai mic ca frecventa
sau lexicografic decat s2, si 0 in caz contrar*/
int compare(satelite *s1, satelite *s2)
{
    if (s1->frecv < s2->frecv)
        return 1;

    if (s1->frecv > s2->frecv)
        return 0;

    if (strcmp(s1->nume, s2->nume) < 0)
        return 1;

    return 0;
}

/*functie de interschimbare a 2 sateliti*/
void swap(satelite **a, satelite **b)
{
    satelite *temp = *a;
    *a = *b;
    *b = temp;
}

/*functie care initializeaza un nou satelit de legatura*/
satelite *new_satelite(satelite *s1, satelite *s2, int nr_sat)
{
    /*alocare dinamica*/
    satelite *s = (satelite *)malloc(sizeof(satelite));
    s->nume = (char *)malloc((16 * nr_sat + 1) * sizeof(char));

    s->nr_copii = 2;
    s->children = (satelite **)malloc(sizeof(satelite *) * 2);

    /*calculez frecventa ca suma a frecventelor celor 2 sateliti*/
    s->frecv = s1->frecv + s2->frecv;

    /*concatenez numele celor 2 sateliti*/
    strcpy(s->nume, s1->nume);
    strcat(s->nume, s2->nume);

    /*s1 si s2 devin copii satelitului nou*/
    s->children[0] = s1;
    s->children[1] = s2;

    /*parintele satelitului nou este NULL, dar acesta este parinte
    pt s1 si s2*/
    s->mother = NULL;
    s1->mother = s;
    s2->mother = s;

    /*returnez satelitul de legatura creat*/
    return s;
}

/*functie de inserare in heap*/
void insert(satelite **h, int *size, satelite *new_data)
{
    /*adaug noul element pe ultima pozitie in heap*/
    h[*size] = new_data;
    (*size)++;

    /* heapify - UP */
    int i = *size - 1;
    int parent = (i - 1) / 2;
    /*cat timp elementul nou este mai mic decat parintele sau*/
    while (i > 0 && compare(h[i], h[parent]))
    {
        /*interschimb elementul nou cu parintele sau*/
        swap(&h[i], &h[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

/*functia de heapify*/
void heapify(satelite **h, int n, int i)
{
    /*ponesc cu indicele parinte, ca fiind cel mai mic*/
    int smallest = i;
    int left = 2 * i + 1;  /*copilul stang*/
    int right = 2 * i + 2; /*copilul drept*/

    /*daca copilul stang e mai mic decat parintele, indicele celui
    mai mic elem este al copilului stang*/
    if (left < n && compare(h[left], h[smallest]))
        smallest = left;

    /*daca copilul drept e mai mic decat parintele, indicele celui
    mai mic elem este al copilului drept*/
    if (right < n && compare(h[right], h[smallest]))
        smallest = right;

    /*daca indicele celui mai mic nu mai este parintele, interschimb
    satelitii*/
    if (smallest != i)
    {
        swap(&h[i], &h[smallest]);
        heapify(h, n, smallest);
    }
}

/*functie de extragere a celui mai mic element din heap*/
satelite *extract(satelite **h, int *size)
{
    /*salvez in s elementul de pe pozitia 0*/
    satelite *s = h[0];

    /*mut pe prima pozitie ultimul element din heap*/
    h[0] = h[--(*size)];

    /*apelez functia de heapify (down) pt a reordona heapul*/
    heapify(h, *size, 0);

    /*returnez s*/
    return s;
}

/*functie care verif daca coada q este goala sau nu*/
int isEmpty(coada *q)
{
    return q->front == NULL;
}

/*functie care pune la finalul cozii un nou element*/
void enqueue(coada *q, satelite *s)
{
    /*aloc dinamic un nou nod pt coada*/
    nod *newnode = (nod *)malloc(sizeof(nod));
    newnode->sat = s;
    newnode->next = NULL;

    /*daca este primul elem pe care il pun in coada, atat front cat
    si rear pointeaza catre el*/
    if (q->rear == NULL)
        q->front = q->rear = newnode;
    else /*altfel, il leg la finalul cozii*/
    {
        q->rear->next = newnode;
        q->rear = newnode;
    }
}

/*functia care scoate si returneaza primul element din coada*/
satelite *dequeue(coada *q)
{
    /*salvez in s primul element din coada*/
    satelite *s = q->front->sat;

    /*salvez in elem_curr adresa primului element din coada*/
    nod *elem_curr = q->front;

    /*sar la urmatorul element*/
    q->front = q->front->next;

    /*daca am o coada acum goala, rear va pointa la null*/
    if (q->front == NULL)
        q->rear = NULL;

    /*eliberez memoria primului element*/
    free(elem_curr);

    /*returnez satelitul s*/
    return s;
}

/*functie care afiseaza reteaua de sateliti conform cerintei 1*/
void print(satelite *s, FILE *out)
{
    /*initializez si aloc memorie unei cozi*/
    coada *q = (coada *)malloc(sizeof(coada));
    q->front = q->rear = NULL;

    /*pun satelitul s in coada*/
    enqueue(q, s);

    /*cat timp sunt elemente in coada*/
    while (!isEmpty(q))
    {
        /*in level salvez nr de sateliti pe nivel*/
        int level = 0;
        nod *ptr_curr = q->front;

        /*numar satelitii de pe nivel - cate elemente sunt in coada*/
        while (ptr_curr != NULL)
        {
            level++;
            ptr_curr = ptr_curr->next;
        }

        /*pt fiecare satelit de pe nivel*/
        for (int i = 0; i < level; i++)
        {
            /*il scot din coada si ii afisez frecventa si numele*/
            s = dequeue(q);
            fprintf(out, "%d-%s ", s->frecv, s->nume);

            /*parcurg vectorul de copii si ii pun copii in coada*/
            for (int k = 0; k < s->nr_copii; k++)
                if (s->children[k] != NULL)
                    enqueue(q, s->children[k]);
        }

        fprintf(out, "\n");
    }

    /*eliberez coada*/
    free(q);
}

/*functie de decodificare a mesajului sursei necunoscute, pt ceritna 2*/
void decodificare(satelite *s, char cod[], FILE *out)
{
    int i = 0;
    satelite *sat_curr = s;

    /*parcurg codul element cu element*/
    while (cod[i] != '\0')
    {
        /*daca elementul este '0'*/
        if (cod[i] == '0')
        {
            /*daca satelitul curent are copil stang, voi merge pe ramura
            stanga a retelei*/
            if (sat_curr->children[0] != NULL)
                sat_curr = sat_curr->children[0];
            else /*altfel*/
            {
                /*afisez numele satelitului curent*/
                fprintf(out, "%s ", sat_curr->nume);
                sat_curr = s;
                i--;
            }
        }
        else /*altfel*/
        {
            /*daca satelitul curent are copil drept, voi merge pe ramura
            dreapta a retelei*/
            if (sat_curr->children[1] != NULL)
                sat_curr = sat_curr->children[1];
            else /*altfel*/
            {
                /*afisez numele sat curent*/
                fprintf(out, "%s ", sat_curr->nume);
                sat_curr = s;
                i--;
            }
        }

        i++;
    }

    /*daca satelitului curent ramas nu are niciun copil, ii afisez numele*/
    if (sat_curr->children[0] == NULL && sat_curr->children[1] == NULL)
        fprintf(out, "%s", sat_curr->nume);

    fprintf(out, "\n");
}

/*functie de gasire a drumului de la radacina pana la satelit, pt cerinta 3*/
void find_path(satelite *s, char nume_sat[], FILE *out)
{
    char rez[1001] = "";
    int k = 0;

    /*initial aflu drumul de la satelit la radacina*/
    /*cat timp satelitul are un parinte*/
    while (s->mother != NULL)
    {
        s = s->mother; /*trec la parinte*/

        /*daca copilul stang este satelitul pe care il caut*/
        if (strcmp(s->children[0]->nume, nume_sat) == 0)
        {
            /*adaug '0' in vectorul codului si salvez numele parintelui*/
            rez[k++] = '0';
            strcpy(nume_sat, s->nume);
        }
        else /*altfel*/
        {
            /*adaug '1' in vectorul codului si salvez numele parintelui*/
            rez[k++] = '1';
            strcpy(nume_sat, s->nume);
        }
    }

    /*pun terminator de sir*/
    rez[k] = '\0';

    /*inversez elementele sirului*/
    for (int i = 0; i < k / 2; i++)
    {
        char aux = rez[i];
        rez[i] = rez[k - i - 1];
        rez[k - i - 1] = aux;
    }

    /*afisez codul*/
    fprintf(out, "%s", rez);
}

/*functie de cautare a satelitului dupa nume, pe nivele*/
satelite *bfs(satelite *s, char nume_sat[])
{
    int ok = 0; /*variabila care tine minte daca am gasit satelitul sau nu*/

    /*initializez si aloc dinamic memorie pt coada*/
    coada *q = (coada *)malloc(sizeof(coada));
    q->front = q->rear = NULL;

    /*pun in coada satelitul s (radacina)*/
    enqueue(q, s);

    /*cat timp sunt elemente in coada*/
    while (!isEmpty(q))
    {
        /*scot primul element*/
        s = dequeue(q);

        /*daca este satelitul pe care il caut, ok ia val lui 1 si dau break*/
        if (strcmp(s->nume, nume_sat) == 0)
        {
            ok = 1;
            break;
        }

        /*parcurg copii satelitului si ii bag in coada*/
        for (int i = 0; i < s->nr_copii; i++)
            enqueue(q, s->children[i]);
    }

    /*daca au mai ramas eleme in coada le scot*/
    while (!isEmpty(q))
        dequeue(q);

    /*eliberez memoria cozii*/
    free(q);

    /*daca nu am gasit satelitul, s devine null*/
    if (ok == 0)
        s = NULL;

    /*il returnez pe s*/
    return s;
}

/*functie care gaseste nivelul pe care se afla un satelit*/
int find_level(satelite *s)
{
    int lev = 0; /*radacina se afla pe nivelul 0*/

    /*cat timp satelitul are mama*/
    while (s->mother != NULL)
    {
        lev++;         /*cresc nivelul*/
        s = s->mother; /*satelitul devine parinte*/
    }

    /*returnez valoarea gasita*/
    return lev;
}

/*functie care gaseste si returneaza cel mai apropiat satelitul
de legatura a 2 sateliti, pt cerinta 4*/
satelite *find_common_satelite(satelite *s1, satelite *s2)
{
    int lev1 = find_level(s1); /*nivelul lui s1*/
    int lev2 = find_level(s2); /*nivelul lui s2*/

    /*daca s1 este radacina, o returnam*/
    if (lev1 == 0)
        return s1;

    /*daca s1 este pe un nivel mai adanc decat s2*/
    if (lev1 > lev2)
    {
        /*s1 urca pe nivel pana cand ajunge pe acelasi nivel cu s2*/
        while (lev1 > lev2)
        {
            s1 = s1->mother;
            lev1--;
        }
    }
    /*altfel daca s2 este pe un nivel mai adanc decat s1*/
    else if (lev2 > lev1)
    {
        /*s2 urca pe nivel pana cand ajunge pe acelasi nivel cu s1*/
        while (lev2 > lev1)
        {
            s2 = s2->mother;
            lev2--;
        }
    }

    /*cat timp nu sunt pe nivelul 0*/
    while (lev1 > 0)
    {
        /*daca am gasit satelitul comun, il returnez*/
        if (strcmp(s1->nume, s2->nume) == 0)
            return s1;

        /*s1 si s2 devin parinti*/
        s1 = s1->mother;
        s2 = s2->mother;

        lev1--; /*scad nivelul*/
    }
}

/*functie care calculeaza distanta dintre 2 sateliti pentru cerinta 5*/
int find_distance(satelite *s1, satelite *s2)
{
    int lev1 = find_level(s1); /*nivelul lui s1*/
    int lev2 = find_level(s2); /*nivelul lui s2*/
    int length = 0;            /*distanta initiala = 0*/

    /*in timp ce fac satelitii sa ajunga pe acelasi nivel,
    cresc lungimea distantei*/
    if (lev1 > lev2)
    {
        while (lev1 > lev2)
        {
            s1 = s1->mother;
            lev1--;
            length++;
        }
    }
    else if (lev2 > lev1)
    {
        while (lev2 > lev1)
        {
            s2 = s2->mother;
            lev2--;
            length++;
        }
    }

    /*cat timp drumurile celor 2 sateliti nu s-au unit
    (si nu sunt pe nivelul radacinii)*/
    while (lev1 > 0)
    {
        /*daca am gasit satelitul comun, returnez distanta*/
        if (strcmp(s1->nume, s2->nume) == 0)
            return length;

        /*s1 si s2 devin parinti*/
        s1 = s1->mother;
        s2 = s2->mother;

        lev1--;      /*scad nivelul*/
        length += 2; /*cresc lungimea distantei cu 2*/
    }

    /*returnez lungimea distantei daca s-a ajuns la radacina*/
    return length;
}

/*functie de eliberare recursiva a memoriei*/
void free_tree(satelite *s)
{
    /*conditie de intoarecere, s==null*/
    if (s == NULL)
        return;

    /*parcurg vectorul de copii si apelez functia pt fiecare dintre ei*/
    for (int i = 0; i < s->nr_copii; i++)
        free_tree(s->children[i]);

    free(s->children); /*eliberez vectorul de copii*/
    free(s->nume);     /*eliberez memoria alocata pt numele satelitului*/
    free(s);           /*eliberez memoria alocata pt satelit*/
}

int main(int argc, char *argv[])
{
    /*salvez in opt, cerinta ce urm a fi rezolvata, transmisa prin
    linia de comanda*/
    char opt[5];
    strcpy(opt, argv[1]);

    /*deschid fisierele, transmise ca argument in linia de comanda*/
    FILE *in = fopen(argv[2], "r");
    FILE *out = fopen(argv[3], "w");

    /*citesc numarul de sateliti*/
    int sat_nr;
    fscanf(in, "%d", &sat_nr);
    fgetc(in);

    /*initializez si aloc memorie vectorului de sateliti*/
    satelite **Satelites = (satelite **)malloc(sizeof(satelite *) * sat_nr);

    int n = 0; /* nr de sateliti din heap */
    for (int i = 0; i < sat_nr; i++)
    {
        int frecv;
        char nume[16];

        /*citesc frecventa si numele fiecarui satelit*/
        fscanf(in, "%d%s", &frecv, nume);

        /*initializez si aloc memoria unui satelit local pe care il voi
        insera in heap*/
        satelite *s = (satelite *)malloc(sizeof(satelite));
        s->nume = (char *)malloc((16 * sat_nr + 1) * sizeof(char));

        s->frecv = frecv;
        strcpy(s->nume, nume);
        s->nume[strlen(nume) + 1] = '\0';

        s->children = (satelite **)malloc(sizeof(satelite *) * 2);
        s->nr_copii = 0;
        s->children[0] = NULL;
        s->children[1] = NULL;
        s->mother = NULL;

        /*inserez satelitul in min-heap, iar nr de sateliti din heap
        va creste*/
        insert(Satelites, &n, s);
    }

    /* construiesc reteaua de sateliti */
    while (n > 1)
    {
        /*extrag cei mai mici sateliti din retea*/
        satelite *s1 = extract(Satelites, &n);
        satelite *s2 = extract(Satelites, &n);

        /*construiesc un satelit nou cu ei*/
        satelite *new_s = new_satelite(s1, s2, sat_nr);

        /*inserez satelitul nou in heap*/
        insert(Satelites, &n, new_s);
    }

    /*afisez reteaua de sateliti pentru cerinta 1*/
    if (strcmp(opt, "-c1") == 0)
        print(Satelites[0], out);

    /*pentru cerinta 2*/
    if (strcmp(opt, "-c2") == 0)
    {
        /*citesc numarul de coduri*/
        int nr_cod;
        fscanf(in, "%d", &nr_cod);
        fgetc(in);

        for (int i = 0; i < nr_cod; i++)
        {
            /*citesc codul*/
            char cod[1001];
            fscanf(in, "%s", cod);
            cod[strlen(cod) + 1] = '\0';

            /*il decodific*/
            decodificare(Satelites[0], cod, out);
        }
    }

    /*pentru cerinta 3*/
    if (strcmp(opt, "-c3") == 0)
    {
        /*citesc numarul de sateliti*/
        int n_sat;
        fscanf(in, "%d", &n_sat);
        fgetc(in);

        for (int i = 0; i < n_sat; i++)
        {
            /*citesc numele satelitului*/
            char nume[16];
            fscanf(in, "%s", nume);

            /*gasesc satelitul in retea, dupa nume*/
            satelite *s = bfs(Satelites[0], nume);
            /*codific drumul pe care mesajul va calatori*/
            find_path(s, nume, out);
        }

        fprintf(out, "\n");
    }

    /*pentru cerinta 4*/
    if (strcmp(opt, "-c4") == 0)
    {
        /*citesc numarul de sateliti*/
        int n_sat;
        fscanf(in, "%d", &n_sat);
        fgetc(in);

        /*citesc numele primului satelit*/
        char nume[16];
        fscanf(in, "%s", nume);

        /*gasesc satelitul in retea*/
        satelite *s1 = bfs(Satelites[0], nume);

        /*initializez satelitul comun cu null*/
        satelite *s_mother = NULL;

        for (int i = 1; i < n_sat; i++)
        {
            /*citesc numele satelitului*/
            fscanf(in, "%s", nume);

            /*il gasesc in retea*/
            satelite *s2 = bfs(Satelites[0], nume);

            /*aflu satelitul comun intre s2 si s2*/
            s_mother = find_common_satelite(s1, s2);
            /*s1 devine satelitul comun curent*/
            s1 = s_mother;
        }

        /*afisez numele satelitului comun*/
        fprintf(out, "%s\n", s_mother->nume);
    }

    /*pentru cerinta 5*/
    if (strcmp(opt, "-c5") == 0)
    {
        /*citesc numarul de arbori multicai*/
        int nr_tree;
        fscanf(in, "%d", &nr_tree);
        fgetc(in);

        /*pt fiecare arbore*/
        for (int i = 0; i < nr_tree; i++)
        {
            /*citesc numele satelitului de care se va prinde arborele multicai*/
            char nume_sat_principal[16];
            fscanf(in, "%s", nume_sat_principal);

            /*il gasesc in reteaua de sateliti*/
            satelite *s_principal = bfs(Satelites[0], nume_sat_principal);

            /*citesc frecventa si numele radacinii arobrelui multicai*/
            int frecv;
            char nume[16];
            fscanf(in, "%d%s", &frecv, nume);
            nume[strlen(nume) + 1] = '\0';

            /*initializez si aloc dinamic memorie pentru radacina*/
            satelite *root = (satelite *)malloc(sizeof(satelite));
            root->nume = (char *)malloc((16 * sat_nr + 1) * sizeof(char));
            root->frecv = frecv;
            strcpy(root->nume, nume);
            /*parintele radacinii este satelitul din reteaua initiala*/
            root->mother = s_principal;
            root->nr_copii = 0;

            s_principal->children[s_principal->nr_copii] = root;
            s_principal->nr_copii++;

            /*citesc nr de parinti din arborele multicai*/
            int nr_parinti;
            fscanf(in, "%d", &nr_parinti);
            fgetc(in);

            /*pt fiecare parinte*/
            for (int j = 0; j < nr_parinti; j++)
            {
                /*ii citesc numele parintelui*/
                char nume_parinte[16];
                fscanf(in, "%s", nume_parinte);

                /*il caut in reteaua de sateliti*/
                satelite *parinte = bfs(root, nume_parinte);

                /*daca parintele nu este radacina si nu are parinte
                atunci parintele sau va fi radacina*/
                if (parinte != root && parinte->mother == NULL)
                    parinte->mother = root;

                /*citesc numarul de copii*/
                int nr_copii;
                fscanf(in, "%d", &nr_copii);

                /*aloc dinamic memorie pentru copiii parintelui*/
                parinte->children = (satelite **)malloc(sizeof(satelite *) * nr_copii);
                parinte->nr_copii = nr_copii;

                for (int k = 0; k < nr_copii; k++)
                    parinte->children[k] = NULL;

                /*pt fiecare copil*/
                for (int k = 0; k < nr_copii; k++)
                {
                    /*citesc frecventa si numele copilului*/
                    char nume_copil[16];
                    int frecv_copil;
                    fscanf(in, "%d%s", &frecv_copil, nume_copil);
                    nume_copil[strlen(nume_copil) + 1] = '\0';

                    /*introduc copilul in vectorul de copii al parintelui*/
                    parinte->children[k] = (satelite *)malloc(sizeof(satelite));
                    parinte->children[k]->nume = (char *)malloc((16 * sat_nr + 1) * sizeof(char));
                    parinte->children[k]->frecv = frecv_copil;
                    strcpy(parinte->children[k]->nume, nume_copil);

                    /*in campul de mama al copilului, ii pun parintele*/
                    parinte->children[k]->mother = parinte;
                    parinte->children[k]->nr_copii = 0;
                    parinte->children[k]->children = NULL;
                }
            }
        }

        /*init lungimea drumului*/
        int path = 0;
        /*citesc numele celor 2 sateliti, pt care vreau sa gasesc
        lungimea drumului*/
        char nume1[16], nume2[16];
        fscanf(in, "%s", nume1);
        fscanf(in, "%s", nume2);

        /*gasesc cei 2 sateliti in reteaua de sateliti*/
        satelite *s1, *s2;
        s1 = bfs(Satelites[0], nume1);
        s2 = bfs(Satelites[0], nume2);

        /*aflu distanta dintre ei*/
        path = find_distance(s1, s2);

        /*afisez distanta*/
        fprintf(out, "%d\n", path);
    }

    /*eliberez memoria*/
    free_tree(Satelites[0]);
    free(Satelites);

    /*inchid fisierele*/
    fclose(in);
    fclose(out);

    return 0;
}
