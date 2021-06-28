#include <stdio.h>
#include <stdlib.h>

typedef struct a{

    int a;
    struct a *next;

} elm;


int read_first_element(elm * a){
    return a->a;

}

elm* delete(elm *a){
    if(a->next == NULL){
        a->a = -1;
        return a;
    }
    elm *to_delete = a;
    a = a->next;
    //free(to_delete);

    return a;
}

void insert(elm *a, int value){
    
    if(a->a == -1){
        a->a = value;
        return;
    }

    while(a->next != NULL){
        a = a->next;
    }

    elm* new = (elm*) malloc(sizeof(elm));
    new->a = value;
    new->next = NULL;
    a->next = new;
}

int count_queue(elm *a){

    if(a->next == NULL){
        if(a->a == -1) return 0;
        else return 1;
    }
    else{
        int x = 1;

        while(a->next != NULL){
            a = a->next;
            x ++;
        }

        return x;
    }

}

void printf_queue(elm * a){

    if(a->next == NULL){
        if(a->a != -1)
            fprintf(stdout, "%d", a->a);
    }
    else{
        fprintf(stdout,"%d ",a->a);

        while(a->next != NULL){
            a = a->next;
            fprintf(stdout,"%d ",a->a);
        }

    }


}