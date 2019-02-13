#include <stdio.h>
#include <stdlib.h>

/* Structures de données pour représenter un automate. */

/** Structure d'un automate fini non-déterministe. @TODO */
struct afn {
};

/** Structure d'un automate fini déterministe. @TODO */
struct afd {
};

/* Fonctions pour reconnaître les automates de base de la méthode. */

/**
 * Retourne un automate standard reconnaissant le langage vide.
 * @TODO
 */
struct afn af_std_gen_empty_lang()
{
    return;
}

/**
 * Retourne un automate standard reconnaissant le langage composé du seul mot vide.
 * @TODO
 */
struct afn af_std_gen_empty_word()
{
    return;
}

/**
 * Retourne un automate standard reconnaissant le langage composé d'un mot d'un
 * caractère passé en paramètre.
 * @TODO
 */
struct afn af_std_gen_word(const char c)
{
    return;
}

/* Fonctions pour reconnaître des langages plus évolués. */

/**
 * Retourne un automate standard qui reconnait la réunion des deux langages
 * reconnus par deux automates.
 * @TODO
 */
struct afn af_std_union(struct afn a1, struct afn a2)
{
    return;
}

/**
 * Retourne un automate standard qui reconnait la concaténation des deux langages
 * reconnus par deux automates.
 * @TODO
 */
struct afn af_std_cat(struct afn a1, struct afn a2)
{
    return;
}

/**
 * Retourne un automate standard qui reconnait la fermeture itérative de Kleene
 * du langage reconnu par un automate.
 * @TODO
 */
struct afn af_std_star(struct afn a)
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
int afd_exec(const struct afd a, const char * m)
{
    return 0;
}

/**
 * Déterminise un automate fini standard non-déterministe en un automate fini
 * déterministe standard.
 * @TODO
 */
struct afd afd_from_af_std(const struct afd a)
{
    return;
}

/**
 * Minimise un automate fini standard déterministe.
 * @TODO
 */
struct afd afd_min_from_afd(const struct afd a)
{
    return;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    printf("Hello World!\n");
    return 0;
}
