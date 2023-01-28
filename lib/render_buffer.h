class RenderBuffer
{
private:
    unsigned int m_RenderBufferId;
public:
    RenderBuffer();
    ~RenderBuffer();

    void attach(unsigned int width, unsigned int height);
};
