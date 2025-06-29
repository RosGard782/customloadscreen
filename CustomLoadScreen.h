#ifndef CustomLoadScreen_H
#define CustomLoadScreen_H

#include <QObject>
#include "loader.h"
#include "d3d9/d3drender.h"
#include "d3d9/texture.h"
#include "d3d9/MenuManager/Menu.h"
#include "d3d9/MenuManager/ContextMenu.h"
#include "d3d9/MenuManager/Text.h"
#include "d3d9/MenuManager/Node.h"
#include "d3d9/MenuManager/Listing.h"
#include "d3d9/MenuManager/VerticalLayout.h"
#include "d3d9/MenuManager/CSlider.h"

class CustomLoadScreen : public QObject
{
    Q_OBJECT
public:
    explicit CustomLoadScreen(QObject *parent = 0);
    virtual ~CustomLoadScreen();

    virtual void Loop();
    virtual bool Event(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CD3DFont *pFont;
    SRTexture* pTexture;
    bool init = false;

signals:

public slots:
    HRESULT Present ( CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride,
                      CONST RGNDATA *pDirtyRegion );
};

#endif // CustomLoadScreen_H
