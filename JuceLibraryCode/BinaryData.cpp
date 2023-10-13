/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== basic.frag ==================
static const unsigned char temp_binary_data_0[] =
"#version 450\r\n"
"\r\n"
"layout(location = 0) in vec4 fragColor;  // Input color from vertex shader\r\n"
"layout(location = 0) out vec4 outColor;  // Output color\r\n"
"\r\n"
"void main() {\r\n"
"    outColor = fragColor;\r\n"
"}\r\n";

const char* basic_frag = (const char*) temp_binary_data_0;

//================== basic.vert ==================
static const unsigned char temp_binary_data_1[] =
"#version 450\r\n"
"\r\n"
"layout(location = 0) in vec3 inPosition; // Vertex position\r\n"
"layout(location = 1) in vec4 inColor;    // Vertex color\r\n"
"\r\n"
"layout(location = 0) out vec4 fragColor; // Outputted color for the fragment shader\r\n"
"\r\n"
"void main() {\r\n"
"    gl_Position = vec4(inPosition, 1.0);\r\n"
"    fragColor = inColor;\r\n"
"}\r\n";

const char* basic_vert = (const char*) temp_binary_data_1;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xbcfb6763:  numBytes = 197; return basic_frag;
        case 0xbd027ea2:  numBytes = 310; return basic_vert;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "basic_frag",
    "basic_vert"
};

const char* originalFilenames[] =
{
    "basic.frag",
    "basic.vert"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
