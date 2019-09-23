/**
 * This module implements a pipeline that parses a DIMACS-formatted CNF file and
 * creates an easy-to-use interface to access the clauses and literals.
 */

/* A linked-list of literals */
typedef struct literal_t {
    int    value;
    struct literal_t *next;
} literal_t;

/* A linked-list of clauses, each containing a linked-list of literals */
typedef struct clause_t {
    literal_t *literals;
    struct clause_t  *next;
} clause_t;

/* This struct represents a DIMACS-formatted CNF file */
typedef struct cnf_t {
    int number_of_vars;
    int number_of_clauses;
    struct clause_t *clauses;
} cnf_t;

/**
 * This function parses a DIMACS-formatted CNF file and populates the CNF
 * struct with clauses to provide a user-friendly interface.
 *
 * @filename: the name of the CNF file
 * @cnf: the CNF struct to be populated
 *
 * Return: SUCCESS or FAILURE
 */
int parse_cnf_file(char* filename, cnf_t* cnf);

/**
 * This function prints out members of the CNF struct.
 *
 * @cnf: the CNF struct to be printed
 *
 * Return: Nothing
 */
void print_cnf(cnf_t*);
