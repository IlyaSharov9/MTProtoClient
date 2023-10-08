#include "gzip.h"

namespace mtproto::gzip {

	mtproto::data::binary_buffer inflate(const mtproto::data::binary_buffer& compressed) {
		std::string decompressed = ::gzip::decompress(compressed.get_data_pointer(), compressed.get_size());
		return mtproto::data::binary_buffer((char*)decompressed.data(), decompressed.size());
	}
}