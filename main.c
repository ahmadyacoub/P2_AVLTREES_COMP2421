// Ahmad Jaghama 1202450 sec #1
 // removed printf the tree is empty and
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct AVLnode *AVL_T;
typedef struct AVLnode {
    char Course[200];
    int hours;
    char CourseCode[200];
    char depart[200];
    char topics[1000];
    AVL_T Left;
    AVL_T Right;
    int Height;

} Tree;

AVL_T tempTree;

AVL_T minValueNode(AVL_T root) {  // A function that finds the smallest value or the leftmost leaf
    AVL_T temp = root;
    while (temp->Left != NULL) // iteration loop
        temp = temp->Left;
    return temp;
}

int height(AVL_T root) { // a function to get the node height
    //  function to avoid an error
    if (root == NULL)  // if the node null return 0
        return 0;
    return root->Height;
}

AVL_T findCourse(AVL_T root, char *code) {   // A function that finds a Course that has the same Code by recursion
    if (root) { // when root not null
        if (strcmp(code, root->CourseCode) == 0 ||
            (strcmp(code, root->CourseCode) == 10 && code[0] == root->CourseCode[0])) {
            // the second condition to avoid trailing ling from fgets func
            return root;
        } else if (strcmp(code, root->CourseCode) < 0) {
            return findCourse(root->Left, code);
        } else if (strcmp(code, root->CourseCode) > 0) {

            return findCourse(root->Right, code);
        }

    }

}

int MAX(int x, int y) { // compare function
    if (x > y)
        return x;
    else
        return
                y;
}


AVL_T singleRotateLeft(AVL_T root) { // TREE rotation function to left
    AVL_T temp;
    temp = root->Right;
    root->Right = temp->Left;
    temp->Left = root;
    root->Height = 1 + MAX(height(root->Left), height(root->Right));
    temp->Height = 1 + MAX(height(root), height(temp->Right));
    return temp;
}

AVL_T singleRotateRight(AVL_T root) {// TREE rotation function to Right
    AVL_T temp;
    temp = root->Left;
    root->Left = temp->Right;
    temp->Right = root;

    root->Height = 1 + MAX(height(root->Right), height(root->Left));
    temp->Height = 1 + MAX(height(root), height(temp->Left));

    return temp;
}

AVL_T doubleRotateLeft(AVL_T K3) {
    K3->Left = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}

AVL_T doubleRotateRight(AVL_T K3) {
    K3->Right = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}

AVL_T insert(AVL_T t, char course[200], char CourseCode[200], int hours, char depart[200], char topics[1000]) {
// function to add into the avl tree

    if (t == NULL) { // allocate the node when find the index or the right place
        t = malloc(sizeof(struct AVLnode));

        strcpy(t->CourseCode, CourseCode); // strcpy because its char []
        strcpy(t->Course, course);
        strcpy(t->depart, depart);
        strcpy(t->topics, topics);
        t->hours = hours;
        t->Height = 0;
        t->Left = t->Right = NULL;

    } else if (strcmp(CourseCode, t->CourseCode) < 0) { // added code is smaller than the root code go to left


        t->Left = insert(t->Left, course, CourseCode, hours, depart, topics);
        if (height(t->Left) - height(t->Right) >= 2) {
            if (strcmp(CourseCode, t->Left->CourseCode) < 0)
                t = singleRotateRight(t);
            else
                t = doubleRotateLeft(t);
        }
    } else if (strcmp(CourseCode, t->CourseCode) > 0) {// added code is smaller than the root code go to right
        t->Right = insert(t->Right, course, CourseCode, hours, depart, topics);
        if (height(t->Right) - height(t->Left) >= 2) {
            if (strcmp(CourseCode, t->Right->CourseCode) > 0)
                t = singleRotateLeft(t);
            else
                t = doubleRotateRight(t);
        }
    }

    t->Height = 1 + MAX(height(t->Right), height(t->Left));

    return t;
}

