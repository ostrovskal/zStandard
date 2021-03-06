
#pragma once

#include <stdint.h>
#include <fcntl.h>

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <SDKDDKVer.h>
    #include <iostream>
    #include <windows.h>
    //#include <time.h>
    #include <mmsystem.h>
    #include <io.h>
    #include <direct.h>
    #define stricmp _stricmp
    #define lseekf _lseek
    #define unlinkf _unlink
    #define openf _open
    #define closef _close
    #define readf _read
    #define writef _write
    #define filenof _fileno
#else
    #define _USE_32BIT_TIME_T
    #include <string>
    #include <utime.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <jni.h>
    #include <android/log.h>
    #include <dirent.h>
    #define stricmp strcasecmp
    #define lseekf lseek
    #define unlinkf unlink
    #define openf ::open
    #define closef ::close
    #define readf ::read
    #define writef ::write
    #define filenof fileno
    extern JavaVM* jvm;
    extern thread_local JNIEnv* jenv;
#endif

using u64       = uint64_t;
using i64       = int64_t;
using u32       = uint32_t;
using i32       = int32_t;
using u8        = uint8_t;
using i8        = int8_t;
using u16       = uint16_t;
using i16       = int16_t;
using cstr      = const char*;
using cwstr     = const wchar_t*;
using ulg       = unsigned long;
using hfile     = int;
using lutime_t  = unsigned;

#include "zTypes.h"

// минимум
template <typename T> T z_min(const T& v1, const T& v2) {
    return v1 <= v2 ? v1 : v2;
}
// максимум
template <typename T> T z_max(const T& v1, const T& v2) {
    return v1 >= v2 ? v1 : v2;
}
// логгирование
void z_log(bool info, cstr file, cstr func, int line, cstr msg, ...);
// повернуть на угол
void z_rotate(cpti& c, zVertex2D* v, int count, float angle);
// масштабировать
void z_scale(cpti& c, zVertex2D* v, int count, float xzoom, float yzoom);
// определение габаритов вершин
rtf z_vertexMinMax(zVertex2D* v, int count);
// обрезка
rti z_clipRect(crti& base, crti& rect);
// проверка на разделитель
bool z_delimiter(u8 c);

#ifndef NDEBUG
	#define DLOG(...)       z_log(false, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
	#define DLOG(...)
#endif

#define ILOG(...)           z_log(true, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

// основные константы/макросы
#define deg2rad(a)          (float)(a * Z_PI / 180.0f)
#define SAFE_DELETE(p)      if(p) delete p, (p) = nullptr
#define SAFE_A_DELETE(p)    if(p) delete[] p, (p) = nullptr
#define RTI_LOG(m, r)       DLOG("%s (%i - %i) : (%i - %i)", m, (r).x, (r).y, (r).w, (r).h)
#define RTF_LOG(m, r)       DLOG("%s (%f - %f) : (%f - %f)", m, (r).x, (r).y, (r).w, (r).h)
#define SZI_LOG(m, s)       DLOG("%s (%i - %i)", m, (s).w, (s).h)
#define SZF_LOG(m, s)       DLOG("%s (%f - %f)", m, (s).w, (s).h)
#define PTI_LOG(m, p)       DLOG("%s (%i - %i)", m, (p).x, (p).y)
#define PTF_LOG(m, p)       DLOG("%s (%f - %f)", m, (p).x, (p).y)
#define COL_LOG(m, c)       DLOG("%s r:%f g:%f b:%f a:%f", m, (c).r, (c).g, (c).b, (c).a)
#define FRTI_LOG(m, r)      DLOG("%s (%i - %i) : (%i - %i)", m, (r).x, (r).y, (r).x + (r).w, (r).y + (r).h)
#define FRTF_LOG(m, r)      DLOG("%s (%f - %f) : (%f - %f)", m, (r).x, (r).y, (r).x + (r).w, (r).y + (r).h)

enum EDITOR_LEXEMS { LD, LW, LN, LS, LO, LQ, LK, LU, L1, L2, L3, L4, L5 };

// система счистления для преобразования строк/чисел
constexpr int RADIX_DEC = 0;
constexpr int RADIX_HEX = 1;
constexpr int RADIX_BIN = 2;
constexpr int RADIX_OCT = 3;
constexpr int RADIX_DBL = 4;
constexpr int RADIX_FLT = 5;
constexpr int RADIX_BOL = 6;
constexpr int RADIX_STR = 7;

