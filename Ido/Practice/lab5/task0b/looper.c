#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void signal_handler(int sig){

    printf("signal : %s is received \n",strsignal(sig));

    if(sig==SIGTSTP)
        signal(SIGCONT,signal_handler);
    else if(sig==SIGCONT)
        signal(SIGTSTP,signal_handler);

    signal(sig,SIG_DFL);

}

int main(int argc, char **argv){ 

	printf("Starting the program\n");

    signal(SIGINT,signal_handler);
    signal(SIGTSTP,signal_handler);

    while(1) {
        printf("Sleeping...\n");
        sleep(2);
        raise(SIGTSTP);
		raise(SIGCONT);
		raise(SIGTSTP);
    }


	return 0;
}