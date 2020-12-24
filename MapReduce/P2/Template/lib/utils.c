#include "utils.h"

char *getChunkData(int mapperID) {
	
	struct msgBuffer msg; 
	key_t key;
	int msgid;
	key = ftok(".", 5598966); 				// generate a specific key id according to the parameters given
    	// printf("Key value: %d\n", key); 			// print out the key value so that we can test
	msgid = msgget(key, 0666 | IPC_CREAT);
	if (msgid == -1){ 					// check whether the error happens when get message queue
		printf("Error: message queue create error\n");
        return NULL; 					
	}
    	/*
	else{
		printf("mapper %d get Chunk Data msgid is %d\n", mapperID, msgid); 
		// print out the msgid and the corrent mapperID so that we can test
	}
    	*/
	memset(msg.msgText, '\0', 1024); 			
    	if((msgrcv(msgid, &msg, sizeof(msg.msgText), mapperID, 0)) < 0){ 		// check whether the error happens when receive message
		printf ("Error: message received error");
        	return NULL; 						
    	}
    	char *str;
    	str = (char *)malloc(1024); 							// allocate 1024 bytes to str
    	strcpy(str,msg.msgText); 							// copy the msgText to str
    	if (strcmp(str,"END") == 0) { 							// if receive the END message
        	return NULL; 
    	}
    	else {
        	return str;
    	}
}

// sends chunks of size 1024 to the mappers in RR fashion

void sendChunkData(char *inputFile, int nMappers) {
	
    
	FILE* fp = fopen(inputFile, "r");
	if(fp == NULL){ 					// check whether the input file is opened
		printf("Error: cannot open the file %s\n", inputFile);
 	        return; 
	}
    
	key_t key;
	int msgid;
	key = ftok(".", 5598966); 				// generate a specific key id according to the parameters given
    	// printf("Key value: %d\n", key); 			// print out the key value so that we can test
	msgid = msgget(key, 0666 | IPC_CREAT);
	if (msgid == -1){ 					// check whether the error happens when get message queue
		printf("Error: message queue create error\n");
        	return; 
	}
    	/*
	else{
		printf("send Chunk Data msgid is %d\n", msgid); // print out the msgid and the corrent mapperID so that we can test
	}
     	*/
	int chunk_size = 0;
	int word_size = 0;
	int flag = 0;
	int size = 1;
	char word[50];
	struct msgBuffer msg;
	while(1){
		msg.msgType = size;
		memset(msg.msgText, '\0', MSGSIZE); 
		if(word_size!= 0){
			for(int i=0;i<word_size;i++)
				msg.msgText[i] = word[i];
			chunk_size = chunk_size + word_size; 	//put the word which can not hold in the previous chunk into a new chunk		
		}
		while(chunk_size < 1023){
			int index = 0;
			int c;
			flag = 0;
			memset(word, '\0', 50);
			word_size = 0;
			while((c = fgetc(fp))!= EOF){	     	
				if((unsigned char)c == ' '){
				    word[index] = ' ';
				    word_size++;
				    flag = 1; 			//normal break or abnormal					    
			            break;
				}
				else{
				    word[index] = (unsigned char)c;
				    index++;
				    word_size++;				
				}
			}
			if((1023-chunk_size) >= word_size){
				for(int i=0, j=chunk_size; i<word_size;i++,j++)
					msg.msgText[j] = word[i]; 
				chunk_size = chunk_size + word_size;
			}
			else
				break;	
			if(flag == 0)
				break;								
		}//one chunk
		if((msgsnd(msgid, &msg, sizeof(msg.msgText), 0)) < 0)			// check whether the error happens when send message
			printf("Error: message send in sendChunkdata error.\n");
		chunk_size = 0;
		size++;
		if(size > nMappers)
			size = 1;	
		if(flag == 0)
			break;
	}//end loop
	
    	if (fclose(fp) == -1){ 				// check whether there's error when close the file
        	printf("Error: %s close error\n", inputFile);
        	return; 
    	};
		
	//send End message to mappers
	for(int i=1; i<=nMappers; i++){
		msg.msgType = i;		
		memset(msg.msgText, '\0', MSGSIZE);
		strcpy(msg.msgText, "END");
		if(msgsnd(msgid, &msg, sizeof(msg.msgText), 0) < 0){ // check whether the error happens when send message
			printf("Error: send END error in sendChunkData\n");
            		return; // terminate the function
		};		
	}

	return;
}



