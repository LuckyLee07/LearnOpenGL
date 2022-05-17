#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

class FrameBuffer
{
private:
    unsigned int m_BufferId;
    unsigned int m_TextureId;
    unsigned int m_RenderId;

public:
    FrameBuffer();
    ~FrameBuffer();

    void Bind() const;
    void Unbind() const;
    void Delete() const;

    unsigned int GetTexID();
};

#endif