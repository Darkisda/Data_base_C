/*
	Command line to PostgreSQL database
 */
#include <stdio.h>
#include <stdlib.h>
#include "libpq-fe.h"


/* IMPORTANT change this line to setup connection */
#define DBDATA "host=localhost dbname=Questoes_Prova user=postgres password=2410";


static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

void print_tuplas(PGresult* res) {
	int nFields = PQnfields(res);
	int i, j;
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(res, i));
    printf("\n");

    /* next, print out the rows */
    for (i = 0; i < PQntuples(res); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(res, i, j));
        printf("\n");
    }
}

int
main(int argc, char **argv)
{
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;
    int         nFields;
    int         i,
                j;

    /*
     * If the user supplies a parameter on the command line, use it as the
     * conninfo string; otherwise default to setting dbname=postgres and using
     * environment variables or defaults for all other connection parameters.
     */
    if (argc > 1)
        conninfo = argv[1];
    else
        conninfo = DBDATA;
		
    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        exit_nicely(conn);
    }
   
	char command[500];

	/* endless loop */
	while(1) {
		/* reads an SQL command */
		printf("Enter command: ");
		fgets(command, 500, stdin);

		/* Type 'exit' to break the endless loop */
		if (strcmp(command,"exit\n")==0) break;

		/* execute the command */
		res = PQexec(conn, command);

		/* handle the response */
		switch (PQresultStatus(res)) {
			case PGRES_EMPTY_QUERY: 
			case PGRES_COMMAND_OK: printf("Done.\n"); break;
			case PGRES_TUPLES_OK: print_tuplas(res); break;
			case PGRES_BAD_RESPONSE: printf("[ERROR] Bad Response"); break;
			case PGRES_NONFATAL_ERROR: 
			case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
			default:
				printf("Unexpected Response");
		}
		/* clear the response pointer */
	    PQclear(res);
	}
    /* close the connection to the database and cleanup */
    exit_nicely(conn);
}

