#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>

using namespace std;
typedef enum {LAB,HSV} COLOR_TYPE;

class image
{
private:
	string fileName;
	string filePath;
public:
	image(COLOR_TYPE,const string&);

};


#endif