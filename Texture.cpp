#include "Texture.h"

Texture::Texture() {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileloc) {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileloc;
}

bool Texture::LoadTexture() {
	int channels = 0;
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &channels, 0);
	if (!texData) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	if (width <= 0 || height <= 0) {
		printf("Invalid image dimensions: %dx%d for %s\n", width, height, fileLocation);
		stbi_image_free(texData);
		return false;
	}

	GLenum format = GL_RGB;
	if (channels == 4) {
		format = GL_RGBA;
	} else if (channels == 3) {
		format = GL_RGB;
	} else if (channels == 1) {
		format = GL_RED;
	} else {
		// Fallback: treat as RGB to avoid passing an invalid format to the driver
		format = GL_RGB;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Ensure no row-alignment padding will cause the driver to read past the buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	this->bitDepth = channels;

	return true;
}

void Texture::LoadDefaultTexture()
{
	const int width = 64;
	const int height = 64;
	const int checkerSize = 8;
	const int channels = 3; // R, G, B

	// Create a raw byte array to hold our pixel data
	unsigned char data[width * height * channels];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// This math flips between true and false every 'checkerSize' pixels
			bool isPurple = ((x / checkerSize) % 2) == ((y / checkerSize) % 2);

			int index = (y * width + x) * channels;

			if (isPurple)
			{
				data[index] = 255;		// Red
				data[index + 1] = 0;	// Green
				data[index + 2] = 255;	// Blue (Red + Blue = Purple!)
			}
			else
			{
				data[index] = 0;		// Red
				data[index + 1] = 0;	// Green
				data[index + 2] = 0;	// Blue (Black)
			}
		}
	}

	// Now we feed this raw array into OpenGL exactly like we loaded STB images!
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// IMPORTANT: We use GL_NEAREST instead of GL_LINEAR here.
	// This stops OpenGL from blurring the edges, keeping the checkerboard sharp and blocky!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Push the data to the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	this->width = width;
	this->height = height;
	this->bitDepth = channels;
}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture() {

}