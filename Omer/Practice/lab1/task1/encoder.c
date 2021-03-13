#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

   
    FILE * input=stdin;
    FILE * output=stdout;
    char before,after;
    int debug=0,letters=0,sign=0,key=0;
   
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0){ //debug mode
            debug=1;
            fprintf(stderr,"-D\n");
        }
        else if(((argv[i])[0]=='-' ||(argv[i])[0]=='+') && (argv[i])[1]=='e'&& (argv[i])[3]==0 &&
         (((argv[i])[2]>='0' && (argv[i])[2]<='9') ||((argv[i])[2]>='A' && (argv[i])[2]<='F'))){ //encryption mode
            sign=((argv[i])[0]=='-') ? -1 : 1;
            key= ((argv[i])[2]>='0' && (argv[i])[2]<='9') ? (argv[i])[2]-48 : (argv[i])[2]-55; 
        }   
        else if((argv[i])[0]=='-' && (argv[i])[1]=='i'){ //read from file
            input=fopen(argv[i]+2,"r");
            if(input==NULL){ 
                fprintf(stderr,"File not found\n");
                return 1;
            }           
        } 
        else if((argv[i])[0]=='-' && (argv[i])[1]=='o'){ //write to file
            output=fopen(argv[i]+2,"w");
        }
        else{//invalid parameter - throw err
            fprintf(stderr,"invalid parameter - %s\n",argv[i]);
	        return 1;
        }
    }  
    do //encoder main work
    {
        before=fgetc(input);
        after=before;
        if((before==EOF ||before==10) && debug){
            fprintf(stderr,"\nthe number of letters: %i\n\n",letters);
            letters=0;

        }
        if(feof(input))
            break;
        if(before!=10){
           
            if(sign){//encryption mode
                after=before+(sign*key);
                letters++;
            }
            else if(before >= 'A' && before <= 'Z'){//to lowercase
                letters++;
                after=before+32;
            }
            if(debug){//debuging mode
                fprintf(stderr,"%i %i\n", before,after);
            }     
        }
        else if(debug){//debuging mode
          
        }
        fprintf(output,"%c",after);
        if(output!=stdout){//flush the file writing stream
            fflush(output);
        }
        
    } while(1);
  
    //closing streams
    if(input!=stdin)
         fclose(input);
    if(output!=stdout)
         fclose(output);
   
    return 0;

 }
