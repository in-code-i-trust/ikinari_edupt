#ifndef _RENDER_H_
#define _RENDER_H_

// #include <iostream>

#if defined(_TWIN32)
#define ARCH_WIN32 (1)
#define ARCH_UEFI  (0)
#elif defined(_TUEFI)
#define ARCH_WIN32 (0)
#define ARCH_UEFI  (1)
#else
#error define something.
#endif

#define fabs __builtin_fabs

void entryPoint();

#if ARCH_WIN32
extern "C" {
	__declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int dwFlags, unsigned int dwBytes);
	__declspec(dllimport) void ExitProcess(int);
	__declspec(dllimport) void OutputDebugStringW(const wchar_t*);
}

void WinMainCRTStartup()
{
	entryPoint();
}
#else
#include "efi.h"

void ConcatChar(wchar_t* s, wchar_t v);
void ConcatDigit(wchar_t* s, int v, int width);
void OutputDebugStringW(const wchar_t* s);

EFI_STATUS _DllMainCRTStartup(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st)
{
    gSt = st;

    EFI_GUID guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    st->BootServices->LocateProtocol(&guid, nullptr, (void**)&gp);
	st->BootServices->SetWatchdogTimer(0, 0, 0, nullptr);

	st->ConOut->ClearScreen(st->ConOut);
	st->ConOut->OutputString(st->ConOut, (CHAR16*)L"Booted.\r\n");

    EFI_TIME startedTime;
    st->RuntimeServices->GetTime(&startedTime, nullptr);

	entryPoint();

    EFI_TIME stoppedTime;
    st->RuntimeServices->GetTime(&stoppedTime, nullptr);
    {
        wchar_t s[128] = {};
        auto duration = 0;

        auto b = startedTime.Hour*36000+startedTime.Minute*600+startedTime.Second*10+(startedTime.Nanosecond/1000)/100;
        auto e = stoppedTime.Hour*36000+stoppedTime.Minute*600+stoppedTime.Second*10+(stoppedTime.Nanosecond/1000)/100;
        auto d = e - b;
        auto sec = stoppedTime.Second - startedTime.Second;
        auto ms = (stoppedTime.Nanosecond - startedTime.Nanosecond) / 1000;
        ConcatDigit(s, d/600, 2); d = d % 600; 
        ConcatChar(s, L':');
        ConcatDigit(s, d/10, 2); d = d % 10;
        ConcatChar(s, L'.');
        ConcatDigit(s, d, 1);
        OutputDebugStringW(s);
    }

    //TODO: Implement shutdown.
    while(1){};

	return 0;
}

void ConcatChar(wchar_t* s, wchar_t v)
{
    auto* headPtr = s;
    while(*headPtr){
        ++headPtr;
    }
    *headPtr++ = v;
    *headPtr++ = L'\0';
}

void ConcatDigit(wchar_t* s, int v, int width)
{
    auto* headPtr = s;
    while(*headPtr){
        ++headPtr;
    }

    int start = 1;
    for(int i=1; i<width; ++i){
        start *= 10;
    }

    bool hit = true;// 
    for(int i=start; i>0; i/=10){
        int d = v/i;
        if(d){
            *headPtr++ = L'0' + d;
            hit = true;
        }else{
            if(hit){
                *headPtr++ = L'0';
            }
        }
        v -= d*i;
        if(v==0){
            break;
        }
    }
    *headPtr++ = L'\0';
}

void OutputDebugStringW(const wchar_t* s)
{
	gSt->ConOut->OutputString(gSt->ConOut, (CHAR16*)s);
}
void ExitProcess(int)
{
	OutputDebugStringW(L"ExitProcess.\r\n");
}
void* GlobalAlloc(unsigned int, unsigned int size)
{
    void* p = nullptr;
    auto ret = gSt->BootServices->AllocatePool(EfiLoaderData, size, &p);
    OutputDebugStringW(L"Allocated memory.\r\n");
    if(ret != 0){
        OutputDebugStringW(L"Memory allocation failed.\r\n");
        while(1){}
    }
	return p;
}
#endif

extern "C" {
	float sqrt(float x)
	{
		union
		{
			int i;
			float x;
		} u;
		u.x = x;
		u.i = (1<<29) + (u.i >> 1) - (1<<22); 

		u.x += x/u.x;
		u.x = 0.25f*u.x + x/u.x;
		return u.x;
	}

	float sin(float x)
	{
		return x - (1.0f/(6.0f)) * x*x*x
				+ (1.0f/(6.0f*20.0f)) * x*x*x*x*x
				- (1.0f/(6.0f*20.0f*43.0f)) * x*x*x*x*x*x*x
				+ (1.0f/(6.0f*20.0f*43.0f*72.0f)) * x*x*x*x*x*x*x*x*x
				- (1.0f/(6.0f*20.0f*43.0f*72.0f*121.0f)) * x*x*x*x*x*x*x*x*x*x*x;		
	}

	float cos(float x)
	{
		return 1.0f - (0.5f) * x*x
				+ (1.0f/(24.0f)) * x*x*x*x
				- (1.0f/(24.0f*30.0f)) * x*x*x*x*x*x
				+ (1.0f/(24.0f*30.0f*56.0f)) * x*x*x*x*x*x*x*x
				- (1.0f/(24.0f*30.0f*56.0f*90.0f)) * x*x*x*x*x*x*x*x*x*x
				+ (1.0f/(24.0f*30.0f*56.0f*90.0f*132.0f)) * x*x*x*x*x*x*x*x*x*x*x*x;
	}

	float pow(float x, float s)
	{
		float r = x;
		for(float i=0; i< s; i+=1.0f){
			x *= x;
		}
		return r;
	}

	void* memcpy(void* dst, const void* src, unsigned long long  sz)
	{
		unsigned char* r = (unsigned char*)src;
		unsigned char* w = (unsigned char*)dst;
		while(sz){
			*w = *r;
			++w; ++r; --sz;
		}
		return dst;
	}

	void* allocate(unsigned long long sz);
}

