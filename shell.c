/* ultima version funcional: */
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#define max 105 /*Numero de caracteres maximo para comando las variables de ambiente*/
char PWD[max];

int main(void) {
    int *procesos[20];/* array el cual almacena los id de los procesos en backgraund */
    char command[256];/* almacena el comando como unidad, sin argumento0s */
    char *argument[256]; /* array adicional para guardar argumentos */
    char *argv[12]; /* array que guarda los argumentos que luego seran enviados para que sean ejecutados, en la posicion 0
                        almacenara el comando en cuestion  */
    int mode = 0;  /* indica si el proceso a ejecutar sera ejecutado en backgraund o en foregraund
                       *si es 0 el proceso se ejecutara en foregraund
                       *si es 1 el proceso se ejecutara en  backgraund */
    int estatus; /* indica si se ingreso un comando especial implementado dentro del programa
    
                      *si estatus es 1, indica que el comando ingresado se encuentra en /bin y puede ser ejecutado normalmente
                      *si estatus es 2, indica que el comando ingresado es jobs, el cual lista los pid de los procesos ejecutandose en 
                       backgraund
                      *si el estatus es 3 indica que el proceso ejecutado es "kill", comando el cual necesita que sea almacenado el pid al que
                       va a matar, para posteriormente borrarlo de los registros internos del foregraundrograma (el array procesos)  */
    incializaProcesos(procesos);  /* inicializa el array procesos, pone cada una de las posiciones en 0 por conveniencia */
    while (1) {
        getcwd(PWD, max); /* obtiene la ruta en la que se encuentra */
        printf("MINI-SHELL --->>> %s\n", PWD); /* imprime la ruta en una linea del shell */
        printf("Mode (one number only -- 0 fg-- 1 bg)> "); /* indica que a continuacion se ingresara el modo de ejecucion
																*si es 0 el proceso se ejecutara en foregraund
                                                                *si es 1 el proceso se ejecutara en  backgraund */
        fflush(stdout);
        scanf("%d", &mode); /* se obtiene el mode */
        estatus = adicionarComando(argv, command, argument);  /* se obtiene el status el cual puede ser 
																   *si estatus es 1, indica que el comando ingresado se encuentra en /bin y puede ser ejecutado normalmente
                                                                   *si estatus es 2, indica que el comando ingresado es jobs, el cual lista los pid de los procesos ejecutandose en 
																	backgraund
																   *si el estatus es 3 indica que el proceso ejecutado es "kill", comando el cual necesita que sea almacenado el pid al que
																    va a matar, para posteriormente borrarlo de los registros internos del foregraundrograma (el array procesos) 
																				*/
        if (estatus == 1) {/* si el comando no es un comando especial abordado en el shell (como cd o jobs)*/
            if (mode == 0) { /* si el mode es 0 el comando se ejecuta en foreground */
                ejecutarfg(argv); /**/
                sleep(1); /* linea para esperar la ejecucion del comando y luego mostrar la linea inicial del programa (no funciona igualmente
							 programas que tarden mas de 1 segundo en ejecutarse)*/
            } else { /* si mode es distinto de  0 el proceso es ejecutado en background*/
                ejecutarbg(argv, procesos); /* se envia el array con los argumentos y el puntero del array que contiene los id de los procesos */
            }
        } else if (estatus == 2) {  
            imprimeProcesos(procesos);/* si el estatus es 2 se imprimen los id de los procesos corriendo actualmente en background */
        }
        else if (estatus == 3) {
			
			/* si la entrada es el proceso kill, se ejecuta este luego se procede a eliminar el id del proceso terminado del array de procesos*/
            ejecutarfg(argv);
            sleep(1);
            int aux = atoi(argv[1]);
            clearprocess(aux, procesos);





        }
    }
}
/* este metodo se encarga de llenar el array de argumentos (argv) para su posterior ejecucion */
int adicionarComando(char **argv, char *command, char **argument) {
    int i; /* variables utilizadas en los for que se encuentran mas abajo */
    int j;
    char *argument2[48];  
    // loop until return
    printf("Command (one word only)> "); /* se procede a pedir el argumento */
    fflush(stdout);
    scanf("%s", command); /* se asigna la linea de entrada a la variable char command */
    *argv++ = command;  /* el array argv en la posicion en la que se encuentre apunta a command y se incrementa en uno */
    if (!strcmp(command, "jobs")) {  /* si el comando de entrada es jobs retorna 2 para su ejecucion a parte */
        return 2;
    }



    if (!strcmp(command, "cd")) {
        char ruta[30];
        printf("Ingrese la ruta: ");
        scanf("%s", ruta);
        chdir(ruta);
        return 0;
    } else {
        printf("Ingrese el numero de argumentos: ");
        fflush(stdout);
        scanf("%d", &i);
        for (j = 0; j < i; j++) {
            printf("Command argument (one word only)> ");
            scanf("%s", &argument[j]);
            *argv++ = &argument[j];
        }

    }
    *argv = '\0';

    if (!strcmp(command, "kill")) {
        return 3;
    }

    return 1;
}

int ejecutarbg(char **argv, int **procesos) {
    pid_t pid = fork();
    int status;
    int i = 0;
    for (i = 0; i < 20; i++) {
        if (procesos[i] == 0) {
            procesos[i] = pid;
            i = 20;
        }
    }
    if (pid < 0) { /* fork a child process */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) { /* for the child process: */
        // printf("%s \n",argv[3]);
        if (execvp(*argv, argv) < 0) { /* execute the command */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    return 0;
}

void clearprocess(int process, int **procesos) {


    int i = 0;
    for (i = 0; i < 20; i++) {
        if (procesos[i] == process) {
            procesos[i] = 0;
        }
    }

}

void ejecutarfg(char **argv) {
    pid_t pid;
    int status;
    if ((pid = fork()) < 0) { /* fork a child process */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) { /* for the child process: */
        // printf("%s \n",argv[3]);
        if (execvp(*argv, argv) < 0) { /* execute the command */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    } else { /* for the parent: */
        while (wait(&status) != pid) /* wait for completion */
            ;
    }
}

void incializaProcesos(int **procesos) {
    int i = 0;
    for (; i < 20; i++) {
        procesos[i] = 0;
    }
}

void imprimeProcesos(int **procesos) {
    int i = 0;
    for (i = 0; i < 20; i++) {

        if (procesos[i] != 0) {
            printf("%d \n", procesos[i]);
        }

    }
}



