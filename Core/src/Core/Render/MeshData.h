#pragma once
#include <memory>

namespace Core {
	namespace Render {
		constexpr const uint32_t kMaxLODs = 8;
		constexpr const uint32_t kMaxStreams = 8;

		//
		//    [ ------- LOD 0 ------- ][ ----------- LOD 1 ----------- ]
		//    [ --- VTX ---][ - IDX - ][ ----- VTX -----][ --- IDX --- ]
		//       Stream 0     Stream 1      Stream 2         Stream 3
		struct Mesh final {
			uint32_t lodCount;                                 // max of kMaxLODs
			uint32_t streamCount;                              // max of kMaxStreams
			uint32_t materialID;                               //
			uint32_t meshSize;                                 // checksum for the mesh [sum of all LOD index array size and the sum of all individual stream size]
			uint32_t vertexCount;                              // total number of verts in the mesh, across all LODs/streams
			uint32_t lodOffset[kMaxLODs];                      // list of LOD offsets, the last member of the list is the size of the last LOD
			uint64_t streamOffset[kMaxStreams]      = { 0 };   // list of offsets of all of the individual vertex data streams
			uint32_t streamElementSize[kMaxStreams] = { 0 };   // stride of the each of the vertex streams, counted in 4 byte chunks (size of a float)

			inline uint64_t lodSize(uint32_t lod) {
				if (lod >= kMaxLODs - 1) {
					return lodOffset[kMaxLODs - 1];
				}
				return static_cast<uint64_t>(lodOffset[lod + 1]) - lodOffset[lod];
			}
		};

		struct MeshFileHeader final {
			uint32_t magicValue;             // 0x12_34_56_78 -- first 4 bytes
			uint32_t meshCount;              //
			uint32_t dataBlockStartOffset;   // offset to the beginning of the mesh data
			uint32_t indexDataSize;          // size of index data
			uint32_t vertexDataSize;         // size of vertex data
		};
	}
}

