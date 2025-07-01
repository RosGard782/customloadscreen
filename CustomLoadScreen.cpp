#include "CustomLoadScreen.h"

CustomLoadScreen::CustomLoadScreen(QObject *parent) : QObject(parent)
{
    // Constructor

    //Register present for draw on screen
    QObject::connect(g_class.DirectX, &CDirectX::Present,
                     this,            &CustomLoadScreen::Present);
}

CustomLoadScreen::~CustomLoadScreen()
{
    // Destructor
    g_class.DirectX->d3d9_ReleaseFont(pFont);
    g_class.DirectX->d3d9_ReleaseTexture(pTexture);
}

void CustomLoadScreen::Loop()
{
    // Main loop
}

bool CustomLoadScreen::Event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Events

    return true;
}

HRESULT CustomLoadScreen::Present(const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
    if ((g_vars.gameSatate >= 7 && GetModuleHandleA("samp.dll") == 0) || g_vars.gameSatate == 9)
        return D3D_OK;

    if (!init){
        init = true;
        pFont = g_class.DirectX->d3d9_CreateFont("Arial", 11, 5);
        pTexture = g_class.DirectX->d3d9_CreateTexture(g_class.params->BackBufferWidth,
                                                       g_class.params->BackBufferHeight);
        pTexture->Load("CustomLoadScreen.png");
        //pTexture->textureSizeAsBkg();
    }

    if (!init)
        return D3D_OK;

    // Render
    pTexture->Begin();
    pTexture->Clear(eCdBlack);

    // Нарисовать полоску загрузки внизу
    int barWidth = g_class.params->BackBufferWidth - 40; // Отступы по 20px слева и справа
    int barHeight = 20;
    int barX = 20;
    int barY = g_class.params->BackBufferHeight - barHeight - 40; // 40px от низа

    // Процент загрузки (0-100)
    float percent = 11.111111f * g_vars.gameSatate;
    if (percent > 100.0f) percent = 100.0f;
    if (percent < 0.0f) percent = 0.0f;

    int filledWidth = static_cast<int>(barWidth * (percent / 100.0f));

    // Рамка полосы
    pTexture->DrawRect(barX, barY, barWidth, barHeight, 0xFF444444); // Тёмно-серый контур

    // Фон полосы
    pTexture->DrawRect(barX + 2, barY + 2, barWidth - 4, barHeight - 4, 0xFF222222); // Более тёмный фон

    // Заполненная часть полосы
    pTexture->DrawRect(barX + 2, barY + 2, filledWidth - 4, barHeight - 4, 0xFF00AAFF); // Голубая полоска

    // Текст процента по центру полосы
    QString percentText = QString::number(static_cast<int>(percent)) + "%";
    int textWidth = pFont->GetTextWidth(percentText);
    int textX = barX + (barWidth - textWidth) / 2;
    int textY = barY + (barHeight - pFont->GetTextHeight()) / 2;
    pFont->PrintShadow(textX, textY, -1, percentText);

    pTexture->End();
    pTexture->Render(0, 0, g_class.params->BackBufferWidth, g_class.params->BackBufferHeight); // Draw texture

    return D3D_OK;
}