// Варианты форматирования чисел
constexpr int ZFV_CODE_LAST = 0; // "3X", "2X"
constexpr int ZFV_CODE      = 2; // "3X ", "2X "
constexpr int ZFV_PADDR16   = 4; // "5(X)", "4(#X)"
constexpr int ZFV_PADDR8    = 6; // "3(X)", "2(#X)"
constexpr int ZFV_OFFS      = 8; // "3+-X)", "2+-#X)"
constexpr int ZFV_NUM16     = 10;// "5X", "4X"
constexpr int ZFV_OPS16     = 12;// "5X", "4#X"
constexpr int ZFV_OPS8      = 14;// "3X", "2#X"
constexpr int ZFV_CVAL      = 16;// "5[X]", "4[#X]"
constexpr int ZFV_PVAL8     = 18;// "3{X}", "2{#X}"
constexpr int ZFV_PVAL16    = 20;// "5{X}", "4{#X}"
constexpr int ZFV_NUMBER    = 22;// "0X", "0#X"
constexpr int ZFV_SIMPLE    = 24;// "0X", "0X"
constexpr int ZFV_BINARY    = 26;// "%0X", "%0X"
constexpr int ZFV_DECIMAL   = 28;// "0X", "0X"

// запись/чтение различных данных
inline u16 wordLE(u8** ptr) {
    auto p(*ptr); (*ptr) += 2;
    return *(uint16_t*)(p);
}

inline void wordLE(u8** buf, u16 val) {
    *(u16*)(*buf) = val; (*buf) += 2;
}

inline u16 wordBE(u8** ptr) {
    auto p(*ptr); (*ptr) += 2;
    return ((p[0] << 8) | p[1]);
}

inline void wordBE(u8** buf, u16 val) {
    auto ptr(*buf); (*buf) += 2;
    ptr[0] = (u8)((val >> 8) & 0xFF);
    ptr[1] = (u8)((val >> 0) & 0xFF);
}

inline u32 dwordLE(u8** ptr) {
    auto p(*ptr); (*ptr) += 4;
    return (*(u16*)(p) | (*(u16*)(p + 2) << 16));
}

inline void dwordLE(u8** buf, u32 val) {
    auto ptr(*buf); (*buf) += 4;
    *(u16*)(ptr + 0) = (u16)((val >> 0)  & 0xFFFF);
    *(u16*)(ptr + 2) = (u16)((val >> 16) & 0xFFFF);
}

inline u32 dwordBE(u8** ptr) {
    auto p(*ptr); (*ptr) += 4;
    return (p[3] | (p[2] << 8) | (p[1] << 16) | (p[0] << 24));
}

inline void dwordBE(u8** buf, u32 val) {
    auto ptr(*buf); (*buf) += 4;
    *(u16*)(ptr + 0) = (u16)((val >> 16) & 0xFFFF);
    *(u16*)(ptr + 2) = (u16)((val >> 0)  & 0xFFFF);
}

inline u64 qwordLE(u8** ptr) {
    auto p(*ptr); (*ptr) += 8;
    u64 _1 = *(u16*)(p + 0) | (*(u16*)(p + 2) << 16);
    u64 _2 = *(u16*)(p + 4) | (*(u16*)(p + 6) << 16);
    return (_1 | (_2 << 32L));
}

inline void qwordLE(u8** buf, u64 val) {
    auto ptr(*buf); (*buf) += 8;
    *(u16*)(ptr + 0) = (u16)((val >> 0)  & 0xFFFF);
    *(u16*)(ptr + 2) = (u16)((val >> 16) & 0xFFFF);
    *(u16*)(ptr + 4) = (u16)((val >> 32) & 0xFFFF);
    *(u16*)(ptr + 6) = (u16)((val >> 48) & 0xFFFF);
}

inline u64 qwordBE(u8** ptr) {
    auto p(*ptr); (*ptr) += 8;
    u64 _1 = (p[3] | (p[2] << 8) | (p[1] << 16) | (p[0] << 24));
    u64 _2 = (p[7] | (p[6] << 8) | (p[5] << 16) | (p[4] << 24));
    return (_2 | (_1 << 32L));
}

inline void qwordBE(u8** buf, u64 val) {
    auto ptr(*buf); (*buf) += 8;
    *(u16*)(ptr + 0) = (u16)((val >> 48) & 0xFFFF);
    *(u16*)(ptr + 2) = (u16)((val >> 32) & 0xFFFF);
    *(u16*)(ptr + 4) = (u16)((val >> 16) & 0xFFFF);
    *(u16*)(ptr + 6) = (u16)((val >> 0)  & 0xFFFF);
}

