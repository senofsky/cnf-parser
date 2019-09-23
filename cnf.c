#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cnf.h"

#define SUCCESS 0
#define FAILURE !SUCCESS

#define FALSE 0
#define TRUE !FALSE

/**
 * This function prints out each literal in a list of literals.
 *
 * @literals: the list of literals
 *
 * Return: Nothing
 */
static void print_literals(literal_t *literals) {

    literal_t *literal_ptr = NULL;

    literal_ptr = literals;

    while (literal_ptr != NULL) {

        printf("%d ", literal_ptr->value);
        literal_ptr = literal_ptr->next;
    }
}
/**
 * This function prints out each clause in a list of clauses and its
 * associated literals.
 *
 * @clauses: the list of clauses
 *
 * Return: Nothing
 */
static void print_clauses(clause_t *clauses) {

    clause_t *clause_ptr = NULL;

    clause_ptr = clauses;

    while (clause_ptr != NULL) {

        print_literals(clause_ptr->literals);
        printf("\n");
        clause_ptr = clause_ptr->next;
    }
}

/**
 * See cnf.h
 */
void print_cnf(cnf_t *cnf) {

    printf("Number of variables = %d\n", cnf->number_of_vars);
    printf("Number of clauses   = %d\n", cnf->number_of_clauses);

    print_clauses(cnf->clauses);
}

/**
 * This function adds the literal to the clause.
 *
 * @literal: the literal to be added
 * @clause: the CNF clause
 *
 * Return: SUCCESS or FAILURE
 */
int add_literal_to_clause(int literal, clause_t *clause) {

    literal_t *literal_ptr;

    if (clause->literals == NULL) {

        clause->literals = calloc(1, sizeof(literal_t));
        if (clause->literals == NULL) {
            printf("[ERROR] %s\n", strerror(errno));
            return FAILURE;
        }

        clause->literals->next = NULL;
        clause->literals->value = literal;

        return SUCCESS;
    }

    literal_ptr = clause->literals;

    while (literal_ptr->next != NULL)
        literal_ptr = literal_ptr->next;

    literal_ptr->next = calloc(1, sizeof(literal_t));
    if (literal_ptr->next == NULL) {
        printf("[ERROR] %s\n", strerror(errno));
        return FAILURE;
    }

    literal_ptr = literal_ptr->next;

    literal_ptr->next = NULL;
    literal_ptr->value = literal;

    return SUCCESS;
}

/**
 * This function determines if a line is a comment in accordance with the
 * DIMACS CNF format.
 *
 * @line: a line in the CNF file
 *
 * Return: TRUE or FALSE
 */
int is_comment(char *line) {

    if (line[0] == 'c')
        return TRUE;

    return FALSE;
}

/**
 * This function determines if a line is in the correct format in accordance
 * with the DIMACS CNF format.
 *
 * @line: a line in the CNF file
 *
 * Return: TRUE or FALSE
 */
int is_correct_format(char *line) {

    char *ptr;
    int index = 0;

    ptr = line;

    while (ptr[index] != '\0') {

        if (isdigit(ptr[index]) == FALSE &&
            isspace(ptr[index]) == FALSE &&
            ptr[index] != '-') {

            printf("[ERROR] invalid character found: %c\n", ptr[index]); 

            return FALSE;
        }

        index++;
    }

    return TRUE;
}
/**
 * This function parses a line and extracts the literals into the clause.
 *
 * @line: the line containing literals
 * @clause: the clause that will store the literals
 *
 * Return: SUCCESS or FAILURE
 */
static int set_clause_with_literals(char *line, clause_t *clause) {

        int rc = 0;
        int literal = 1;
        int index = 0;
        int offset = 0;
        char *line_ptr;

        line_ptr = line;

        while (TRUE) {

            /* sscanf will only get the first literal from the string, so
             * the pointer is incremented to get the following literals
             */
            rc = sscanf(line_ptr, " %i %n", &literal, &offset);
            if (rc == EOF) {
                printf("[ERROR] unexpected EOF while reading literals\n");
                return FAILURE;
            }

            if (literal == 0)
                return SUCCESS;

            rc = add_literal_to_clause(literal, clause);
            if (rc == FAILURE)
                return FAILURE;

            index++;
            line_ptr += offset;
        } 
}

/**
 * This function reads the CNF file and retrieves the first completed clause
 * it finds.
 *
 * @cnf_file: the open CNF file
 * @clause: the clause to be populated with literals
 *
 * Return: SUCCESS or FAILURE
 */
static int get_clause_from_cnf(FILE *cnf_file, clause_t *clause) {

    int rc = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t chars_read;

    while (TRUE) {

        chars_read = getline(&line, &len, cnf_file);
        if (chars_read < 0) {
            free(line);
            if (errno != 0) {
                printf("[ERROR] %s\n", strerror(errno));
                return FAILURE;
            }
            else
                return SUCCESS;
        }

        if (is_comment(line)) {
            free(line);
            continue;
        }

        if (is_correct_format(line) == FALSE) {
            printf("[ERROR] invalid format: %s\n", line);
            free(line);
            return FAILURE;
        }

        rc = set_clause_with_literals(line, clause);
        free(line);
        if (rc == FAILURE)
            return FAILURE;
        else
            return SUCCESS;
    } 
}

/**
 * This function adds the clause to the CNF struct.
 *
 * @clause: the clause to be added
 * @cnf: the CNF struct
 *
 * Return: Nothing
 */
void add_clause_to_cnf(clause_t *clause, cnf_t *cnf) {

    clause_t *ptr;

    if (cnf->clauses == NULL) {
        cnf->clauses = clause;
        return;
    }

    ptr = cnf->clauses;

    while (ptr->next != NULL)
        ptr = ptr->next; 

    ptr->next = clause;
}

/**
 * See cnf.h
 */
int parse_cnf_file(char *filename, cnf_t *cnf) {

    int rc = 0;
    int index = 0;
    FILE *cnf_file = NULL;
    clause_t *clause;

    cnf_file = fopen(filename, "r");
    if (cnf_file == NULL) {
        printf("[ERROR] %s\n", strerror(errno));
        return FAILURE;
    }

    /* TODO: This loop is ugly and probably has errors. Need to fix. */
    do {
        rc = fscanf(
            cnf_file,
            " p cnf %i %i ",
            &(cnf->number_of_vars),
            &(cnf->number_of_clauses));
        if (rc > 0 && rc != EOF)
            break;

        rc = fscanf(cnf_file, "%*s "); /* skip comments */

    } while (rc != 2 && rc != EOF);

    for (index = 0;
         index < cnf->number_of_clauses;
         index++) {

        clause = calloc(1, sizeof(clause_t));
        if (clause == NULL) {
            printf("[ERROR] %s\n", strerror(errno));
            return FAILURE;
        }

        rc = get_clause_from_cnf(cnf_file, clause);
        if (rc == FAILURE) {
            //cleanup_cnf(cnf);
            return FAILURE;
        }

        add_clause_to_cnf(clause, cnf);
    }

    fclose(cnf_file);

    return SUCCESS;
}
