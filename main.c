//Cioranu Nickolass-Alexandru

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define CMD_LENTGH 1050

void execute(char *command, FilesList *files, Tree tree, FILE *fout)
{
	if (command[strlen(command) - 1] == '\n')
		command[strlen(command) - 1] = '\0';
	char *oper = strtok(command, " ");
	if (strcmp(oper, "ADD") == 0) {
		char *id = strtok(NULL, " "), *score = strtok(NULL, " "), **words;
		int nrWords = atoi(strtok(NULL, " "));
		words = allocStrMatrix(nrWords + 1, WORD_LENGTH);
		for(int i = 0; i < nrWords; i++) {
			strcpy(words[i], strtok(NULL, " "));
		}
		addFile(id, atoi(score), nrWords, words, files, tree, fout);
		freeStrMatrix(words, nrWords + 1);
	} else if (strcmp(oper, "DEL") == 0) {
		char *id = strtok(NULL, " ");
		*files = removeFile(id, *files, tree, fout);
	} else if (strcmp(oper, "ADDKW") == 0) {
		char *id = strtok(NULL, " "), *word = strtok(NULL, " ");
		addKw(id, word, *files, tree, fout);
	} else if (strcmp(oper, "DELKW") == 0) {
		char *id = strtok(NULL, " "), *word = strtok(NULL, " ");
		delKw(id, word, files, tree, fout);
	} else if (strcmp(oper, "FIND") == 0) {
		char *word = strtok(NULL, " ");
		find(word, tree, fout);
	} else if (strcmp(oper, "TOPK") == 0) {
		char *word = strtok(NULL, " "), *k = strtok(NULL, " ");
		topK(word, atoi(k), tree, fout);
	} else if (strcmp(oper, "PRINT") == 0) {
		char *string = allocStr(WORD_LENGTH);
		string[0] = '\0';
		printWords(tree, string, 0, fout, tree);
		free(string);
	}
}

int main(void)
{
	FILE *fin = fopen("index.in", "r"), *fout = fopen("index.out", "w") ;
	FilesList files = NULL;
	Tree tree = initTree('\0', false);
	char *command = allocStr(CMD_LENTGH);
	int nrOper;
	fscanf(fin, "%d ", &nrOper);
	for(int i = 0; i < nrOper; i++) {
		fgets(command, CMD_LENTGH, fin);
		execute(command, &files, tree, fout);
	}	
	free(command);
	FreeFilesList(files);
	tree = freeTree(tree);
	fclose(fin); fclose(fout);
	return 0;
}