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

/* Fonctions pour reconnaître les automates de base de la méthode. */

/**
 * Retourne un automate standard reconnaissant le langage vide.
 */
afn af_std_gen_empty_lang()
{
    afn af = {0};
    af.st_nb = 1;                                   /* 1 seul état. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac)); /* Aucun état accepteur. */
    af.tr = calloc(af.st_nb, sizeof(*af.tr));       /* Ancune transition. */
    return af;
}

/**
 * Retourne un automate standard reconnaissant le langage composé du seul mot vide.
 */
afn af_std_gen_empty_word()
{
    afn af = {0};
    af.st_nb = 1;                             /* 1 seul état. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac));
    af.st_ac[ST_INIT] = 1;                    /* 1 seul état accepteur : l'état initial. */
    af.tr = calloc(af.st_nb, sizeof(*af.tr)); /* Ancune transition. */
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
    af.tr = calloc(af.st_nb, sizeof(*af.tr));
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
 * @TODO
 */
afn af_std_union(afn a1, afn a2)
{
    afn af = {0};
    /* Nombre d'état. */
    af.st_nb = a1.st_nb + a2.st_nb - 1;
    
    /* États finaux. */
    af.st_ac = calloc(af.st_nb, sizeof(*af.st_ac));
    /* Indique si le nouvel état initial doit être accepteur. */
    af.st_ac[ST_INIT] = a1.st_ac[ST_INIT] || a2.st_ac[ST_INIT];
    /* Copie les listes d'états finaux successivement hormis les anciens états initiaux. */
    memcpy(&af.st_ac[ST_INIT + 1], &a1.st_ac[ST_INIT + 1], (a1.st_nb - 1) * sizeof(*a1.st_ac));
    memcpy(&af.st_ac[a1.st_nb],    &a2.st_ac[ST_INIT + 1], (a2.st_nb - 1) * sizeof(*a2.st_ac));

    /* Transitions. */
    af.tr = calloc(af.st_nb, sizeof(*af.tr));
    /* Copie les listes de transitions successivement hormis les anciens états initiaux. */
    memcpy(&af.tr[ST_INIT + 1], &a1.tr[ST_INIT + 1], (a1.st_nb - 1) * sizeof(*a1.tr));
    memcpy(&af.tr[a1.st_nb],    &a2.tr[ST_INIT + 1], (a2.st_nb - 1) * sizeof(*a2.tr));
    /* Concatène les transitions des états initiaux des deux automates dans un seul état inital. */
    af.tr[ST_INIT].nb = a1.tr[ST_INIT].nb + a2.tr[ST_INIT].nb;
    af.tr[ST_INIT].st = calloc(af.tr[ST_INIT].nb, sizeof(*af.tr[ST_INIT].st));
    af.tr[ST_INIT].c  = calloc(af.tr[ST_INIT].nb, sizeof(*af.tr[ST_INIT].c));
    memcpy(af.tr[ST_INIT].st,                       a1.tr[ST_INIT].st, a1.tr[ST_INIT].nb * sizeof(*a1.tr[ST_INIT].st));
    memcpy(af.tr[ST_INIT].c,                        a1.tr[ST_INIT].c,  a1.tr[ST_INIT].nb * sizeof(*a1.tr[ST_INIT].c));
    memcpy(&af.tr[ST_INIT].st[a1.tr[ST_INIT].nb],   a2.tr[ST_INIT].st, a2.tr[ST_INIT].nb * sizeof(*a2.tr[ST_INIT].st));
    memcpy(&af.tr[ST_INIT].c[a1.tr[ST_INIT].nb],    a2.tr[ST_INIT].c,  a2.tr[ST_INIT].nb * sizeof(*a2.tr[ST_INIT].c));
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
    return 0;
}
