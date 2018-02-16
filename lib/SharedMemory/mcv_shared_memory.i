%module mcv_shared_memory
%{
#include "shared_memory.h"
%}

%inline %{
typedef void * ISharedMemory;
%}

typedef void * ISharedMemory;
%include "shared_memory.h"



