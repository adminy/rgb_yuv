#include <string>
#include <fstream>

using namespace std;

unsigned char * flip(unsigned char * buffer, unsigned width, unsigned height) {
    // https://stackoverflow.com/a/26706165/7055163
    unsigned char * rgb = new unsigned char[width * height * 3];
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            rgb[(i + j * width) * 3 + 0] = buffer[(i + (height - 1 - j) * width) * 3 + 0];
            rgb[(i + j * width) * 3 + 1] = buffer[(i + (height - 1 - j) * width) * 3 + 1];
            rgb[(i + j * width) * 3 + 2] = buffer[(i + (height - 1 - j) * width) * 3 + 2];
        }
    }
    return rgb;
}

string readBMP(const char* file_name) {
    ifstream bmpFile(file_name);
	string bmpStrData((istreambuf_iterator<char>(bmpFile)), istreambuf_iterator<char>());
    const int width = (int) bmpStrData[18];
    const int height = (int) bmpStrData[22];
    const int size = width * height * 3; //3 bytes per pixels
    int header = 138;
    for(int i = header; i < header + size; i += 3) {
        swap(bmpStrData[i], bmpStrData[i+2]); // flip the order of every 3 bytes
    }

    return bmpStrData.substr(header);
}


unsigned char* rgbToYuv420(int width, int height, unsigned char* rgb) {
    long length = width * height;
    long yuv420FrameLength = (float)length * 3 / 2;
    unsigned char *yuv = (unsigned char *)malloc(yuv420FrameLength * sizeof(unsigned char));
    int r, g, b, y, u, v, ypos, upos, vpos;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            r = (int)rgb[(j * width + i) * 3 + 0];
            g = (int)rgb[(j * width + i) * 3 + 1];
            b = (int)rgb[(j * width + i) * 3 + 2];
            y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            u = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
            v = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
            ypos = j * width + i;
            upos = (j / 2) * (width / 2) + (i / 2) + length;
            vpos = (j / 2) * (width / 2) + (i / 2) + length + (length / 4);
            yuv[ypos] = y;
            yuv[upos] = u;
            yuv[vpos] = v;
        }
    }
    return yuv;
}

int main( int argc, char** argv) {
    string rgb_pixels = readBMP("test.bmp");
    unsigned char *rgb = flip((unsigned char*) rgb_pixels.c_str(), 720, 480);
    unsigned char *yuv = rgbToYuv420(720, 480, rgb);
    free(rgb);
    //write yuv to file
    FILE* out = fopen("test.yuv", "w");
    fwrite(yuv, sizeof(unsigned char), 720 * 480 * 3 / 2, out);
    fclose(out);
}

