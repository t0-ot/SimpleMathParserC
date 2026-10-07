#include <stdio.h>
#include <string.h>

typedef struct {
    double num;
    int startPos;
    int stopPos;
} numbers;

typedef struct {
    char op;
    int pos;
} operationers;

double calcMonsterInterp(numbers numList[], operationers operList[], int *count);
char operatorEater(char const *strText, int numEndPos, int *pos);
double numberRecon(char const *strNum, int startRead, int *stopRead);

int main(void) {
    char mathprob[100];
    numbers numberSet[100];
    operationers opers[100];
    int endPos = 0, startPos = 0, operPos = 0, i = 0;

    printf("Enter a simple math problem: ");
    fgets(mathprob, sizeof(mathprob), stdin);
    mathprob[strcspn(mathprob, "\n")] = '\0';
    while (mathprob[endPos] != '\0') {
        numberSet[i].num = numberRecon(mathprob, startPos, &endPos);
        numberSet[i].startPos = startPos;
        numberSet[i].stopPos = endPos;
        startPos = endPos + 1;
        while ((mathprob[startPos] > '9' || mathprob[startPos] < '0') && mathprob[startPos] != '\0')
            startPos++;
        opers[i].op = operatorEater(mathprob, endPos, &opers[i].pos);
        i++;
    }
    printf("This is your operation: \n");
    for (int k = 0; k < i; k++) {
        printf("%lf %c ", numberSet[k].num, opers[k].op);
    }
    printf("\n");
    printf("Calculated result = %.2lf", calcMonsterInterp(numberSet, opers, &i));
    return 0;
}

double numberRecon(char const *strNum, int startRead, int *stopRead) {
    int hasDeci = 0, placeVal = 0, i = 0, negateNum = 1;
    float deciNum = 0;
    double currentNum = 0;
    for (i = startRead; strNum[i] != ' ' && strNum[i] != '\0'; i++) {
        if (strNum[i] == '.') {
            hasDeci = 1;
            continue;
        }
        if (strNum[i] > '9' || strNum[i] < '0')
            break;
        if ( i != 0 && strNum[i-1] == '-')
            negateNum = -1;
        int tempVar = (int)strNum[i] - 48;
        if (hasDeci) {
            deciNum = deciNum * 10 + tempVar;
            placeVal++;
            continue;
        }
        currentNum = currentNum * 10 + tempVar;
    }
    *stopRead = i;
    for (int i = placeVal; i > 0; i--)
        deciNum /= 10;
    return (currentNum + deciNum) * negateNum;
}

char operatorEater(char const *strText, int numEndPos, int *pos) {
    for (int i = numEndPos; strText[i] != '\0'; i++) {
        if (strText[i] != '+' && strText[i] != '-' && strText[i] != '*' && strText[i] != '/')
            continue;
        *pos = i;
        if (strText[i] == '-' && (strText[i+1] <= '9' && strText[i+1] >= '0'))
            return '+';
        return strText[i];
    }
    return '\0';
}

double calcMonsterInterp(numbers numList[], operationers operList[], int *count) {
    double workingChunk, finalout;
    for (int i = 0; operList[i].op != '\0'; i++) {
        if (operList[i].op == '+' || operList[i].op == '-')
            continue;
        switch (operList[i].op) {
            case '*':
                numList[i].num *= numList[i+1].num;
                break;
            case '/':
                numList[i].num /= numList[i+1].num;
                break;
        }
        for (int k = i; k < *count; k++) {
            numList[k+1].num = (k + 2 == *count) ? 0 : numList[k+2].num;
            operList[k].op = operList[k+1].op;
        }
        *count = *count -1;
        i--;
    }
    for (int i = 0; operList[i].op != '\0'; i++) {
        if (operList[i].op == '*' || operList[i].op == '/')
            continue;
        switch (operList[i].op) {
            case '+':
                numList[i].num += numList[i+1].num;
                break;
            case '-':
                numList[i].num -= numList[i+1].num;
                break;
        }
        for (int k = i; k < *count; k++) {
            numList[k+1].num = (k + 2 == *count) ? 0 : numList[k+2].num;
            operList[k].op = operList[k+1].op;
        }
        *count = *count - 1;
        i--;
    }
    return numList[0].num;
}
