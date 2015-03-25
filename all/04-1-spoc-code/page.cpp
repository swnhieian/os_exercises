#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
using namespace std;
typedef unsigned int UINT;
typedef unsigned char BYTE;
BYTE page[256][32];
BYTE disk[256][32];
UINT pdbr;
BYTE getmemory(UINT addr) {
    UINT inx = (addr & 0x00000fe0) >> 5;
    UINT pos = (addr & 0x0000001f);
    return page[inx][pos];
}

void transfer(UINT va) {
    UINT pd = (va & 0x00007c00) >> 10;
    BYTE pde = getmemory(pdbr+pd);
    UINT pde_v = pde >> 7;
    UINT pde_c = pde & 0x0000007f;
    printf("Virtual Address 0x%04x:\n", va);
    printf("  --> pde index:0x%x ", pd);
    printf("pde contents:(valid %d, pfn 0x%02x)\n", pde_v, pde_c);
    if (pde_v == 0) {
        cout << "      --> Fault (page directory entry not valid)" << endl;
        return;
    }    
    UINT pte_index = (va & 0x000003e0) >> 5;
    UINT pte = page[pde_c][pte_index];
    UINT pte_v = pte >> 7;
    UINT pte_c = pte & 0x0000007f;
    printf("    --> pte index:0x%02x", pte_index);
    printf(" pte contents:(valid %d, pfn 0x%02x)\n", pte_v, pte_c);
    UINT page_offset = (va & 0x0000001f);
    UINT phyaddr = (pte_c << 5 )+ page_offset;
    if (pte_v == 0) {//to disk address
        UINT value = disk[pte_c][page_offset];
        printf("      --> To Disk Sector Address 0x%x -->Value: 0x%02x\n", phyaddr, value);
        return;
    } else {//to physical address
        UINT value = getmemory(phyaddr);
        printf("      --> To Physical Address 0x%x -->Value: 0x%02x\n", phyaddr, value);
    }
}
void readmemory() {
    UINT s;
    for (int i=0; i<0x80; i++) {
        scanf("page %x: ", &s);
        for (int j=0; j<32; j++) {
            UINT temp;
            scanf("%x ", &temp);
            page[i][j] = BYTE(temp);
        }
    }
    for (int i=0; i<0x80; i++) {
        scanf("disk %x: ", &s);
        for (int j=0; j<32; j++) {
            UINT temp;
            scanf("%x ", &temp);
            disk[i][j] = BYTE(temp);
        }
    }
    scanf("PDBR content: 0x%x\n", &pdbr);
    UINT va;
    while (scanf("Virtual Address %x:\n", &va)!=EOF) {
        transfer(va);
        printf("\n");
    }
}

int main() {
    readmemory();
    return 0;
}