void printInOrder(AVL_T t) { // A function that prints the tree in  (AOB) INORDER TRAVERSING MODE
    if (t != NULL) {
        printInOrder(t->Left);
        printf("%s\t", t->CourseCode);
        printInOrder(t->Right);
    }
}

void printInOrderWithTopics(AVL_T t) {
    // A function that prints the tree in  (AOB) INORDER TRAVERSING MODE + the topics
    if (t != NULL) {
        printInOrderWithTopics(t->Left);
        printf("%s its topics : %s\n ", t->CourseCode, t->topics);

        printInOrderWithTopics(t->Right);
    }
}

void printInOrderDepart(AVL_T t, char *dep) {
    // A function that prints the tree in  (AOB) INORDER TRAVERSING MODE +by department
    if (t != NULL) {
        printInOrderDepart(t->Left, dep);
        if (strcmp(t->depart, dep) == 0)
            printf("\t\t\t%s\n", t->CourseCode);
        printInOrderDepart(t->Right, dep);
    }
}

void traverseTree(struct AVLnode *root,
                  FILE *fp) { // IN CASE 10 THAT NEEDS TO PRINT A FILE I FACED A PROBLEM ,I SOLVED IT BY USING FUNCTIONS
    if (root != NULL) { //TRAVERSE USING RECURSION
        traverseTree(root->Left, fp);
        fprintf(fp, "%s:%d#%s#%s#/#/%s", root->Course, root->hours, root->CourseCode, root->depart, root->topics); //
        fprintf(fp, "\n");
        traverseTree(root->Right, fp);
    }
}

void printCase4(struct AVLnode *root) {
    // List courses in lexicographic order with their associated
    if (root != NULL) { //TRAVERSE USING RECURSION
        printCase4(root->Left);
        printf("*********************************\n");
        printf("%s : %d # %s \n %s /%s\n", root->Course, root->hours, root->CourseCode, root->depart, root->topics);
        printf("*********************************");
        printCase4(root->Right);
    }
}

void printToFile(struct AVLnode *root) {
    FILE *fp; // DECLARE A FILE TO ADD THE OUTPUT IN CASE 10
    fp = fopen("offered_courses.txt", "w");

    traverseTree(root, fp); // TRAVERSING THE TREE AND PRINT THE DATA

    fclose(fp); // CLOSE THE FILE
}

int nodeBalance(AVL_T root) { // function to calculate the balance for a node
    if (root == NULL)
        return 0;
    return height(root->Left) - height(root->Right);
}

struct Node *deleteNode(AVL_T root, char *CourseCode) {

    AVL_T temp = malloc(sizeof(AVL_T)); // allocating the temp
    if (root == NULL)
        return root;
    // if the course we want to delete is smaller than root code then go to the left
    if (strcmp(CourseCode, root->CourseCode) < 0)
        root->Left = deleteNode(root->Left, CourseCode); // recursive call
        // if the course we want to delete is bigger than root code then go to the Right
    else if (strcmp(CourseCode, root->CourseCode) > 0)
        root->Right = deleteNode(root->Right, CourseCode);// recursive call
    else {

        if ((root->Left == NULL) || (root->Right == NULL)) {
            temp = root->Left ? root->Left : root->Right;

            // no  child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else //1 child case
                root = temp; // save child in root
        } else {
            // 2 child xase
            AVL_T temp = minValueNode(root->Right); // save the smallest value from the left
            strcpy(root->CourseCode, temp->CourseCode);
            root->Right = deleteNode(root->Right, temp->CourseCode);
        }
    }

    if (root == NULL)
        return root;

    // calculate the height and save it
    root->Height = 1 + MAX(height(root->Left),
                           height(root->Right));
    //check the balance
    int balance = nodeBalance(root);

    //UNBALANCED!!!!
    // Left Left Case
    if (balance > 1 && nodeBalance(root->Left) >= 0)
        return singleRotateRight(root);

    // Left Right Case
    if (balance > 1 && nodeBalance(root->Left) < 0) {
        root->Left = singleRotateLeft(root->Left);
        return singleRotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && nodeBalance(root->Right) <= 0)
        return singleRotateLeft(root);

