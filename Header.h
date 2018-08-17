typedef struct freeNode{

	size_t offset;
	size_t size;
	freeNode* next;

}freeNode;

typedef struct block{

	size_t free;
	void* ptr;
	freeNode* freeList;
	struct block* next;
}block;

block* head;

freeNode* createNode(size_t offset, size_t size){

	freeNode* freenode = (freeNode*)malloc(sizeof(freeNode));
	freenode->next = NULL;
	freenode->offset = offset;
	freenode->size = size;

	return freenode;
}

block* createBlock(){

	block* newBlock = (block*)malloc(sizeof(block));
	newBlock->free = 1024;
	newBlock->freeList = createNode(0,1024);
	newBlock->next = NULL;
	newBlock->ptr = malloc(1024);

	return newBlock;

}
void addToList(block* b, size_t append, size_t size){

	freeNode* newNode = createNode(append, size);

	freeNode* temp = b->freeList;

	if (b->freeList == NULL || ((b->freeList->size + b->freeList->offset) > (append))){

		newNode->next = temp;
		b->freeList = newNode;
	}

	
	else{

		while (temp->next != NULL &&
			(temp->next->offset+temp->next->size) < (append)){
			temp = temp->next;
		}
		newNode->next = temp->next;
		temp->next = newNode;
	}
}

void mergeList(block* b){


	freeNode* temp = b->freeList;

	if (temp == NULL || temp->next == NULL){

		return;

	}

	while (temp->next){

		if ((temp->offset + temp->size) == temp->next->offset){

			temp->offset = temp->offset + temp->next->size;

			temp->next = temp->next->next;
		}

		else
			temp = temp->next;
	}

}

void* getPointer(block* tempHead, freeNode* node,int size){

	while (node){

		if (node->size >= size + sizeof(size_t)){
			size_t temp = node->offset;
			*(size_t*)(tempHead->ptr) = size;
			node->size -= size + sizeof(size_t);
			node->offset += size + sizeof(size_t);
			void *ptr = (void *)((char *)tempHead->ptr + temp + sizeof(size_t));
			return ptr;
		}
		node = node->next;

	}
	return NULL;

}
void* bs_malloc(size_t size){

	int count = 0;
	block* tempHead;
	block* prev;
	
	if (head == NULL)
		head = createBlock();
	
	tempHead = head;
	prev = NULL;

	if (tempHead->next == NULL){

		freeNode* f = tempHead->freeList;
		
		void* n = getPointer(tempHead, tempHead->freeList, size);
		
		if (n == NULL){

			tempHead->next = createBlock();

			block* newNode = tempHead->next;
			freeNode* freeNode = newNode->freeList;
			size_t temp = freeNode->offset;
			*(size_t*)(newNode->ptr) = size;

			freeNode->size -= (size + sizeof(size_t));
			freeNode->offset += (size + sizeof(size_t));
			return  (void *)((char *)newNode->ptr + temp + sizeof(size_t));
			
		}
		return n;
		
	}

	else{
		
		while (tempHead->next != NULL){

			count++;
			freeNode* f = tempHead->freeList;

			void* n = getPointer(tempHead, tempHead->freeList, size);
			
			if (n != NULL)
				return n;
			tempHead = tempHead->next;
		}

		if (count < 12){

			tempHead->next = createBlock();
			
			block* newNode = tempHead->next;

			freeNode* freeNode = newNode->freeList;
			size_t temp = freeNode->offset;
			*(size_t*)(tempHead->ptr) = size;
			
			freeNode->size -= (size + sizeof(size_t));
			freeNode->offset += (size + sizeof(size_t));
			return (void *)((char *)newNode->ptr + temp + sizeof(size_t));
			
		}
	}
	return NULL;
}

void bs_free(void* ptr){

	void* tempPtr = ptr;
	block* tempHead = head;

	while (tempHead){

		tempPtr = (void *)((char*)ptr - 4);
		size_t val;

		val = *(size_t*)(tempPtr);
		void* tempPtr2 = (void*)((char*)tempHead->ptr + 1024);

		if (tempHead->ptr <=ptr && ptr <= tempPtr2){
	
			size_t diff = ((char *)tempPtr - (char *)tempHead->ptr);
			addToList(tempHead, diff,val + sizeof(size_t));
		}

		tempHead = tempHead->next;
	}
	return;
}





