#include "clayersetup.h"

#include <QDebug>
#include <QImage>

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <sys/mman.h>

CLayerSetup *CLayerSetup::s_obj = NULL;

CLayerSetup *CLayerSetup::getInstance()
{
    if(!s_obj)
        s_obj = new CLayerSetup();
    return s_obj;
}

CLayerSetup::CLayerSetup()
    : m_bCurBuf(true)
    , m_AlphaColor(QColor(Qt::magenta))
{
    struct mxcfb_pos pos;
    struct fb_var_screeninfo vinfo;

    int fd0 = ::open("/dev/fb0", O_RDWR);
    m_nFD = ::open("/dev/fb1", O_RDWR);
    assert(m_nFD > 0);

    if (::ioctl(fd0, FBIOGET_VSCREENINFO, &vinfo)) {
        ::perror("FBIOGET_VSCREENINFO");
    }
    ::close(fd0);

    if(::ioctl(m_nFD, FBIOPUT_VSCREENINFO, &vinfo)) {
        ::perror("FBIOPUT_VSCREENINFO");
    }

    pos.x = 0;
    pos.y = 0;
    if (::ioctl(m_nFD, MXCFB_SET_OVERLAY_POS, &pos) < 0)
    {
        ::perror("MXCFB_SET_OVERLAY_POS");
    }

    m_nSW = vinfo.xres;
    m_nSH = vinfo.yres;

    int sz = m_nSW * m_nSH;

    m_loc_alpha.enable = 1;
    m_loc_alpha.alpha_in_pixel = 0;
    m_loc_alpha.alpha_phy_addr0 = 0;
    m_loc_alpha.alpha_phy_addr1 = 0;
    ::ioctl (m_nFD, MXCFB_SET_LOC_ALPHA, &m_loc_alpha);

    m_paddr[0] = m_loc_alpha.alpha_phy_addr0;
    m_paddr[1] = m_loc_alpha.alpha_phy_addr1;
    m_vaddr[0] = (uchar *)mmap (NULL, sz, PROT_READ | PROT_WRITE,
                        MAP_SHARED, m_nFD, m_paddr[0]);
    m_vaddr[1] = (uchar *)mmap (NULL, sz, PROT_READ | PROT_WRITE,
                        MAP_SHARED, m_nFD, m_paddr[1]);

    printf("LocalAlpha p[%p, %p], v[%p, %p]\n",
           (void*)m_paddr[0], (void*)m_paddr[1], (void*)m_vaddr[0], (void*)m_vaddr[1]);

    m_pAlphaImage[0] = new QImage(m_vaddr[0], m_nSW, m_nSH, QImage::Format_Grayscale8);
    m_pAlphaImage[1] = new QImage(m_vaddr[1], m_nSW, m_nSH, QImage::Format_Grayscale8);

    memset(m_vaddr[0], 0xFF,  sz);
    memset(m_vaddr[1], 0xFF,  sz);
    memset(m_vaddr[0] + m_nSW * 550, 0x80,  m_nSW * 120);
    memset(m_vaddr[1] + m_nSW * 450, 0x80,  m_nSW * 120);
    switchNextAlphaBuffer();

    m_gbl_alpha.enable = 0;
    m_gbl_alpha.alpha = 0;
    ::ioctl(m_nFD, MXCFB_SET_GBL_ALPHA, &m_gbl_alpha);

    m_color_key.enable = 1;
    m_color_key.color_key = m_AlphaColor.rgb();
    ::ioctl(m_nFD, MXCFB_SET_CLR_KEY, &m_color_key);
}

CLayerSetup::~CLayerSetup()
{
    hide();
    delete m_pAlphaImage[0];
    delete m_pAlphaImage[1];
    munmap(m_vaddr[0], m_nSW * m_nSH);
    munmap(m_vaddr[1], m_nSW * m_nSH);
    ::close(m_nFD);
}

void CLayerSetup::show()
{
    ::ioctl(m_nFD, FBIOBLANK, FB_BLANK_UNBLANK);
}

void CLayerSetup::hide()
{
    ::ioctl(m_nFD, FBIOBLANK, FB_BLANK_NORMAL);
}

void CLayerSetup::setAlpha(int alpha)
{
    m_gbl_alpha.alpha = alpha;
    ::ioctl(m_nFD, MXCFB_SET_GBL_ALPHA, &m_gbl_alpha);
}

void CLayerSetup::setColorKey(QRgb &color)
{
    m_AlphaColor = QColor(color);
    m_color_key.color_key = m_AlphaColor.rgb();
    ::ioctl(m_nFD, MXCFB_SET_CLR_KEY, &m_color_key);
}

void CLayerSetup::setColorKey(QColor &color)
{
    m_AlphaColor = color;
    m_color_key.color_key = color.rgb();
    ::ioctl(m_nFD, MXCFB_SET_CLR_KEY, &m_color_key);
}

void CLayerSetup::enableLocalAlpha(bool )
{

}

void CLayerSetup::switchNextAlphaBuffer()
{
    m_bCurBuf = !m_bCurBuf;
    if (::ioctl (m_nFD, MXCFB_SET_LOC_ALP_BUF, &m_paddr[m_bCurBuf]) < 0)
        ::perror("MXCFB_SET_LOC_ALP_BUF");
}

QImage *CLayerSetup::getNextAlphaImage()
{
//    qDebug() << "get next idx =" << !m_bCurBuf;
    return m_pAlphaImage[!m_bCurBuf];
}
