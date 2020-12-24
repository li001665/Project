#ifndef MAPPER_H
#define MAPPER_H

#include "utils.h"

char *mapOutDir; // expecting maximum path length from current folder to be 150 bytes

int mapperID;

// <1, 1, 1...>
typedef struct valueList {
	char value[100];
	struct valueList *next;
}valueList;

// <word, <1, 1, 1...>>
typedef struct intermediateDS{
	char key[100];
	valueList *value;
	struct intermediateDS *next;
}intermediateDS;

intermediateDS *interDS;
//-------------------------------------------

valueList *createNewValueListNode(char *value);
valueList *insertNewValueToList(valueList *root, char *count);
void freeValueList(valueList *root);

intermediateDS *createNewInterDSNode(char *word, char *count);
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count);
void freeInterDS(intermediateDS *root);

void emit(char *key, char *value);
void map(char *chunkData);

char *generateWordFileName(char *word);
void writeIntermediateDS();

#endif