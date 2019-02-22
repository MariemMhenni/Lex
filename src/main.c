#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structures de données pour représenter un automate. */

/* Par défaut, tout nos automates travaillent sur l'alphabet ASCII. De plus,
 * comme tout nos automates sont standards, nous définissons l'état initial
 * d'un automate comme étant l'état d'indice 0. */

/* Numéro de l'état initial. */
#define ST_INIT 0

/** Structure de fonctions de transition. */
typedef struct trans {
    int nb;   /*!< Nombre de transitions possibles. nb = 1 pour un AFD, nb >= 1
                   pour un AFN. nb = 0 signifique que l'état ne possède aucune
                   transition. */
    int * st; /*!< st[i] correspond à l'état après la transition sur c[i]. */
    char * c; /*!< c[i] correspond au caractère de la transition i. */
} trans_s;

/** Structure d'un automate fini. */
typedef struct af {
    int st_nb;    /*!< Nombre d'état de l'automate. Les états sont numérotés de 0 à (states - 1). */
    int * st_ac;  /*!< Si state_fin[i] est à 1, alors l'état numéro i est un état accepteur. */
    trans_s * tr; /*!< trans[i] correspond à la transition pour l'état i. */
} af_s;

/** Type d'un automate fini déterministe. */
typedef struct af afd;
/** Type d'un automate fini non-déterministe. */
typedef struct af afn;

/* Fonctions utilitaires. */

/**
 * Met à jour le numéro des transitions d'un état.
 * @param st tableau des numéros des états.
 * @param size taille du tableau.
 * @param offset décalage à appliquer à chaque élément.
 */
static void af_std_trans_update(int * st, int size, int offset)
{
    for (int i = 0; i < size ; i++)
        st[i] += offset;
}

/**
 * Copie la liste des transitions d'un automate à un autre.
 * @param a1 automate de destination.
 * @param a2 automate source.
 * @param a1i itérateur sur les itérations de a1.
 * @param a2i itérateur sur les itérations de a2.
 * @param size nombre de transitions à copier.
 * @param update si 1, indique qu'il faut mettre à jour avec un offset de a1i.
 */
static void af_std_trans_copy(struct af * a1, struct af * a2, int a1i, int a2i, const int size, int update)
{
    /* "update" permet de mettre à jour les numéros des états de a1 avec un
     * décalage du à une copie précédente (lors de la concaténation des états de
     * deux automates). */
    update = update ? a1i : 0;
    for (int i = 0; i < size; i++, a1i++, a2i++) {
        a1->tr[a1i].nb = a2->tr[a2i].nb;
        a1->tr[a1i].st = malloc(a1->tr[a1i].nb * sizeof(*a1->tr[a1i].st));
        a1->tr[a1i].c  = malloc(a1->tr[a1i].nb * sizeof(*a1->tr[a1i].c));
        memcpy(a1->tr[a1i].st, a2->tr[a2i].st, a1->tr[a1i].nb * sizeof(*a1->tr[a1i].st));
        memcpy(a1->tr[a1i].c,  a2->tr[a2i].c,  a1->tr[a1i].nb * sizeof(*a1->tr[a1i].c));
        if (update)
            af_std_trans_update(a1->tr[a1i].st, a1->tr[a1i].nb, update);
    }
}

/* Fonctions pour reconnaître les automates de base de la méthode. */

/**
 * Retourne un automate standard reconnaissant le langage vide.
 */
afn af_std_gen_empty_lang()
{
    afn af = {0};
    af.st_nb = 1;                                   /* 1 seul état. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac)); /* Aucun état accepteur. */
    af.tr    = calloc(af.st_nb, sizeof(*af.tr));    /* Ancune transition. */
    return af;
}

/**
 * Retourne un automate standard reconnaissant le langage composé du seul mot vide.
 */
afn af_std_gen_empty_word()
{
    afn af = {0};
    af.st_nb = 1;                                /* 1 seul état. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac));
    af.st_ac[ST_INIT] = 1;                       /* 1 seul état accepteur : l'état initial. */
    af.tr    = calloc(af.st_nb, sizeof(*af.tr)); /* Ancune transition. */
    return af;
}