extern void* operator new[](unsigned long long s)
{
	return allocate(s);
}

#include "radiance.h"
#include "ppm.h"
#include "random.h"

int tov(float v)
{
    return (int)(edupt::clamp(v) * 255 + 0.5f);
}

void display(const edupt::Color* image, int startHeight, int endHeight, int width, int height)
{
    bool flip = false;
    if(startHeight > endHeight){
        auto tmp = startHeight;
        startHeight = endHeight;
        endHeight = tmp;
        flip = true;
    }
    endHeight = endHeight > height ? height : endHeight;

#if ARCH_UEFI
    auto* headPtr = (unsigned char*)gp->Mode->FrameBufferBase;
    auto* currentPtr = headPtr;

    const auto vr = gp->Mode->Info->VerticalResolution;
    const auto hr = gp->Mode->Info->HorizontalResolution;

    currentPtr += startHeight * hr * 4;

if(!flip)
{
    for(int y=startHeight; y<vr; ++y) {
        if(y>=endHeight){
            break;
        }
        for(int x=0; x<hr; ++x){
            if(x>=width){
                currentPtr += (hr-width)*4;
                break;
            }
			const int image_index = (height - y - 1) * width + x;
            const auto& pixel = image[image_index];
            int v = x > 255 ? 255 : x;
            *currentPtr++ = tov(pixel.z);
            *currentPtr++ = tov(pixel.y);
            *currentPtr++ = tov(pixel.x);
            *currentPtr++ = 255;
        }
    }
}else{
    for(int y=vr-1; y>=startHeight; --y) {
        if(y>=endHeight){
            continue;
        }
        for(int x=0; x<hr; ++x){
            if(x>=width){
                currentPtr += (hr-width)*4;
                break;
            }
			const int image_index = (height - y - 1) * width + x;
            const auto& pixel = image[image_index];
            int v = x > 255 ? 255 : x;
            *currentPtr++ = tov(pixel.z);
            *currentPtr++ = tov(pixel.y);
            *currentPtr++ = tov(pixel.x);
            *currentPtr++ = 255;
        }
    }
}
#endif
}

namespace edupt {

int render(const int width, const int height, const int samples, const int supersamples) {
	// カメラ位置
	const Vec camera_position = Vec(50.0, 52.0, 220.0);
	const Vec camera_dir      = normalize(Vec(0.0, -0.04, -1.0));
	const Vec camera_up       = Vec(0.0, 1.0, 0.0);

	// ワールド座標系でのスクリーンの大きさ
	const double screen_width = 30.0 * width / height;
	const double screen_height= 30.0;
	// スクリーンまでの距離
	const double screen_dist  = 40.0;
	// スクリーンを張るベクトル
	const Vec screen_x = normalize(cross(camera_dir, camera_up)) * screen_width;
	const Vec screen_y = normalize(cross(screen_x, camera_dir)) * screen_height;
	const Vec screen_center = camera_position + camera_dir * screen_dist;

	Color *image = new Color[width * height];

    OutputDebugStringW(L"Rendering...");
	// std::cout << width << "x" << height << " " << samples * (supersamples * supersamples) << " spp" << std::endl;

	// OpenMP
// #pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int y = 0; y < height; y ++) {
#if 0
#if ARCH_UEFI
        gSt->ConOut->ClearScreen(gSt->ConOut);
        wchar_t s[64] = {};
        memcpy(s, L"Rendering... ", 13*sizeof(wchar_t));
        ConcatDigit(s, 100.0 * y / (height - 1), 2);
        ConcatChar(s, '%');
		OutputDebugStringW(s);
#else
		OutputDebugStringW(L"Rendering %\r\n");
#endif
#endif
		// std::cerr << "Rendering (y = " << y << ") " << (100.0 * y / (height - 1)) << "%" << std::endl;
        // display(image, y-1, y, width, height);

		Random rnd(y + 1);
		for (int x = 0; x < width; x ++) {
			const int image_index = (height - y - 1) * width + x;
			// supersamples x supersamples のスーパーサンプリング
			for (int sy = 0; sy < supersamples; sy ++) {
				for (int sx = 0; sx < supersamples; sx ++) {
					Color accumulated_radiance = Color();
					// 一つのサブピクセルあたりsamples回サンプリングする
					for (int s = 0; s < samples; s ++) {
						const double rate = (1.0 / supersamples);
						const double r1 = sx * rate + rate / 2.0;
						const double r2 = sy * rate + rate / 2.0;
						// スクリーン上の位置
						const Vec screen_position = 
							screen_center + 
							screen_x * ((r1 + x) / width - 0.5) +
							screen_y * ((r2 + y) / height- 0.5);
						// レイを飛ばす方向
						const Vec dir = normalize(screen_position - camera_position);

						accumulated_radiance = accumulated_radiance + 
							radiance(Ray(camera_position, dir), &rnd, 0) / samples / (supersamples * supersamples);
					}
					image[image_index] = image[image_index] + accumulated_radiance;
				}
			}
		}
	}
	
	OutputDebugStringW(L"Finished.\r\n");
	// 出力
	// save_ppm_file(std::string("image.ppm"), image, width, height);
    display(image, height, 0, width, height);
	return 0;
}


};

#endif
