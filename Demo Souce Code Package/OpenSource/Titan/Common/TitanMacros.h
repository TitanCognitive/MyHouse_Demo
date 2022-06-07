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

#ifndef TITAN_BASE_MACROS_H
#define TITAN_BASE_MACROS_H

   // Casting macros
   #define _S(X, Y)                                   static_cast<X>(Y)
   #define _D(X, Y)                                   dynamic_cast<X>(Y)
   #define _R(X, Y)                                   reinterpret_cast<X>(Y)
   #define _SP(X, Y)                                  static_cast<X *>(Y)
   #define _DP(X, Y)                                  dynamic_cast<X *>(Y)
   #define _RP(X, Y)                                  reinterpret_cast<X *>(Y)
   #define _HI64(X)                                   static_cast<TITAN_UINT>((X >> 32L) & 0xFFFFFFFF)
   #define _LO64(X)                                   static_cast<TITAN_UINT>(X & 0xFFFFFFFF)
   #define _HI32(X)                                   static_cast<TITAN_USHORT>((X >> 16L) & 0xFFFF)
   #define _LO32(X)                                   static_cast<TITAN_USHORT>(X & 0xFFFF)
   #define _HI16(X)                                   static_cast<TITAN_UBYTE>((X >> 8L) & 0xFF)
   #define _LO16(X)                                   static_cast<TITAN_UBYTE>(X & 0xFF)
   #define _TO_HI64(X)                                static_cast<TITAN_HANDLE64>((_S(TITAN_HANDLE64, X) << 32L) & 0xFFFFFFFF00000000)
   #define _TO_LO64(X)                                static_cast<TITAN_HANDLE64>(X & 0xFFFFFFFF)
   #define _TO_HI32(X)                                static_cast<TITAN_USHORT>((X << 16L) & 0xFFFF0000)
   #define _TO_LO32(X)                                static_cast<TITAN_USHORT>(X & 0xFFFF)
   #define _TO_HI16(X)                                static_cast<TITAN_UBYTE>((X << 8L) & 0xFF00)
   #define _TO_LO16(X)                                static_cast<TITAN_UBYTE>(X & 0xFF)
   #define TO_STRING(X)                               _R(TITAN_STRING, X)
   #define TO_POINTER(X)                              _R(TITAN_POINTER, X)
   #define TO_HANDLE64(X)                             _R(TITAN_HANDLE64, X)
   #define TO_HANDLE32(X)                             _R(TITAN_HANDLE32, X)
   #define TO_FLOAT(X)                                _S(TITAN_FLOAT, X)
   #define TO_DOUBLE(X)                               _S(TITAN_DOUBLE, X)
   #define TO_PTR(X)                                  _R(TITAN_POINTER, X)
   #define TO_08(X)                                   _S(TITAN_UBYTE, X)
   #define TO_16(X)                                   _S(TITAN_USHORT, X)
   #define TO_32(X)                                   _S(TITAN_UINT, X)
   #define TO_64(X)                                   _S(TITAN_ULONG, X)
   #define TO_08s(X)                                  _S(TITAN_BYTE, X)
   #define TO_16s(X)                                  _S(TITAN_SHORT, X)
   #define TO_32s(X)                                  _S(TITAN_INT, X)
   #define TO_64s(X)                                  _S(TITAN_LONG, X)

   // Atlas simulation method response macros
   #define TO_RESPONSE(Value)                         atlas_probe_result_t { Value, 0, 0, 0, 0 }
   #define TO_RESPONSE2(Value, Can)                   atlas_probe_result_t { Value, Can, 0, 0, 0 }
   #define TO_RESPONSE3(Value, Can, Should)           atlas_probe_result_t { Value, Can, Should, 0, 0 }
   #define TO_RESPONSE4(Value, Can, Should, Want)     atlas_probe_result_t { Value, Can, Should, Want, 0 }
   #define NO_RESPONSE()                              atlas_probe_result_t { 0, 0, 0, 0, 0 }

   // Stochastic macros
   #define random()                                   (TO_FLOAT(rand() & 0x7fff) * (1.0f / 32767.0f))

   // General mathematic macros
   #define ToPositive(a, b)                           { if (a >= 0) b = a; else b = -a; } 
   #define MakePositive(a)                            { if (a < 0) a = -a; }
   #define Sort(a, b)                                 { if (a > b) { float c; c = a; a = b; b = c;}
   
   // Matrix macros (right-handed)
   #define TITAN_MAT_IDENTITY(m)                      (m[0] = m[5] = m[10] = m[15] = 1.0f)
   #define TITAN_MAT_LOAD(m, x)                       m[0] = m[5] = m[10] =  x; m[15] = 1.0f;

   // Trigonometry macros
   #define DotProduct(x,y)                            (x[0]*y[0]+x[1]*y[1]+x[2]*y[2])
   #define CrossProduct(a, b, c)                      (c[0]=a[1]*b[2]-a[2]*b[1],c[1]=a[2]*b[0]-a[0]*b[2],c[2]=a[0]*b[1]-a[1]*b[0])
   #define VectorSA(a, s, b, c)                       (c[0]=a[1]*s*b[2]-a[2]*s*b[1],c[1]=a[2]*s*b[0]-a[0]*s*b[2],c[2]=a[0]*s*b[1]-a[1]*s*b[0])
   #define VectorMA2(a, s, b, c)                      (c[0]=a[0]*s+b[0],c[1]=a[1]*s+b[1])
   #define VectorMA3(a, s, b, c)                      (c[0]=a[0]*s+b[0],c[1]=a[1]*s+b[1],c[2]=a[2]*s+b[2])
   #define VectorMA4(a, s, b, c)                      (c[0]=a[0]*s+b[0],c[1]=a[1]*s+b[1],c[2]=a[2]*s+b[2],c[3]=a[3]*s+b[3])
   #define VectorSubtract2(a, b, c)                   (c[0]=a[0]-b[0],c[1]=a[1]-b[1])
   #define VectorSubtract3(a, b, c)                   (c[0]=a[0]-b[0],c[1]=a[1]-b[1],c[2]=a[2]-b[2])
   #define VectorSubtract4(a, b, c)                   (c[0]=a[0]-b[0],c[1]=a[1]-b[1],c[2]=a[2]-b[2],c[3]=a[3]-b[3])
   #define VectorScale2(a, s, b)                      (b[0]=a[0]*s,b[1]=a[1]*s)
   #define VectorScale3(a, s, b)                      (b[0]=a[0]*s,b[1]=a[1]*s,b[2]=a[2]*s)
   #define VectorScale4(a, s, b)                      (b[0]=a[0]*s,b[1]=a[1]*s,b[2]=a[2]*s,b[3]=a[3]*s)
   #define VectorAdd2(a, b, c)                        (c[0]=a[0]+b[0],c[1]=a[1]+b[1])
   #define VectorAdd3(a, b, c)                        (c[0]=a[0]+b[0],c[1]=a[1]+b[1],c[2]=a[2]+b[2])
   #define VectorAdd4(a, b, c)                        (c[0]=a[0]+b[0],c[1]=a[1]+b[1],c[2]=a[2]+b[2],c[3]=a[3]+b[3])
   #define VectorCopy2(a, b)                          (b[0]=a[0],b[1]=a[1])
   #define VectorCopy3(a, b)                          (b[0]=a[0],b[1]=a[1],b[2]=a[2])
   #define VectorCopy4(a, b)                          (b[0]=a[0],b[1]=a[1],b[2]=a[2],b[3]=a[3])
   #define VectorClear2(a, b)                         (a[0]=a[1]=b)
   #define VectorClear3(a, b)                         (a[0]=a[1]=a[2]=b)
   #define VectorClear4(a, b)                         (a[0]=a[1]=a[2]=a[3]=b)
   #define VectorNegate2(a, b)                        (b[0]=-a[0],b[1]=-a[1]))
   #define VectorNegate3(a, b)                        (b[0]=-a[0],b[1]=-a[1],b[2]=-a[2])
   #define VectorNegate4(a, b)                        (b[0]=-a[0],b[1]=-a[1],b[2]=-a[2],b[3]=-a[3])
   #define VectorSet2(v, x, y)                        (v[0]=(x),v[1]=(y))
   #define VectorSet3(v, x, y, z)                     (v[0]=(x),v[1]=(y),v[2]=(z))
   #define VectorSet4(v, x, y, z, w)                  (v[0]=(x),v[1]=(y),v[2]=(z),v[3]=(w))
   #define VectorSquaredDist2(u,v)                    (u[0]-v[0])*(u[0]-v[0])+(u[1]-v[1])*(u[1]-v[1])
   #define VectorSquaredDist3(u,v)                    (u[0]-v[0])*(u[0]-v[0])+(u[1]-v[1])*(u[1]-v[1])+(u[2]-v[2])*(u[2]-v[2])
   #define VectorSquaredDist4(u,v)                    (u[0]-v[0])*(u[0]-v[0])+(u[1]-v[1])*(u[1]-v[1])+(u[2]-v[2])*(u[2]-v[2])+(u[3]-v[3])*(u[3]-v[3])
   #define VectorDist2(u,v)                           sqrt(VectorSquaredDist2(u,v))
   #define VectorDist3(u,v)                           sqrt(VectorSquaredDist3(u,v))
   #define VectorDist4(u,v)                           sqrt(VectorSquaredDist4(u,v))

#endif

/*
   End of file
*/
