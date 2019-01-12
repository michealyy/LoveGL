#include <glad/glad.h>
//#include <FreeImage.h>
#include "Texture.h"
#include "engine.h"

using namespace std;
using namespace kd;

Texture::Texture(const std::string& name) {
	auto tex = Engine::GetInstance()->GetTexture(name);
	if (tex != nullptr) {
		fprintf(stderr, "[Texture] have same name texture: %s\n", name.c_str());
		return;
	}
	name_ = name;
	Engine::GetInstance()->AddTexture(name, this);
}

Texture::~Texture(){
	glDeleteTextures(1, &texture_id_);
}

void Texture::Bind() {
	if (texture_id_ == 0) {
		fprintf(stderr, "[Texture] opengl texture id error: %s\n", name_.c_str());
	}
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::LoadFormFile(const std::string& path) {
	// unsigned texture(0);
	// bool loadSuccess = false;
	// auto c_path = path.c_str();

	// FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(c_path, 0);
	// if (fif == FIF_UNKNOWN)
	// 	fif = FreeImage_GetFIFFromFilename(c_path);

	// if (fif != FIF_UNKNOWN) {
	// 	if (FreeImage_FIFSupportsReading(fif)) {
	// 		FIBITMAP* dib = FreeImage_Load(fif, c_path);
	// 		if (dib) {
	// 			//FIT_BITMAP:һ������һbit�������洢��ʽ����ͨ�����᲻��
	// 			if (FreeImage_GetImageType(dib) == FIT_BITMAP) {
	// 				//FreeImage_FlipVertical(dib);
	// 				BYTE* bits(0);
	// 				unsigned int width(0), height(0);
	// 				bits = FreeImage_GetBits(dib);
	// 				width = FreeImage_GetWidth(dib);
	// 				height = FreeImage_GetHeight(dib);
	// 				this->width_ = width;
	// 				this->height_ = height;
	// 				if (bits != 0 && width != 0 && height != 0) {
	// 					glGenTextures(1, &texture);
	// 					glBindTexture(GL_TEXTURE_2D, texture);
	// 					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// 					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 					//ͨ��ÿ����ռ�ÿռ����ж�ͨ������24��ʾ3ͨ��
	// 					if (FreeImage_GetBPP(dib) == 24) {
	// 						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	// 						this->texture_id_ = texture;
	// 						loadSuccess = true;
	// 					}
	// 					else if (FreeImage_GetBPP(dib) == 32) {
	// 						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
	// 						this->texture_id_ = texture;
	// 						loadSuccess = true;
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	// if (!loadSuccess) {
	// 	fprintf(stderr, "[Texture] Texture File Not Find or Not Support: %s\n", path.c_str());
	// }
}
