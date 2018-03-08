/* 
 * File:   main.c
 * Author: Kenneth Bonilla
 * Description: Simple program to manage courses using enum and struct
 * User can add, drop, or print class list. The classes are sorted by
 * class number eg SER240 is sorted by 240. Classes include subject, number,
 * teacher name, and credit hours. Total credit hours is tallied.
 *
 * Created on March 18, 2017, 7:41 PM
 * Skeletal provided by ASU SER 334 baseschedule.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS
////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
enum Subject
    {
        SER=0,
        EGR=1,
        CSE=2,
        EEE=3
    };
struct Course {
    enum Subject subject;
    int course_num;
    char teacher_name[1024];
    int credit_hour;
};


//GLOBAL VARIABLES
int tot_credit_hours = 0;
int tot_courses = 0;
int sz_arr = 0;
//place to store course information
struct Course* CourseCollection = NULL;
struct Course* CHold = NULL; //temp array used for incrementing size by 1


//FORWARD DECLARATIONS
void branching(char option);
void course_insert(struct Course);
void course_drop(int drop);
void schedule_print();
void printCCode(enum Subject sub);
void shift_r(int i);
void shift_l(int i);

/* 
 * 
 */
int main(int argc, char** argv) {
    char input_buffer;

    printf("\n\nWelcome to ASU Class Schedule\n");

    //menu and input loop
    do {
            printf("\nMenu Options\n");
            printf("------------------------------------------------------\n");
            printf("a: Add a class\n");
            printf("d: Drop a class\n");
            printf("s: Show your classes\n");
            printf("q: Quit\n");
            printf("\nTotal Credits: %d\n\n", tot_credit_hours);
//            printf("\nTotal classes: %d\n", tot_courses); 
            //used for troubleshooting
//            printf("sz_arr %d \n", sz_arr);
            printf("Please enter a choice ---> ");

            scanf(" %c", &input_buffer);

            branching(input_buffer);
    } while (input_buffer != 'q');

    return 0;

}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.
void branching(char option) {
	switch (option) {
	case 'a':;
            //Collect course information to add
            struct Course course_hold;
            printf("\nWhat is the subject? (SER=0, EGR=1, CSE=2, EEE=3)?");
            scanf("%d", &course_hold.subject);
            printf("\nWhat is the number (e.g 240)?");
            scanf("%d", &course_hold.course_num);
            printf("\nHow many credits is the class (e.g. 3)?");
            scanf("%d", &course_hold.credit_hour);
            printf("\nWhat is the name of the teacher?");
            scanf("%s", &course_hold.teacher_name);
            
            course_insert(course_hold);
		break;

	case 'd':;
            //Gets course id to drop
            int drop;
            printf("\nWhat class to drop (e.g. 240)?");
            scanf("%d", &drop);
            
            course_drop(drop);
		break;

	case 's':
            schedule_print();
		break;

	case 'q':
		// main loop will take care of this.
		break;

	default:
		printf("\nError: Invalid Input.  Please try again...");
		break;
	}
}

void course_insert(struct Course course_hold){
    //if CourseCollection is null pointer
    
    if (!CourseCollection){
        //initialize array with 1
        CourseCollection =  malloc(sizeof(CourseCollection));
        //copy initial course
        CourseCollection[0]=course_hold;
        // Set tracking variables
        tot_credit_hours += course_hold.credit_hour;
        tot_courses++;
        sz_arr = 1;
        
        return; //exit after setting initial conditions
    }
    
    //increases array size by 1
    sz_arr++;
    CHold = malloc(sizeof(CHold)*sz_arr);
    memcpy (&CHold, &CourseCollection, sizeof(CourseCollection));
    CourseCollection = CHold;
        

    
    int i=0;
    for(i; i< tot_courses; i++){
        //inspects if element must be placed in between elements
        //that already populate the array
        if(course_hold.course_num<CourseCollection[i].course_num){
            shift_r(i);
            CourseCollection[i]=course_hold;
            tot_credit_hours += course_hold.credit_hour;
            tot_courses++;
            printf("\nCourse added1\n");
            
            return; //courses added successfully
        }
    }
    //appends to end of run if not between elements
    CourseCollection[i]=course_hold;
    tot_courses++;
    tot_credit_hours+=course_hold.credit_hour;
    printf("\nCourse added2\n");

    
}



void course_drop(int drop){
    //if CourseCollection is null pointer or there are no courses
    if (!CourseCollection || tot_courses==0){
        printf("\nThere are no Courses to drop.\n");
        return; 
    }
    
    
        
    
    int i;
    for(i = 0; i < tot_courses; i++){
        if(drop == CourseCollection[i].course_num){
            //adjust trackers
            tot_credit_hours -= CourseCollection[i].credit_hour;
            tot_courses--;
            //remove course
            shift_l(i);
            //decrease array size by one
            sz_arr--;
            CHold = malloc(sizeof(CHold)*sz_arr);
            memcpy (&CHold, &CourseCollection, sizeof(CourseCollection));
            CourseCollection = CHold;
            return; //class was successfully added
        }
    }
    
    printf("\nCourse does not exist.\n\n");

}

//prints schedule formatted by tabs
void schedule_print() {
    printf("\n\nClass Schedule:\n");
    int k;
    for(k=0; k<tot_courses; k++){
        printCCode(CourseCollection[k].subject);
        printf("%d \t\t",CourseCollection[k].course_num);
        printf("%d \t\t", CourseCollection[k].credit_hour);
        printf("%s\n", CourseCollection[k].teacher_name);
    }
}

//prints Enum to String, may refactor later, not sure if better way
//to handle Enum in C
void printCCode(enum Subject sub) {
    switch(sub){
        case SER: printf("SER"); break;
        case EGR: printf("EGR"); break;
        case CSE: printf("CSE"); break;
        case EEE: printf("EEE"); break;
        default : break;
        
    }
}

//shifts elements of array right one from point of entry to right end
void shift_r(int i) {
    int j;
    for(j=sz_arr-1; j>=i; j--){
        CourseCollection[j]=CourseCollection[j-1];
    }
}

//shifts elements of array left one to remove element at point of insertion
void shift_l(int i) {
    int j = tot_courses;
    for(i; i<j; i++){
        CourseCollection[i]=CourseCollection[i+1];
    }
    
}

