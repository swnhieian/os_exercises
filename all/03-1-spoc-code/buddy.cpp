#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
using namespace std;
const int total_size = 1024;
bool is2power(int n) {
    if (n < 0) return false;
    return ((n & (n - 1))== 0);
}
int justify(int size) {
    if (is2power(size)) return size;
    int n = 0;
    int temp = size;
    while (temp>0) {
        n ++;
        temp >>= 1;
    }
    return (1 << n);
}
class pmm_manager {
    private:
        int size;
        int* longest;
        bool* flag;
        void setflag(int index, int size);
    public:
        pmm_manager(int size);
        ~pmm_manager();
        int malloc(int size);
        void free(int address);
        void showmem();
};
void pmm_manager::setflag(int index, int size) {
    if (longest[index] == 0) {
        int start = size*index-this->size;
        for (int i = 0; i<size; i++)
            flag[start+i] = true;
        return;
    }
    if (longest[index] == size) {
        int start = size*index-this->size;
        for (int i = 0; i<size; i++)
            flag[start+i] = false;
        return;
    }
    setflag(index << 1, size>>1);
    setflag((index << 1)+1, size >>1);
    return;
}
pmm_manager::pmm_manager(int size = total_size) {
    if ((size <=0) || (!is2power(size))) {
        cout << "Initialize failed!" << endl;
        return;
    }
    this->size = size;
    longest = new int[2*size];
    flag = new bool[size];
    int temp = size << 1;
    for (int i=1; i<size*2; i++) {
        if (is2power(i)) {
            temp >>= 1;
        }
        longest[i] = temp;
    }
    cout << "Initialize success!" << endl;
}

int pmm_manager::malloc(int size) {
    if (size <= 0 || size > this->size) {
        cout << "Allocate failed! Invalid size!" << endl;
        return -1;
    }
    int realsize = justify(size);
    int tempsize;
    int index = 1;
    if (longest[index] < realsize) {
         cout << "Allocate failed! no more free space!" << endl;
         return -1;
    }
    for (tempsize=this->size;tempsize!=realsize;tempsize>>=1) {
        if (longest[index << 1] >= realsize)
            index = index << 1;
        else if (longest[(index << 1)+1] >= realsize)
            index = (index << 1) + 1;
        else return -1;
    }
    int start_addr = index * longest[index] - this->size;
    longest[index] = 0;
    for (index = index >> 1;index > 0;index >>=1) {
        longest[index] = max(longest[index<<1],longest[(index<<1)+1]);
    }
    return start_addr;
}

void pmm_manager::free(int address) {
    if ((address < 0) || (address >= this->size)) {
        cout << "Free failed! Invalid address!" << endl;
        return;
    }
    int index = this->size + address;
    int tempsize = 1;
    while (longest[index]>0) {
        index = index >> 1;
        tempsize <<= 1;
        if (index == 0) {
            cout << "Free failed! Invalied address!" << endl;
            return;
        }
    }
    longest[index] = tempsize;
    for (index = index >> 1; index > 0;index >>= 1) {
        tempsize <<= 1;
        if (longest[index<<1] + longest[(index<<1)+1] == tempsize)
            longest[index] = tempsize;
        else
            longest[index] = max(longest[index<<1],longest[(index<<1)+1]);
    }
    cout << "Free success!" << endl;
    return;
}

void pmm_manager::showmem() {
    cout << "==========Memory Detail==========" << endl;
    if (this->size < 80) {
        for (int i=0; i<this->size; i++) 
            cout << setw(3) << i;
        cout << endl;
    }
    setflag(1, this->size);
    for (int i=0; i<this->size; i++) {
        if (flag[i]) cout << setw(3) << '*';
        else cout << setw(3) << '_';
    }
    cout << endl;
    return;
}
pmm_manager::~pmm_manager() {
    delete[] flag;
    delete[] longest;
}

int main() {
    pmm_manager buddy(16);
    int pos;
    char ch;
    int temp;
    while (true) {
        cout << ">> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit" << endl;
        cout << ">>";
        cin >> ch;
        if (ch == 'm') {
            cin >> temp;
            if ((pos = buddy.malloc(temp))>=0) {
                cout << "Allocate address:" << pos << endl;
            }
            buddy.showmem();
        } else if (ch == 'f') {
            cin >> temp;
            buddy.free(temp);
            buddy.showmem();
        } else if (ch == 'q') {
            break;
        }
    }
    return 0;
}

