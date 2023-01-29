#include <vector>
#include <string>

class CubeTexture
{
private:
    unsigned int m_CubeTextureId;
public:
    CubeTexture(const std::vector<std::string>& images);
    void bind(unsigned int slot = 0) const;
    void unbind() const;
};
