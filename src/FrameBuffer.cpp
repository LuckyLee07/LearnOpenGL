#include "FrameBuffer.h"
#include "Common.h"

FrameBuffer::FrameBuffer()
{
    // framebuffer configuration
	glGenFramebuffers(1, &m_BufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);

    float Tex_Width = SCR_WIDTH;
    float Tex_Height = SCR_HEIGHT;
    
#ifdef __APPLE__//Retina屏设置
    Tex_Width = SCR_WIDTH * 2;
    Tex_Height = SCR_HEIGHT * 2;
#endif
    // create a color attachment texture
	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Tex_Width, Tex_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &m_RenderId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Tex_Width, Tex_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderId);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_INFO("Fxkk=======>>> %s", "Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    //GLCall(glDeleteBuffers(1, &m_BufferId));
}

void FrameBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &m_BufferId));
}

void FrameBuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId));
}

void FrameBuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int FrameBuffer::GetTexID()
{
    return m_TextureId;
}