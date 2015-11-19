#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

int main(int argc, char *argv[]){
	
	if ( argc != 7 ){
		printf("Invalid no. of arguments\n");
		printf("Usage: %s <server ip> <server port> <user> <passwd> <database> \"<query>\"\n",argv[0]);
		exit(1);
	}
 
	const char *server_ip = argv[1];
	unsigned int server_port = atoi(argv[2]);
	const char *user = argv[3];
	const char *passwd = argv[4];
	const char *db = argv[5];
	const char *query = argv[6];
	int status = 0;  //Status of mysql_next_result
	char *server_ip_res;
	
	MYSQL *conn = mysql_init(NULL);
	
	if ( conn == NULL ){
		printf("Init failed: %s",mysql_error(conn));
		exit(1);
	}
	if ( mysql_real_connect( conn, server_ip, user, passwd, db, server_port, NULL, 0 ) == NULL ){
		printf( "\nConnect to %s:%u failed - Error: %s",server_ip,server_port,mysql_error(conn));
	}else{
		printf("\nConnect to %s:%u succeeded",server_ip,server_port);
		if( mysql_query(conn,query) != 0 ){
			printf( "\nQuery execution failed: %s",mysql_error(conn));
			exit(1);
		}else{
			printf("\nQuery %s execution successful",query);
			
			do{
			   MYSQL_RES *result = mysql_store_result(conn);
	
			   if(result != NULL){
				MYSQL_ROW row = mysql_fetch_row(result);
				printf("\nQuery response: \n%s\n",row[0]);
				mysql_free_result(result);
			   }else{
				printf("\nQuery response is empty - %s",mysql_error(conn));
				exit(1);
		           }
			   status = mysql_next_result(conn);
			   
			   if(status > 0){
				printf("Error occurred while fetching result set");
				exit(1);
			   }
			}while(status == 0);
			
			/*Change_user with NULL password*/
			if (mysql_change_user(conn, "root", NULL, "pms")){
				fprintf(stderr, "Failed to change user.  Error: %s\n", mysql_error(conn));
			}

			if( mysql_query(conn,query) != 0 ){
				printf( "\nQuery execution failed: %s",mysql_error(conn));
				exit(1);
			}else{
				printf("\nQuery %s execution successful",query);
			}
		
		}		
	}
	mysql_close(conn);
	exit(0);
}