/**
 * Retourne un automate standard reconnaissant le langage composé d'un mot d'un
 * caractère passé en paramètre.
 */
afn af_std_gen_word(const char c)
{
    afn af = {0};
    af.st_nb = 2;             /* 2 états. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac));
    af.st_ac[1] = 1;          /* 1 seul état accepteur : l'état 1. */
    af.tr    = calloc(af.st_nb, sizeof(*af.tr));
    af.tr[ST_INIT].nb = 1;    /* 1 seule transition. */
    af.tr[ST_INIT].st = calloc(af.tr[ST_INIT].nb, sizeof(*af.tr[ST_INIT].st));
    af.tr[ST_INIT].c  = calloc(af.tr[ST_INIT].nb, sizeof(*af.tr[ST_INIT].c));
    af.tr[ST_INIT].st[0] = 1; /* Transition possible vers l'état 1. */
    af.tr[ST_INIT].c[0]  = c; /* Transition sur le caractère donné. */
    return af;
}

/**
 * Libère la mémoire d'un automate.
 */
void af_free(af_s af)
{
    free(af.st_ac);
    for (int i = 0; i < af.st_nb; i++)
        free(af.tr[i].st), free(af.tr[i].c);
    free(af.tr);
}

/* Fonctions pour reconnaître des langages plus évolués. */

/**
 * Retourne un automate standard qui reconnait la réunion des deux langages
 * reconnus par deux automates.
 */
afn af_std_union(afn a1, afn a2)
{
    afn af = {0};
    /* Nombre d'état. */
    af.st_nb = a1.st_nb + a2.st_nb - 1;
    
    /* États accepteurs. */
    af.st_ac = malloc(af.st_nb * sizeof(*af.st_ac));
    /* Indique si le nouvel état initial doit être accepteur. */
    af.st_ac[ST_INIT] = a1.st_ac[ST_INIT] || a2.st_ac[ST_INIT];
    /* Copie les listes d'états finaux successivement hormis les anciens états initiaux. */
    memcpy(&af.st_ac[ST_INIT + 1], &a1.st_ac[ST_INIT + 1], (a1.st_nb - 1) * sizeof(*a1.st_ac));
    memcpy(&af.st_ac[a1.st_nb],    &a2.st_ac[ST_INIT + 1], (a2.st_nb - 1) * sizeof(*a2.st_ac));

    /* Transitions. */
    af.tr = malloc(af.st_nb * sizeof(*af.tr));
    /* Copie les listes de transitions successivement hormis les anciens états initiaux. */
    af_std_trans_copy(&af, &a1, 1,        1, a1.st_nb - 1, 0);
    af_std_trans_copy(&af, &a2, a1.st_nb, 1, a2.st_nb - 1, 1);
    /* Concatène les transitions des états initiaux des deux automates dans un seul état inital. */
    af.tr[ST_INIT].nb = a1.tr[ST_INIT].nb + a2.tr[ST_INIT].nb;
    af.tr[ST_INIT].st = malloc(af.tr[ST_INIT].nb * sizeof(*af.tr[ST_INIT].st));
    af.tr[ST_INIT].c  = malloc(af.tr[ST_INIT].nb * sizeof(*af.tr[ST_INIT].c));
    memcpy(af.tr[ST_INIT].st,                     a1.tr[ST_INIT].st, a1.tr[ST_INIT].nb * sizeof(*a1.tr[ST_INIT].st));
    memcpy(af.tr[ST_INIT].c,                      a1.tr[ST_INIT].c,  a1.tr[ST_INIT].nb * sizeof(*a1.tr[ST_INIT].c));
    memcpy(&af.tr[ST_INIT].st[a1.tr[ST_INIT].nb], a2.tr[ST_INIT].st, a2.tr[ST_INIT].nb * sizeof(*a2.tr[ST_INIT].st));
    memcpy(&af.tr[ST_INIT].c[a1.tr[ST_INIT].nb],  a2.tr[ST_INIT].c,  a2.tr[ST_INIT].nb * sizeof(*a2.tr[ST_INIT].c));
    af_std_trans_update(&af.tr[ST_INIT].st[a1.tr[ST_INIT].nb], a2.tr[ST_INIT].nb, a1.st_nb - 1);
    return af;
}

