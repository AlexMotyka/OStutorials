#include <stdlib.h>
#include <stdio.h>
int main(void)
{
    //select our file
    FILE *myFile;
    myFile = fopen("question2.txt", "r");

    //our array for holding the values
    int numberArray[10];
    int i;

    //go through the file and extract the numbers
    for (i = 0; i < 10; i++)
    {
        fscanf(myFile, "%d", &numberArray[i]);
    }

    //print the contents to the terminal
    for (i = 0; i < 10; i++)
    {
        printf("Number is: %d\n\n", numberArray[i]);
    }


}

