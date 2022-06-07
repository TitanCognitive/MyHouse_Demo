/* 
   Copyright (c) 2020 Titan Virtual Corp. All rights reserved.

   http://www.titanvx.com
   Support: support@titanvx.com
   
   The source code and/or documentation ("Licensed Deliverables") are 
   subject to Titan Virtual Corp. intellectual property rights under U.S. and 
   international Copyright laws. 

   The Licensed Deliverables contained herein are PROPRIETARY and
   CONFIDENTIAL to Titan Virtual Corp. and are being provided under the terms and
   conditions of a form of Titan Virtual Corp. software license agreement by and
   between Titan Virtual Corp. and Licensee ("License Agreement") or electronically
   accepted by Licensee. Notwithstanding any terms or conditions to the contrary 
   in the License Agreement, reproduction or disclosure of the Licensed Deliverables 
   to any third party without the express written consent of Titan Virtual Corp. is prohibited.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
   USE OR OTHER DEALINGS IN THE SOFTWARE.   
   
   Any use of the Licensed Deliverables in individual and commercial software must include, 
   in the user documentation and internal comments to the code, the above Disclaimer 
   and U.S. Government End Users Notice.
*/

#ifndef TITAN_CORE_MEMORY_API_H
#define TITAN_CORE_MEMORY_API_H

   // Titan memory interface definition
   struct _TitanMemoryAPI
      {
      // Allocate memory block of <block_size> bytes and store pointer into <*ptr>
      virtual bool AllocateMemory(void **ptr, TITAN_SIZE64 block_size) = 0;
      // Allocate memory block of <block_size> bytes aligned to a <align_size> fence and store pointer into <*ptr>
      virtual bool AllocateMemoryAligned(void **ptr, TITAN_SIZE64 block_size, TITAN_SIZE64 align_size) = 0;
      // Free supplied memory block and allocate a new memory block of <block_size> bytes
      virtual bool RecycleMemory(void **ptr, TITAN_SIZE64 block_size) = 0;
      // Allocate memory block of <block_size> bytes, cleared to zero, and store pointer into <*ptr>
      virtual bool AllocateAndFlushMemory(void **ptr, TITAN_SIZE64 block_size) = 0;
      // Free memory block and allocate a new memory block of <block_size> bytes and cleared them zero
      virtual bool RecycleAndFlushMemory(void **ptr, TITAN_SIZE64 block_size) = 0;
      // Allocate memory block of <block_size> bytes, store pointer into <*ptr>, and copy as many as <copy_size> bytes from block <src_ptr>
      virtual bool AllocateAndCopyMemory(void **ptr, TITAN_SIZE64 block_size, void *src_ptr, TITAN_SIZE64 copy_size = 0) = 0;
      // Free supplied pointer memory block, allocate memory block of <block_size> bytes, store pointer into <*ptr>, and copy as many as <copy_size> bytes from block <src_ptr>
      virtual bool RecycleAndCopyMemory(void **ptr, TITAN_SIZE64 block_size, void *src_ptr, TITAN_SIZE64 copy_size = 0) = 0;
      // Allocate memory block of <block_size> bytes and copy as many as <copy_size> bytes from block <src_ptr>
      virtual bool AllocateAndCopyMemory(void **ptr, const TITAN_POINTER src_ptr, TITAN_SIZE64 block_size) = 0;
      // Resize supplied memory block from <old_size> to <new_size>, and copy as many as <copy_size> bytes from supplied block
      virtual bool ResizeMemory(void **ptr, TITAN_SIZE64 old_size, TITAN_SIZE64 new_size, TITAN_SIZE64 copy_size = 0) = 0;
      // Free supplied memory block
      virtual bool FreeMemory(void **ptr) = 0;
      };

   // Memory macros
   #define _TA(X)                                     (TITAN_POINTER *) &X
   #define TITAN_ALLOCATE_COPY_RAW(X, Y, Z)           Memory->AllocateAndCopyMemory((TITAN_POINTER *) &X, (Y), (Z))
   #define TITAN_ALLOCATE_COPY_ARRAY(X, Y, Z, W)      Memory->AllocateAndCopyMemory((TITAN_POINTER *) &X, (Y), sizeof(Z) * (W))
   #define TITAN_ALLOCATE_FLUSH(X, Y)                 Memory->AllocateAndFlushMemory((TITAN_POINTER *) &X, sizeof(Y))
   #define TITAN_ALLOCATE_FLUSH_ARRAY(X, Y, Z)        Memory->AllocateAndFlushMemory((TITAN_POINTER *) &X, sizeof(Y) * (Z))
   #define TITAN_ALLOCATE_FLUSH_RAW(X, Y)             Memory->AllocateAndFlushMemory((TITAN_POINTER *) &X, (Y))
   #define TITAN_ALLOCATE(X, Y)                       Memory->AllocateMemory((TITAN_POINTER *) &X, sizeof(Y))
   #define TITAN_ALLOCATE_ALIGNED(X, Y, Z)            Memory->AllocateMemoryAligned((TITAN_POINTER *) &X, sizeof(Y), (Z))
   #define TITAN_ALLOCATE_ALIGNED_ARRAY(X, Y, Z, W)   Memory->AllocateMemoryAligned((TITAN_POINTER *) &X, sizeof(Y) * (Z), (W))
   #define TITAN_ALLOCATE_ALIGNED_RAW(X, Y, Z)        Memory->AllocateMemoryAligned((TITAN_POINTER *) &X, (Y), (Z))
   #define TITAN_ALLOCATE_ARRAY(X, Y, Z)              Memory->AllocateMemory((TITAN_POINTER *) &X, sizeof(Y) * (Z))
   #define TITAN_ALLOCATE_RAW(X, Y)                   Memory->AllocateMemory((TITAN_POINTER *) &X, (Y))
   #define TITAN_FREE(X)                              Memory->FreeMemory((TITAN_POINTER *) &(X))
   #define TITAN_CLEAR_RAW(X, Y)                      memset(X, 0, Y)
   #define TITAN_CLEAR(X, Y)                          memset(X, 0, sizeof(Y))
   #define TITAN_CLEAR_ARRAY(X, Y, Z)                 memset(X, 0, sizeof(Y) * (Z))
   #define TITAN_COPY(X, Y, Z)                        memcpy(X, Y, sizeof(Z))
   #define TITAN_COPY_ARRAY(X, Y, Z, W)               memcpy(X, Y, sizeof(Z) * (W))

   // Formal API declaration
   typedef _TitanMemoryAPI *TitanMemoryAPI;

#endif

/*
   End of file
*/
