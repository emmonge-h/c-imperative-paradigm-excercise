#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FILE_PATH "text.txt"
#define STRING_TERMINATOR_CHAR '\0'
#define MAX_BUFF_SIZE 2048
#define MAX_LINE_WIDTH 128
#define MAX_WORD_LENGTH 20
#define COMMON_WORDS_COUNT 20
#define MAX_RAW_WORDS 1000
#define MAX_DIFFERENT_WORDS 200
#define MAX_LINES 100

typedef struct Word {
    char *word;
    int count;
    int lines[MAX_LINES];
} Word;

bool WordIsInArray(char *word, const char *array[], int count);

void GetFileContent(char buff[MAX_BUFF_SIZE], const char* fileName);

bool StringIsLowerAlphaOnly(char *str, int count);

void UpdateWord(char *word, Word words[], int *wordCount, int *line);

void Swap(Word *i, Word *j);
void BubbleSort(Word arr[], int n);

int main()
{
    Word finalWords[MAX_DIFFERENT_WORDS];
    int currentFinalWordsCount = 0;

    const char *commonWords[COMMON_WORDS_COUNT] = {"el", "la", "los", "las", "y", "de", "a", "un", "una", "en", "que", "con"};
    const char *invalidWords[4] = {".", ",", " ", ""}; 

    int currentLine = 1;
    char buffer[MAX_BUFF_SIZE] = {STRING_TERMINATOR_CHAR};
    GetFileContent(buffer, FILE_PATH);

    char *bufferHead = &buffer[0];
    int currentWordChar = 0;
    char currentWord[MAX_WORD_LENGTH];

    // Read file content
    while (*bufferHead != STRING_TERMINATOR_CHAR)
    {
        if (*bufferHead == '\n')
        {
            currentLine++;
        }
        else if (*bufferHead == ' ' || *bufferHead == ',' || *bufferHead == '.')    // Delimiter
        {
            currentWord[currentWordChar] = STRING_TERMINATOR_CHAR;
            if (!WordIsInArray(currentWord, invalidWords, 4) && !(WordIsInArray(currentWord, commonWords, COMMON_WORDS_COUNT)) && StringIsLowerAlphaOnly(currentWord, currentWordChar))
            {
                UpdateWord(currentWord, finalWords, &currentFinalWordsCount, &currentLine);
            }
            currentWordChar = 0;
            memset(currentWord, 0, MAX_WORD_LENGTH);
        }
        else if (*bufferHead >= 0x61 && *bufferHead <= 0x7a)    // Add only lowercase characters
        {
            currentWord[currentWordChar] = *bufferHead;
            currentWordChar++;
        }
        bufferHead++;
    }
    
    // Sort words by instance amount
    BubbleSort(finalWords, currentFinalWordsCount);

    printf("Palabras y lineas de aparicion:\n");
    // Print words next to the lines where they are found
    for (int i = 0; i < currentFinalWordsCount; i++)
    {
        Word* currentWord = &finalWords[i];
        printf("%s: ", currentWord->word);
        for (int j = 0; j < MAX_LINES; j++)
        {
            if (currentWord->lines[j] == 0)
            {
                break;
            }
            printf("%i ", currentWord->lines[j]);
            
        }
        
        printf("\n");
    }

    // Print words ordered from most to least ocurrences
    printf("Palabras ordenadas por cantidad de apariciones:\n");
    for (int i = 0; i < currentFinalWordsCount; i++)
    {
        printf("%s: %i\n", finalWords[i].word, finalWords[i].count);
    }

    // Free memory used to store the words
    for (int i = 0; i < currentFinalWordsCount; i++)
    {
        Word* currentWord = &finalWords[i];
        free(currentWord->word);
    }
    
    return 0;
}

bool WordIsInArray(char *word, const char *array[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (array[i] == NULL) break;
        if (!strcmp(word, array[i])) return true;
    }
    return false;
}

void GetFileContent(char buff[MAX_BUFF_SIZE], const char* fileName)
{
    FILE *file = fopen(fileName, "r");
    char line[MAX_LINE_WIDTH];
    while (1)
    {
        char *lineIsNotNull = fgets(line, MAX_LINE_WIDTH, file);
        if (!lineIsNotNull)
        {
            break;
        }

        strcat(buff, line);
    }

    for (int i = 0; i < MAX_BUFF_SIZE; i++) buff[i] = tolower(buff[i]);
}

bool StringIsLowerAlphaOnly(char *str, int count)
{
    char *strHead = str;
    if (count == 0)
    {
        return false;
    }
    
    for (int i = 0; i < count; i++)
    {
        if (!(*strHead >= 0x61 && *strHead <= 0x7a))
        {
            return false;
        }
        strHead++;
        
    }
    return true;
}

void UpdateWord(char *word, Word words[], int *wordCount, int *line)
{
    for (int i = 0; i < *wordCount; i++)
    {
        Word *tempWord = &words[i];
        if (!strcmp(word, tempWord->word))
        {
            tempWord->lines[tempWord->count] = *line;
            tempWord->count += 1;
            return;
        }
    }
    Word newWord = {NULL, 1, {0}};
    newWord.word = malloc(strlen(word) + 1);
    strcpy(newWord.word, word);
    newWord.lines[0] = *line;
    words[*wordCount] = newWord;
    *wordCount += 1;
}

void Swap(Word *a, Word *b) {
    Word temp = *a;
    *a = *b;
    *b = temp;
}

void BubbleSort(Word arr[], int n) {
    for (int i = 0; i < n - 1; i++) {

        // swapped variable to signal if there is a
        // swap happened in the inner loop
        // initially set to false
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].count < arr[j + 1].count) {
                Swap(arr + j, arr + j + 1);

                // swapped is set to true if the swap is
                // done
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }
}