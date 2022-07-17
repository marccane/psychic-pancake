#include <stdio.h>

void intermediate(void (*func)(int, int)){
	func(1,1337);
}

void normalFn(int *array, int size, int offset)
{
	void nestedStoreFn(int base, int value)
	{
		array[base + offset] = value;
		printf("offset=%d\n", offset);
	}

	intermediate(nestedStoreFn);

	offset=3;
	intermediate(nestedStoreFn);
}

int main(){
	int arr[10];
	normalFn(arr, 10, 2);
	printf("%d\n",arr[3]);
}
