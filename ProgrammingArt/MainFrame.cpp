#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <stdint.h>

class Color {
    static uint32_t makeColor32(int r, int g, int b){
        uint32_t color =0;
        return color;
    }
};

class RGBImage {
public:
    RGBImage(int width, int height){
        m_data = nullptr;
        m_ownsData = true;
        create(width, height);
    }
    
    ~RGBImage(){
        release();
    }
    
    void release(){
        if(m_data != nullptr && m_ownsData){
            free(m_data);
        }
        m_data = nullptr;
        m_width = 0;
        m_height = 0;
    }
    
    /** if called, it will not deallocate
     * currently associated image data
     */
    void releaseOwnership(){
        m_ownsData = false;
    }
    
    void create(int width, int height){
        release();
        m_data = (uint8_t*)malloc(width*height*3);
        m_width = width;
        m_height = height;
        m_ownsData = true;
    }
    
    uint8_t *data(){
        return m_data;
    }
    
    uint8_t *at(int x, int y){
        return m_width*3*y + x*3 + m_data;
    }
    
    void setPixel(int x, int y, uint32_t color){
        uint8_t *address = at(x, y);
        address[0] = color & 0xFF;
        address[1] = (color >> 8) & 0xFF;
        address[2] = (color >> 16) & 0xFF;
    }
    int width(){
        return m_width;
    }
    
    int height(){
        return m_height;
    }
private:
    int m_width;
    int m_height;
    uint8_t *m_data;
    
    bool m_ownsData;
};
MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    RGBImage image(300, 300);
    
    for(int y = 0; y < image.height(); ++y){
        for(int x = 0; x < image.width(); ++x){
            image.setPixel(x, y, x*y ^ ((x*y) << 10));
        }
    }
    
    wxImage pidImage(image.width(), image.height(), image.data());
    image.releaseOwnership();
    
    wxBitmap bitmap(pidImage);
    
    m_outputBitmap->SetBitmap(bitmap);
}

MainFrame::~MainFrame()
{

}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}
