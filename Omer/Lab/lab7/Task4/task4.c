#include <stdio.h>

int digit_cnt(char *str){
    int cnt=0;
    for(int i=0;str[i]!=0;i++){
        if(str[i]<='9' && str[i]>='0'){
            cnt++;
        }
    }
    return cnt;
}

int main(int argc,char **argv){
    printf("Num of digits: %d\n",digit_cnt(*(argv+1)));
}