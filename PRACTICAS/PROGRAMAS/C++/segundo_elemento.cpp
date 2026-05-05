#include <iostream>

using namespace std;

int segundo(int arr[], int n)
{
    int may, seg;
    if (arr[0] > arr[1]) {
        may = arr[0];
        seg = arr[1];
    } else {
        may = arr[1];
        seg = arr[0];
    }

    for (int i = 2; i < n; i++) {
        if (arr[i] > may) {
            seg = may;
            may = arr[i];
        } else if (arr[i] > seg) {
            seg = arr[i];
        }
    }

    return seg;
}

int main()
{
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    cout << "El segundo elemento mas grande es: " << segundo(arr, n) << endl;
    return 0;
}