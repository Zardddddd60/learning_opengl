class TextureFrameBuffer
{
public:
    TextureFrameBuffer();
    ~TextureFrameBuffer();

    void attach(unsigned int width, unsigned int height) const;
    void bind(unsigned int slot = 0) const;

private:
    unsigned int m_TextureFrameBufferId;
};