/**
 * Retourne un automate standard qui reconnait la concaténation des deux langages
 * reconnus par deux automates.
 * @TODO
 */
afn af_std_cat(afn a1, afn a2)
{
    afn af = {0};
    /* Nombre d'état. */
    af.st_nb = a1.st_nb + a2.st_nb - 1;
    
    /* États accepteurs. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac));
    /* Copie les listes des états accepteurs, hormis l'ancien l'état initial de a2. */
    memcpy(af.st_ac,             a1.st_ac,               a1.st_nb * sizeof(*a1.st_ac));
    memcpy(&af.st_ac[a1.st_nb], &a2.st_ac[ST_INIT + 1], (a2.st_nb - 1) * sizeof(*a1.st_ac));
    /* Si l'état initial de a2 est non-accepteur, alors tout les états de af appartenant à a1 deviennent non-accepteur. */
    if (!a2.st_ac[ST_INIT])
        memset(af.st_ac, 0, a1.st_nb * sizeof(*af.st_ac));
    
    /* Transitions. */
    af.tr = calloc(af.st_nb, sizeof(*af.tr));
    /* Copie les listes de transitions successivement hormis l'ancien état initial de a2. */
    af_std_trans_copy(&af, &a1, 0,        0, a1.st_nb, 0);
    af_std_trans_copy(&af, &a2, a1.st_nb, 1, a2.st_nb - 1, 1);
    /* Tout les états accepteurs de af appartennant à a1 doivent récupérer les transitions qui partait de l'ancien état inital de a2. */
    for (int i = 0; i < a1.st_nb; i++) {
        if (a1.st_ac[i]) {
            af.tr[i].nb += a2.tr[ST_INIT].nb;
            af.tr[i].st = realloc(af.tr[i].st, af.tr[i].nb * sizeof(*af.tr[i].st));
            af.tr[i].c  = realloc(af.tr[i].c,  af.tr[i].nb * sizeof(*af.tr[i].c));
            memcpy(&af.tr[i].st[a1.tr[i].nb], a2.tr[ST_INIT].st, a2.tr[ST_INIT].nb * sizeof(*af.tr[i].st));
            memcpy(&af.tr[i].c[a1.tr[i].nb],  a2.tr[ST_INIT].c,  a2.tr[ST_INIT].nb * sizeof(*af.tr[i].c));
            af_std_trans_update(&af.tr[i].st[a1.tr[i].nb], a2.tr[ST_INIT].nb, a1.st_nb - 1);
        }
    }
    return af;
}

/**
 * Retourne un automate standard qui reconnait la fermeture itérative de Kleene
 * du langage reconnu par un automate.
 * @TODO
 */
afn af_std_star(afn a)
{
    afn af = {0};
    return af;
}

/* Fonctions pour manipuler un automate fini déterministe. */

/**
 * Exécute l'automate fini déterministe sur le mot en entrée.
 * @param afd Automate fini déterministe à exécuter.
 * @param m Mot sur lequelle exécuter l'automate.
 * @return 1 si le mot est reconnu, 0 sinon. 
 * @TODO
 */
int afd_exec(const afd a, const char * m)
{
    return 0;
}

/**
 * Déterminise un automate fini standard non-déterministe en un automate fini
 * déterministe standard.
 * @TODO
 */
afd afd_from_af_std(const afd a)
{
    afd af = {0};
    return af;
}

/**
 * Minimise un automate fini standard déterministe.
 * @TODO
 */
afd afd_min_from_afd(const afd a)
{
    afd af = {0};
    return af;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    printf("Hello World!\n");
    afn empty = af_std_gen_empty_word();
    afn a = af_std_gen_word('a');
    afn b = af_std_gen_word('b');
    afn c = af_std_gen_word('c');
    afn ab = af_std_cat(a, b);
    afn abc = af_std_union(ab, c);
    af_free(empty);
    af_free(a);
    af_free(b);
    af_free(c);
    af_free(ab);
    af_free(abc);
    return 0;
}
