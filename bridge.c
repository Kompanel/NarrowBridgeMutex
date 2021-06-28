#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <unistd.h>

#define CITY_A 10
#define CITY_B 11

int number_of_cars;
int curr_on_bridge = -1;
int debug;
int direction;
elm queue_to_city_A;
elm* q_city_a = &queue_to_city_A;
elm queue_to_city_B;
elm* q_city_b = &queue_to_city_B;

void set_debugg(int x){
    debug = x;
}

int *cars_in_city_A;
int *cars_in_city_B;

pthread_mutex_t bridge = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue = PTHREAD_MUTEX_INITIALIZER;

pthread_t *cars;

int change_direction(){

    if(count_queue(q_city_a)> count_queue(q_city_b)) return CITY_B;
    else return CITY_A;

}

int count_city(int *tab){
    int x = 0;

    for(int i = 0; i < number_of_cars; i++){

        if(tab[i] != 0) x++;

    }

    return x;
}

void print_line(){
    if(pthread_mutex_lock(&monitor) != 0){
        fprintf(stderr, "monitor mutex lock error");
        exit(-1);
    }    

    int count_city_A = count_city(cars_in_city_A), 
        count_city_B = count_city(cars_in_city_B),
        count_queue_A = count_queue(q_city_a),
        count_queue_B = count_queue(q_city_b);

    if(direction == CITY_B){
        
        if(debug == 1){
            for(int i=0; i<number_of_cars; i++){
                if(cars_in_city_A[i] == 1) printf("%d ", i);
            }
        }

        fprintf(stdout, "A-%d %d", count_city_A, count_queue_A);
        if(debug == 1){
            fprintf(stdout, "(q: ");
            printf_queue(q_city_a);
            fprintf(stdout,")");
        }
        if(curr_on_bridge != -1)
            fprintf(stdout, ">>> [>> %d >>] <<<%d ", curr_on_bridge, count_queue_B);
        else
            fprintf(stdout, ">>> [>>   <<] <<<%d ", count_queue_B);
        if(debug == 1){
            fprintf(stdout, "(q: ");
            printf_queue(q_city_b);
            fprintf(stdout,")");
        }
        fprintf(stdout, "%d-B ", count_city_B);
        
        if(debug == 1){
            for(int i=0; i<number_of_cars; i++){
                if(cars_in_city_B[i] == 1) printf("%d ", i);
            }
        }


    }
    else{

        if(debug == 1){
            for(int i=0; i<number_of_cars; i++){
                if(cars_in_city_A[i] == 1) printf("%d ", i);
            }
        }

        fprintf(stdout, "A-%d %d", count_city_A, count_queue_A);
        if(debug == 1){
            fprintf(stdout, "(q: ");
            printf_queue(q_city_a);
            fprintf(stdout,")");
        }
        if(curr_on_bridge != -1)
            fprintf(stdout, ">>> [<< %d <<] <<<%d ", curr_on_bridge, count_queue_B);
        else
            fprintf(stdout, ">>> [>>   <<] <<<%d ", count_queue_B);
        if(debug == 1){
            fprintf(stdout, "(q: ");
            printf_queue(q_city_b);
            fprintf(stdout,")");
        }
        fprintf(stdout, "%d-B", count_city_B);
        
        if(debug == 1){
            fprintf(stdout, " ");
            for(int i=0; i<number_of_cars; i++){
                if(cars_in_city_B[i] == 1) printf("%d ", i);
            }
        }
    }

    printf("\n");
    pthread_mutex_unlock(&monitor);

}



void add_to_queue(int city, int car_number){

    sleep(1);
    switch(city){

        case CITY_A:{
            
            if(cars_in_city_A[car_number] == 0)
                return;
        
            cars_in_city_A[car_number]--;
            insert(q_city_a, car_number);

            break;
        }

        case CITY_B:{

            if(cars_in_city_B[car_number] == 0)
                return;
            
            cars_in_city_B[car_number]--;
            insert(q_city_b, car_number);

            break;
        }
    }

    print_line();

}

int choose_random_city(){

    if(rand() % 2 == 0)
        return CITY_A;
    else
        return CITY_B;

}

int random_direction(){
    if(rand() %2 ==0) {
        return CITY_A;
    }
    else
        return CITY_B;
}

void *car_thread(void* car_number){
    int city = choose_random_city();
    int car_no = *(int*) car_number;
    int is_in_queue = 0;
    
    switch (city)
    {
    case CITY_A:
        cars_in_city_A[car_no]++;
        break;
    
    case CITY_B:
        cars_in_city_B[car_no]++;
        break;
    }
    while(1){
        sleep(rand() % 5 + 1);

        
        // mutex blokuje kolejke aby nie zaburzyć poprawności kolejności samochodów
        if(pthread_mutex_lock(&queue) != 0){
            fprintf(stderr, "queue mutex lock error");
            exit(-1);
        }
        if(is_in_queue == 0 && curr_on_bridge != car_no){ 
            sleep(1);
            add_to_queue(city, car_no);
            is_in_queue = 1;
        }
        pthread_mutex_unlock(&queue);

        //czeka na przejechanie
        direction = change_direction();

        //blokada mutexu mostu aby w czasie przejazdu przejeżdżał tylko jeden samochód
            if(pthread_mutex_lock(&bridge) !=0 ){
                fprintf(stderr, "bridge mutex lock error");
                exit(-1);
            }        
        if(car_no == read_first_element(q_city_a) && direction == CITY_B)
        {
            

            curr_on_bridge = car_no;         

            //wjezdza do drugiego miasta
            city = CITY_B;

            q_city_a = delete(q_city_a);
            print_line();
            cars_in_city_B[curr_on_bridge]++;
            curr_on_bridge = -1;
            sleep(3);
            print_line();
            is_in_queue = 0;
            
        }
        else if(car_no == read_first_element(q_city_b) && direction == CITY_A)
        {

            curr_on_bridge = car_no;

            //wjezdza do drugiego miasta
            city = CITY_A;
        
            q_city_b = delete(q_city_b);
            print_line();
            cars_in_city_A[curr_on_bridge]++;
            curr_on_bridge = -1;
            sleep(3);
            print_line();
            is_in_queue = 0;
            
        }
        //odblokowanie mostu
        pthread_mutex_unlock(&bridge);
    }

}



void initialization(int no_of_cars){
    number_of_cars = no_of_cars;


    cars = malloc(number_of_cars * sizeof(pthread_t));

    cars_in_city_A = malloc(number_of_cars * sizeof(int));
    cars_in_city_B = malloc(number_of_cars * sizeof(int));

    q_city_a->a = -1;
    q_city_b->a = -1;
    q_city_a->next = NULL;
    q_city_b->next = NULL;
    
    direction = random_direction();

    for(int i = 0; i < number_of_cars; i++){
        cars_in_city_A[i] = 0;
        cars_in_city_B[i] = 0;
    }

    count_city(cars_in_city_A);

    int tab[number_of_cars]; 

    for(int i = 0; i < number_of_cars; i++){
        tab[i] = i;

        if(pthread_create(&cars[i], NULL, &car_thread, (void*)&tab[i])){

            perror("pthread_create error");
            exit(-1);

        }

    }

    for(int i =0; i < number_of_cars; i++){
        if(pthread_join(cars[i], NULL)){
            
            perror("pthread_join error");
            exit(-1);

        }
    }
}
