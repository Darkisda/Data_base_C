/*
	Script reader for PostgreSQL
	User must provide an SQL script as input
*/

#include <stdio.h>
#include <stdlib.h>
//#include "libpq-fe.h"

// Change this line to setup database
#define DBDATA "host=localhost dbname=tads2019 user=postgres password=secreta"

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    system("pause");
    exit(1);
}

void handleResults(PGresult *res, char* cmd) {
	switch (PQresultStatus(res)) {
		case PGRES_COMMAND_OK: printf("Executed: %s\n", cmd);
		break;
		case PGRES_EMPTY_QUERY: printf("Empty response\n"); break;
		case PGRES_TUPLES_OK: printf("Returned a set of tuples\n"); break;
		case PGRES_BAD_RESPONSE: printf("[ERROR] bad response"); break;
		case PGRES_NONFATAL_ERROR: 
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break;
		default:
			printf("Unexpected response");
	}
    PQclear(res);
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

    conninfo = DBDATA;

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);
    
    // check for argument
    if (argc<1) {
    	printf("Invalid arguments...\n");
    	system("pause");
    	exit(1);
	}
    
    char* fileName = argv[1];
    FILE* file = fopen(fileName, "r");
    
    if (file==NULL) {
    	printf("Couldn't open file %s'\n",fileName);
    	system("pause");
    	exit(1);
	}
	
	// now connect to the DB
	conn = PQconnectdb(conninfo);
    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
        PQerrorMessage(conn));
        exit_nicely(conn);
    }

	char line[255];

	// aqui a conexao com o BD está ok
	while(fgets(line, 255, file) != NULL){
		res = PQexec(conn, line);
		handleResults(res, line);
	}
      
    exit_nicely(conn);
}