    // Right Left Case
    if (balance < -1 && nodeBalance(root->Right) > 0) {
        root->Right = singleRotateRight(root->Right);
        return singleRotateLeft(root);
    }

    return root;
}

void firstLetterFinder(AVL_T root, char *letter) {
    // A Function that traverse over the tree and checks by the first letter of code and given char
    if (root != NULL) {
        firstLetterFinder(root->Left, letter);
        firstLetterFinder(root->Right, letter);

        if (root->CourseCode[0] == letter[0]) // the condition
            // adding the node to global tree to delete it or save it
            tempTree = insert(tempTree, root->Course,
                              root->CourseCode, root->hours, root->depart, root->topics);
    }
}

void departmentFinder(AVL_T root, char *dep) {
    // A Function that traverse over the tree and checks by the Course Department and given string
    if (root != NULL) {
        departmentFinder(root->Left, dep);
        departmentFinder(root->Right, dep);
        if (strcmp(dep, root->depart) == 0 || (strcmp(dep, root->CourseCode) == 10 && dep[0] == root->CourseCode[0])) {
            // adding the node to global tree to delete it or save it
            tempTree = insert(tempTree, root->Course,
                              root->CourseCode, root->hours, root->depart, root->topics);
        }
    }
}

AVL_T deleteByLetter(AVL_T root) { // a function that deletes the global tree nodes
    if (tempTree != NULL) {
        root = deleteNode(root, tempTree->CourseCode);
        tempTree = deleteNode(tempTree, tempTree->CourseCode);
        deleteByLetter(root); // recursion
    }
    return root;
}

char *scanString() {
    // this function is used to scan a char from console better
    // than scanf that is not efficient
    char *temp[200];
    fgets(temp, 200, stdin);

    int len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n')
        temp[len - 1] = '\0';
    return temp;
}

