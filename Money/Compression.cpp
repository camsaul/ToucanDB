//
//  Compression.cpp
//  Money
//
//  Created by Cam Saul on 9/2/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Compression.h"
#include "Logging.h"

using namespace toucan_db::logging;

namespace toucan_db {
	
	std::string Compress(const string& val) {
		assert(!val.empty());
		
		string compressed;
		google::protobuf::io::StringOutputStream o(&compressed);
		auto co = unique_ptr<zerocc::AbstractCompressedOutputStream>(get_compressed_output_stream(&o, zerocc::LZ4, 1));
		{
			google::protobuf::io::CodedOutputStream c(co.get());
			c.WriteVarint32(val.length());
			c.WriteString(val.data());
		}
		co->Flush();
		auto compressionPercentage = round(((100.0 * val.length()) / compressed.length()) - 100.0);
		Logger(ORANGE) << "[compressed " << compressionPercentage << "%]";
		
		return compressed;
	}
	
	std::string Decompress(const string& val) {
		assert(!val.empty());
		
		string decompressed;
		
		google::protobuf::io::ArrayInputStream i(val.data(), val.length());
		auto ci = unique_ptr<zerocc::AbstractCompressedInputStream>(get_compressed_input_stream(&i, zerocc::LZ4));
		{
			google::protobuf::io::CodedInputStream c(ci.get());
			uint32_t size;
			c.ReadVarint32(&size);
			c.ReadString(&decompressed, size);
		}
		
		return decompressed;
	}
}
