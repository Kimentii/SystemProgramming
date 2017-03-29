struct Stack {
	#if defined(_WIN32) || defined(_WIN64)
		HANDLE hand;
	#else
		pthread_t pThread;
	#endif
		struct Stack *next;
};

void push(struct Stack **, struct Data);
void pop(struct Stack **);
