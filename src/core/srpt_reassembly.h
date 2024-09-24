#pragma once

#include "srpt_package.h"
#include "srpt_chunking.h"
#include <vector>

class SRPTReassembly {
public:
    SRPTPackage reassembleChunks(const std::vector<SRPTChunk>& chunks);
};