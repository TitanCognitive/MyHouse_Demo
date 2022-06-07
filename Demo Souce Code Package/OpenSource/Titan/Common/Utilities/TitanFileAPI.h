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

#ifndef TITAN_CORE_FILE_API_H
#define TITAN_CORE_FILE_API_H

   // Atlas file pointer seek parameters
   enum titan_file_seek_t
      {
      TITAN_FILE_SET_START = 1,                       // New pointer index is relative to the beginning of the file
      TITAN_FILE_SET_CUR = 2,                         // New pointer index is relative to current pointer index
      TITAN_FILE_SET_END = 3,                         // New pointer index is relative to the end of file
      };

   // Atlas file interface
   struct _TitanFileAPI
      {
      // 
      virtual TITAN_HANDLE64 GetVersion() = 0;
      virtual titan_result_t OpenRead(const TITAN_STRING path, TITAN_HANDLE64 &file) = 0;
      virtual titan_result_t OpenWrite(const TITAN_STRING path, TITAN_HANDLE64 &file) = 0;
      virtual titan_result_t Close(TITAN_HANDLE64 file) = 0;
      virtual titan_result_t Flush(TITAN_HANDLE64 file) = 0;
      virtual titan_result_t Read(TITAN_HANDLE64 file, TITAN_POINTER data, TITAN_SIZE64 unit_size, TITAN_ULONG num_units, TITAN_ULONG &num_units_read) = 0;
      virtual titan_result_t Write(TITAN_HANDLE64 file, TITAN_POINTER data, TITAN_SIZE64 unit_size, TITAN_ULONG num_units, TITAN_ULONG &num_units_written) = 0;
      virtual TITAN_SIZE64 _GetSize(const TITAN_STRING path) = 0;
      virtual TITAN_SIZE64 GetSize(TITAN_HANDLE64 file) = 0;
      virtual TITAN_SIZE64 GetPointerIndex(TITAN_HANDLE64 file) = 0;
      virtual titan_result_t  SetPointerIndex(TITAN_HANDLE64 file, TITAN_SIZE64 offset, titan_file_seek_t set_param = TITAN_FILE_SET_START) = 0;
      };

   // Formal API declaration
   typedef _TitanFileAPI *TitanFileAPI;

#endif

/*
   End of file
*/
