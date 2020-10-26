///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and The min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2020-05-20
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <vector>

class SphereGenerator
{
public:
    // ctor/dtor
    SphereGenerator(float radius=1.0f, int sectorCount=36, int stackCount=18, bool smooth=true);
    ~SphereGenerator() {}

    // getters/setters
    float getRadius() const                 { return radius; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    void set(float radius, int sectorCount, int stackCount, bool smooth=true);
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);
    void setSmooth(bool smooth);

    // for vertex data
    size_t getVertexCount() const               { return vertices.size() / 3; }
    size_t getNormalCount() const               { return normals.size() / 3; }
    size_t getTexCoordCount() const             { return texCoords.size() / 2; }
    size_t getIndexCount() const                { return indices.size(); }
    size_t getLineIndexCount() const            { return lineIndices.size(); }
    size_t getTriangleCount() const             { return getIndexCount() / 3; }
    size_t getVertexSize() const                { return vertices.size() * sizeof(float); }
    size_t getNormalSize() const                { return normals.size() * sizeof(float); }
    size_t getTexCoordSize() const              { return texCoords.size() * sizeof(float); }
    size_t getIndexSize() const                 { return indices.size() * sizeof(unsigned int); }
    size_t getLineIndexSize() const             { return lineIndices.size() * sizeof(unsigned int); }
    const float* getVertices() const            { return vertices.data(); }
    const float* getNormals() const             { return normals.data(); }
    const float* getTexCoords() const           { return texCoords.data(); }
    const unsigned int* getIndices() const      { return indices.data(); }
    const unsigned int* getLineIndices() const  { return lineIndices.data(); }

    // for interleaved vertices: V/N/T
    size_t getInterleavedVertexCount() const        { return getVertexCount(); }    // # of vertices
    size_t getInterleavedVertexSize() const         { return interleavedVertices.size() * sizeof(float); }    // # of bytes
    int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes
    const float* getInterleavedVertices() const     { return interleavedVertices.data(); }

    // debug
    void printSelf() const;

protected:

private:
    // member functions
    void buildVerticesSmooth();
    void buildVerticesFlat();
    void buildInterleavedVertices();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

};

#endif