// из строки в hex
char z_toHex(char** _buf);
// преобразование escape
void z_escape(char** dst, char** src);
// вычисление CRC
u16 z_crc(u8 v, u16 init = 0xcdb4);
u16 z_crc(u8* src, u32 size);
u64 z_crc64(cstr str);
u32 z_hash(cstr str, i32 len);
// ремаппинг
i32 z_remap(i32 value, i32* place);
// число в строку с формированием
char* z_fmtValue(i32 value, u32 type, bool hex, u32 radix = RADIX_DEC, bool _showHex = false);
// число в строку
char* z_ntos(void* v, i32 r, bool sign, int size = 4, char** end = nullptr);
// строку в число
void* z_ston_(cstr s, i32 r, char** end = nullptr);
template<typename T = int> T z_ston(cstr s, i32 r, char** end = nullptr) {
    return *(T*)z_ston_(s, r, end);
}
// UTF->ASCII
u8* z_utfToAscii(u8* u, int l);
// найти после вхождения
inline char* z_strAfter(cstr s, int f) {
    s = strchr(s, f); return const_cast<char*>(s ? s + 1 : nullptr);
}
// RLE упаковщик
u8* z_rle_compress(u8* in, int size, int& nsize);
// RLE распаковщик
u8* z_rle_decompress(u8* in, int size, int nsize);
// создание директории
inline void z_mkdir(cstr dir) {
#ifdef WIN32
    _mkdir(dir);
#else
    mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}
// дату в строку с форматированием
inline cstr z_date(cstr fmt, time_t rawtime) {
    static char buf[260];
    if(!rawtime) time(&rawtime);
    strftime(buf, 260, fmt, localtime(&rawtime));
    return buf;
}
// возвращает количество миллисекунд
inline i64 z_timeMillis() {
#ifdef WIN32
    return timeGetTime();
#else
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (u_long)(((tv.tv_sec * 1000) + (tv.tv_usec / 1000)));
#endif
}

inline bool z_isspace(char ch) {
    return ((ch >= 0x09 && ch <= 0x0D) || ch == 0x20);
}

// пропуск пробелов
inline void z_skip_spc(char** s, int& ln) {
    auto t(*s); t--;
    while(z_isspace(*++t)) { if(*t == '\n') ln++; }
    *s = t;
}

// вычисление процентов
inline int z_percent(int value, int percent) {
    return (value * percent) / 100;
}

// заполнение памяти
inline void z_memset(u8** ptr, i32 set, u32 count) {
    if(ptr) *ptr = (u8*)(memset(*ptr, set, count)) + count;
}

inline void z_memset2(u8** ptr, u16 set, u32 count) {
    if(ptr) while(count-- > 0) wordLE(ptr, set);
}

inline void z_memset4(u8** ptr, u32 set, u32 count) {
    if(ptr) while(count-- > 0) dwordLE(ptr, set);
}

inline void z_memzero(u8* ptr, u32 count) {
    z_memset(&ptr, 0, count);
}

inline void z_memcpy(u8** dst, const void* src, u32 count) {
    if(dst && src) *dst = (u8*)(memcpy(*dst, src, count)) + count;
}

inline void z_char(char** dst, char ch, i32 count) {
    if(dst && count) {
        auto tmp(*dst);
        while(count-- > 0) *tmp++ = ch;
        *dst = tmp;
    }
}

inline void z_strcpy(char** dst, cstr src) {
    if(dst && src) {
        strcpy(*dst, src);
        *dst += strlen(src);
    }
}

// преобразовать значение в ближайшую степень двойки
inline i32 z_pow2(i32 val, bool nearest) {
    auto v(val - 1);
    v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
    v++;
    return ((v == val || nearest) ? v >> 1 : v);
}

struct HZIP {
    HZIP(void* _unz, void* _zip) : unz(_unz), zip(_zip) { }
    void* unz{ nullptr };
    void* zip{ nullptr };
};

#define _zipClose(hz)    { hz->zip ? zipClose(hz->zip) : unzClose(hz->unz); }

#include "zZip.h"
#include "zUnzip.h"
#include "zRand.h"
#include "zArray.h"
#include "zString.h"
#include "zFile.h"
#include "zJSON.h"
#include "zXml.h"

#ifndef WIN32
    #include "zGRef.h"
#endif
