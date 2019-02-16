#include <stdio.h>
#include <stdlib.h>

/* Par défaut, tout nos automates travaillent sur l'alphabet ASCII. */

/* Structures de données pour représenter un automate. */

/** Structure d'une fonction de transition pour automate fini déterministe. */
typedef struct transf {
    int st_in;  /*!< Numéro de l'état sur lequelle peut arriver la transition. */
    int st_out; /*!< Numéro de l'état après la transition. */
    char c;     /*!< Caractère de l'alphabet sur lequelle a lieu la transition. */
} transf_s;

/** Structure d'une fonction de transition pour automate fini non-déterministe. */
typedef struct transfn {
    int st_in;      /*!< Numéro de l'état sur lequelle peut arriver la transition. */
    int st_out_nb;  /*!< Nombre d'état possibles après la transition. */
    int * st_out;   /*!< Tableau de numéros d'états possibles après la transition. */
    char c;         /*!< Caractère de l'alphabet sur lequelle a lieu la transition. */
} transfn_s;

/** Structure d'un automate fini non-déterministe. */
typedef struct afn {
    int states;         /*!< Nombre d'état de l'automate. Les états sont numérotés de 0 à (states - 1). */
    int state_init;     /*!< Numéro de l'état initial. */
    int * state_fin;    /*!< Si state_fin[i] est à 1, alors l'état numéro i est un état final. */
    int trans_nb;       /*!< Nombre de transitions. */
    transfn_s * trans;  /*!< Tableau de fonction de transition. */
} afn_s;

/** Structure d'un automate fini déterministe. */
typedef struct afd {
    int states;       /*!< Nombre d'état de l'automate. Les états sont numérotés de 0 à (states - 1). */
    int state_init;   /*!< Numéro de l'état initial. */
    int * state_fin;  /*!< Si state_fin[i] est à 1, alors l'état numéro i est un état final. */
    int trans_nb;     /*!< Nombre de transitions. */
    transf_s * trans; /*!< Tableau de fonction de transition. */
} afd_s;

/* Fonctions pour reconnaître les automates de base de la méthode. */

/**
 * Retourne un automate standard reconnaissant le langage vide.
 */
afn_s af_std_gen_empty_lang()
{
    afn_s af;
    af.states = 1;          /* 1 seul état. */
    af.state_init = 0;      /* L'état initial est l'état 0. */
    af.state_fin = NULL;    /* Aucun état final. */
    af.trans_nb = 0;        /* 0 transition. */
    af.trans = NULL;        /* Ancune transition. */
    return af;
}

/**
 * Retourne un automate standard reconnaissant le langage composé du seul mot vide.
 */
afn_s af_std_gen_empty_word()
{
    afn_s af;
    af.states = 1;          /* 1 seul état. */
    af.state_init = 0;      /* L'état initial est l'état 0. */
    af.state_fin = calloc(af.states, sizeof(*af.state_fin));
    af.state_fin[0] = 1;    /* 1 seul état final : l'état initial. */
    af.trans_nb = 0;        /* 0 transition. */
    af.trans = NULL;        /* Ancune transition. */
    return af;
}

/**
 * Retourne un automate standard reconnaissant le langage composé d'un mot d'un
 * caractère passé en paramètre.
 */
afn_s af_std_gen_word(const char c)
{
    afn_s af;
    af.states = 2;                 /* 2 états. */
    af.state_init = 0;             /* L'état initial est l'état 0. */
    af.state_fin = calloc(af.states, sizeof(*af.state_fin));
    af.state_fin[1] = 1;           /* 1 seul état final : l'état 1. */
    af.trans_nb = 1;               /* 1 seule transition. */
    af.trans = calloc(af.trans_nb, sizeof(*af.trans));
    af.trans[0].st_in = 0;         /* Transition depuis l'état inital. */
    af.trans[0].st_out_nb = 1;     /* Transition vers qu'un seul état. */
    af.trans[0].st_out = calloc(af.trans[0].st_out_nb, sizeof(*af.trans[0].st_out));
    af.trans[0].st_out[0] = 1;     /* Transition possible vers l'état 1. */
    af.trans[0].c = c;             /* Transition sur le caractère donné. */
    return af;
}

/* Fonctions pour reconnaître des langages plus évolués. */

/**
 * Retourne un automate standard qui reconnait la réunion des deux langages
 * reconnus par deux automates.
 * @TODO
 */
afn_s af_std_union(afn_s a1, afn_s a2)
{
    return;
}

/**
 * Retourne un automate standard qui reconnait la concaténation des deux langages
 * reconnus par deux automates.
 * @TODO
 */
afn_s af_std_cat(afn_s a1, afn_s a2)
{
    return;
}

/**
 * Retourne un automate standard qui reconnait la fermeture itérative de Kleene
 * du langage reconnu par un automate.
 * @TODO
 */
afn_s af_std_star(afn_s a)
{
    return;
}

/* Fonctions pour manipuler un automate fini déterministe. */

/**
 * Exécute l'automate fini déterministe sur le mot en entrée.
 * @param afd Automate fini déterministe à exécuter.
 * @param m Mot sur lequelle exécuter l'automate.
 * @return 1 si le mot est reconnu, 0 sinon. 
 * @TODO
 */
int afd_exec(const afd_s a, const char * m)
{
    return 0;
}

/**
 * Déterminise un automate fini standard non-déterministe en un automate fini
 * déterministe standard.
 * @TODO
 */
afd_s afd_from_af_std(const afd_s a)
{
    return;
}

/**
 * Minimise un automate fini standard déterministe.
 * @TODO
 */
afd_s afd_min_from_afd(const afd_s a)
{
    return;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    printf("Hello World!\n");
    return 0;
}