// hash function to divide the list of word.txt files across reducers
//http://www.cse.yorku.ca/~oz/hash.html
int hashFunction(char* key, int reducers){
	unsigned long hash = 0;
    int c;

    while ((c = *key++)!='\0')
        hash = c + (hash << 6) + (hash << 16) - hash;

    return (hash % reducers);
}



int getInterData(char *key, int reducerID) {
	key_t key1;
	int msgid;
	key1 = ftok(".", 5598966); 						// generate a specific key id according to the parameters given
    	// printf("Key value: %d\n", key1); 					// print out the key value so that we can test
	msgid = msgget(key1, 0666 | IPC_CREAT);
	if (msgid == -1){ // check whether the error happens when get message queue
		printf("Error: message queue create error");
        	return 0; 
	}
    	/*
	else
		printf("get Inter Data msgid is %d\n", msgid); 			// print out the msgid and the corrent mapperID so that we can test
	
     	*/
	struct msgBuffer msg;
    	if((msgrcv(msgid, &msg, sizeof(msg.msgText), reducerID, 0)) < 0) { 	// check whether the error happens when receive message
		printf("Error: message received error");
        	return 0; 
    	}
	
	strcpy(key, msg.msgText); 						// copy the msgText to str
    	if(strcmp(key, "END") == 0) { 						// if receive the END message
        	strcpy(key, "\0"); 						// set key to empty
		return 0;
    	}
    	else
		return 1;
    
	
}


