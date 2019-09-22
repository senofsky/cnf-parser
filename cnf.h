/**
 * This module implements a pipeline that parses a DIMACS-formatted CNF file and
 * creates an easy-to-use interface to access the clauses and literals.
 */

/* A linked-list of literals */
struct literal {
    int    value;
    struct literal *next;
};

/* A linked-list of clauses, each containing a linked-list of literals */
struct clause {
    struct literal *literals;
    struct clause  *next;
};

/* This struct represents a DIMACS-formatted CNF file */
struct cnf {
    int number_of_vars;
    int number_of_clauses;
    struct clause *clauses;
};

/**
 * This function parses a DIMACS-formatted CNF file and populates the CNF
 * struct with clauses to provide a user-friendly interface.
 *
 * @filename: the name of the CNF file
 * @cnf: the CNF struct to be populated
 *
 * Return: SUCCESS or FAILURE
 */
int parse_cnf_file(char* filename, struct cnf* cnf);

/**
 * This function prints out members of the CNF struct.
 *
 * @cnf: the CNF struct to be printed
 *
 * Return: Nothing
 */
void print_cnf(struct cnf*);
