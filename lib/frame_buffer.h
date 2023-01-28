class FrameBuffer
{
private:
    unsigned int m_FrameBufferId;
public:
    FrameBuffer();
    ~FrameBuffer();
    void bind() const;
    void unbind() const;
    bool checkComplete() const;
};
