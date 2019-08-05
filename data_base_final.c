#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

const char *conninfo;
	PGconn	*conn;
	PGresult	*res;
	int		nFields;
	int		i, j;
	

char comando[255];

	
	int id_pergunta;
	
	char dominio_pergunta[100];
	
	char tema_pergunta[100];
	
	int dificuldade_pergunta;
	
	char texto_pergunta[255];
	
	char resposta_pergunta[255];


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

void cadastro_questao(){
	
	system("cls");
	
	char query[255];
		
	printf("Digite o ID da pergunta: ");
		scanf("%d", &id_pergunta);
			
	printf("Digite o Dominio da pergunta: ");
		setbuf(stdin, NULL);
			gets(dominio_pergunta);
			
	printf("Digite o Tema da pergunta: ");
		setbuf(stdin, NULL);
			gets(tema_pergunta);
	
	printf("Digite a Dificuldade 1,2,3: ");
		scanf("%d", &dificuldade_pergunta);
		
	printf("Digite o Texto da pergunta: ");
		setbuf(stdin, NULL);
			gets(texto_pergunta);
	
	printf("Digite a Respota da pergunta: ");
		setbuf(stdin, NULL);
			gets(resposta_pergunta);
			
	sprintf(comando, "INSERT INTO questoes(id, dominio, tema, dificuldade, texto, resposta) VALUES ( %d,'{%s}', '{%s}', %d, '{%s}', '{%s}');",id_pergunta, dominio_pergunta, tema_pergunta, dificuldade_pergunta, texto_pergunta, resposta_pergunta);

	res = PQexec(conn, comando);
	
	switch (PQresultStatus(res)) {
		
		case PGRES_EMPTY_QUERY: printf("Query Vazia.\n");
		
		case PGRES_COMMAND_OK: printf("Feito.\n"); break;
		
		case PGRES_TUPLES_OK: print_tuplas(res); break;
		
		case PGRES_BAD_RESPONSE: printf("[ERRO] Resposta Ruim"); break; 
		
		case PGRES_NONFATAL_ERROR: 
		
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break; 
		
		default:
			
			printf("Erro inesperado");
	}
}

void buscar_questao(){
	
	system("cls");
	
	char txtQuery[255];
	
	int opcao;
	
	printf("Buscar por...\n");
	
	do{
		printf("{1} - Tema\n{2} - Dominio\n{3} - Dificuldade\n{4} - Texto da Questao\n{5} - Sair\n");
			scanf("%d", &opcao);
			
			switch(opcao){
				
				case 1:
					
					system("cls");
					
					printf(">> ");
						scanf("%s", txtQuery);
						
					sprintf(comando, "SELECT * FROM public.questoes WHERE tema = '%s';\n", txtQuery );
					
					break;
					
				case 2:
					
					system("cls");
					
					printf(">> ");
						scanf("%s", txtQuery);
						
					sprintf(comando, "SELECT * FROM public.questoes WHERE dominio = '%s';\n", txtQuery );
					
					break;
				
				case 3:
					
					system("cls");
					
					printf(">> ");
						scanf("%s", txtQuery);
						
					sprintf(comando, "SELECT * FROM public.questoes WHERE dificuldade = '%s';\n", txtQuery );
					
					break;
					
				case 4:
					
					system("cls");
					
					printf(">> ");
						scanf("%s", txtQuery);
					
					sprintf(comando, "SELECT * FROM public.questoes WHERE texto = '%s';\n", txtQuery );
					
					break;
				
				case 5:
					
					break;
			}
	}while(opcao != 5);
}

void main_menu(){
	
	printf("BANCO DE DADOS\n");
	
	int opcao = 0;
	
	do{
		
		printf("{1} - Cadastrar questoes\n{2} - Buscar questao\n{3} - Sair\n");
			scanf("%d", &opcao);
		
		switch(opcao){
			
			case 1:
								
				cadastro_questao();
				
				break;
				
			case 2:
				
				buscar_questao();
				
				break;
			
			case 3:
			
				break;	
		}
		
	}while(opcao != 3);
}

int main( ){

	conninfo = "host=localhost dbname=postgres user=postgres password=2410";
	
	conn = PQconnectdb(conninfo);
	
	(PQstatus(conn) != CONNECTION_OK) ? printf("Failure\n") : printf("Connected!\n");
	
	main_menu();
		
	res = PQexec(conn, comando);
	
	switch (PQresultStatus(res)) {
		
		case PGRES_EMPTY_QUERY: printf("Query Vazia.\n");
		
		case PGRES_COMMAND_OK: printf("Feito.\n"); break;
		
		case PGRES_TUPLES_OK: print_tuplas(res); break;
		
		case PGRES_BAD_RESPONSE: printf("[ERRO] Resposta Ruim"); break; 
		
		case PGRES_NONFATAL_ERROR: 
		
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break; 
		
		default:
			
			printf("Erro inesperado");
	}
    
	PQclear(res);
    
   	PQfinish(conn);
	
	exit(1);
		
	return 0;
}
