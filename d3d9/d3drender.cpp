/*

    PROJECT:		mod_sa
    LICENSE:		See LICENSE in the top level directory
    COPYRIGHT:		Copyright we_sux, BlastHack

    mod_sa is available from https://github.com/BlastHackNet/mod_s0beit_sa/

    mod_sa is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mod_sa is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
////////////////////////////////////////////////////////////////////////
// ... (оставьте весь ваш существующий код выше)
//

#include "d3drender.h"
#define M_PI       3.14159265358979323846

// === ДЛЯ ПОЛОСЫ ЗАГРУЗКИ ===
#include <vector>

// Глобальный прогресс (0.0f ... 1.0f)
float g_LoadProgress = 0.0f;

// Подключите свои переменные для фоновой текстуры и размеров.
// Определите их где вы загружаете фон, здесь только extern!
extern IDirect3DTexture9* g_BackgroundTexture;
extern int g_BackgroundWidth;
extern int g_BackgroundHeight;

// Получение цветов из горизонтальной линии фонового изображения
std::vector<D3DCOLOR> GetBarColorsFromImage(IDirect3DTexture9* texture, int width, int height) {
    std::vector<D3DCOLOR> colors(width, 0xFFFFFFFF);
    if(!texture) return colors;
    D3DLOCKED_RECT rect;
    // Берём строку на 90% высоты
    int y = (int)(height * 0.9);
    if (SUCCEEDED(texture->LockRect(0, &rect, 0, D3DLOCK_READONLY))) {
        for (int x = 0; x < width; ++x) {
            BYTE* pPixel = (BYTE*)rect.pBits + y * rect.Pitch + x * 4;
            D3DCOLOR color = *(D3DCOLOR*)pPixel;
            colors[x] = color;
        }
        texture->UnlockRect(0);
    }
    return colors;
}

// Функция рисования полосы загрузки
void DrawLoadingBar(IDirect3DDevice9* dev, float progress) {
    if(!g_BackgroundTexture || g_BackgroundWidth <= 0 || g_BackgroundHeight <= 0) return;
    const int barHeight = 12;
    int screenW = g_BackgroundWidth;
    int screenH = g_BackgroundHeight;
    int barY = screenH - barHeight - 10;

    // Кэшируем цвета чтобы не тянуть каждый кадр
    static std::vector<D3DCOLOR> colors;
    static int cachedW = 0, cachedH = 0;
    if (cachedW != screenW || cachedH != screenH || colors.empty()) {
        colors = GetBarColorsFromImage(g_BackgroundTexture, screenW, screenH);
        cachedW = screenW;
        cachedH = screenH;
    }

    struct VERTEX { float x, y, z, rhw; D3DCOLOR color; };
    std::vector<VERTEX> verts;
    verts.reserve(screenW * 2);

    for (int x = 0; x < screenW; ++x) {
        D3DCOLOR color = colors[x];
        verts.push_back({(float)x, (float)(barY + barHeight), 0.0f, 1.0f, color});
        verts.push_back({(float)x, (float)barY, 0.0f, 1.0f, color});
    }

    int filled = (int)(screenW * progress);
    if (filled <= 1) return;

    dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    dev->SetTexture(0, NULL);
    dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (filled - 1) * 2, verts.data(), sizeof(verts[0]));
    dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
// === КОНЕЦ: для полосы загрузки ===

// ... (весь остальной ваш исходный код ниже)
