#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Yardımcı: Diziyi yazdır
void printArray(const vector<int>& arr) {
    for (int x : arr) cout << setw(3) << x << " ";
    cout << endl;
}

// --- QuickSort -------------------------------------------------
void quickSort(vector<int>& arr, int l, int r, int depth = 0) {
    if (l >= r) return;
    int pivot = arr[r];
    int i = l;
    cout << string(depth*2, ' ') << "QuickSort(" << l << "," << r << ") pivot=" << pivot << endl;

    for (int j = l; j < r; j++) {
        cout << string(depth*2, ' ') << "  Karşılaştır: arr[" << j << "]=" << arr[j] 
             << " < " << pivot << " ?" << endl;
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            cout << string(depth*2, ' ') << "  Swap -> "; 
            printArray(arr);
            i++;
        }
    }
    swap(arr[i], arr[r]);
    cout << string(depth*2, ' ') << "Pivot yerine kondu -> "; 
    printArray(arr);

    quickSort(arr, l, i - 1, depth + 1);
    quickSort(arr, i + 1, r, depth + 1);
}

// --- MergeSort -------------------------------------------------
void merge(vector<int>& arr, int l, int m, int r, int depth) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    cout << string(depth*2, ' ') << "Merge(" << l << "," << r << ")" << endl;
    while (i < (int)left.size() && j < (int)right.size()) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
        printArray(arr);
    }
    while (i < (int)left.size()) arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
    cout << string(depth*2, ' ') << "Merge Sonrası -> "; 
    printArray(arr);
}

void mergeSort(vector<int>& arr, int l, int r, int depth = 0) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(arr, l, m, depth + 1);
    mergeSort(arr, m + 1, r, depth + 1);
    merge(arr, l, m, r, depth);
}

// --- Linear Search ---------------------------------------------
int linearSearch(const vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        cout << "Kontrol: arr[" << i << "]=" << arr[i] << endl;
        if (arr[i] == target) {
            cout << "Bulundu! Index = " << i << endl;
            return i;
        }
    }
    cout << "Bulunamadı!" << endl;
    return -1;
}

// --- Binary Search ---------------------------------------------
int binarySearch(const vector<int>& arr, int target) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        cout << "Kontrol: arr[" << m << "]=" << arr[m] << endl;
        if (arr[m] == target) {
            cout << "Bulundu! Index = " << m << endl;
            return m;
        }
        if (arr[m] < target) l = m + 1;
        else r = m - 1;
    }
    cout << "Bulunamadı!" << endl;
    return -1;
}

// --- MAIN ------------------------------------------------------
int main() {
    vector<int> arr = {34, 7, 23, 32, 5, 62};

    cout << "Orijinal Dizi: ";
    printArray(arr);

    cout << "\n--- QuickSort Adımları ---\n";
    vector<int> arrQuick = arr;
    quickSort(arrQuick, 0, arrQuick.size() - 1);

    cout << "\n--- MergeSort Adımları ---\n";
    vector<int> arrMerge = arr;
    mergeSort(arrMerge, 0, arrMerge.size() - 1);

    cout << "\n--- Linear Search ---\n";
    linearSearch(arr, 23);

    cout << "\n--- Binary Search (önce MergeSort sonrası dizi) ---\n";
    binarySearch(arrMerge, 23);

    return 0;
}