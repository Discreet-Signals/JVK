/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== Waveform.frag ==================
static const unsigned char temp_binary_data_0[] =
"#version 450\r\n"
"\r\n"
"layout(location = 0) in vec4 fragColor;  // Input color from vertex shader\r\n"
"layout(location = 0) out vec4 outColor;  // Output color\r\n"
"\r\n"
"layout(binding = 0) buffer BufferType\r\n"
"{\r\n"
"    float data[];\r\n"
"};\r\n"
"\r\n"
"float softMax(float x, float edge, float softness) {\r\n"
"    if (x > edge + softness) return x; // Above the soft transition range\r\n"
"    return mix(edge, x, smoothstep(edge, edge + softness, x));\r\n"
"}\r\n"
"\r\n"
"void main()\r\n"
"{\r\n"
"    float value = max(0.0, data[int(fragColor.r * 1023.999999)]);\r\n"
"    float pixel = 1.0 - fragColor.g;\r\n"
"    pixel = abs(2.0 * pixel - 1.0);\r\n"
"    bool waveform = pixel <= value;\r\n"
"    float dist = value - pixel;\r\n"
"\r\n"
"    vec3 background = vec3(0.1, 0.1, 0.1);\r\n"
"    vec3 col = vec3(0.0);\r\n"
"    float alpha = 0.0;\r\n"
"    if (waveform)\r\n"
"    {\r\n"
"        dist = 1.0 - dist;\r\n"
"        dist = 0.2 + 0.8 * cos((dist * dist) * 0.5 * 3.14159);\r\n"
"        col = smoothstep(background, vec3(1.0), vec3(0.825, 1.1, 0.675) * dist);\r\n"
"        alpha = dist;\r\n"
"    }\r\n"
"    \r\n"
"    outColor = vec4(col, alpha);\r\n"
"}\r\n";

const char* Waveform_frag = (const char*) temp_binary_data_0;

