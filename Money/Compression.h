//
//  Compression.h
//  Money
//
//  Created by Cam Saul on 9/2/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace toucan_db {
	std::string Compress(const string& val);
	std::string Decompress(const string& val);
}