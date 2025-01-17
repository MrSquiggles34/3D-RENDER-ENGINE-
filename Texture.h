#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D {
	public: 
		// Hold ID of texture object
		unsigned int ID;

		unsigned int Width, Height;

		// Texture format
		unsigned int Internal_Format;
		unsigned int Image_Format;

		// Texture configuration
		unsigned int Wrap_S; // wrapping mode on S axis
		unsigned int Wrap_T; // wrapping mode on T axis
		unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
		unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels

		// constructor (sets default texture modes)
		Texture2D();

		// generates texture from image data
		void Generate(unsigned int width, unsigned int height, unsigned char* data);
		
		// binds the texture as the current active GL_TEXTURE_2D texture object
		void Bind() const;
};

#endif // TEXTURE_H
