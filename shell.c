#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

#define CHAR_MAX_LENGTH 2000
#define INVITE_SYMBOL "shell>"

int Ndelimiteur(char *string_in){
	int i, l = 0, n = 0, len = strlen(string_in) + 1;
	char delimiter = ' ';
	for(i = 0; i < len; i++){
		if( !( (string_in[i] != '\0') && (string_in[i] != delimiter)) ){
			n++;
		}
	}
	return n;
}


void delimiteur(char *string_in, char *string_out, int nbr)
{

    char c = ' ';

    int i, l = 0, n = 0, len = strlen(string_in) + 1;

    for (i = 0; i < len; i++) {

	if ((string_in[i] != '\0') && (string_in[i] != c)) {

	    string_out[l] = string_in[i];

	    l++;

	} else {

	    if (l > 0) {

		string_out[l] = 0;

		l = 0;

		if (n == nbr)
		    return;

		n++;
	    }

	}

    }

}


void quitt(){
	fprintf(stdout,"========== CLOSING COURSE ==========\n");
	fprintf(stdout,"3\n"); sleep(1);
	fprintf(stdout,"2\n"); sleep(1);
	fprintf(stdout,"1\n"); sleep(1);
}

void clean_buf(){
	int c;
	do{
		c = getchar();
	}while(c != '\n' && c != EOF);
}

void exec_user_command(char *param[]){ // ["ls","-l"];
	execvp(param[0], param);
}

/* CREATE A NEW PROCESS */

int create_proc(){
	
	pid_t pid_son;
	do{
		pid_son = fork();
	}while((pid_son == -1) && (errno == EAGAIN));
	
	return pid_son;
}

int main(int argc, char *argv[]){
	
	char exiting[5] = { 0 };
	int i, n;
	char buff_in[CHAR_MAX_LENGTH] = {0};
	
	
	/* COMMAND ELEMENT */
	char user_string[CHAR_MAX_LENGTH] = {0};
	//char command[50] = {0};
	/* END COMMAND ELEMENT */


	while(1){
		write(STDOUT_FILENO, INVITE_SYMBOL, sizeof(INVITE_SYMBOL));
		read(STDIN_FILENO,buff_in, CHAR_MAX_LENGTH);
		char *pt; if( (pt = strchr(buff_in, '\n')) != NULL ){*pt = '\0';}

		// QUIT THE PROGRAMM IF USER's STRING IS quit OR exit
		strncpy(exiting, buff_in, 4); //copy 4th first char
		if( (strcmp(exiting, "exit") == 0) || (strcmp(exiting, "quit") == 0) ){
			if(atexit(quitt) == -1){
				perror("Error while closing programm");
			}
			break;
		}

		/* GET COMMAND */
		n = Ndelimiteur(buff_in);
		//fprintf(stdout, "%d\n",n);
		int el_number = n;
		char *param[el_number+1]; // ["ls","-l"];
		
		for(i = 0; i < n; i++){
			delimiteur(buff_in, user_string, i);
			param[i] = strdup(user_string);
		}

		param[n] = NULL;
		
		pid_t pid_first_son = create_proc();
			
		if(pid_first_son == 0){
			//if(strcmp(param[n-1], "&") == 0){
				/*
				 pid_t pid_new_son = create_proc();
				if(pid_new_son == 0){
					exec_user_command(param);
				}else{
				waitpid(pid_new_son, NULL, 0);
				}
				*/
				//continue;
			//}
			execvp(param[0], param);
			//exec_user_command(param);
			fprintf(stderr, "Error %d\n", errno);
		}else{
			
			/*if(strcmp(param[n-1], "&") != 0){
				waitpid(pid_first_son, NULL, 0);
			}*/
			waitpid(pid_first_son, NULL, 0);
			//wait(NULL);
		}

		//free(param);
	}


	return EXIT_SUCCESS;
}
		

