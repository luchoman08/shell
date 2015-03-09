/* ultima version funcional: */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#define max 105 /*Numero de caracteres maximo para comando las variables de ambiente*/
char PWD[max];
int main(){
        char command[256];
	char *argument[256];
	char *argv[12];
	int mode = 0;
	int estatus_cd;
	while(1){
		
getcwd(PWD,max);
printf("MINI-SHELL --->>> %s\n", PWD);
printf("Mode (one number only -- 0 fg-- 1 bg)> "); fflush(stdout);
scanf("%d",&mode);
   estatus_cd = adicionarComando(argv,command, argument);
   if(estatus_cd==1)
   {
if(mode == 0){
    ejecutarfg(argv);
sleep(1);
            }
else
{
	ejecutarbg(argv);

}
}
}
}
int adicionarComando(char **argv, char *command, char **argument)
{
   int i;
   int j;
   char *argument2[48];
	 // loop until return
        printf("Command (one word only)> "); fflush(stdout);
        scanf("%s",command);
        *argv++=command;
         if(!strcmp(command,"cd"))
        {
			
			char ruta[30];
			printf("Ingrese la ruta: ");
			scanf("%s",ruta);
			
			chdir(ruta);
			return 0;
			
		}
		else{
            printf("Ingrese el numero de argumentos: "); fflush(stdout);
        scanf("%d",&i);
    
       
        for(j=0; j<i;j++)
        {
	    printf("Command argument (one word only)> "); 
	    scanf("%s" , &argument[j]);
	 	*argv++=&argument[j];
	    
	 
		
        }
	}
	*argv='\0';
return 1;
	
    
}
int ejecutarbg(char **argv)
{
	 pid_t  pid;
     int    status;
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
		  
		 // printf("%s \n",argv[3]);
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
			 
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     return 0;
 }

void ejecutarfg(char **argv)
{
 pid_t  pid;
     int    status;
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
		  
		 // printf("%s \n",argv[3]);
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
			 
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
	
}




