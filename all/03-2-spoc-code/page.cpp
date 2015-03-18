#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;
typedef unsigned int UINT;
typedef unsigned char BYTE;
BYTE memory[256][32];
void readmemory() {
    string s1, s2;
    for (int i=0; i<0x80; i++) {
        cin >> s1 >> s2;
        for (int j=0; j<32; j++) {
            UINT temp;
            cin >> hex >> temp;
            memory[i][j] = BYTE(temp);
        }
    }
}
UINT PDBR = 0x220;
BYTE getmemory(UINT addr) {
    UINT page = (addr & 0x00000fe0) >> 5;
    UINT pos = (addr & 0x0000001f);
    return memory[page][pos];
}
void transfer(UINT va) {
    UINT pd = (va & 0x00007c00) >> 10;
    BYTE pde = getmemory(PDBR+pd);
    UINT pde_v = pde >> 7;
    UINT pde_c = pde & 0x0000007f;
    cout << "Virtual Address 0x" << hex << setw(4) << va << ":" << endl;
    cout << "  --> pde index:0x" << hex << setw(2) << setfill('0') << pd;
    cout << "  pde contents:(valid " << pde_v << ", pfn 0x" << 
            hex << setw(2) << setfill('0') << pde_c << ")" << endl;
    if (pde_v == 0) {
        cout << "      --> Fault (page directory entry not valid)" << endl;
        return;
    }
    UINT pte_index = (va & 0x000003e0) >> 5;
    UINT pte = memory[pde_c][pte_index];
    UINT pte_v = pte >> 7;
    UINT pte_c = pte & 0x0000007f;
    cout << "    --> pte index:0x" << hex << setw(2) << setfill('0') << pte_index;
    cout << " pte contents:(valid " << pte_v << ", pfn 0x" << hex << setw(2) << setfill('0') << pte_c << ")" << endl;
    if (pte_v == 0) {
        cout << "        --> Fault (page table entry not valid)" << endl;
        return;
    }
    UINT page_offset = (va & 0x0000001f);
    UINT phyaddr = (pte_c << 5 )+ page_offset;
    cout << "       -->Translate to Physical Address 0x" << hex << phyaddr;
    cout << " --> Value:0x" << hex << setw(2) << (UINT)getmemory(phyaddr) << endl;
}
int main() {
    readmemory();
    string s1, s2;
    UINT vaddr;
    while (cin >> s1 >> s2 >> hex >> vaddr) {
        transfer(vaddr);
        cout << endl;
    }
    return 0;
}