//================== Waveform.frag.spv ==================
static const unsigned char temp_binary_data_1[] =
{ 3,2,35,7,0,0,1,0,11,0,13,0,95,0,0,0,0,0,0,0,17,0,2,0,1,0,0,0,11,0,6,0,1,0,0,0,71,76,83,76,46,115,116,100,46,52,53,48,0,0,0,0,14,0,3,0,0,0,0,0,1,0,0,0,15,0,7,0,4,0,0,0,4,0,0,0,109,97,105,110,0,0,0,0,18,0,0,0,88,0,0,0,16,0,3,0,4,0,0,0,7,0,0,0,3,0,3,0,2,
0,0,0,194,1,0,0,4,0,10,0,71,76,95,71,79,79,71,76,69,95,99,112,112,95,115,116,121,108,101,95,108,105,110,101,95,100,105,114,101,99,116,105,118,101,0,0,4,0,8,0,71,76,95,71,79,79,71,76,69,95,105,110,99,108,117,100,101,95,100,105,114,101,99,116,105,118,101,
0,5,0,4,0,4,0,0,0,109,97,105,110,0,0,0,0,5,0,4,0,8,0,0,0,118,97,108,117,101,0,0,0,5,0,5,0,11,0,0,0,66,117,102,102,101,114,84,121,112,101,0,0,6,0,5,0,11,0,0,0,0,0,0,0,100,97,116,97,0,0,0,0,5,0,3,0,13,0,0,0,0,0,0,0,5,0,5,0,18,0,0,0,102,114,97,103,67,111,
108,111,114,0,0,0,5,0,4,0,31,0,0,0,112,105,120,101,108,0,0,0,5,0,5,0,44,0,0,0,119,97,118,101,102,111,114,109,0,0,0,0,5,0,4,0,48,0,0,0,100,105,115,116,0,0,0,0,5,0,5,0,54,0,0,0,98,97,99,107,103,114,111,117,110,100,0,0,5,0,3,0,57,0,0,0,99,111,108,0,5,0,
4,0,59,0,0,0,97,108,112,104,97,0,0,0,5,0,5,0,88,0,0,0,111,117,116,67,111,108,111,114,0,0,0,0,71,0,4,0,10,0,0,0,6,0,0,0,4,0,0,0,72,0,5,0,11,0,0,0,0,0,0,0,35,0,0,0,0,0,0,0,71,0,3,0,11,0,0,0,3,0,0,0,71,0,4,0,13,0,0,0,34,0,0,0,0,0,0,0,71,0,4,0,13,0,0,0,33,
0,0,0,0,0,0,0,71,0,4,0,18,0,0,0,30,0,0,0,0,0,0,0,71,0,4,0,88,0,0,0,30,0,0,0,0,0,0,0,19,0,2,0,2,0,0,0,33,0,3,0,3,0,0,0,2,0,0,0,22,0,3,0,6,0,0,0,32,0,0,0,32,0,4,0,7,0,0,0,7,0,0,0,6,0,0,0,43,0,4,0,6,0,0,0,9,0,0,0,0,0,0,0,29,0,3,0,10,0,0,0,6,0,0,0,30,0,3,
0,11,0,0,0,10,0,0,0,32,0,4,0,12,0,0,0,2,0,0,0,11,0,0,0,59,0,4,0,12,0,0,0,13,0,0,0,2,0,0,0,21,0,4,0,14,0,0,0,32,0,0,0,1,0,0,0,43,0,4,0,14,0,0,0,15,0,0,0,0,0,0,0,23,0,4,0,16,0,0,0,6,0,0,0,4,0,0,0,32,0,4,0,17,0,0,0,1,0,0,0,16,0,0,0,59,0,4,0,17,0,0,0,18,
0,0,0,1,0,0,0,21,0,4,0,19,0,0,0,32,0,0,0,0,0,0,0,43,0,4,0,19,0,0,0,20,0,0,0,0,0,0,0,32,0,4,0,21,0,0,0,1,0,0,0,6,0,0,0,43,0,4,0,6,0,0,0,24,0,0,0,0,0,128,68,32,0,4,0,27,0,0,0,2,0,0,0,6,0,0,0,43,0,4,0,6,0,0,0,32,0,0,0,0,0,128,63,43,0,4,0,19,0,0,0,33,0,0,
0,1,0,0,0,43,0,4,0,6,0,0,0,37,0,0,0,0,0,0,64,20,0,2,0,42,0,0,0,32,0,4,0,43,0,0,0,7,0,0,0,42,0,0,0,23,0,4,0,52,0,0,0,6,0,0,0,3,0,0,0,32,0,4,0,53,0,0,0,7,0,0,0,52,0,0,0,43,0,4,0,6,0,0,0,55,0,0,0,205,204,204,61,44,0,6,0,52,0,0,0,56,0,0,0,55,0,0,0,55,0,0,
0,55,0,0,0,44,0,6,0,52,0,0,0,58,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,43,0,4,0,6,0,0,0,65,0,0,0,205,204,76,62,43,0,4,0,6,0,0,0,66,0,0,0,205,204,76,63,43,0,4,0,6,0,0,0,70,0,0,0,0,0,0,63,43,0,4,0,6,0,0,0,72,0,0,0,208,15,73,64,44,0,6,0,52,0,0,0,78,0,0,0,32,0,0,
0,32,0,0,0,32,0,0,0,43,0,4,0,6,0,0,0,79,0,0,0,51,51,83,63,43,0,4,0,6,0,0,0,80,0,0,0,205,204,140,63,43,0,4,0,6,0,0,0,81,0,0,0,205,204,44,63,44,0,6,0,52,0,0,0,82,0,0,0,79,0,0,0,80,0,0,0,81,0,0,0,32,0,4,0,87,0,0,0,3,0,0,0,16,0,0,0,59,0,4,0,87,0,0,0,88,0,
0,0,3,0,0,0,54,0,5,0,2,0,0,0,4,0,0,0,0,0,0,0,3,0,0,0,248,0,2,0,5,0,0,0,59,0,4,0,7,0,0,0,8,0,0,0,7,0,0,0,59,0,4,0,7,0,0,0,31,0,0,0,7,0,0,0,59,0,4,0,43,0,0,0,44,0,0,0,7,0,0,0,59,0,4,0,7,0,0,0,48,0,0,0,7,0,0,0,59,0,4,0,53,0,0,0,54,0,0,0,7,0,0,0,59,0,4,0,
53,0,0,0,57,0,0,0,7,0,0,0,59,0,4,0,7,0,0,0,59,0,0,0,7,0,0,0,65,0,5,0,21,0,0,0,22,0,0,0,18,0,0,0,20,0,0,0,61,0,4,0,6,0,0,0,23,0,0,0,22,0,0,0,133,0,5,0,6,0,0,0,25,0,0,0,23,0,0,0,24,0,0,0,110,0,4,0,14,0,0,0,26,0,0,0,25,0,0,0,65,0,6,0,27,0,0,0,28,0,0,0,13,
0,0,0,15,0,0,0,26,0,0,0,61,0,4,0,6,0,0,0,29,0,0,0,28,0,0,0,12,0,7,0,6,0,0,0,30,0,0,0,1,0,0,0,40,0,0,0,9,0,0,0,29,0,0,0,62,0,3,0,8,0,0,0,30,0,0,0,65,0,5,0,21,0,0,0,34,0,0,0,18,0,0,0,33,0,0,0,61,0,4,0,6,0,0,0,35,0,0,0,34,0,0,0,131,0,5,0,6,0,0,0,36,0,0,
0,32,0,0,0,35,0,0,0,62,0,3,0,31,0,0,0,36,0,0,0,61,0,4,0,6,0,0,0,38,0,0,0,31,0,0,0,133,0,5,0,6,0,0,0,39,0,0,0,37,0,0,0,38,0,0,0,131,0,5,0,6,0,0,0,40,0,0,0,39,0,0,0,32,0,0,0,12,0,6,0,6,0,0,0,41,0,0,0,1,0,0,0,4,0,0,0,40,0,0,0,62,0,3,0,31,0,0,0,41,0,0,0,
61,0,4,0,6,0,0,0,45,0,0,0,31,0,0,0,61,0,4,0,6,0,0,0,46,0,0,0,8,0,0,0,188,0,5,0,42,0,0,0,47,0,0,0,45,0,0,0,46,0,0,0,62,0,3,0,44,0,0,0,47,0,0,0,61,0,4,0,6,0,0,0,49,0,0,0,8,0,0,0,61,0,4,0,6,0,0,0,50,0,0,0,31,0,0,0,131,0,5,0,6,0,0,0,51,0,0,0,49,0,0,0,50,
0,0,0,62,0,3,0,48,0,0,0,51,0,0,0,62,0,3,0,54,0,0,0,56,0,0,0,62,0,3,0,57,0,0,0,58,0,0,0,62,0,3,0,59,0,0,0,9,0,0,0,61,0,4,0,42,0,0,0,60,0,0,0,44,0,0,0,247,0,3,0,62,0,0,0,0,0,0,0,250,0,4,0,60,0,0,0,61,0,0,0,62,0,0,0,248,0,2,0,61,0,0,0,61,0,4,0,6,0,0,0,63,
0,0,0,48,0,0,0,131,0,5,0,6,0,0,0,64,0,0,0,32,0,0,0,63,0,0,0,62,0,3,0,48,0,0,0,64,0,0,0,61,0,4,0,6,0,0,0,67,0,0,0,48,0,0,0,61,0,4,0,6,0,0,0,68,0,0,0,48,0,0,0,133,0,5,0,6,0,0,0,69,0,0,0,67,0,0,0,68,0,0,0,133,0,5,0,6,0,0,0,71,0,0,0,69,0,0,0,70,0,0,0,133,
0,5,0,6,0,0,0,73,0,0,0,71,0,0,0,72,0,0,0,12,0,6,0,6,0,0,0,74,0,0,0,1,0,0,0,14,0,0,0,73,0,0,0,133,0,5,0,6,0,0,0,75,0,0,0,66,0,0,0,74,0,0,0,129,0,5,0,6,0,0,0,76,0,0,0,65,0,0,0,75,0,0,0,62,0,3,0,48,0,0,0,76,0,0,0,61,0,4,0,52,0,0,0,77,0,0,0,54,0,0,0,61,0,
4,0,6,0,0,0,83,0,0,0,48,0,0,0,142,0,5,0,52,0,0,0,84,0,0,0,82,0,0,0,83,0,0,0,12,0,8,0,52,0,0,0,85,0,0,0,1,0,0,0,49,0,0,0,77,0,0,0,78,0,0,0,84,0,0,0,62,0,3,0,57,0,0,0,85,0,0,0,61,0,4,0,6,0,0,0,86,0,0,0,48,0,0,0,62,0,3,0,59,0,0,0,86,0,0,0,249,0,2,0,62,0,
0,0,248,0,2,0,62,0,0,0,61,0,4,0,52,0,0,0,89,0,0,0,57,0,0,0,61,0,4,0,6,0,0,0,90,0,0,0,59,0,0,0,81,0,5,0,6,0,0,0,91,0,0,0,89,0,0,0,0,0,0,0,81,0,5,0,6,0,0,0,92,0,0,0,89,0,0,0,1,0,0,0,81,0,5,0,6,0,0,0,93,0,0,0,89,0,0,0,2,0,0,0,80,0,7,0,16,0,0,0,94,0,0,0,
91,0,0,0,92,0,0,0,93,0,0,0,90,0,0,0,62,0,3,0,88,0,0,0,94,0,0,0,253,0,1,0,56,0,1,0,0,0 };

const char* Waveform_frag_spv = (const char*) temp_binary_data_1;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x67130194:  numBytes = 1015; return Waveform_frag;
        case 0xbbfd048e:  numBytes = 2328; return Waveform_frag_spv;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "Waveform_frag",
    "Waveform_frag_spv"
};

const char* originalFilenames[] =
{
    "Waveform.frag",
    "Waveform.frag.spv"
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
