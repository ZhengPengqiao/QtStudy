#ifndef CLAYERSETUP_H
#define CLAYERSETUP_H

#include <linux/mxcfb.h>

#include <QString>
#include <QRgb>
#include <QColor>

class QImage;

class CLayerSetup
{
public:
    ~CLayerSetup();

    static CLayerSetup *getInstance();

    void show();
    void hide();

    void setAlpha(int alpha);
    void setColorKey(QRgb &color);
    void setColorKey(QColor &color);

    QColor getAlphaColor() { return m_AlphaColor; }

    void enableLocalAlpha(bool enable = true);
    void switchNextAlphaBuffer();

    QImage *getNextAlphaImage();

protected:
    CLayerSetup();

private:
    int m_nFD;
    bool m_bCurBuf;
    struct mxcfb_gbl_alpha m_gbl_alpha;
    struct mxcfb_loc_alpha m_loc_alpha;
    struct mxcfb_color_key m_color_key;
    uchar *m_vaddr[2];
    unsigned long m_paddr[2];

    int m_nSW;
    int m_nSH;

    QColor m_AlphaColor;

    QImage *m_pAlphaImage[2];

    static CLayerSetup *s_obj;
};

#endif // CLAYERSETUP_H
