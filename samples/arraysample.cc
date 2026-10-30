#include "common/system.h"
#include "common/array.h"
#include <cstdio>

int main(int argc, char** argv)
{
    SystemSetup();

    ArrayCreateInfo arrayInfo;
    arrayInfo.itemSize = sizeof(int);
    arrayInfo.maxNumItems = 1024;
    Array array = CreateArray(arrayInfo);

    // Push some items
    for (int i = 0; i < 10; i++) {
        ArrayPush(&array, i);
    }
    
    // Get items
    for (int i = 0; i < array.size; i++) {
        int* item = ArrayGetPtr<int>(&array, i);
        printf("%d\n", *item);
    }

    // Destroy array
    DestroyArray(&array);
}
