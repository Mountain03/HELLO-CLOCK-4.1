#ifndef CRSTRLIB_H_INCLUDED
#define CRSTRLIB_H_INCLUDED

//#include "common.h"
#include <Arduino.h>

#define MAX_STR_NUM 100

/* status ״̬�� */
typedef int Status;

#define STATUS_SUCCESS 0
#define STATUS_FAIL -1
#define STATUS_NOT_FOUND 404

#define TRUE    1
#define FALSE   0

typedef bool Boolean;

Boolean isNum(char c);

Boolean isSymbol(char c);

int CRStrLib_char2Int(char c);

int CRStrLib_strLen(char *str);

Status CRStrLib_findNextString(char *str, int strLen, int *strIdx, char *result);
Status CRStrLib_findNextStringWithNumber(char *str, int strLen, int *strIdx, char *result);

Status CRStrLib_findNextInt(char *str, int strLen, int *strIdx, int *result);

Status CRStrLib_findNextFloat(char *str, int strLen, int *strIdx, float *result);

Boolean CRStrLib_isValidData(char *str, char *head, char *tail);
Boolean CRStrLib_isValidDataWithDefaultEnd(char *str, char *head);

Boolean CRStrLib_startWith(char *str, char *head);

Boolean CRStrLib_endWith(char *str, char *tail);

#endif // CRSTRLIB_H_INCLUDED
