void normalFn (int *array, int size)
{
  void nestedStoreFn (int base, int value)
    { array[base] = value; }

  nestedStoreFn(5,1337);
}

int main(){
    int arr[10];
    normalFn(arr, 10);
}