int main() {
    char line[255];
    char array[1000][255];
    int a = 0;
    int loadCoursesLimit = 0;
    AVL_T root = NULL;
    char switchLine[10];
    int num;
    char *code;
    char codes[200];
    char depart[20];
    char courseName[20];
    char topics[100];
    int hours, len;
    AVL_T temp;
    FILE *coursesFile = fopen("courses.txt", "r");


    while (1) { // a while loop for program menu
        printf("\n------------------------------------------------------------------------------------------------\n");
        printf("\t\t\t1-Load Courses information file\n");
        printf("\t\t\t2-Insert a new course \n");
        printf("\t\t\t3-Find a course and update  its information \n");
        printf("\t\t\t4-List courses in lexicographic order \n");
        printf("\t\t\t5-List all topics associated with a given courses \n");
        printf("\t\t\t6-List all courses in lexicographic order that belong to the same department \n");
        printf("\t\t\t7-Delete a course \n");
        printf("\t\t\t8-Delete all courses that start with a specific letter\n");
        printf("\t\t\t9- Delete all courses belong to a given department\n");
        printf("\t\t\t10- Save all words in a file\n");
        printf("\t\t\t-1 - Exit :) \n");
        printf(" ENTER YOUR CHOICE :\t");

        fgets(switchLine, 10, stdin);
        int len = strlen(switchLine);
        if (len > 0 && switchLine[len - 1] == '\n')
            switchLine[len - 1] = '\0';
        num = atoi(switchLine);
        printf("\n------------------------------------------------------------------------------------------------\n");
        switch (num) {


            case 1:
                if (loadCoursesLimit == 0) {
                    while (!feof(coursesFile)) { // READING THE COURSES FILE
                        fgets(line, 255, coursesFile);
                        len = strlen(line);
                        if (len > 0 && line[len - 1] == '\n') // TO CHECK THE TRAILING LINE FROM FGETS
                            line[len - 1] = '\0';
                        strcpy(array[a], line);
                        a++;
                    }

                    fclose(coursesFile); // closing the file
                    for (int i = 0; i < a; ++i) {
                        char *tok; // to poin into the separated string
                        char topics[1000];
                        tok = strtok(array[i], ":");

                        AVL_T courseTemp = (AVL_T) malloc(sizeof(AVL_T));
                        int c = 1;

                        while (tok != NULL && c < 7) {

                            switch (c) {

                                case 1:
                                    strcpy(courseTemp->Course, tok);

                                    tok = strtok(NULL, "#");

                                    break;
                                case 2:
                                    courseTemp->hours = atoi(tok);

                                    tok = strtok(NULL, "#");

                                    break;
                                case 3:
                                    strcpy(courseTemp->CourseCode, tok);

                                    tok = strtok(NULL, "/");

                                    break;

                                case 4:
                                    strcpy(courseTemp->depart, tok);

                                    //   tok = strtok(NULL, ",");

                                    break;
                                case 6:
                                    tok = strtok(NULL, ",");
                                    while (tok != NULL) {

                                        strcat(topics, tok);
                                        tok = strtok(NULL, ",");
                                    }
                                    strcpy(courseTemp->topics, topics);


                                    free(tok);
                                    break;

                                default:

                                    break;
                            }
                            c++;
                        }
                        root = insert(root, courseTemp->Course, courseTemp->CourseCode, courseTemp->hours,
                                      courseTemp->depart,
                                      topics);
                        free(courseTemp);
                        memset(topics, 0, strlen(topics)); // to clear the string



                    }
                    printf("\t\t\t <<<< COURSES LOADED SUCCESSFULLY >>>> :)");
                    loadCoursesLimit++; // to avoid load the file again
                    break;
                } else {
                    printf("\t\t\t>>>>The file is already LOADED  <<<< :)");
                    break;
                }
            case 2:
                fflush(stdin); // to clear console
                printf("\n \t\t\tEnter the course code to be ADDED :>\t");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n') // remove trailing line
                    code[len - 1] = '\0';
                strcpy(codes, code); //  i used pointer func
                if (!findCourse(root, code)) { //WHEN I WRITE A NON IN TREE CODE BREAKS HERE
                    fflush(stdin);
                    printf("\n \t\t\tEnter the course name to be ADDED :>\t");

                    code = scanString();
                    len = strlen(code);
                    if (len > 0 && code[len - 1] == '\n')
                        code[len - 1] = '\0';
                    strcpy(courseName, code); //  i used pointer func

                    fflush(stdin); // clear the console
                    printf("\n \t\t\t Enter the course department to be ADDED :>\t");
                    code = scanString();
                    len = strlen(code);
                    if (len > 0 && code[len - 1] == '\n')
                        code[len - 1] = '\0';
                    strcpy(depart, code); //  i used pointer func
                    fflush(stdin);
                    printf("\n \t\t\t Enter the course hours to be ADDED :>\t");
                    scanf("%d", &hours);
                    fflush(stdin);
                    printf("\n \t\t\t Enter the course topics to be ADDED :>\t");
                    code = scanString();
                    len = strlen(code);
                    if (len > 0 && code[len - 1] == '\n')
                        code[len - 1] = '\0';
                    strcpy(topics, code); //  i used pointer func
                    root = insert(root, courseName, codes, hours, depart, topics);

                    break;
                } else {

                    printf("\t<<<<Course is already ADDED with code :%s >>>>", code);
                    break;
                }

            case 3: // Course Code? will it be updated ? and topics?
                fflush(stdin);
                printf("ENTER COURSE CODE TO BE UPDATED : ");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';

                if (!findCourse(root, code)) {
                    printf("\t <<<<THE COURSE WITH CODE : %s IS NOT FOUND >>>>");
                    break;


                } else if ((strcmp(findCourse(root, code)->CourseCode, code)) ==
                           0) { // func works but cant read it from console good
                    temp = findCourse(root, code);
                    printf("%s %s %s %d h : %s", temp->Course, temp->CourseCode, temp->depart, temp->hours,
                           temp->topics);

                    printf(" \n <<<<TO CHANGE COURSE INFORMATION ENTER A NUMBER >>>>\n");
                    printf("1- CHANGE COURSE NAME\n");
                    printf("2- CHANGE COURSE DEPART \n");
                    printf("3- CHANGE COURSE HOURS \n");
                    printf("4- CHANGE COURSE TOPICS \n");
                    printf("   TO FINISH PRESS ANYTHING ELSE \n");
                    printf(" ENTER YOUR CHOICE :\t");
                    fgets(switchLine, 10, stdin);
                    int len = strlen(switchLine);
                    if (len > 0 && switchLine[len - 1] == '\n')
                        switchLine[len - 1] = '\0';

                    num = atoi(switchLine);
                    printf("\n------------------------------------------------------------------------------------------------\n");
                    switch (num) {


                        case 1:
                            fflush(stdin);
                            printf(" NEW COURSE NAME \n");
                            code = scanString();
                            len = strlen(code);
                            if (len > 0 && code[len - 1] == '\n')
                                code[len - 1] = '\0';
                            strcpy(temp->Course, code);
                            break;
                        case 2:
                            fflush(stdin);
                            printf(" NEW DEPARTMENT NAME \n");
                            code = scanString();
                            len = strlen(code);
                            if (len > 0 && code[len - 1] == '\n')
                                code[len - 1] = '\0';
                            strcpy(temp->depart, code);
                            break;
                        case 3:
                            fflush(stdin);
                            printf(" NEW HOURS NUM \n");
                            scanf("%d", &hours);
                            temp->hours = hours;
                            break;
                        case 4:
                            fflush(stdin);
                            printf(" NEW TOPICS  \n");
                            code = scanString();
                            len = strlen(code);
                            if (len > 0 && code[len - 1] == '\n')
                                code[len - 1] = '\0';
                            strcpy(temp->topics, code);
                            break;
                        default:
                            break;


                    }


                }

                break;
            case 4:
                printCase4(root);
                break;
            case 5:
                printInOrderWithTopics(root);

                break;
            case 6:
                fflush(stdin);
                printf("\n   <<Enter department of courses name:>>\t ");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';
                printf("\n");
                printInOrderDepart(root, code);

                break;
            case 7:

                fflush(stdin);
                printf("ENTER COURSE CODE TO BE DELETED : ");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';

                if (!findCourse(root, code)) {
                    printf("\t <<<<THE COURSE WITH CODE : %s IS NOT FOUND >>>>");
                    break;


                } else if ((strcmp(findCourse(root, code)->CourseCode, code)) ==
                           0) { // func works but cant read it from console good
                    temp = findCourse(root, code);
                    printf("%s %s %s %d h : %s", temp->Course, temp->CourseCode, temp->depart, temp->hours,
                           temp->topics);
                    root = deleteNode(root, temp->CourseCode);
                    printf("\n<<<<COURSE WITH CODE : %s IS DELETED SUCCESSFULLY >>>>", code);
                    break;


                }
            case 8:
                fflush(stdin);
                printf("ENTER A LETTER TO DELETE ALL COURSES STARTS WITH IT  : ");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n') {
                    code[len - 1] = '\0';
                }

                firstLetterFinder(root, code);
                root = deleteByLetter(root);

                break;
            case 9:
                fflush(stdin);
                printf("\n   <<Enter department of courses name:>>\t ");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n') {
                    code[len - 1] = '\0';
                }
                departmentFinder(root, code);
                if (tempTree == NULL)
                    printf("\t\t\t THERE NO COURSES TO DELETE :)");
                printInOrder(tempTree);
                root = deleteByLetter(root);


                break;
            case 10:

                printToFile(root);

                break;
            case -1:
                printf("\t\t\t\t\t<    <    <    F  I  N  I  S  H  E  D    >    >    >    ");
                printf("\n------------------------------------------------------------------------------------------------\n");
                exit(0);
            default:
                printf("\n Wrong Input Re Enter \n ");
                break;


        }


    }
}