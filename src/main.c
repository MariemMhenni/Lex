#include <stdio.h>
#include <stdlib.h>

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
    int * st_out;   /*!< Tableau de numéros d'états possibles après la transition. */
    char c;         /*!< Caractère de l'alphabet sur lequelle a lieu la transition. */
} transfn_s;

/** Structure d'un automate fini non-déterministe. @TODO */
typedef struct afn {
} afn_s;

/** Structure d'un automate fini déterministe. @TODO */
typedef struct afd {
    int states;       /*!< Nombre d'état de l'automate. Les états sont numérotés de 0 à (states - 1). */
    int state_init;   /*!< Numéro de l'état initial. */
    int * state_fin;  /*!< Si state_fin[i] est à 1, alors l'état numéro i est un état final. */
    char * alph;      /*!< Alphabet sur lequel l'automate opère. */
    transf_s * trans; /*!< Tableau de fonction de transition. */
} afd_s;

/* Fonctions pour reconnaître les automates de base de la méthode. */

/**
 * Retourne un automate standard reconnaissant le langage vide.
 * @TODO
 */
afn_s af_std_gen_empty_lang()
{
    return;
}

/**
 * Retourne un automate standard reconnaissant le langage composé du seul mot vide.
 * @TODO
 */
afn_s af_std_gen_empty_word()
{
    return;
}

/**
 * Retourne un automate standard reconnaissant le langage composé d'un mot d'un
 * caractère passé en paramètre.
 * @TODO
 */
afn_s af_std_gen_word(const char c)
{
    return;
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