void shuffle(int nMappers, int nReducers) {
	key_t key;
	int msgid;
	key = ftok(".", 5598966); 			// generate a specific key id according to the parameters given
    	// printf("Key value: %d\n", key); 		// print out the key value so that we can test
	msgid = msgget(key, 0666 | IPC_CREAT);
	if (msgid == -1){ 				// check whether the error happens when get message queue
		printf("Error: message queue create error\n");
        	return; // terminate the function
	}
    	/*
	else
		printf("shuffle msgid is %d\n", msgid); // print out the msgid and the corrent mapperID so that we can test
     	*/
    
	struct msgBuffer msg;
	char work_dir[50] = {'\0'};
	getcwd(work_dir, sizeof(work_dir));
	strcat(work_dir, "/output/MapOut");
	DIR* dir;
	DIR* dir2;
	struct dirent* dir_entry;
	struct dirent* dir_entry2;
	char num[5];
	int reducerID;
	char subdir[200];
	char subdir2[200];
	for(int i=1; i<=nMappers; i++){
		dir = opendir(work_dir); 					// open "./output/MapOut"
		if(dir == NULL){ 						// check whether the directory is opened
			printf("Error: directory open error of %s\n", work_dir);
            		return; 
		}
   		while((dir_entry = readdir(dir)) != NULL){
			if (strcmp(dir_entry->d_name, ".") == 0 ||strcmp(dir_entry->d_name, "..") == 0) 
            			continue;
			memset(num, '\0', 5); 
			sprintf(num, "%d", i); 
			memset(subdir, '\0', 200);
			strcat(subdir, "Map_");
			strcat(subdir, num); 					// subbir = Map_i
			if (strcmp(dir_entry->d_name, subdir) == 0){			
				memset(subdir2, '\0', 200);
				getcwd(subdir2, sizeof(subdir2));
				strcat(subdir2,"/output/MapOut/Map_");
				strcat(subdir2, num); 				// subdir2 is /output/MapOut/Map_i
				dir2 = opendir(subdir2); 			
				if(dir2 == NULL){ 				// check whether the directory is opened
					printf("Error: directory open error of %s\n", subdir2);
                    			return; 
				}
				while((dir_entry2 = readdir(dir2))!=NULL){ 				//open one Map in MapOut
                    			if (strcmp(dir_entry2->d_name, ".") == 0 ||strcmp(dir_entry2->d_name, "..") == 0)
                        			continue;
                    			char path[500];
					sprintf(path, "%s/%s", subdir2, dir_entry2->d_name); 		// generatete the path of wordName.txt files
					reducerID = hashFunction(dir_entry2->d_name, nReducers); 	//using hash function to return reducerID
                    			memset(msg.msgText, '\0', MSGSIZE); 				// initialize msg.msgText
                    			strcpy(msg.msgText, path); 
                    			msg.msgType = reducerID + 1; 
					if(msgsnd(msgid, &msg, sizeof(msg.msgText), 0) < 0){ 	// check whether the error happens when send message
						printf("Error: send %s error\n", path);
                        			return;
					}
                    
				}
                		if (closedir(dir2) == -1) { 					// check whether there's error when close the directory
                    			printf("Error: %s closed error\n", subdir2);
                    			return;
                		};
			}
		}//end one Map in MapOut
        	if (closedir(dir) == -1) { 							// check whether there's error when close the directory
            		printf("Error: %s closed error\n", work_dir);
            		return; 
        	};
	}//end loop

	for(int i=1; i<=nReducers; i++){
		msg.msgType = i;		
		memset(msg.msgText, '\0', 1024);//change 1024
		strcpy(msg.msgText, "END");
		if(msgsnd(msgid, &msg, sizeof(msg.msgText), 0) < 0){ 				// check whether the error happens when send message
			printf("Error: send END error in Shuffle\n");
            		return; // terminate the function
		};		
	}

	return;
	
}

// check if the character is valid for a word
int validChar(char c){
	return ((tolower(c) >= 'a') && tolower((c) <='z')) ||
					(c >= '0' && c <= '9');
}

char *getWord(char *chunk, int *i){
	char *buffer = (char *)malloc(sizeof(char) * chunkSize);
	memset(buffer, '\0', chunkSize);
	int j = 0;
	while((*i) < strlen(chunk)) {
		// read a single word at a time from chunk
		// printf("%d\n", i);
		if (chunk[(*i)] == '\n' || chunk[(*i)] == ' ' || !validChar(chunk[(*i)]) || chunk[(*i)] == 0x0) {
			buffer[j] = '\0';
			if(strlen(buffer) > 0){
				(*i)++;
				return buffer;
			}
			j = 0;
			(*i)++;
			continue;
		}
		buffer[j] = chunk[(*i)];
		j++;
		(*i)++;
	}
	return NULL;
}

void createOutputDir(){
	mkdir("output", ACCESSPERMS);
	mkdir("output/MapOut", ACCESSPERMS);
	mkdir("output/ReduceOut", ACCESSPERMS);
}

char *createMapDir(int mapperID){
	char *dirName = (char *) malloc(sizeof(char) * 100);
	memset(dirName, '\0', 100);
	sprintf(dirName, "output/MapOut/Map_%d", mapperID);
	mkdir(dirName, ACCESSPERMS);
	return dirName;
}

void removeOutputDir(){
	pid_t pid = fork();
	if(pid == 0){
		char *argv[] = {"rm", "-rf", "output", NULL};
		if (execvp(*argv, argv) < 0) {
			printf("ERROR: exec failed\n");
			exit(1);
		}
		exit(0);
	} else{
		wait(NULL);
	}
}

void bookeepingCode(){
	removeOutputDir();
	sleep(1);
	createOutputDir();
}
