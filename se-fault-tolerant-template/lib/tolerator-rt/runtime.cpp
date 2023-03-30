
#include <cstdint>
#include <cstdio>
#include <stdlib.h>
#include <map>


extern "C" {


// This macro allows us to prefix strings so that they are less likely to
// conflict with existing symbol names in the examined programs.
// e.g. TOLERATE(entry) yields ToLeRaToR_entry
#define TOLERATE(X) ToLeRaToR_##X


std::map<void*, size_t> memoryRegions;

void* 
TOLERATE(safeMalloc)(size_t size, int analysisType) {
    void* ptr = std::malloc(size);
    if (ptr) {
      memoryRegions[ptr] = size;
    } else {
      // Invalid case
      if (analysisType >= 1) {
        fprintf(stderr, "FOUND: Invalid write to memory\n");
      }
      if (analysisType == 2) {
        return ptr;
      }
      if (analysisType == 3) {
        return 0;
      }
      exit(-1);
    }
    return ptr;
}

void 
TOLERATE(safeFree)(void* ptr, int analysisType) {
    auto it = memoryRegions.find(ptr);
    if (it != memoryRegions.end()) {
        memoryRegions.erase(it);
        std::free(ptr);
    } else {
        if (analysisType >= 1) {
          fprintf(stderr, "FOUND: Invalid free of memory\n");
        }
        if (analysisType >= 2) {
          return;
        }
        exit(-1);
    }
}

int
TOLERATE(divisionWithZero)(int a, int b, int analysisType) {
  if (b == 0) {
    if (analysisType >= 1) {
      fprintf(stderr, "FOUND: Division by zero\n");
    }
    if (analysisType == 3) {
      return 0;
    }
    exit(-1);
  }
  return a/b;
}
}
