#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void custom_handler(int signum){

    printf("signal : %s is received \n",strsignal(signum));

    if(signum==SIGTSTP)
        signal(SIGCONT,custom_handler);
    else if(signum==SIGCONT)
        signal(SIGTSTP,custom_handler);
    raise(signum);
    signal(signum,SIG_DFL);

}

int main(int argc, char **argv){ 

    signal(SIGINT,custom_handler);
    signal(SIGTSTP,custom_handler);

    while(1) {
        sleep(2);
    }

	return 0;
}