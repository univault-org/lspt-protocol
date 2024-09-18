#pragma once

#include "lspt_package.h"
#include "lspt_chunking.h"
#include <vector>

class LSPTReassembly {
public:
    LSPTPackage reassembleChunks(const std::vector<LSPTChunk>& chunks);
};