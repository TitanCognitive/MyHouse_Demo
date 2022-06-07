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

#ifndef TITAN_ATLAS_DEFINITIONS_H
#define TITAN_ATLAS_DEFINITIONS_H

   // Forward declarations
   struct AtlasAxiomInterface;
        
   // OS Dependencies
   #ifdef WIN32
      // DLL Axiom export 
      extern "C" __declspec(dllexport) AtlasAxiomInterface *CreateAxiom(const TITAN_STRING codex_name, const TITAN_STRING axiom_name, TITAN_HANDLE64 axiom_handle);
   #endif

   // Atlas simulation method perspective types
   #define ATLAS_PERSPECTIVE_SUBJECT                  0                                      // Perspective of concept is Subject
   #define ATLAS_PERSPECTIVE_OBJECT                   1                                      // Perspective of concept is Object
   #define ATLAS_PERSPECTIVE_OBSERVER                 2                                      // Perspective of concept is Observer
   #define ATLAS_PERSPECTIVE_COUNT                    3                                      // Maximum number of perspectives

   // Atlas auxiliaries
   #define ATLAS_AUX_DO                               0                                      // Do auxiliary
   #define ATLAS_AUX_BE                               1                                      // Be auxiliary
   #define ATLAS_AUX_HAVE                             2                                      // Have auxiliary
   #define ATLAS_AUX_COUNT                            3                                      // Maximum number of auxiliaries

   // Atlas method types
   #define ATLAS_METHOD_ELEMENT                       0x00                                   // Method will give different result for each element
   #define ATLAS_METHOD_INSTANCE                      0x01                                   // Method will give same result for all instance elements
   #define ATLAS_METHOD_CONCEPT                       0x02                                   // Method will give same result for all concept instances
   #define ATLAS_METHOD_GROUP                         0x03                                   // Method will give same result for all group members

   // Tense bits can be ORed to generate common grammatical tenses
   #define ATLAS_EXP_TENSE_CONTINUOUS                 TITAN_BIT0                             // Expression is in continuous tense
   #define ATLAS_EXP_TENSE_PAST                       TITAN_BIT1                             // Expression is in the past tense
   #define ATLAS_EXP_TENSE_FUTURE                     TITAN_BIT2                             // Expression is in the future tense
   #define ATLAS_EXP_TENSE_PERFECT    T               TITAN_BIT3                             // Expression is in the perfect tense
   #define ATLAS_EXP_TENSE_INFINITIVE                 TITAN_BIT4                             // Expression is in the infinitive

   // Query values are exclusive. Only one query type can be probed at a time
   #define ATLAS_EXP_QUERY_NONE                       0                                      // Expression is not a query
   #define ATLAS_EXP_QUERY                            1                                      // Expression is a TRUE/FALSE query
   #define ATLAS_EXP_QUERY_SUBJECT                    2                                      // Expression is asking a (who/what) subject question -> reference
   #define ATLAS_EXP_QUERY_OBJECT1                    3                                      // Expression is asking a (who/what) first object question -> reference
   #define ATLAS_EXP_QUERY_OBJECT2                    4                                      // Expression is asking a (who/what) second object question -> reference
   #define ATLAS_EXP_QUERY_OBJECT3                    5                                      // Expression is asking a (who/what) third object question -> reference
   #define ATLAS_EXP_QUERY_TOOL                       6                                      // Expression is asking a (who/what) tool question -> reference
   #define ATLAS_EXP_QUERY_WHEN                       7                                      // Expression is asking a when question -> temporal relation
   #define ATLAS_EXP_QUERY_WHERE                      8                                      // Expression is asking a where question -> spatial relation
   #define ATLAS_EXP_QUERY_STATE                      9                                      // Expression is asking a what (adjective/verb) question -> adjective/verb
   #define ATLAS_EXP_QUERY_CAUSE                      10                                     // Expression is asking a why (cause) question -> event(s), rule(s)
   #define ATLAS_EXP_QUERY_INTENT                     11                                     // Expression is asking a why (intent) question -> event(s), rule(s)
   #define ATLAS_EXP_QUERY_PROCESS                    12                                     // Expression is asking a how (process) question -> event(s), rule(s)
   #define ATLAS_EXP_QUERY_ADVERB                     13                                     // Expression is asking a how (adverb) question -> event(s), rule(s)

   // Expression types
   #define ATLAS_EXP_TYPE_CAUSE                       TITAN_BIT7                             // Expression is asking a who question -> event(s), rule(s)
   #define ATLAS_EXP_TYPE_CONSEQUENCE                 TITAN_BIT8                             // Expression is asking a who question -> event(s), rule(s)
   #define ATLAS_EXP_TYPE_CONDITION                   TITAN_BIT9                             // Expression is asking a who question -> event(s), rule(s)

   #define ATLAS_EXP_TYPE_REACTION                    TITAN_BIT10                            // Expression is asking a true/false question -> (-100, 0, 100)
   #define ATLAS_EXP_TYPE_OBSERVATION                 TITAN_BIT11                            // Expression is conversational (a statement) and not imperative (a command)
   #define ATLAS_EXP_TYPE_PROSPECTION                 TITAN_BIT12                            // Expression states possible outcomes (cannot/?/can)
   #define ATLAS_EXP_TYPE_PREDICTION                  TITAN_BIT13                            // Expression states predicted outcomes (must not/should not/?/should/must)
   #define ATLAS_EXP_TYPE_OBJECTIVE                   TITAN_BIT14                            // Expression states desired outcomes (do not want/do not like/?/like/want)

   // Expression components
   #define ATLAS_EXP_COMP_AUTHOR                      TITAN_BIT0                             // Directive has a valid author component
   #define ATLAS_EXP_COMP_SUBJECT                     TITAN_BIT1                             // Directive has a valid a subject component
   #define ATLAS_EXP_COMP_EXTRA_SUBJECT               TITAN_BIT2                             // Directive has a valid extra subject components
   #define ATLAS_EXP_COMP_OBJECT1                     TITAN_BIT3                             // Directive has a valid a first object component
   #define ATLAS_EXP_COMP_EXTRA_OBJECT1               TITAN_BIT4                             // Directive has a valid extra first object author components
   #define ATLAS_EXP_COMP_OBJECT2                     TITAN_BIT5                             // Directive has a valid a second object component
   #define ATLAS_EXP_COMP_EXTRA_OBJECT2               TITAN_BIT6                             // Directive has a valid extra second object components
   #define ATLAS_EXP_COMP_OBJECT3                     TITAN_BIT7                             // Directive has a valid a third object component
   #define ATLAS_EXP_COMP_EXTRA_OBJECT3               TITAN_BIT8                             // Directive has a valid extra third object components
   #define ATLAS_EXP_COMP_SPACE                       TITAN_BIT9                             // Directive has a valid a space component
   #define ATLAS_EXP_COMP_NEXT_SPACE                  TITAN_BIT10                            // Directive has a valid extra space components
   #define ATLAS_EXP_COMP_TIME                        TITAN_BIT11                            // Directive has a valid a time component
   #define ATLAS_EXP_COMP_NEXT_TIME                   TITAN_BIT12                            // Directive has a valid extra time components
   #define ATLAS_EXP_COMP_INSTRUMENT                  TITAN_BIT13                            // Directive has a valid an instrument component
   #define ATLAS_EXP_COMP_NEXT_INSTRUMENT             TITAN_BIT14                            // Directive has a valid extra instrument components
   #define ATLAS_EXP_COMP_STATE                       TITAN_BIT15                            // Directive has a valid a state component
   #define ATLAS_EXP_COMP_NEXT_STATE                  TITAN_BIT16                            // Directive has a valid extra state components
                                                     
   // Atlas directive component IDs
   enum atlas_directive_component_t                               
      {                                                          
      ATLAS_DIRECTIVE_SUBJECT = 0,                                                           // Directive subject field
      ATLAS_DIRECTIVE_OBJECT1,                                                               // Directive first object field
      ATLAS_DIRECTIVE_OBJECT2,                                                               // Directive second object field
      ATLAS_DIRECTIVE_OBJECT3,                                                               // Directive second object field
      ATLAS_DIRECTIVE_INSTRUMENT,                                                            // Directive instrument object field
      ATLAS_DIRECTIVE_SPACE,                                                                 // Directive spatial relations field
      ATLAS_DIRECTIVE_TIME,                                                                  // Directive temporal relations field
      ATLAS_DIRECTIVE_ABSTRACT,                                                              // Directive abstract relation field
      ATLAS_DIRECTIVE_OBSERVER,                                                              // Directive observer field
      ATLAS_DIRECTIVE_AUTHOR,                                                                // Directive author field
      };                                                         
                                                                
   // Atlas relation query types                      
   #define ATLAS_GET_NONE                             0x00                                   // No query, but name can still be retrieved
   #define ATLAS_GET_REFERENCE                        0x01                                   // Retrieve reference ID
   #define ATLAS_GET_INSTANCE                         0x02                                   // Retrieve reference instance and element handles
   #define ATLAS_GET_AMOUNT                           0x04                                   // Retrieve reference amount
   #define ATLAS_GET_RELATION                         0x08                                   // Retrieve reference relation or preposition
   #define ATLAS_GET_DEFAULT                          (ATLAS_GET_REFERENCE |\
                                                       ATLAS_GET_INSTANCE )                  // Query common parameters
   #define ATLAS_GET_ALL                              (ATLAS_GET_DEFAULT   |\
                                                       ATLAS_GET_RELATION  )                 // Query all parameters

   // Atlas agent administration methods
   #define ATLAS_AGENCY_NONE                          0x00                                   // Calling reference has no access to cognitive services
   #define ATLAS_AGENCY_HIDDEN                        0x01                                   // Calling reference is hidden from the agent
   #define ATLAS_AGENCY_ZONE                          0x02                                   // Calling reference can modify agent context
   #define ATLAS_AGENCY_DOMAIN                        0x04                                   // Calling reference can modify agent domain
   #define ATLAS_AGENCY_TELL                          0x08                                   // Calling reference can say to another agent on behalf of agent
   #define ATLAS_AGENCY_PING                          0x10                                   // Calling reference can ping another agent on behalf of agent
   #define ATLAS_AGENCY_SEND                          0x20                                   // Calling reference can tell another agent on behalf of agent
   #define ATLAS_AGENCY_CREATE                        0x40                                   // Calling reference can create another reference under agent
   #define ATLAS_AGENCY_LABEL                         0x80                                   // Calling reference can access labels of agent
   #define ATLAS_AGENCY_ATTRIBUTE                     0x100                                  // Calling reference can control the attribute features agent
   #define ATLAS_AGENCY_COMMUNICATION                 (ATLAS_AGENCY_TELL | ATLAS_AGENCY_PING | ATLAS_AGENCY_SEND) 
   #define ATLAS_AGENCY_OBJECT                        (ATLAS_AGENCY_CREATE | ATLAS_AGENCY_LABEL | ATLAS_AGENCY_ATTRIBUTE)
   #define ATLAS_AGENCY_WORKER                        (ATLAS_AGENCY_COMMUNICATION)
   #define ATLAS_AGENCY_ADMINISTRATOR                 (ATLAS_AGENCY_ZONE | ATLAS_AGENCY_DOMAIN | ATLAS_AGENCY_COMMUNICATION)

   // Atlas attribute IDs
   #define ATLAS_ATTRIBUTE_LOCATION                   0x01                                   // Handle location feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_SIZE                       0x02                                   // Handle size feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_SHAPE                      0x04                                   // Handle shape feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_TEXTURE                    0x08                                   // Handle texture feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_COLOR                      0x10                                   // Handle color feature internally when this bit is ON

   // Atlas attachment types
   enum
      {
      ATLAS_ATTACHMENT_WHO = 1,                                                              // Who response attachment
      ATLAS_ATTACHMENT_WHEN,                                                                 // When response attachment
      ATLAS_ATTACHMENT_WHERE,                                                                // Where response attachment
      ATLAS_ATTACHMENT_WHAT,                                                                 // What response attachment
      ATLAS_ATTACHMENT_WHY,                                                                  // Why response attachment
      ATLAS_ATTACHMENT_HOW                                                                   // How response attachment
      };

   // Transcendental values
   #ifndef M_E
      #define M_E                                     2.71828182845904523536                 // e
      #define M_LOG2E                                 1.44269504088896340736                 // log2(e)
      #define M_LOG10E                                0.434294481903251827651                // log10(e)
      #define M_LN2                                   0.693147180559945309417                // ln(2)
      #define M_LN10                                  2.30258509299404568402                 // ln(10)
      #define M_PI                                    3.14159265358979323846                 // pi
      #define M_PI_2                                  1.57079632679489661923                 // pi/2
      #define M_PI_4                                  0.785398163397448309616                // pi/4
      #define M_1_PI                                  0.318309886183790671538                // 1/pi
      #define M_2_PI                                  0.636619772367581343076                // 2/pi
      #define M_2_SQRTPI                              1.12837916709551257390                 // 2/sqrt(pi)
      #define M_SQRT2                                 1.41421356237309504880                 // sqrt(2)
      #define M_SQRT1_2                               0.707106781186547524401                // 1/sqrt(2)
      #define M_E_F                                   TO_FLOAT(2.71828182845904523536)
      #define M_LOG2E_F                               TO_FLOAT(1.44269504088896340736)
      #define M_LOG10E_F                              TO_FLOAT(0.434294481903251827651)
      #define M_LN2_F                                 TO_FLOAT(0.693147180559945309417)
      #define M_LN10_F                                TO_FLOAT(2.30258509299404568402)
      #define M_PI_F                                  TO_FLOAT(3.14159265358979323846)
      #define M_PI_2_F                                TO_FLOAT(1.57079632679489661923)
      #define M_PI_4_F                                TO_FLOAT(0.785398163397448309616)
      #define M_1_PI_F                                TO_FLOAT(0.318309886183790671538)
      #define M_2_PI_F                                TO_FLOAT(0.636619772367581343076)
      #define M_2_SQRTPI_F                            TO_FLOAT(1.12837916709551257390)
      #define M_SQRT2_F                               TO_FLOAT(1.41421356237309504880)
      #define M_SQRT1_2_F                             TO_FLOAT(0.707106781186547524401)
   #endif   

   // Atlas amount data structure definition (2 * 64-bit)
   struct atlas_amount_t
      {
      TITAN_HANDLE64                                  format_handle;                         // Format handle of the value
      union                                                                                  // Value data
         {
         TITAN_HANDLE64                               handle_value;                          // 64-bit unsigned integer TITAN_HANDLE64
         TITAN_ULONG                                  ulong_value;                           // 64-bit unsigned integer TITAN_FORMAT_ULONG
         TITAN_LONG                                   long_value;                            // 64-bit signed integer TITAN_FORMAT_LONG
         TITAN_DOUBLE                                 double_value;                          // 64-bit double precision real (TITAN_FORMAT_DOUBLE)
         TITAN_FLOAT                                  float_value;                           // 32-bit single precision real (TITAN_FORMAT_AMOUNT, TITAN_FORMAT_FLOAT)
         TITAN_STRING                                 string_value;                          // String data (TITAN_FORMAT_STRING, TITAN_FORMAT_DESCRIPTION)
         TITAN_POINTER                                raw_value;                             // Pointer to raw data in memory (TITAN_FORMAT_RAW)
         TITAN_BUFFER                                 buffer_value;                          // UBYTE pointer to buffer (UBYTE) data in memory (TITAN_FORMAT_BUFFER)
         TITAN_INT                                    int_value;                             // 32-bit signed integer (TITAN_FORMAT_INT)
         TITAN_UINT                                   uint_value;                            // 32-bit unsigned integer (TITAN_FORMAT_UINT)
         TITAN_SHORT                                  short_value;                           // 16-bit signed integer  (TITAN_FORMAT_SHORT)
         TITAN_USHORT                                 ushort_value;                          // 16-bit unsigned integer  (TITAN_FORMAT_USHORT) 
         };
      };

   // Atlas auxiliary header definition
   struct atlas_auxiliary_parameters_t
      {
      TITAN_UBYTE                                     aux_type;                              // Aux index (Do, Be, Have)
      TITAN_UBYTE                                     expression_type;                       // Auxiliary type (Directive, Query, Ability, Prediction, Desire)
      TITAN_UBYTE                                     aux_tense;                             // Auxiliary tense (Past, Present, Future)
      TITAN_UBYTE                                     valence;                               // Valence of the directive
      };

   // Atlas value structure definition (3 * 64-bit)
   struct atlas_value_t
      {
      TITAN_HANDLE64                                  name_handle;                           // Name handle of the value
      atlas_amount_t                                  amount;                                // Value amount
      };

   // Atlas reference information structure (10 * 64-bit)
   struct atlas_reference_info_t
      {
      TITAN_STRING                                    name;                                  // Reference name
      TITAN_SIZE64                                    name_max_size;                         // Name size
      TITAN_STRING                                    concept_name;                          // Concept name
      TITAN_SIZE64                                    concept_name_max_size;                 // Name size
      TITAN_HANDLE64			                           relation;                              // Member relation type handle
      atlas_amount_t                                  amount;                                // Member relation amount
      TITAN_HANDLE64			                           reference;                             // Member relation base reference
      TITAN_HANDLE64                                  instance;                              // Member instance handle
      TITAN_HANDLE64                                  element;                               // Member element handle
      };

   // Atlas action information structure (5 * 64-bit)
   struct atlas_action_info_t
      {
      TITAN_STRING                                    name;                                  // TITAN_STRING where the action name will be copied
      TITAN_SIZE64                                    name_max_size;                         // Maximum number of characters to be copied
      TITAN_HANDLE64			                           label;                                 // Action label
      atlas_amount_t                                  amount;                                // Amount of the action 
      };

   // Atlas probe result structure definition (64-bit)
   struct atlas_probe_result_t
      {
      TITAN_BYTE                                      value;                                 // Probe value result
      TITAN_BYTE                                      can;                                   // Probe value result possibility
      TITAN_BYTE                                      should;                                // Probe value result probability
      TITAN_BYTE                                      want;                                  // Probe value result goal
      TITAN_BITMAP32                                  reserved;                              // Reserved for future use
      };

#endif

/*
   End of file
*/
