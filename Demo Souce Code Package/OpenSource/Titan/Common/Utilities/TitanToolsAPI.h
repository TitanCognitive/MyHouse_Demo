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

#ifndef TITAN_CORE_TOOLS_API_H
#define TITAN_CORE_TOOLS_API_H

   // System date information
   struct titan_system_date_t
      {
      TITAN_UINT                                      year;                                  // System year
      TITAN_UINT                                      month;                                 // System month of the year
      TITAN_UINT                                      week_day;                              // System day of the week
      TITAN_UINT                                      day;                                   // System day of the month
      TITAN_UINT                                      hour;                                  // System hour of the day
      TITAN_UINT                                      minute;                                // System minute of the hour
      TITAN_UINT                                      second;                                // System second of the minute
      TITAN_UINT                                      milli_second;                          // System millisecond
      };  

   // Atlas tools interface definition
   struct _TitanToolsAPI
      {
      // Make copy of supplied string and return pointer.
      virtual TITAN_STRING CopyText(const TITAN_STRING text) = 0;
      // Make copy of supplied string up to <length> bytes and return pointer.
      virtual TITAN_STRING CopyText(const TITAN_STRING text, TITAN_SIZE32 length) = 0;
      // Make copy of supplied data block up to <length> bytes and return pointer.
      virtual TITAN_POINTER CopyData(const TITAN_POINTER text, TITAN_SIZE32 length) = 0;
      // Compare supplied strings and return a case-insensitive ranking similar to strcmp results.
      virtual TITAN_INT CompareText(const TITAN_STRING str1, const TITAN_STRING str2) = 0;
      // Format (sprintf) text into a temporary string of 512 bytes. String will be overwritten by subsequent call to FormatText.
      virtual TITAN_STRING FormatText(TITAN_STRING fmt, ...) = 0;
      // Tokenize input text <text> into supplied token array <tokens> with a maximum number of tokens, <max_tokens>, to be populated.
      virtual TITAN_UINT TokenizeText(TITAN_STRING text, TITAN_STRING *tokens, TITAN_UINT max_tokens = 0) = 0;
      // Convert supplied text string to a file-system compatible text string.
      virtual TITAN_UINT ToFilename(const TITAN_STRING text, TITAN_STRING output_filename, TITAN_UINT max_filename_size = 0) = 0;
      // Report activity to system log.
      virtual bool Report(const TITAN_STRING name, titan_result_t result, const TITAN_STRING activity, const TITAN_STRING extra) = 0;
      // Copy current system date to supplied structure <system_time> (See titan_system_date_t).
      virtual void GetSystemDate(titan_system_date_t *system_time) = 0;
      // Get system 64-bit time since Atlas started, in milliseconds.
      virtual TITAN_DOUBLE GetTime() = 0;
      // Yield processing time to other threads in system.
      virtual void YieldThread() = 0;
      };

   // Formal API declaration
   typedef _TitanToolsAPI *TitanToolsAPI;

#endif

/*
   End of file
*/
